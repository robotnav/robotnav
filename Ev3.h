/*
 *
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

#ifndef EV3_H
#define EV3_H

#include "Robot.h"
#include "lms2012.h"

const char MOTOR_PORT_OFFSET = 'A'; //First motor port label in EV3 Brick
const char SENSOR_PORT_OFFSET = 1; //First sensor port label in EV3 Brick

class Ev3 : public Robot
{
	int mEncoderDevFile;
	int mLeftMotorPort;
	int mRightMotorPort;
	int mMarkerMotorPort;
	int mLeftEncoderPort;
	int mRightEncoderPort;
	int mMarkerEncoderPort;
	
	bool mUsesMarkerMotor;
	int mOrigMarker;
	int mCountMarker;
	protected:
		MOTORDATA *pMotorData;
		int mMotorDevFile;
		float mLastLeftEncoderCount ;
		float mLastRightEncoderCount;
	public:
		Ev3(float period, float track, float encoderScaleFactor, char *pMotorInfo);
		virtual ~Ev3();
		virtual int readSensors();
		virtual void setExtraMotor(char motorInfo);
		virtual void setActuators(char *pMotorSpeed);
		virtual void setActuators(float speed, float rate);
		void setMarker(int markerTarget);
};

#endif
