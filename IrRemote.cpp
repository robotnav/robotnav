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
#include <sys/ioctl.h>
#include "IrRemote.h"

using namespace std;

//More information about modes can be found at http://python-ev3.org/types.html
const char IR_REMOTE_MODE = 2; // Use IR in remote control mode 
const char IR_REMOTE_CONTROL_CHANNEL = 1; //Channel number as shown in IR control unit
const char IR_SENSOR_TYPE = 33; 
IrRemote::IrRemote(int devPort)
{
	mDevPort = devPort - 1;
	if((mIrRemoteFile = open(UART_DEVICE_NAME, O_RDWR | O_SYNC)) == -1)
	{
		cout << "Failed to open IrRemote device\n";
		return; 
	}
	//Map kernel device to be used at the user space level
	pIrRemote  =  (UART*)mmap(0, sizeof(UART), PROT_READ | PROT_WRITE, MAP_FILE | MAP_SHARED, mIrRemoteFile, 0);
	if (pIrRemote == MAP_FAILED)
	{
		cout << "Failed to map IrRemote device\n";
		return;
	}
	//Set IR as remote control
	DEVCON dev_con; // Configuration parameters
	dev_con.Mode[mDevPort] = IR_REMOTE_MODE;
	dev_con.Connection[mDevPort] = CONN_INPUT_UART;
	dev_con.Type[mDevPort] = IR_SENSOR_TYPE; //This instruction has no effect in the code
	ioctl(mIrRemoteFile, UART_SET_CONN, &dev_con);
	cout << "IrRemote ready\n";
}

IrRemote::~IrRemote()
{
	cout << "Closing device\n";
	close(mIrRemoteFile);
}

char IrRemote::getKey()
{
	char static s_last_key = NO_INPUT;
	char key = pIrRemote->Raw[mDevPort][pIrRemote->Actual[mDevPort]][IR_REMOTE_CONTROL_CHANNEL-1];
	if (key == s_last_key)
			return NO_INPUT;
	cout << "IR_REMOTE " << (int) key << endl;
	s_last_key = key;
	switch(key)
	{
		case UP_DOWN_LEFT:
			return EXIT;
		case FRONT_BUTTON:
			return ENABLE_CONTROL;
		case UP_RIGHT:
		case DOWN_LEFT:
			return TURN_RIGHT;
		case UP_LEFT:
		case DOWN_RIGHT:
			return TURN_LEFT;
		case UP_LEFT_RIGHT:
			return MOVE_FORWARD;
		case DOWN_LEFT_RIGHT:
			return MOVE_BACKWARDS;
	}
	s_last_key = NO_INPUT;
	return NO_INPUT;
}

