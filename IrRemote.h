/*
 * Robot Navigation Program
 * www.robotnav.com
 *
 * (C) Copyright 2010 - 2016 Lauro Ojeda
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

#ifndef IR_REMOTE_H
#define IR_REMOTE_H

#include "lms2012.h"
#include "InputKeys.h"

class IrRemote: public InputKeys
{
	int mIrRemoteFile;
	UART *pIrRemote;
	int mDevPort;
	enum {UP_LEFT=1, DOWN_LEFT, UP_RIGHT, DOWN_RIGHT, UP_LEFT_RIGHT, UP_LEFT_DOWN_RIGHT, UP_RIGHT_DOWN_LEFT, DOWN_LEFT_RIGHT, FRONT_BUTTON, UP_DOWN_LEFT, UP_DOWN_RIGHT};
	public:
		IrRemote(int devPort);
		~IrRemote();
		char getKey();
};

#endif
