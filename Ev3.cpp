/*
 * Robot Navigation Program
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
#include <cmath>
#include <fcntl.h>
#include <sys/mman.h>
#include <sys/ioctl.h>
#include <unistd.h>
#include "Ev3.h"
#include "MathFunctions.h"

using namespace std;

Ev3::Ev3(float period, float track, float encoderScaleFactor, char *motorInfo, char *sensorInfo) : Robot(period, track, encoderScaleFactor)
{
	//This type of robot does not use any sensor, besides encoders, which are defined by the motor information
	//The motor port is the one shown in the Ev3 label
	mLeftEncoderPort = 	motorInfo[LEFT] - 1;
	mRightEncoderPort = 	motorInfo[RIGHT] - 1;
	mLeftMotorPort = pow(2, mLeftEncoderPort);
	mRightMotorPort = pow(2, mRightEncoderPort);
	cout << "MOTOR_PORT_LEF" << mLeftMotorPort << " " << mRightMotorPort << " " << mLeftEncoderPort << " " << mRightEncoderPort << endl;

	//Open the device file asscoiated to the motor controlers
	if((mMotorDevFile = open(PWM_DEVICE_NAME, O_WRONLY)) == -1)
	{
		cout << "Failed to open motor device\n";
		return; //Failed to open device
	}

	//Open the device file asscoiated to the motor encoders 
	if((mEncoderDevFile = open(MOTOR_DEVICE_NAME, O_RDWR | O_SYNC)) == -1)
	{
		cout << "Failed to open encoder device\n";
		return; //Failed to open device
	}

	pMotorData = (MOTORDATA*)mmap(0, sizeof(MOTORDATA)*vmOUTPUTS, PROT_READ | PROT_WRITE, MAP_FILE | MAP_SHARED, mEncoderDevFile, 0);
	if (pMotorData == MAP_FAILED)
	{
		cout << "Mapping Encoders failed\n";
		return;
	}

	char motor_command[3];
	// All motor operations use the first command byte to indicate the type of operation
	// and the second one to indicate the motor(s) port(s)
	motor_command[0] = opOUTPUT_SPEED;
	motor_command[1] = mLeftMotorPort | mRightMotorPort; 
	motor_command[2] = 0;
	write(mMotorDevFile,motor_command, 3);
	// Start the motor
	motor_command[0] = opOUTPUT_START;
	write(mMotorDevFile, motor_command, 2);
	
	//Read sensors a first time in order to initialize some of the states
	readSensors();
	cout << "Ev3 Robot ready!\n";
}

Ev3::~Ev3()
{
	close(mEncoderDevFile);
	close(mMotorDevFile);
	cout << "Ev3 Robot closed!\n";
}

int Ev3::readSensors()
{
	// Get robot displacement from encoders
	static float s_last_count_left = 0;
	static float s_last_count_right = 0;
	int new_count_left = pMotorData[mLeftEncoderPort].TachoSensor;
	int new_count_right = pMotorData[mRightEncoderPort].TachoSensor;

	//Compute wheel linear displacements
	mDisplacementLeft = (new_count_left - s_last_count_left) * mEncoderScaleFactor;
	mDisplacementRight = (new_count_right - s_last_count_right) * mEncoderScaleFactor;
	mAngle = (mDisplacementLeft - mDisplacementRight) / mTrack;

	//Store last encoder state
	s_last_count_left = new_count_left;
	s_last_count_right = new_count_right;
	
	cout << "EV3: " << " " << mDisplacementLeft << " " << mDisplacementRight << " " << math_functions::rad2deg(mAngle) << endl;
	return DATA_READY;
}

void Ev3::setActuators(char *motorSpeed)
{
	char left_speed = motorSpeed[LEFT];
	char right_speed = motorSpeed[RIGHT];
	char motor_command[3];
	motor_command[0] = opOUTPUT_SPEED;
	motor_command[1] = mLeftMotorPort; 
	motor_command[2] = left_speed;
	write(mMotorDevFile, motor_command, 3);

	motor_command[0] = opOUTPUT_SPEED;
	motor_command[1] = mRightMotorPort; 
	motor_command[2] = right_speed;
	write(mMotorDevFile, motor_command, 3);
	
	cout << "SPEED: " << (int)left_speed << "," << (int)right_speed << endl;
	checkTimming();
}

void Ev3::checkTimming()
{
	//This function is not fully implemented, but does not affect positioning accuracy
	usleep(mPeriod*1000000);
	Robot::checkTimming();
}
