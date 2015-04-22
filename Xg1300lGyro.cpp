/*
 * Robot Navigation Program
 * www.robotnav.com
 *
 * (C) Copyright 2010 - 2014 Lauro Ojeda
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
#include <unistd.h>
#include <sys/mman.h>
#include <sys/ioctl.h>
#include "Xg1300lGyro.h"
#include "MathFunctions.h"

using namespace std;

Xg1300lGyro::Xg1300lGyro(float period, float track, float encoderScaleFactor, char *pMotorInfo, char *sensorInfo) : Ev3(period, track, encoderScaleFactor, pMotorInfo)
{
	IICDAT iic_dat;
	mGyroPort = sensorInfo[0] - SENSOR_PORT_OFFSET;
	//Open XGL device file
	if((mXglDevFile = open(IIC_DEVICE_NAME, O_RDWR | O_SYNC)) == -1)
	{
		cout << "Failed to open device\n";
		return; 
	}
	pIic = (IIC*)mmap(0, sizeof(IIC), PROT_READ | PROT_WRITE, MAP_FILE | MAP_SHARED, mXglDevFile, 0);
	if (pIic == MAP_FAILED)
 {
		cout << "Mapping XGL failed\n";
		return;
	}
	//Setup IIC to read 2 packets
	iic_dat.Port = mGyroPort;
	iic_dat.Time = 0;
	iic_dat.Repeat = 0;
	iic_dat.RdLng = 2;
	iic_dat.WrLng = 2;
	// Set the device I2C address
	iic_dat.WrData[0] = 0x01;
	// Specify the register that will be read (0x42 = angle)
	iic_dat.WrData[1] = 0x42;
	// Setup I2C comunication
	ioctl(mXglDevFile, IIC_SETUP, &iic_dat);

	//Read sensors a first time in order to initialize some of the states
	readSensors();
	strcpy(mName,"XGL");
	cout << "Xg1300lGyro Robot ready!\n";
}

Xg1300lGyro::~Xg1300lGyro()
{
	close(mXglDevFile);
	cout << "Xg1300lGyro Robot closed!\n";
}

int Xg1300lGyro::readSensors()
{
	//Get Encoder information
	Ev3::readSensors();
	//Get angle from XGL gyro
	static float s_last_angle = 0;
	short new_angle = pIic->Raw[mGyroPort][pIic->Actual[mGyroPort]][0]*256 + pIic->Raw[mGyroPort][pIic->Actual[mGyroPort]][1];
	mRotation = - (new_angle - s_last_angle)/100.0; //XGL angle must be inverted
	mRotation = math_functions::deg2rad(mRotation);
	mRotation = math_functions::unwrap(mRotation);
	s_last_angle = new_angle;
	
	cout << "XG1300L: " << math_functions::rad2deg(mRotation) << endl;
	return DATA_READY;
}

