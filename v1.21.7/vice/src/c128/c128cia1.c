/*
 * c128cia1.c - Definitions for the first MOS6526 (CIA) chip in the C128
 * ($DC00).
 *
 * Written by
 *  Andr� Fachat <fachat@physik.tu-chemnitz.de>
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
 * */

#include "vice.h"

#include <stdio.h>

#include "c128fastiec.h"
#include "c64.h"
#include "c64cia.h"
#include "cia.h"
#include "clkguard.h"
#include "drivecpu.h"
#include "interrupt.h"
#include "keyboard.h"
#include "lib.h"
#include "log.h"
#include "maincpu.h"
#include "types.h"
#include "vicii.h"

#ifdef HAVE_RS232
#include "rsuser.h"
#endif


void REGPARM2 cia1_store(WORD addr, BYTE data)
{
    ciacore_store(machine_context.cia1, addr, data);
}

BYTE REGPARM1 cia1_read(WORD addr)
{
    return ciacore_read(machine_context.cia1, addr);
}

BYTE REGPARM1 cia1_peek(WORD addr)
{
    return ciacore_peek(machine_context.cia1, addr);
}

static void cia_set_int_clk(cia_context_t *cia_context, int value, CLOCK clk)
{
    interrupt_set_irq(maincpu_int_status, cia_context->int_num, value, clk);
}

static void cia_restore_int(cia_context_t *cia_context, int value)
{
    interrupt_restore_irq(maincpu_int_status, cia_context->int_num, value);
}

/*************************************************************************
 * I/O
 */

/* Mask for the extended keyboard rows.  */
static BYTE extended_keyboard_rows_mask;

void cia1_set_extended_keyboard_rows_mask(BYTE value)
{
    extended_keyboard_rows_mask = value;
}

static inline void pulse_ciapc(cia_context_t *cia_context, CLOCK rclk)
{

}

static void pre_store(void)
{
    vicii_handle_pending_alarms_external(maincpu_num_write_cycles());
}

static void pre_read(void)
{
    vicii_handle_pending_alarms_external(0);
}

static void pre_peek(void)
{
    vicii_handle_pending_alarms_external(0);
}

static void do_reset_cia(cia_context_t *cia_context)
{

}

static void store_ciapa(cia_context_t *cia_context, CLOCK rclk, BYTE b)
{

}

static void undump_ciapa(cia_context_t *cia_context, CLOCK rclk, BYTE b)
{

}

static void store_ciapb(cia_context_t *cia_context, CLOCK rclk, BYTE byte)
{
    /* Falling edge triggers light pen.  */
    if ((byte ^ 0x10) & cia_context->old_pb & 0x10)
        vicii_trigger_light_pen(rclk);
}

static void undump_ciapb(cia_context_t *cia_context, CLOCK rclk, BYTE byte)
{

}

static BYTE read_ciapa(cia_context_t *cia_context)
{
    BYTE byte;
    BYTE val = 0xff;
    BYTE msk = cia_context->old_pb & ~joystick_value[1];
    BYTE m;
    int i;

    for (m = 0x1, i = 0; i < 8; m <<= 1, i++)
        if (!(msk & m))
            val &= ~rev_keyarr[i];

    byte = (val & (cia_context->c_cia[CIA_PRA]
           | ~(cia_context->c_cia[CIA_DDRA]))) & ~joystick_value[2];

    return byte;
}

static BYTE read_ciapb(cia_context_t *cia_context)
{
    BYTE byte;
    BYTE val = 0xff;
    BYTE msk = cia_context->old_pa & ~joystick_value[2];
    BYTE m;
    int i;

    for (m = 0x1, i = 0; i < 8; m <<= 1, i++)
        if (!(msk & m))
            val &= ~keyarr[i];

    for (m = 0x1, i = 8; i < 11; m <<= 1, i++)
        if (!(extended_keyboard_rows_mask & m))
            val &= ~keyarr[i];

    byte = (val & (cia_context->c_cia[CIA_PRB]
           | ~(cia_context->c_cia[CIA_DDRB]))) & ~joystick_value[1];

    return byte;
}

static void read_ciaicr(cia_context_t *cia_context)
{
    drivecpu_execute_all(maincpu_clk);
}

static void read_sdr(cia_context_t *cia_context)
{
    drivecpu_execute_all(maincpu_clk);
}

static void store_sdr(cia_context_t *cia_context, BYTE byte)
{
    c128fastiec_fast_cpu_write((BYTE)byte);
#ifdef HAVE_RS232
    if (rsuser_enabled) {
        rsuser_tx_byte((BYTE)byte);
    }
#endif
}

void cia1_init(cia_context_t *cia_context)
{
    ciacore_init(machine_context.cia1, maincpu_alarm_context,
                 maincpu_int_status, maincpu_clk_guard);
}

void cia1_setup_context(machine_context_t *machine_context)
{
    cia_context_t *cia;

    machine_context->cia1 = lib_malloc(sizeof(cia_context_t));
    cia = machine_context->cia1;

    cia->prv = NULL;
    cia->context = NULL;

    cia->rmw_flag = &maincpu_rmw_flag;
    cia->clk_ptr = &maincpu_clk;

    cia->todticks = 100000;

    ciacore_setup_context(cia);

    cia->debugFlag = 0;
    cia->irq_line = IK_IRQ;
    cia->myname = lib_msprintf("CIA1");

    cia->undump_ciapa = undump_ciapa;
    cia->undump_ciapb = undump_ciapb;
    cia->store_ciapa = store_ciapa;
    cia->store_ciapb = store_ciapb;
    cia->store_sdr = store_sdr;
    cia->read_ciapa = read_ciapa;
    cia->read_ciapb = read_ciapb;
    cia->read_ciaicr = read_ciaicr;
    cia->read_sdr = read_sdr;
    cia->cia_set_int_clk = cia_set_int_clk;
    cia->cia_restore_int = cia_restore_int;
    cia->do_reset_cia = do_reset_cia;
    cia->pulse_ciapc = pulse_ciapc;
    cia->pre_store = pre_store;
    cia->pre_read = pre_read;
    cia->pre_peek = pre_peek;
}

