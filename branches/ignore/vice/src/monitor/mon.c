/*
 * mon.c - The VICE built-in monitor.
 *
 * Written by
 *  Daniel Sladic <sladic@eecg.toronto.edu>
 *  Ettore Perazzoli <ettore@comm2000.it>
 *  Andreas Boose <viceteam@t-online.de>
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

#include "vice.h"

#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#ifdef HAVE_SIGNAL_H
#include <signal.h>
#else
#error "Need signals"
#endif

#ifdef __IBMC__
#include <direct.h>
#endif

#include "archdep.h"
#include "charset.h"
#include "console.h"
#include "interrupt.h"
#include "ioutil.h"
#include "log.h"
#include "mem.h"
#include "mon.h"
#include "mon_breakpoint.h"
#include "mon_disassemble.h"
#include "mon_memory.h"
#include "mon_parse.h"
#include "mon_register.h"
#include "mon_ui.h"
#include "mon_util.h"
#include "montypes.h"
#include "sysfile.h"
#include "types.h"
#include "uimon.h"
#include "utils.h"
#include "vsync.h"

/* May be called different things on various platforms... */
typedef void (*signal_handler_t)(int);

int mon_stop_output;

/* Defines */

#define MAX_LABEL_LEN 255
#define MAX_MEMSPACE_NAME_LEN 10
#define HASH_ARRAY_SIZE 256
#define HASH_ADDR(x) ((x)%0xff)
#define OP_JSR 0x20
#define OP_RTI 0x40
#define OP_RTS 0x60

#define ADDR_LIMIT(x) (LO16(x))
#define BAD_ADDR (new_addr(e_invalid_space, 0))

#define GET_OPCODE(mem)                                                  \
    (mon_get_mem_val(mem,                                                \
                     (WORD)((monitor_cpu_type.mon_register_get_val)(mem, \
                     e_PC))))

console_t *console_log = NULL;

/* External functions */
#ifdef HAVE_READLINE
extern void add_history(const char *str);
#endif

extern void parse_and_execute_line(char *input);

/* Types */

struct symbol_entry {
   WORD addr;
   char *name;
   struct symbol_entry *next;
};
typedef struct symbol_entry symbol_entry_t;

struct symbol_table {
   symbol_entry_t *name_list;
   symbol_entry_t *addr_hash_table[HASH_ARRAY_SIZE];
};
typedef struct symbol_table symbol_table_t;

/* Global variables */

/* Defined in file generated by bison. Set to 1 to get
 * parsing debug information. */
extern int yydebug;

static char *last_cmd = NULL;
int exit_mon = 0;
int mon_console_close_on_leaving = 1;


int sidefx;
RADIXTYPE default_radix;
MEMSPACE default_memspace;
static bool inside_monitor = FALSE;
static unsigned int instruction_count;
static bool skip_jsrs;
static int wait_for_return_level;
struct break_list_s *watchpoints_load[NUM_MEMSPACES];
struct break_list_s *watchpoints_store[NUM_MEMSPACES];
MEMSPACE caller_space;

const char *_mon_space_strings[] = {
    "Default", "Computer", "Disk8", "Disk9", "<<Invalid>>"
};

static WORD watch_load_array[10][NUM_MEMSPACES];
static WORD watch_store_array[10][NUM_MEMSPACES];
static unsigned int watch_load_count[NUM_MEMSPACES];
static unsigned int watch_store_count[NUM_MEMSPACES];
bool force_array[NUM_MEMSPACES];
static symbol_table_t monitor_labels[NUM_MEMSPACES];
monitor_interface_t *mon_interfaces[NUM_MEMSPACES];

MON_ADDR dot_addr[NUM_MEMSPACES];
unsigned char data_buf[256];
unsigned int data_buf_len;
bool asm_mode;
MON_ADDR asm_mode_addr;
static unsigned int next_or_step_stop;
unsigned mon_mask[NUM_MEMSPACES];

static bool watch_load_occurred;
static bool watch_store_occurred;

static bool recording;
static FILE *recording_fp;
static char *recording_name;
bool playback;
char *playback_name;
static void playback_commands(const char *filename);

/* Disassemble the current opcode on entry.  Used for single step.  */
static int disassemble_on_entry = 0;

monitor_cpu_type_t monitor_cpu_type;

struct monitor_cpu_type_list_s {
    monitor_cpu_type_t monitor_cpu_type;
    struct monitor_cpu_type_list_s *next_monitor_cpu_type;
};
typedef struct monitor_cpu_type_list_s monitor_cpu_type_list_t;

static monitor_cpu_type_list_t *monitor_cpu_type_list;

static const char *cond_op_string[] = { "",
                                        "==",
                                        "!=",
                                        ">",
                                        "<",
                                        ">=",
                                        "<=",
                                        "&&",
                                        "||"
                                       };

const char *mon_memspace_string[] = {"default", "C", "8", "9" };

static const char *register_string[] = { "A",
                                         "X",
                                         "Y",
                                         "PC",
                                         "SP"
                                        };


/* *** ADDRESS FUNCTIONS *** */


static void set_addr_memspace(MON_ADDR *a, MEMSPACE m)
{
    *a = LO16(*a) | LO16_TO_HI16(m);
}

bool mon_is_valid_addr(MON_ADDR a)
{
    return HI16_TO_LO16(a) != e_invalid_space;
}

bool mon_inc_addr_location(MON_ADDR *a, unsigned inc)
{
    unsigned new_loc = LO16(*a) + inc;
    *a = HI16(*a) | LO16(new_loc);

    return !(new_loc == LO16(new_loc));
}

void mon_evaluate_default_addr(MON_ADDR *a)
{
    if (addr_memspace(*a) == e_default_space)
        set_addr_memspace(a,default_memspace);
}

bool mon_is_in_range(MON_ADDR start_addr, MON_ADDR end_addr, unsigned loc)
{
    unsigned start, end;

    start = addr_location(start_addr);

    if (!mon_is_valid_addr(end_addr))
        return (loc == start);

    end = addr_location(end_addr);

    if (end < start)
        return ((loc >= start) || (loc <= end));

    return ((loc >= start) && (loc<=end));
}

static bool is_valid_addr_range(MON_ADDR start_addr, MON_ADDR end_addr)
{
    if (addr_memspace(start_addr) == e_invalid_space)
        return FALSE;

    if ((addr_memspace(start_addr) != addr_memspace(end_addr)) &&
         ((addr_memspace(start_addr) != e_default_space) ||
         (addr_memspace(end_addr) != e_default_space))) {
        return FALSE;
    }
    return TRUE;
}

static unsigned get_range_len(MON_ADDR addr1, MON_ADDR addr2)
{
    WORD start, end;
    unsigned len = 0;

    start = addr_location(addr1);
    end  = addr_location(addr2);

    if (start <= end) {
       len = end - start + 1;
    } else {
       len = (0xffff - start) + end + 1;
    }

    return len;
}

long mon_evaluate_address_range(MON_ADDR *start_addr, MON_ADDR *end_addr,
                                bool must_be_range, WORD default_len)
{
    long len = default_len;

    /* Check if we DEFINITELY need a range. */
    if (!is_valid_addr_range(*start_addr, *end_addr) && must_be_range)
        return -1;

    if (is_valid_addr_range(*start_addr, *end_addr)) {
        MEMSPACE mem1, mem2;
        /* Resolve any default memory spaces. We wait until now because we
         * need both addresses - if only 1 is a default, use the other to
         * resolve the memory space.
         */
        mem1 = addr_memspace(*start_addr);
        mem2 = addr_memspace(*end_addr);

        if (mem1 == e_default_space) {
            if (mem2 == e_default_space) {
                set_addr_memspace(start_addr, default_memspace);
                set_addr_memspace(end_addr, default_memspace);
            } else {
                if (mem2 != e_invalid_space) {
                set_addr_memspace(start_addr, mem2);
                } else {
                    set_addr_memspace(start_addr, default_memspace);
                }
            }
        } else {
            if (mem2 == e_default_space) {
                set_addr_memspace(end_addr, mem1);
            } else {
                if (mem2 != e_invalid_space) {
                    if (!(mem1 == mem2)) {
                        log_error(LOG_ERR, "Invalid memspace!");
                        return 0;
                    }
                } else {
                    log_error(LOG_ERR, "Invalid memspace!");
                    return 0;
                }
            }
        }

        len = get_range_len(*start_addr, *end_addr);
    } else {
        if (!mon_is_valid_addr(*start_addr))
            *start_addr = dot_addr[(int)default_memspace];
        else
            mon_evaluate_default_addr(start_addr);

        if (!mon_is_valid_addr(*end_addr)) {
            *end_addr = *start_addr;
            mon_inc_addr_location(end_addr, len);
        } else {
            set_addr_memspace(end_addr,addr_memspace(*start_addr));
            len = get_range_len(*start_addr, *end_addr);
        }
    }

    return len;
}


/* *** REGISTER AND MEMORY OPERATIONS *** */

mon_reg_list_t *mon_register_list_get(int mem)
{
    return monitor_cpu_type.mon_register_list_get(mem);
}

bool check_drive_emu_level_ok(int drive_num)
{
    if (drive_num == 8 && mon_interfaces[e_disk8_space] == NULL) {
        mon_out("True drive emulation not supported for this machine.\n");
        return FALSE;
    }
    if (drive_num == 9 && mon_interfaces[e_disk8_space] == NULL) {
        mon_out("True drive emulation not supported for this machine.\n");
        return FALSE;
    }

/* FIXME: IMO, the user should be allowed to access the true drive emulation
   even when it's (temporarily?) disabled.
   - but this may confuse the user since they might think the results are valid
   with respect to what is currently being emulated.
*/
#if 0
    else if (!app_resources.true1541) {
        mon_out("True drive emulation is not turned on.\n");
        return FALSE;
    }
#endif

   return TRUE;
}

void mon_cpu_type(const char *cpu_type)
{
    CPU_TYPE_t serchcpu;
    monitor_cpu_type_list_t *monitor_cpu_type_list_ptr;

    if (!strcasecmp(cpu_type, "6502")) {
        serchcpu = CPU_6502;
    } else {
        if (!strcasecmp(cpu_type, "z80")) {
            serchcpu = CPU_Z80;
        } else {
            mon_out("Unknown CPU type `%s'\n", cpu_type);
            return;
        }
    }

    monitor_cpu_type_list_ptr = monitor_cpu_type_list;
    while(monitor_cpu_type_list_ptr->monitor_cpu_type.cpu_type != serchcpu) {
        monitor_cpu_type_list_ptr
            = monitor_cpu_type_list_ptr->next_monitor_cpu_type;
        if (monitor_cpu_type_list_ptr == NULL) {
            mon_out("Unknown CPU type `%s'\n", cpu_type);
            return;
        }
    }

    memcpy(&monitor_cpu_type, &(monitor_cpu_type_list_ptr->monitor_cpu_type),
           sizeof(monitor_cpu_type_t));

    uimon_notify_change();
}

void mon_bank(MEMSPACE mem, const char *bankname)
{
    if (mem == e_default_space)
        mem = default_memspace;

    if (!mon_interfaces[mem]->mem_bank_list) {
        mon_out("Banks not available in this memspace\n");
        return;
    }

    if (bankname == NULL) {
        const char **bnp;

        bnp = mon_interfaces[mem]->mem_bank_list();
        mon_out("Available banks (some may be equivalent to others):\n");
        while (*bnp) {
            mon_out("%s\t", *bnp);
            bnp++;
        }
        mon_out("\n");
    } else {
        int newbank;

        newbank = mon_interfaces[mem]->mem_bank_from_name(bankname);
        if (newbank < 0) {
            mon_out("Unknown bank name `%s'\n", bankname);
            return;
        }
        mon_interfaces[mem]->current_bank = newbank;
    }
}

BYTE mon_get_mem_val_ex(MEMSPACE mem, int bank, WORD mem_addr)
{
    if (mem == e_disk8_space) {
        if (!check_drive_emu_level_ok(8))
            return 0;
    }
    if (mem == e_disk9_space) {
        if (!check_drive_emu_level_ok(9))
            return 0;
    }

    return mon_interfaces[mem]->mem_bank_read(bank, mem_addr);
}

BYTE mon_get_mem_val(MEMSPACE mem, WORD mem_addr)
{
    return mon_get_mem_val_ex(mem, mon_interfaces[mem]->current_bank, mem_addr);
}

void mon_set_mem_val(MEMSPACE mem, WORD mem_addr, BYTE val)
{
    int bank;

    bank = mon_interfaces[mem]->current_bank;

    switch (mem) {
      case e_comp_space:
        break;
      case e_disk8_space:
        if (!check_drive_emu_level_ok(8))
            return;
        break;
      case e_disk9_space:
        if (!check_drive_emu_level_ok(9))
            return;
        break;
      default:
        log_error(LOG_ERR, "Unknow memspace!");
        return;
    }

    mon_interfaces[mem]->mem_bank_write(bank, mem_addr, val);
}

void mon_jump(MON_ADDR addr)
{
    mon_evaluate_default_addr(&addr);
    (monitor_cpu_type.mon_register_set_val)(addr_memspace(addr), e_PC,
                                            (WORD)(addr_location(addr)));
    exit_mon = 1;
}


/* *** ULTILITY FUNCTIONS *** */

void mon_print_bin(int val, char on, char off)
{
    int divisor;
    char digit;

    if (val > 0xfff)
        divisor = 0x8000;
    else if (val > 0xff)
        divisor = 0x800;
    else
        divisor = 0x80;

    while (divisor) {
        digit = (val & divisor) ? on : off;
        mon_out("%c",digit);
        if (divisor == 0x100)
            mon_out(" ");
        divisor /= 2;
    }
}

static void print_hex(int val)
{
    mon_out(val > 0xff ? "$%04x\n" : "$%02x\n", val);
}

static void print_octal(int val)
{
    mon_out(val > 0777 ? "0%06o\n" : "0%03o\n", val);
}


void mon_print_convert(int val)
{
    mon_out("+%d\n", val);
    print_hex(val);
    print_octal(val);
    mon_print_bin(val,'1','0');
    mon_out("\n");
}

void mon_add_number_to_buffer(int number)
{
    data_buf[data_buf_len++] = (number & 0xff);
    if (number > 0xff)
        data_buf[data_buf_len++] = ( (number>>8) & 0xff);
    data_buf[data_buf_len] = '\0';
}


void mon_add_string_to_buffer(char *str)
{
    strcpy((char *) &(data_buf[data_buf_len]), str);
    data_buf_len += strlen(str);
    data_buf[data_buf_len] = '\0';
    free(str);
}


/* *** MISC COMMANDS *** */

void monitor_init(monitor_interface_t *maincpu_interface_init,
                  monitor_interface_t *drive8_interface_init,
                  monitor_interface_t *drive9_interface_init,
                  monitor_cpu_type_t **asmarray)
{
    int i, j;
    monitor_cpu_type_list_t *monitor_cpu_type_list_ptr;

    yydebug = 0;
    sidefx = e_OFF;
    default_radix = e_hexadecimal;
    default_memspace = e_comp_space;
    instruction_count = 0;
    skip_jsrs = FALSE;
    wait_for_return_level = 0;
    mon_breakpoint_init();
    data_buf_len = 0;
    asm_mode = 0;
    next_or_step_stop = 0;
    recording = FALSE;
    playback = FALSE;

    mon_ui_init();

    monitor_cpu_type_list = (monitor_cpu_type_list_t *)xmalloc(sizeof(monitor_cpu_type_list_t));
    monitor_cpu_type_list_ptr = monitor_cpu_type_list;

    i = 0;
    while(asmarray[i] != NULL) {
        memcpy(&(monitor_cpu_type_list_ptr->monitor_cpu_type),
               asmarray[i], sizeof(monitor_cpu_type_t));
        monitor_cpu_type_list_ptr->next_monitor_cpu_type
            = (monitor_cpu_type_list_t *)xmalloc(sizeof(monitor_cpu_type_list_t));
        monitor_cpu_type_list_ptr
            = monitor_cpu_type_list_ptr->next_monitor_cpu_type;
        monitor_cpu_type_list_ptr->next_monitor_cpu_type = NULL;
        i++;
    }

    memcpy(&monitor_cpu_type, asmarray[0], sizeof(monitor_cpu_type_t));

    watch_load_occurred = FALSE;
    watch_store_occurred = FALSE;

    for (i = 1; i < NUM_MEMSPACES; i++) {
        dot_addr[i] = new_addr(e_default_space + i, 0);
        watch_load_count[i] = 0;
        watch_store_count[i] = 0;
        mon_mask[i] = MI_NONE;
        monitor_labels[i].name_list = NULL;
        for (j = 0; j < HASH_ARRAY_SIZE; j++)
            monitor_labels[i].addr_hash_table[j] = NULL;
    }

    caller_space = e_comp_space;

    asm_mode_addr = BAD_ADDR;

    mon_interfaces[e_comp_space] = maincpu_interface_init;
    mon_interfaces[e_disk8_space] = drive8_interface_init;
    mon_interfaces[e_disk9_space] = drive9_interface_init;
}

void mon_start_assemble_mode(MON_ADDR addr, char *asm_line)
{
    asm_mode = 1;

    mon_evaluate_default_addr(&addr);
    asm_mode_addr = addr;
}

/* ------------------------------------------------------------------------- */

/* Memory.  */

void mon_display_screen(void)
{
    WORD base;
    BYTE rows, cols;
    unsigned int r, c;

    mem_get_screen_parameter(&base, &rows, &cols);
    for (r = 0; r < rows; r++) {
        for (c = 0; c < cols; c++) {
            BYTE data;

            data = mon_get_mem_val(e_comp_space, (WORD)ADDR_LIMIT(base++));
            data = charset_p_toascii(charset_screencode_to_petcii(data), 1);

            mon_out("%c", data);
        }
        mon_out("\n");
    }
}

void mon_display_io_regs(void)
{
    mem_ioreg_list_t *mem_ioreg_list, *mem_ioreg_list_base;
    MON_ADDR start,end;

    mem_ioreg_list_base
        = mon_interfaces[default_memspace]->mem_ioreg_list_get();

    mem_ioreg_list = mem_ioreg_list_base;

    do {
        mon_out("%s:\n", mem_ioreg_list->name);
        start = new_addr(default_memspace, mem_ioreg_list->start);
        end = new_addr(default_memspace, mem_ioreg_list->end);
        mon_memory_display(e_hexadecimal, start, end);

        mem_ioreg_list = mem_ioreg_list->next;
    } while (mem_ioreg_list != NULL);

    free(mem_ioreg_list_base);
}

void mon_ioreg_add_list(mem_ioreg_list_t **list, const char *name,
                        WORD start, WORD end)
{
    mem_ioreg_list_t *base, *curr;
    unsigned int n;

    base = *list;
    curr = *list;
    n = 0;

    while (curr != NULL) {
        n++;
        curr = curr->next;
    }

    base = (mem_ioreg_list_t *)xrealloc(base, sizeof(mem_ioreg_list_t)
           * (n + 1));

    if (n > 0)
        base[n - 1].next = &base[n];

    base[n].name = name;
    base[n].start = start;
    base[n].end = end;
    base[n].next = NULL;

    *list = base;
}


/* *** FILE COMMANDS *** */

void mon_change_dir(const char *path)
{
    if (ioutil_chdir((char*)path) < 0)
        mon_out("Cannot change to directory `%s':\n", path);

    mon_out("Changing to directory: `%s'\n", path);
}

void mon_load_symbols(MEMSPACE mem, const char *filename)
{
    /* Switched to a command-line format for the symbol file
     * so loading just involves "playing back" the commands.
     * It is no longer possible to just load symbols from a
     * single memory space.
     */
    playback_commands(filename);
#if 0
    FILE   *fp;
    WORD adr;
    char memspace[MAX_MEMSPACE_NAME_LEN], name[MAX_LABEL_LEN];
    char *name_ptr;
    bool found = FALSE;
    int rc, line_num = 2;

    if (NULL == (fp = fopen(filename, MODE_READ))) {
        mon_out("Loading for `%s' failed.\n", filename);
        return;
    }

    mon_out("Loading symbol table from `%s'...\n", filename);

    if (mem == e_default_space) {
        if (fscanf(fp, "%10s\n", name) == 1) {
            for (mem = FIRST_SPACE; mem <= LAST_SPACE; mem++) {
                if (strcmp(name, mon_memspace_string[mem]) == 0) {
                    found = TRUE;
                    break;
                }
            }
        }
        if (!found) {
            mon_out(
                      "Bad label file : expecting a memory space in the first line but found %s\n",
                      name);
            return;
        }
    }

    while (!feof(fp)) {
        rc = fscanf(fp, "%6x %255s\n", (int *) &adr, name);
        if (rc != 2) {
            mon_out(
                      "Bad label file: (line %d) cannot parse argument %d.\n",
                      line_num, rc + 1);
            break;
        }
        /* FIXME: Check name is a valid label name */
        name_ptr = (char *) xmalloc((strlen(name)+1) * sizeof(char));
        strcpy(name_ptr, name);
        mon_out("Read ($%x:%s)\n", adr, name_ptr);
        mon_add_name_to_symbol_table(new_addr(mem, adr), name_ptr);

        line_num++;
    }

    fclose(fp);
#endif
}

void mon_save_symbols(MEMSPACE mem, const char *filename)
{
    FILE *fp;
    symbol_entry_t *sym_ptr;

    if (NULL == (fp = fopen(filename, MODE_WRITE))) {
        mon_out("Saving for `%s' failed.\n", filename);
        return;
    }

    mon_out("Saving symbol table to `%s'...\n", filename);

    /* FIXME: Write out all memspaces? */
    if (mem == e_default_space)
       mem = default_memspace;

    sym_ptr = monitor_labels[mem].name_list;

    while (sym_ptr) {
        fprintf(fp, "al %s:%04x %s\n", mon_memspace_string[mem], sym_ptr->addr,
                sym_ptr->name);
        sym_ptr = sym_ptr->next;
    }

    fclose(fp);
}


/* *** COMMAND FILES *** */


void mon_record_commands(char *filename)
{
    if (recording) {
        mon_out("Recording already in progress. Use 'stop' to end recording.\n");
        return;
    }

    recording_name = filename;

    if (NULL == (recording_fp = fopen(recording_name, MODE_WRITE))) {
        mon_out("Cannot create `%s'.\n", recording_name);
        return;
    }

    setbuf(recording_fp, NULL);

    recording = TRUE;
}

void mon_end_recording(void)
{
    if (!recording) {
        mon_out("No file is currently being recorded.\n");
        return;
    }

    fclose(recording_fp);
    mon_out("Closed file %s.\n", recording_name);
    recording = FALSE;
}

static void playback_commands(const char *filename)
{
    FILE *fp;
    char string[256];

    if (NULL == (fp = fopen(filename, MODE_READ_TEXT))
        && NULL == (fp = sysfile_open(filename, NULL, MODE_READ_TEXT))) {
        mon_out("Playback for `%s' failed.\n", filename);
        return;
    }

    while (fgets(string, 255, fp) != NULL) {
        if (strcmp(string, "stop\n") == 0)
            break;

        string[strlen(string) - 1] = '\0';
        parse_and_execute_line(string);
    }

    fclose(fp);
    playback = FALSE;
}


/* *** SYMBOL TABLE *** */


static void free_symbol_table(MEMSPACE mem)
{
    symbol_entry_t *sym_ptr, *temp;
    int i;

    /* Remove name list */
    sym_ptr = monitor_labels[mem].name_list;
    while (sym_ptr) {
        /* Name memory is freed below. */
        temp = sym_ptr;
        sym_ptr = sym_ptr->next;
        free(temp);
    }

    /* Remove address hash table */
    for (i = 0; i < HASH_ARRAY_SIZE; i++) {
        sym_ptr = monitor_labels[mem].addr_hash_table[i];
        while (sym_ptr) {
            free (sym_ptr->name);
            temp = sym_ptr;
            sym_ptr = sym_ptr->next;
            free(temp);
        }
    }
}

char *mon_symbol_table_lookup_name(MEMSPACE mem, WORD addr)
{
    symbol_entry_t *sym_ptr;

    if (mem == e_default_space)
        mem = default_memspace;

    sym_ptr = monitor_labels[mem].addr_hash_table[HASH_ADDR(addr)];
    while (sym_ptr) {
        if (addr == sym_ptr->addr)
            return sym_ptr->name;
        sym_ptr = sym_ptr->next;
    }

    return NULL;
}

int mon_symbol_table_lookup_addr(MEMSPACE mem, char *name)
{
    symbol_entry_t *sym_ptr;

    if (mem == e_default_space)
        mem = default_memspace;

    if (strcmp(name, ".PC") == 0) {
        return (monitor_cpu_type.mon_register_get_val)(mem, e_PC);
    }

    sym_ptr = monitor_labels[mem].name_list;
    while (sym_ptr) {
        if (strcmp(sym_ptr->name, name) == 0)
            return sym_ptr->addr;
        sym_ptr = sym_ptr->next;
    }

    return -1;
}

void mon_add_name_to_symbol_table(MON_ADDR addr, char *name)
{
    symbol_entry_t *sym_ptr;
    char *old_name;
    int old_addr;
    MEMSPACE mem = addr_memspace(addr);
    WORD loc = addr_location(addr);

    if (strcmp(name, ".PC") == 0) {
        mon_out("Error: .PC is a reserved label.\n");
        return;
    }

    if (mem == e_default_space)
        mem = default_memspace;

    old_name = mon_symbol_table_lookup_name(mem, loc);
    old_addr = mon_symbol_table_lookup_addr(mem, name);
    if (old_name && (WORD)(old_addr) != addr ) {
        mon_out("Warning: label(s) for address $%04x already exist.\n",
                  loc);
    }
    if (old_addr >= 0 && old_addr != loc) {
        mon_out("Changing address of label %s from $%04x to $%04x\n",
                  name, old_addr, loc);
    }

    /* Add name to name list */
    sym_ptr = (symbol_entry_t *)xmalloc(sizeof(symbol_entry_t));
    sym_ptr->name = name;
    sym_ptr->addr = loc;

    sym_ptr->next = monitor_labels[mem].name_list;
    monitor_labels[mem].name_list = sym_ptr;

    /* Add address to hash table */
    sym_ptr = (symbol_entry_t *)xmalloc(sizeof(symbol_entry_t));
    sym_ptr->name = name;
    sym_ptr->addr = addr;

    sym_ptr->next = monitor_labels[mem].addr_hash_table[HASH_ADDR(loc)];
    monitor_labels[mem].addr_hash_table[HASH_ADDR(loc)] = sym_ptr;
}

void mon_remove_name_from_symbol_table(MEMSPACE mem, char *name)
{
    int addr;
    symbol_entry_t *sym_ptr, *prev_ptr;

    if (mem == e_default_space)
        mem = default_memspace;

    if (name == NULL) {
        /* FIXME - prompt user */
        free_symbol_table(mem);
        return;
    }

    if ( (addr = mon_symbol_table_lookup_addr(mem, name)) < 0) {
        mon_out("Symbol %s not found.\n", name);
        return;
    }

    /* Remove entry in name list */
    sym_ptr = monitor_labels[mem].name_list;
    prev_ptr = NULL;
    while (sym_ptr) {
        if (strcmp(sym_ptr->name, name) == 0) {
            /* Name memory is freed below. */
            addr = sym_ptr->addr;
            if (prev_ptr)
                prev_ptr->next = sym_ptr->next;
            else
                monitor_labels[mem].name_list = NULL;
            free(sym_ptr);
            break;
        }
        prev_ptr = sym_ptr;
        sym_ptr = sym_ptr->next;
    }

    /* Remove entry in address hash table */
    sym_ptr = monitor_labels[mem].addr_hash_table[HASH_ADDR(addr)];
    prev_ptr = NULL;
    while (sym_ptr) {
        if (addr == sym_ptr->addr) {
            free (sym_ptr->name);
            if (prev_ptr)
                prev_ptr->next = sym_ptr->next;
            else
                monitor_labels[mem].addr_hash_table[HASH_ADDR(addr)] = NULL;
            free(sym_ptr);
            return;
        }
        prev_ptr = sym_ptr;
        sym_ptr = sym_ptr->next;
    }
}

void mon_print_symbol_table(MEMSPACE mem)
{
    symbol_entry_t *sym_ptr;

    if (mem == e_default_space)
        mem = default_memspace;

    sym_ptr = monitor_labels[mem].name_list;
    while (sym_ptr) {
        mon_out("$%04x %s\n",sym_ptr->addr, sym_ptr->name);
        sym_ptr = sym_ptr->next;
    }
}


/* *** INSTRUCTION COMMANDS *** */


void mon_instructions_step(int count)
{
    if (count >= 0)
        mon_out("Stepping through the next %d instruction(s).\n",
                  count);
    instruction_count = (count >= 0) ? count : 1;
    wait_for_return_level = 0;
    skip_jsrs = FALSE;
    exit_mon = 1;

    if (instruction_count == 1)
        mon_console_close_on_leaving = 0;

    mon_mask[caller_space] |= MI_STEP;
    interrupt_monitor_trap_on(mon_interfaces[caller_space]->int_status);
}

void mon_instructions_next(int count)
{
    if (count >= 0)
        mon_out("Nexting through the next %d instruction(s).\n",
                   count);
    instruction_count = (count >= 0) ? count : 1;
    wait_for_return_level = (int)((GET_OPCODE(caller_space) == OP_JSR) ? 1 : 0);
    skip_jsrs = TRUE;
    exit_mon = 1;

    if (instruction_count == 1)
        mon_console_close_on_leaving = 0;

    mon_mask[caller_space] |= MI_STEP;
    interrupt_monitor_trap_on(mon_interfaces[caller_space]->int_status);
}

void mon_instruction_return(void)
{
    instruction_count = 1;
    wait_for_return_level = (int)((GET_OPCODE(caller_space) == OP_RTS)
                            ? 0
                            : (GET_OPCODE(caller_space) == OP_JSR) ? 2 : 1);
    skip_jsrs = TRUE;
    exit_mon = 1;

    mon_mask[caller_space] |= MI_STEP;
    interrupt_monitor_trap_on(mon_interfaces[caller_space]->int_status);
}

void mon_stack_up(int count)
{
    mon_out("Going up %d stack frame(s).\n",
              (count>=0)?count:1);
}

void mon_stack_down(int count)
{
    mon_out("Going down %d stack frame(s).\n",
              (count>=0)?count:1);
}


/* *** CONDITIONAL EXPRESSIONS *** */


void mon_print_conditional(cond_node_t *cnode)
{
    /* Do an in-order traversal of the tree */
    if (cnode->is_parenthized)
        mon_out("( ");

    if (cnode->operation != e_INV) {
        if(!(cnode->child1 && cnode->child2)) {
            log_error(LOG_ERR, "No conditional!");
            return;
        }
        mon_print_conditional(cnode->child1);
        mon_out(" %s ",cond_op_string[cnode->operation]);
        mon_print_conditional(cnode->child2);
    } else {
        if (cnode->is_reg)
            mon_out(".%s", register_string[reg_regid(cnode->reg_num)]);
        else
            mon_out("%d", cnode->value);
    }

    if (cnode->is_parenthized)
        mon_out(" )");
}


int mon_evaluate_conditional(cond_node_t *cnode)
{
    /* Do a post-order traversal of the tree */
    if (cnode->operation != e_INV) {
        if (!(cnode->child1 && cnode->child2)) {
            log_error(LOG_ERR, "No conditional!");
            return 0;
        }
        mon_evaluate_conditional(cnode->child1);
        mon_evaluate_conditional(cnode->child2);

        switch(cnode->operation) {
          case e_EQU:
            cnode->value = ((cnode->child1->value) == (cnode->child2->value));
            break;
          case e_NEQ:
            cnode->value = ((cnode->child1->value) != (cnode->child2->value));
            break;
          case e_GT :
            cnode->value = ((cnode->child1->value) > (cnode->child2->value));
            break;
          case e_LT :
            cnode->value = ((cnode->child1->value) < (cnode->child2->value));
            break;
          case e_GTE:
            cnode->value = ((cnode->child1->value) >= (cnode->child2->value));
            break;
          case e_LTE:
            cnode->value = ((cnode->child1->value) <= (cnode->child2->value));
            break;
          case e_AND:
            cnode->value = ((cnode->child1->value) && (cnode->child2->value));
            break;
          case e_OR :
            cnode->value = ((cnode->child1->value) || (cnode->child2->value));
            break;
          default:
            log_error(LOG_ERR, "Unexpected conditional operator: %d\n",
                      cnode->operation);
            return 0;
        }
    } else {
        if (cnode->is_reg)
            cnode->value = (monitor_cpu_type.mon_register_get_val)
                           (reg_memspace(cnode->reg_num),
                           reg_regid(cnode->reg_num));
    }

    return cnode->value;
}


void mon_delete_conditional(cond_node_t *cnode)
{
    if (!cnode)
        return;

    if (cnode->child1)
        mon_delete_conditional(cnode->child1);

    if (cnode->child2)
        mon_delete_conditional(cnode->child2);

    free(cnode);
}


/* *** WATCHPOINTS *** */


void mon_watch_push_load_addr(WORD addr, MEMSPACE mem)
{
    if (inside_monitor)
        return;

    if (watch_load_count[mem] == 9)
         return;

    watch_load_occurred = TRUE;
    watch_load_array[watch_load_count[mem]][mem] = addr;
    watch_load_count[mem]++;
}

void mon_watch_push_store_addr(WORD addr, MEMSPACE mem)
{
    if (inside_monitor)
        return;

    if (watch_store_count[mem] == 9)
        return;

    watch_store_occurred = TRUE;
    watch_store_array[watch_store_count[mem]][mem] = addr;
    watch_store_count[mem]++;
}

bool watchpoints_check_loads(MEMSPACE mem)
{
    bool trap = FALSE;
    unsigned count;
    WORD addr = 0;

    count = watch_load_count[mem];
    watch_load_count[mem] = 0;
    while (count) {
        count--;
        addr = watch_load_array[count][mem];
        if (mon_breakpoint_check_checkpoint(mem, addr, watchpoints_load[mem]))
            trap = TRUE;
    }
    return trap;
}

bool watchpoints_check_stores(MEMSPACE mem)
{
    bool trap = FALSE;
    unsigned count;
    WORD addr = 0;

    count = watch_store_count[mem];
    watch_store_count[mem] = 0;

    while (count) {
        count--;
        addr = watch_store_array[count][mem];
        if (mon_breakpoint_check_checkpoint(mem, addr, watchpoints_store[mem]))
            trap = TRUE;
    }
    return trap;
}


/* *** CPU INTERFACES *** */


int mon_force_import(MEMSPACE mem)
{
    bool result;

    result = force_array[mem];
    force_array[mem] = FALSE;

    return result;
}

void mon_check_icount(WORD a)
{
    if (!instruction_count)
        return;

    if (wait_for_return_level == 0)
        instruction_count--;

    if (skip_jsrs == TRUE) {
        if (GET_OPCODE(caller_space) == OP_JSR)
            wait_for_return_level++;

        if (GET_OPCODE(caller_space) == OP_RTS)
            wait_for_return_level--;

        if (GET_OPCODE(caller_space) == OP_RTI)
            wait_for_return_level--;

        if (wait_for_return_level < 0) {
            wait_for_return_level = 0;

            /* FIXME: [SRT], 01-24-2000: this is only a workaround.
             this occurs when the commands 'n' or  'ret' are executed
             out of an active IRQ or NMI processing routine.

             the following command immediately stops executing when used
             with 'n' and parameter > 1, but it's necessary because else,
             it can occur that the monitor will not come back at all.
             Don't know so far how this can be avoided. The only
             solution I see is to keep track of every IRQ and NMI
             invocation and every RTI. */
            instruction_count = 0;
        }
    }

    if (instruction_count != 0)
        return;

    if (mon_mask[caller_space] & MI_STEP) {
        mon_mask[caller_space] &= ~MI_STEP;
        disassemble_on_entry = 1;
    }
    if (!mon_mask[caller_space])
        interrupt_monitor_trap_off(mon_interfaces[caller_space]->int_status);

    mon(a);
}

void mon_check_icount_interrupt(void)
{
    /* this is a helper for mon_check_icount.
    It's called whenever a IRQ or NMI is executed
    and the mon_mask[caller_space] | MI_STEP is
    active, i.e., we're in the single step mode */

    if (instruction_count)
        if (skip_jsrs == TRUE)
            wait_for_return_level++;
}

void mon_check_watchpoints(WORD a)
{
    if (watch_load_occurred) {
        if (watchpoints_check_loads(e_comp_space)) {
            caller_space = e_comp_space;
            mon(a);
        }
        if (watchpoints_check_loads(e_disk8_space)) {
            caller_space = e_disk8_space;
            mon(a);
        }
        if (watchpoints_check_loads(e_disk9_space)) {
            caller_space = e_disk9_space;
            mon(a);
        }
        watch_load_occurred = FALSE;
    }

    if (watch_store_occurred) {
        if (watchpoints_check_stores(e_comp_space)) {
            caller_space = e_comp_space;
            mon(a);
        }
        if (watchpoints_check_stores(e_disk8_space)) {
            caller_space = e_disk8_space;
            mon(a);
        }
        if (watchpoints_check_stores(e_disk9_space)) {
            caller_space = e_disk9_space;
            mon(a);
        }
        watch_store_occurred = FALSE;
    }
}

static void make_prompt(char *str)
{
    if (asm_mode)
        sprintf(str, ".%04x  ", addr_location(asm_mode_addr));
    else
        sprintf(str, "(%s:$%04x) ",
                mon_memspace_string[default_memspace],
                addr_location(dot_addr[default_memspace]));
}

static signal_handler_t old_handler;

void mon_abort(void)
{
    mon_stop_output = 1;
}

static void handle_abort(int signo)
{
    mon_abort();
    signal(SIGINT, (signal_handler_t) handle_abort);
}

void mon_open(WORD a)
{
    if (mon_console_close_on_leaving) {
        console_log = uimon_window_open();
        uimon_set_interface(mon_interfaces, NUM_MEMSPACES);
    } else {
        console_log = uimon_window_resume();
        mon_console_close_on_leaving = 1;
    }


    old_handler = signal(SIGINT, handle_abort);

    inside_monitor = TRUE;
    vsync_suspend_speed_eval();

    uimon_notify_change();

    dot_addr[caller_space] = new_addr(caller_space, a);

    mon_out("\n** Monitor\n");

    if (disassemble_on_entry) {
        mon_disassemble_instr(new_addr(caller_space, a));
        disassemble_on_entry = 0;
    }
}

int mon_process(char *cmd)
{
    mon_stop_output = 0;
    if (cmd == NULL) {
        mon_out("\n");
    } else {
        if (!cmd[0]) {
            if (!asm_mode) {
                /* Repeat previous command */
                free(cmd);

                cmd = last_cmd ? stralloc(last_cmd) : NULL;

            } else {
                /* Leave asm mode */
            }
        }
#ifdef HAVE_READLINE
        else {
            /* Nonempty line */
            add_history(cmd);
        }
#endif

        if (cmd) {
            if (recording) {
                if (fprintf(recording_fp, "%s\n", cmd) < 0) {
                   mon_out("Error while recording commands. "
                             "Output file closed.\n");
                   fclose(recording_fp);
                   recording_fp = NULL;
                   recording = FALSE;
                }
            }

            parse_and_execute_line(cmd);

            if (playback)
                playback_commands(playback_name);
        }
    }
    if (last_cmd)
        free(last_cmd);

    last_cmd = cmd;

    uimon_notify_change(); /* @SRT */

    return exit_mon;
}

void mon_close(int check)
{
    inside_monitor = FALSE;
    vsync_suspend_speed_eval();

    exit_mon--;

    if (check && exit_mon)
        exit(0);

    exit_mon = 0;

    signal(SIGINT, old_handler);

    if (console_log->console_can_stay_open == 0)
                mon_console_close_on_leaving = 1;

    if (mon_console_close_on_leaving) {
        uimon_window_close();
    } else {
        uimon_window_suspend();
    }
}


void mon(WORD a)
{
    char prompt[40];

    mon_open(a);
    while (!exit_mon) {
        make_prompt(prompt);
        mon_process(uimon_in(prompt));
    }
    mon_close(1);
}

