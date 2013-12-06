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

#ifndef BUTTONS_H
#define BUTTONS_H

#include "lms2012.h"
#include "InputKeys.h"

class Buttons: public InputKeys
{
	int mButtonsFile;
	UI *pButtons;
	enum {UP, CENTER, DOWN, RIGHT, LEFT, BACK};
	public:
		Buttons();
		~Buttons();
		char getKey();
};

#endif
