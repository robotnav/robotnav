/*
 * Robot Navigation Program
 * www.robotnav.com
 *
 * (C) Copyright 2013 - 2016 Lauro Ojeda
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
#include <cmath>
#include <fcntl.h>
#include <sys/mman.h>
#include <sys/ioctl.h>
#include <unistd.h>
#include "Ev3.h"
#include "MathFunctions.h"

using namespace std;
const float MARKER_SPEED_P_CONTROL = 0.2;

Ev3::Ev3(float period, float track, float encoderScaleFactor, char *pMotorInfo) : Robot(period, track, encoderScaleFactor)
{
	//This type of robot does not use any sensor, besides encoders, which are defined by the motor information
	//The motor port is the one shown in the Ev3 label
	mLeftEncoderPort = 	pMotorInfo[LEFT] - MOTOR_PORT_OFFSET;
	mRightEncoderPort = 	pMotorInfo[RIGHT] - MOTOR_PORT_OFFSET;
	mLeftMotorPort = pow(2, mLeftEncoderPort);
	mRightMotorPort = pow(2, mRightEncoderPort);
		
	//Open the device file asscoiated to the motor controlers
	if((mMotorDevFile = open(PWM_DEVICE_NAME, O_WRONLY)) == -1)
	{
		cout << "Failed to open motor device\n";
		return;
	}

	//Open the device file asscoiated to the motor encoders 
	if((mEncoderDevFile = open(MOTOR_DEVICE_NAME, O_RDWR | O_SYNC)) == -1)
	{
		cout << "Failed to open encoder device\n";
		return;
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
	strcpy(mName,"EV3");
	readSensors();
	cout << "Ev3 Robot ready!\n";
}

Ev3::~Ev3()
{
	char motor_aux[] = {0, 0};
	setActuators(motor_aux);
	close(mEncoderDevFile);
	close(mMotorDevFile);
	cout << "Ev3 Robot closed!\n";
}

void Ev3::setExtraMotor(char motorInfo)
{
	mMarkerEncoderPort = 	motorInfo - MOTOR_PORT_OFFSET;
	mMarkerMotorPort = pow(2, mMarkerEncoderPort);
	char motor_command[3];
	// All motor operations use the first command byte to indicate the type of operation
	// and the second one to indicate the motor(s) port(s)
	motor_command[0] = opOUTPUT_SPEED;
	motor_command[1] = mMarkerMotorPort;
	motor_command[2] = 0;
	write(mMotorDevFile,motor_command, 3);
	// Start the motor
	motor_command[0] = opOUTPUT_START;
	write(mMotorDevFile, motor_command, 2);
	
	//Determine marker original angle
	mOrigMarker = pMotorData[mMarkerEncoderPort].TachoSensor;
}

int Ev3::readSensors()
{
	// Get robot displacement from encoders
	int new_count_left = pMotorData[mLeftEncoderPort].TachoSensor;
	int new_count_right = pMotorData[mRightEncoderPort].TachoSensor;

	//Compute wheel linear displacements
	mDisplacementLeft = (new_count_left - mLastLeftEncoderCount) * mEncoderScaleFactor;
	mDisplacementRight = (new_count_right - mLastRightEncoderCount) * mEncoderScaleFactor;
	
	//Compute robot average displacement and rotation
	mDisplacement = (mDisplacementLeft + mDisplacementRight) / 2.0;
	mRotation = (mDisplacementRight - mDisplacementLeft) / mTrack;

	//Store last encoder state
	mLastLeftEncoderCount = new_count_left;
	mLastRightEncoderCount = new_count_right;
	
	cout << "EV3 ACTUAL SPEED: " << " " << mDisplacementLeft/mEncoderScaleFactor/mPeriod << " " << mDisplacementRight/mEncoderScaleFactor/mPeriod << " " << mDisplacement << " " << math_functions::rad2deg(mRotation) << endl;
	return DATA_READY;
}

void Ev3::setActuators(char *pMotorSpeed)
{
	char motor_command[3];
	//Left motor command
	motor_command[0] = opOUTPUT_SPEED;
	motor_command[1] = mLeftMotorPort; 
	motor_command[2] = pMotorSpeed[LEFT];
	write(mMotorDevFile, motor_command, 3);

	//Right motor command
	motor_command[0] = opOUTPUT_SPEED;
	motor_command[1] = mRightMotorPort; 
	motor_command[2] = pMotorSpeed[RIGHT];
	write(mMotorDevFile, motor_command, 3);
	
	cout << "EV3 SET SPEED: " << (int)pMotorSpeed[LEFT] << " " << (int)pMotorSpeed[RIGHT] << endl;
	checkTimming();
}

void Ev3::setActuators(float speed, float rate)
{
	int counts_sec_aux[RIGHT + 1];
	speedRate2Counts(speed, rate, counts_sec_aux);
	char counts_sec[RIGHT + 1];
	//The following is needed, because the EV3 wants the speed in tenths of count per second
	counts_sec[LEFT] = (char)(counts_sec_aux[LEFT] / 10);
	counts_sec[RIGHT] = (char)(counts_sec_aux[RIGHT] / 10);

	// Make sure that if not zero, it sets some speed
	if(!counts_sec[LEFT] && counts_sec_aux[LEFT]) counts_sec[LEFT] = (counts_sec_aux[LEFT] > 0) ? 1 : -1;
	if(!counts_sec[RIGHT] && counts_sec_aux[RIGHT]) counts_sec[RIGHT] = (counts_sec_aux[RIGHT] > 0) ? 1 : -1;

	//Send motor commans
	setActuators(counts_sec);
	cout << "EV3 SPEED RATE: " << speed << " " << math_functions::rad2deg(rate) << endl;
}

void Ev3::setMarker(int markerTarget)
{
	//Implements a simple P-Control for the extra motor
	char motor_command[3];
	int speed;

	mCountMarker = pMotorData[mMarkerEncoderPort].TachoSensor;
	speed = (mOrigMarker - mCountMarker + markerTarget) * MARKER_SPEED_P_CONTROL;
	speed = (speed>100) ? 100 :((speed<-100) ? -100 : speed);
	//Marker motor command
	motor_command[0] = opOUTPUT_SPEED;
	motor_command[1] = mMarkerMotorPort; 
	motor_command[2] = speed;
	write(mMotorDevFile,motor_command,3);
	cout << "MARKER CMD: " << markerTarget << " " << mCountMarker << " " << mOrigMarker << " " << speed << endl;
}

