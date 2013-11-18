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

#ifndef EV3_H
#define EV3_H
#include "Robot.h"
#include "lms2012.h"

class Ev3 : public Robot
{
	MOTORDATA *pMotorData;
	int mMotorDevFile;
	int mEncoderDevFile;
	int mLeftMotorPort;
	int mRightMotorPort;
	int mLeftEncoderPort;
	int mRightEncoderPort;
	virtual void checkTimming();
	public:
		Ev3(float period, float track, float encoderScaleFactor, char *motorInfo, char *sensorInfo = 0);
		virtual ~Ev3();
		virtual int readSensors();
		virtual void setActuators(char *motorSpeed);
};

#endif
