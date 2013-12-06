/*
 * Robot Navigation Program
 * www.robotnav.com
 *
 * (C) Copyright 2013 Navigation Solutions, LLC
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 * 
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 * 
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#include <iostream>
#include <fcntl.h>
#include  <sys/mman.h>
#include "Buttons.h"

using namespace std;
Buttons::Buttons()
{
	if((mButtonsFile = open(UI_DEVICE_NAME, O_RDWR | O_SYNC)) == -1)
	{
		cout << "Failed to open Buttons device\n";
		return; 
	}
	//Map kernel device to be used at the user space level
	pButtons  =  (UI*)mmap(0, sizeof(UI), PROT_READ | PROT_WRITE, MAP_FILE | MAP_SHARED, mButtonsFile, 0);
	if (pButtons == MAP_FAILED)
	{
		cout << "Failed to map Buttons device\n";
		return;
	}
	cout << "Buttons ready\n";
}

Buttons::~Buttons()
{
	cout << "Closing device\n";
	close(mButtonsFile);
}

char Buttons::getKey()
{
	char static s_last_key = NO_INPUT;
	for(int key = 0; key < BUTTONS; key++)
	{
		if(pButtons->Pressed[key]) 
		{
			//Allow only one key return per push
			if (key == s_last_key)
				return NO_INPUT;
			cout << "BUTTONS " << key << endl;
			s_last_key = key;
			switch(key)
			{
				case BACK:
					return EXIT;
				case CENTER:
					return ENABLE_CONTROL;
				case RIGHT:
					return TURN_LEFT;
				case UP:
					return MOVE_BACKWARDS;
				case DOWN:
					return MOVE_FORWARD;
				case LEFT:
					return TURN_RIGHT;
			}
			break;
		}
	}
	s_last_key = NO_INPUT;
	return NO_INPUT;
}

