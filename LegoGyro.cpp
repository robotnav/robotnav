/*
 * Robot Navigation Program
 * www.robotnav.com
 *
 * (C) Copyright 2013 - 2014 Navigation Solutions, LLC
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
#include <cstring>
#include <fcntl.h>
#include <sys/mman.h>
#include <sys/ioctl.h>
#include "LegoGyro.h"
#include "MathFunctions.h"

using namespace std;

//More information about modes can be found at http://python-ev3.org/types.html
const char GYRO_ANG_MODE = 0; // Use Gyro in Angle mode
const char GYRO_SENSOR_TYPE = 32;

LegoGyro::LegoGyro(float period, float track, float encoderScaleFactor, char *pMotorInfo, char *sensorInfo) : Ev3(period, track, encoderScaleFactor, pMotorInfo)
{
	mGyroPort = sensorInfo[0] - SENSOR_PORT_OFFSET;
	//Open the device uart device file
	if((mLegoGyroDevFile = open(UART_DEVICE_NAME, O_RDWR | O_SYNC)) == -1)
	{
		cout << "Failed to open device\n";
		return; 
	}
	//Map kernel device to be used at the user space level
	pUart  =  (UART*)mmap(0, sizeof(UART), PROT_READ | PROT_WRITE, MAP_FILE | MAP_SHARED, mLegoGyroDevFile, 0);
	if(pUart == MAP_FAILED)
	{
		cout << "Failed to map device\n";
		return;
	}

	DEVCON dev_con; // Configuration parameters
	dev_con.Mode[mGyroPort] = GYRO_ANG_MODE;
	dev_con.Connection[mGyroPort] = CONN_INPUT_UART;
	dev_con.Type[mGyroPort] = GYRO_SENSOR_TYPE; //This instruction has no effect in the code
	ioctl(mLegoGyroDevFile, UART_SET_CONN, &dev_con);
	
	strcpy(mName,"LEG");

	//Read sensors a first time in order to initialize some of the states
	readSensors();
	cout << "LegoGyro Robot ready!\n";
}

LegoGyro::~LegoGyro()
{
	close(mLegoGyroDevFile);
	cout << "LegoGyro Robot closed!\n";
}

int LegoGyro::readSensors()
{
	//Get Encoder information
	Ev3::readSensors();
	//Get angle from XGL gyro
	static float s_last_angle = 0;
	short  new_angle;
	memcpy(&new_angle, pUart->Raw[mGyroPort][pUart->Actual[mGyroPort]], 2);
	mRotation = -(new_angle - s_last_angle); //Invert gyro output
	mRotation = math_functions::deg2rad(mRotation);
	//Add here a gyro correction algorithm
	// mRotation  = your correction ...
	
	s_last_angle = new_angle;

	cout << "LEGO GYRO: " << math_functions::rad2deg(mRotation) << " " << new_angle << endl;
	return DATA_READY;
}

