/*
 * vicemenu.cc - Implementation of the BeVICE's menubar
 *
 * Written by
 *  Andreas Matthies <andreas.matthies@arcormail.de>
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

#include <Menu.h>
#include <MenuBar.h>
#include <MenuItem.h>

#include "constants.h"
#include "machine.h"
#include "vicemenu.h"


BMenuBar *menu_create(int machine_class) {

	BMenuBar *menubar;
	BMenu *menu, *submenu;
	BMenuItem *item;
	
	menubar = new BMenuBar (BRect(0,0,10,10),"Menubar");
	
	/* create the FILE menu */
	menu = new BMenu("File");
	menu->AddItem(new BMenuItem("Autostart",
		new BMessage(MENU_AUTOSTART), 'A'));
	menu->AddItem(submenu = new BMenu("Attach Disk"));
	submenu->AddItem(new BMenuItem("Drive 8", 
		new BMessage(MENU_ATTACH_DISK8), '8'));
	submenu->AddItem(new BMenuItem("Drive 9", 
		new BMessage(MENU_ATTACH_DISK9), '9'));
	submenu->AddItem(new BMenuItem("Drive 10", 
		new BMessage(MENU_ATTACH_DISK10)));
	submenu->AddItem(new BMenuItem("Drive 11", 
		new BMessage(MENU_ATTACH_DISK11)));
	menu->AddItem(submenu = new BMenu("Detach Disk"));
	submenu->AddItem(new BMenuItem("Drive 8", 
		new BMessage(MENU_DETACH_DISK8)));
	submenu->AddItem(new BMenuItem("Drive 9", 
		new BMessage(MENU_DETACH_DISK9)));
	submenu->AddItem(new BMenuItem("Drive 10", 
		new BMessage(MENU_DETACH_DISK10)));
	submenu->AddItem(new BMenuItem("Drive 11", 
		new BMessage(MENU_DETACH_DISK11)));
	menu->AddSeparatorItem();
	menu->AddItem(new BMenuItem("Attach Tape", 
		new BMessage(MENU_ATTACH_TAPE), 'T'));
	menu->AddItem(new BMenuItem("Detach Tape", 
		new BMessage(MENU_DETACH_TAPE)));
	menu->AddItem(submenu = new BMenu("Datasette Control"));
	submenu->AddItem(new BMenuItem("Start", 
		new BMessage(MENU_DATASETTE_START)));
	submenu->AddItem(new BMenuItem("Stop", 
		new BMessage(MENU_DATASETTE_STOP)));
	submenu->AddItem(new BMenuItem("Forward", 
		new BMessage(MENU_DATASETTE_FORWARD)));
	submenu->AddItem(new BMenuItem("Rewind", 
		new BMessage(MENU_DATASETTE_REWIND)));
	submenu->AddItem(new BMenuItem("Record", 
		new BMessage(MENU_DATASETTE_RECORD)));
	submenu->AddItem(new BMenuItem("Reset", 
		new BMessage(MENU_DATASETTE_RESET)));
	submenu->AddItem(new BMenuItem("Reset Counter", 
		new BMessage(MENU_DATASETTE_COUNTER)));
	menu->AddSeparatorItem();
	menu->AddItem(item = new BMenuItem("Soft Reset", 
		new BMessage(MENU_RESET_SOFT), 'R'));
	menu->AddItem(new BMenuItem("Hard Reset", 
		new BMessage(MENU_RESET_HARD), 'R', B_CONTROL_KEY));
	menubar->AddItem(menu);
	menu->AddSeparatorItem();
	menu->AddItem(new BMenuItem("Quit", 
		new BMessage(MENU_EXIT_REQUESTED), 'Q'));
	
	/* create the OPTIONS menu */
	menu = new BMenu("Options");

	/* refresh rate */
	menu->AddItem(submenu = new BMenu("Refresh Rate"));
	submenu->SetRadioMode(true);
	submenu->AddItem(new BMenuItem("Auto", 
		new BMessage(MENU_REFRESH_RATE_AUTO)));
	submenu->AddItem(new BMenuItem("1/1", 
		new BMessage(MENU_REFRESH_RATE_1)));
	submenu->AddItem(new BMenuItem("1/2", 
		new BMessage(MENU_REFRESH_RATE_2)));
	submenu->AddItem(new BMenuItem("1/3", 
		new BMessage(MENU_REFRESH_RATE_3)));
	submenu->AddItem(new BMenuItem("1/4", 
		new BMessage(MENU_REFRESH_RATE_4)));
	submenu->AddItem(new BMenuItem("1/5", 
		new BMessage(MENU_REFRESH_RATE_5)));
	submenu->AddItem(new BMenuItem("1/6", 
		new BMessage(MENU_REFRESH_RATE_6)));
	submenu->AddItem(new BMenuItem("1/7", 
		new BMessage(MENU_REFRESH_RATE_7)));
	submenu->AddItem(new BMenuItem("1/8", 
		new BMessage(MENU_REFRESH_RATE_8)));
	submenu->AddItem(new BMenuItem("1/9", 
		new BMessage(MENU_REFRESH_RATE_9)));
	submenu->AddItem(new BMenuItem("1/10", 
		new BMessage(MENU_REFRESH_RATE_10)));

	/* maximum speed */
	menu->AddItem(submenu = new BMenu("Maximum Speed"));
	submenu->SetRadioMode(true);
	submenu->AddItem(new BMenuItem("No Limit", 
		new BMessage(MENU_MAXIMUM_SPEED_NO_LIMIT)));
	submenu->AddItem(new BMenuItem("200%", 
		new BMessage(MENU_MAXIMUM_SPEED_200)));
	submenu->AddItem(new BMenuItem("100%", 
		new BMessage(MENU_MAXIMUM_SPEED_100)));
	submenu->AddItem(new BMenuItem("50%", 
		new BMessage(MENU_MAXIMUM_SPEED_50)));
	submenu->AddItem(new BMenuItem("20%", 
		new BMessage(MENU_MAXIMUM_SPEED_20)));
	submenu->AddItem(new BMenuItem("10%", 
		new BMessage(MENU_MAXIMUM_SPEED_10)));

	menu->AddItem(new BMenuItem("Warp Mode", 
		new BMessage(MENU_TOGGLE_WARP_MODE),'W'));
	menu->AddSeparatorItem();

	if (machine_class == VICE_MACHINE_C64) {
		menu->AddItem(new BMenuItem("Double Size", 
			new BMessage(MENU_TOGGLE_DOUBLESIZE),'D'));
		menu->AddItem(new BMenuItem("Double Scan", 
			new BMessage(MENU_TOGGLE_DOUBLESCAN)));
	}
		
	/* sound options */
	menu->AddItem(new BMenuItem("Sound", 
		new BMessage(MENU_TOGGLE_SOUND)));
	/* C64 */
	if (machine_class == VICE_MACHINE_C64) {
#ifdef HAVE_RESID
		menu->AddItem(new BMenuItem("Use reSID", 
			new BMessage(MENU_TOGGLE_SOUND_RESID)));
#endif
		menu->AddItem(submenu = new BMenu("SID type"));
		submenu->SetRadioMode(true);
		submenu->AddItem(new BMenuItem("6581 (old)", 
			new BMessage(MENU_SIDTYPE_6581)));
		submenu->AddItem(new BMenuItem("8580 (new)", 
			new BMessage(MENU_SIDTYPE_8580)));
	}
	
	menu->AddItem(new BMenuItem("True Drive Emulation", 
		new BMessage(MENU_TOGGLE_DRIVE_TRUE_EMULATION)));
	menu->AddItem(new BMenuItem("Virtual Devices", 
		new BMessage(MENU_TOGGLE_VIRTUAL_DEVICES)));
	menubar->AddItem(menu);
	
	/* create the SETTINGS menu */
	menu = new BMenu("Settings");
	menu->AddItem(new BMenuItem("Drive Settings", 
		new BMessage(MENU_DRIVE_SETTINGS)));
	menu->AddItem(new BMenuItem("Device Settings", 
		new BMessage(MENU_DEVICE_SETTINGS)));
	menu->AddItem(new BMenuItem("Sound Settings", 
		new BMessage(MENU_SOUND_SETTINGS)));
	menu->AddItem(new BMenuItem("Joystick Settings", 
		new BMessage(MENU_JOYSTICK_SETTINGS)));
	menu->AddItem(new BMenuItem("Datasette Settings", 
		new BMessage(MENU_DATASETTE_SETTINGS)));
	menu->AddSeparatorItem();
	menu->AddItem(new BMenuItem("Load Settings", 
		new BMessage(MENU_SETTINGS_LOAD)));
	menu->AddItem(new BMenuItem("Save Settings", 
		new BMessage(MENU_SETTINGS_SAVE)));
	menu->AddItem(new BMenuItem("Default Settings", 
		new BMessage(MENU_SETTINGS_DEFAULT)));
	menubar->AddItem(menu);
	
	/* create the HELP menu */
	menu = new BMenu("Help");
	menu->AddItem(new BMenuItem("About BeVICE...", 
		new BMessage(MENU_ABOUT)));
	menu->AddItem(new BMenuItem("Contributors", 
		new BMessage(MENU_CONTRIBUTORS)));
	menu->AddItem(new BMenuItem("License", 
		new BMessage(MENU_LICENSE)));
	menu->AddItem(new BMenuItem("No Warranty", 
		new BMessage(MENU_WARRANTY)));
	menu->AddItem(new BMenuItem("Commandline Options", 
		new BMessage(MENU_CMDLINE)));
	menubar->AddItem(menu);
	
	return menubar;
}
