/*
 * autostart-prg.c - Handle autostart of program files
 *
 * Written by
 *  Christian Vogelgsang <chris@vogelgsang.org>
 *
 * This file is part of VICE, the Versatile Commodore Emulator.
 * See README for copyright notice.
 *
 *  This program is free software; you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation; either version 2 of the License, or
 *  (at your option) any later version.
 *
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with this program; if not, write to the Free Software
 *  Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA
 *  02111-1307  USA.
 *
 */

#include "archdep.h"
#include "attach.h"
#include "autostart-prg.h"
#include "fsdevice.h"
#include "lib.h"
#include "machine.h"
#include "mem.h"
#include "resources.h"
#include "util.h"

/* ----- Globals ----- */

/* program from last injection */
static autostart_prg_t *inject_prg;


static autostart_prg_t * load_prg(const char *file_name, fileio_info_t *finfo, log_t log)
{
    WORD ptr;
    int  end;
    BYTE lo,hi;
    int  i;
    autostart_prg_t *prg;
    
    prg = (autostart_prg_t *)lib_malloc(sizeof(autostart_prg_t));
    if(prg == NULL) {
        return NULL;
    }

    /* get data size of file */
    prg->size = fileio_get_bytes_left(finfo);
    prg->data = NULL;

    /* read start address */
    if((fileio_read(finfo, &lo, 1) != 1) ||
       (fileio_read(finfo, &hi, 1) != 1)) {
        log_error(log, "Cannot read start address from '%s'", file_name);
        return NULL;
    }
    prg->start_addr = (WORD)hi << 8 | (WORD)lo;
    prg->size -= 2; /* skip load addr */

    /* check range */
    end = prg->start_addr + prg->size - 1;
    if(end > 0xffff) {
        log_error(log, "Invalid size of '%s': %d", file_name, prg->size);
        return NULL;
    }

    /* load to memory */
    prg->data = (BYTE *)lib_malloc(prg->size);
    if(prg->data == NULL) {
        log_error(log, "No memory for '%s'", file_name);
        return NULL;        
    }
    
    /* copy data to memory */
    ptr = prg->start_addr;
    i = 0;
    while(ptr <= end) {
        if(fileio_read(finfo, &(prg->data[i]), 1) != 1) {
            log_error(log, "Error loading data from '%s'", file_name);
            lib_free(prg->data);
            return NULL;
        }
        ptr++;
        i++;
    }

    return prg;
}

static void free_prg(autostart_prg_t *prg)
{
    if(prg->data != NULL) {
        lib_free(prg->data);
    }
    lib_free(prg);
}

/* ---------- main interface ---------- */

void autostart_prg_init(void)
{
    inject_prg = NULL;
}

void autostart_prg_shutdown(void)
{
    if(inject_prg != NULL) {
        free_prg(inject_prg);
    }
}

int autostart_prg_with_virtual_fs(const char *file_name,
                                  fileio_info_t *fh,
                                  log_t log)
{
    char *directory;
    char *file;

    /* Extract the directory path to allow FS-based drive emulation to
       work.  */
    util_fname_split(file_name, &directory, &file);

    if (archdep_path_is_relative(directory)) {
        char *tmp;
        archdep_expand_path(&tmp, directory);
        lib_free(directory);
        directory = tmp;

        /* FIXME: We should actually eat `.'s and `..'s from `directory'
           instead.  */
    }

    /* Setup FS-based drive emulation.  */
    fsdevice_set_directory(directory ? directory : ".", 8);
    resources_set_int("DriveTrueEmulation", 0);
    resources_set_int("VirtualDevices", 1);
    resources_set_int("FSDevice8ConvertP00", 1);
    file_system_detach_disk(8);
    
    lib_free(directory);
    lib_free(file);
    
    return 0;
}

int autostart_prg_with_ram_injection(const char *file_name,
                                     fileio_info_t *fh,
                                     log_t log)
{
    /* clean up old injection */
    if(inject_prg != NULL) {
        free_prg(inject_prg);
    }
    
    /* load program file into memory */
    inject_prg = load_prg(file_name, fh, log);
    return (inject_prg == NULL) ? -1 : 0;
}

int autostart_prg_with_disk_image(const char *file_name,
                                  fileio_info_t *fh,
                                  log_t log)
{
    /* TBD */
    return -1;
}

int autostart_prg_perform_injection(log_t log)
{
    int i;
    autostart_prg_t *prg = inject_prg;
    
    if(prg == NULL) {
        log_error(log, "Nothing to inject!");
        return -1;
    }
    
    log_message(log, "Injecting program data at $%04x (size $%04x)", 
                prg->start_addr,
                prg->size);
                
    /* store data in emu memory */
    for(i = 0; i < prg->size; i++) {
        mem_store(prg->start_addr + i, prg->data[i]);
    }
    
    /* now simulate a basic load */
    WORD end = prg->start_addr + prg->size;
    mem_set_basic_text(prg->start_addr, end);
    
    /* clean up injected prog */
    free_prg(inject_prg);
    inject_prg = NULL;
            
    return 0;
}

