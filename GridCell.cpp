/*
 * Robot Navigation Program
 * www.robotnav.com
 *
 * (C) Copyright 2015 - 2016 Lauro Ojeda
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
#include "GridCell.h"

using namespace std;
GridCell::GridCell()
{
	// Start in the center of the cells
	mXcell = 1;
	mYcell = 1;
	mIsPlaying = false;
}

char GridCell::getKey()
{
	char key = getKeyRaw();
	if(key == BACK)
		return EXIT;
	if(key == NO_INPUT)
		return NO_INPUT;

	if(mIsPlaying)
	{
		switch(key)
		{
			case CENTER:
				key = mYcell * 3 + mXcell  + '0'; 
				mXcell = 1;
				mYcell = 1;
				return key;
			case LEFT:
				mXcell = 0;
				break;
			case RIGHT:
				mXcell = 2;
				break;
			case UP:
				mYcell = 2;
				break;
			case DOWN:
				mYcell = 0;
				break;
		}
	}
	else
	{
		switch(key)
		{
			case CENTER:
				mIsPlaying = true;
				return ENABLE_CONTROL;
			case UP:
				return MARKER_UP;
			case DOWN:
				return MARKER_DOWN;
		}
	}
	return NO_INPUT;
}

