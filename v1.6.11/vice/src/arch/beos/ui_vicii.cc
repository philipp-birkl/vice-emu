/*
 * ui_vicii.cc - VIC-II settings
 *
 * Written by
 *  Andreas Matthies <andreas.matthies@gmx.net>
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
 
#include <Box.h>
#include <CheckBox.h>
#include <RadioButton.h>
#include <string.h>
#include <Window.h>

extern "C" { 
#include "resources.h"
#include "ui.h"
#include "ui_vicii.h"
#include "vsync.h"
}

static char *palette_name[] = {
	"default",
	"c64s",
	"ccs64",
	"frodo",
	"godot",
	"pc64",
	NULL
};


class ViciiWindow : public BWindow {
	public:
		ViciiWindow();
		~ViciiWindow();
		virtual void MessageReceived(BMessage *msg);
};	


static ViciiWindow *viciiwindow = NULL;


ViciiWindow::ViciiWindow() 
	: BWindow(BRect(50,50,210,300),"VIC-II settings",
		B_TITLED_WINDOW, 
		B_NOT_ZOOMABLE | B_NOT_RESIZABLE) 
{
	BMessage *msg;
	BRadioButton *radiobutton;
	BCheckBox *checkbox;
	BRect r;
	BBox *box;
	BView *background;
	int palette_num;
	int res_val;
	char *res_palette;
	
	r = Bounds();
	background = new BView(r, NULL,  B_FOLLOW_NONE, B_WILL_DRAW);
	background->SetViewColor(220,220,220,0);
	AddChild(background);
	
	r.InsetBy(10,10);
	r.bottom = r.top + 150;
	box = new BBox(r);
	box->SetLabel("Palette");
	background->AddChild(box);
		
	/* palette */
    resources_get_value("PaletteFile", (resource_value_t *) &res_palette);
	for (palette_num=0; palette_name[palette_num]; palette_num++) {
		msg = new BMessage(MESSAGE_VICII_PALETTE);
		msg->AddInt32("palettenum", palette_num);
		radiobutton = new BRadioButton(
			BRect(10,20+palette_num*20, 120, 35+palette_num*20),
			NULL,
			palette_name[palette_num],
			msg);
		radiobutton->SetValue(!strcmp(palette_name[palette_num], res_palette));
		box->AddChild(radiobutton);
	}
			
	/* sprite collisions */
	r = Bounds();
	r.InsetBy(10,10);
	r.top += 160;
	box = new BBox(r);
	box->SetLabel("Sprite Collision");
	background->AddChild(box);
		
	checkbox = new BCheckBox(
		BRect(10, 20, 120, 35),
		NULL,
		"Sprite-Sprite",
		new BMessage(MESSAGE_VICII_SSCOLL));
	resources_get_value("CheckSsColl", (resource_value_t *) &res_val);
	checkbox->SetValue(res_val);
	box->AddChild(checkbox);	
	
	checkbox = new BCheckBox(
		BRect(10, 40, 120, 55),
		NULL,
		"Sprite-Background",
		new BMessage(MESSAGE_VICII_SBCOLL));
	resources_get_value("CheckSbColl", (resource_value_t *) &res_val);
	checkbox->SetValue(res_val);
	box->AddChild(checkbox);	
	
	Show();
}

ViciiWindow::~ViciiWindow() 
{
	viciiwindow = NULL;	
}

void ViciiWindow::MessageReceived(BMessage *msg) {
	int32 palette_num;	
	resource_value_t dummy;
	
	switch (msg->what) {
		case MESSAGE_VICII_PALETTE:
			msg->FindInt32("palettenum", &palette_num);	
            if (resources_set_value("PaletteFile",
            	(resource_value_t) palette_name[palette_num]) < 0)
            	ui_error("Cannot load palette file"); 
			break;
		case MESSAGE_VICII_SSCOLL:
			resources_toggle("CheckSsColl", &dummy);
			break;
		case MESSAGE_VICII_SBCOLL:
			resources_toggle("CheckSbColl", &dummy);
			break;
		default:
			BWindow::MessageReceived(msg);
	}
}

void ui_vicii() {
	
	if (viciiwindow != NULL)
		return;

	viciiwindow = new ViciiWindow;

	suspend_speed_eval();
	while (viciiwindow); /* wait until window closed */
}

