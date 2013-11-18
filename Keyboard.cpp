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

#include <unistd.h>
#include <sys/time.h>
#include "Keyboard.h"

Keyboard::Keyboard()
{
	// Sets the terminal to get real-time keyboard values
	tcgetattr(0, &mOrgTerm);
	mNewTerm=mOrgTerm;
	mNewTerm.c_lflag &= ~ICANON;
	mNewTerm.c_lflag &= ~ECHO;
	mNewTerm.c_cc[VMIN] = 1;
	mNewTerm.c_cc[VTIME] = 0;
	tcsetattr(0, TCSANOW, &mNewTerm);
}

Keyboard::~Keyboard()
{
	tcsetattr(0, TCSANOW, &mOrgTerm);
}

char Keyboard::getKey()
{

	// Return keyboard value if any
	struct timeval aux_time = {0, 0};
	fd_set rfds;
	char key = NO_INPUT;
	FD_SET(0, &rfds);
	if(select(1, &rfds, 0, 0, &aux_time))
		read(0, &key, 1);
	switch(key)
	{
		case 'A'://Forward arrow key
			return MOVE_FORWARD;
		case 'B'://Backwards arrow key
			return MOVE_BACKWARDS;
		case 'C'://Left arrow key
			return TURN_LEFT;
		case 'D'://Right arrow key
			return TURN_RIGHT;
		case 'x':
		case 'X':
			return EXIT;
		case 'r':
		case 'R':
			return RESET;
		case 32: //Space bar
			return STOP_ROBOT;
	}
	return key;
}
