/*
 * catweaselmkiii.c
 *
 * Written by
 *  Andreas Boose <viceteam@t-online.de>
 *  Dirk Jadgmann <doj@cubic.org>
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

#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>

#include "catweaselmkiii.h"
#include "log.h"
#include "types.h"


typedef void (*voidfunc_t)(void);

static BYTE sidbuf[0x20];
static int sidfh=-1;


int catweaselmkiii_init(void)
{
  atexit((voidfunc_t)catweaselmkiii_close);

  if(sidfh<0)
    {
      sidfh = open("/dev/sid", O_WRONLY);

      if (sidfh < 0)
        sidfh = open("/dev/misc/sid", O_WRONLY);

      if (sidfh < 0) {
        log_error(LOG_DEFAULT,
                  "could not open sid device /dev/sid or /dev/misc/sid");
        return -1;
      }

      memset(sidbuf, 0, sizeof(sidbuf));
      lseek(sidfh, 0, SEEK_SET);
      write(sidfh, sidbuf, 0x19);
    }
  log_message(LOG_DEFAULT, "CatWeasel MK3 PCI SID: found");

  return 0;
}

int catweaselmkiii_open(void)
{
  if(sidfh<0)
    {
      sidfh = open("/dev/sid", O_WRONLY);

      if (sidfh < 0)
        sidfh = open("/dev/misc/sid", O_WRONLY);

      if (sidfh < 0) {
        log_error(LOG_DEFAULT,
                  "could not open sid device /dev/sid or /dev/misc/sid");
        return -1;
      }
    }

  memset(sidbuf, 0, sizeof(sidbuf));
  lseek(sidfh, 0, SEEK_SET);
  write(sidfh, sidbuf, 0x19);

  log_message(LOG_DEFAULT, "CatWeasel MK3 PCI SID: opened");

  return 0;
}

int catweaselmkiii_close(void)
{
  if(sidfh>=0)
    {
      memset(sidbuf, 0, sizeof(sidbuf));
      lseek(sidfh, 0, SEEK_SET);
      write(sidfh, sidbuf, 0x19);

      close(sidfh);
      sidfh=-1;

      log_message(LOG_DEFAULT, "CatWeasel MK3 PCI SID: closed");
    }
  return 0;
}

int catweaselmkiii_read(ADDRESS addr, int chipno)
{
  if(chipno==0 && addr<0x20)
    {
      if(addr>=0x19 && addr<=0x1C && sidfh>=0)
        {
          lseek(sidfh, addr, SEEK_SET);
          read(sidfh, &sidbuf[addr], 1);
        }
      return sidbuf[addr];
    }
  return 0;
}

void catweaselmkiii_store(ADDRESS addr, BYTE val, int chipno)
{
  if(chipno==0 && addr<=0x18)
    {
      sidbuf[addr]=val;
      if(sidfh>=0)
        {
          lseek(sidfh, addr, SEEK_SET);
          write(sidfh, &val, 1);
        }
    }
}

void catweaselmkiii_set_machine_parameter(long cycles_per_sec)
{
}
 
