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

#ifndef ROBOT_H
#define ROBOT_H

#include <sys/time.h>

const int DATA_READY = 1;
const int NAME_SIZE = 10;
const float SEC2USEC = 1e6;

class Robot
{
	private:
	timeval mStartTimeSec;
	protected:
	char mName[NAME_SIZE];
	//Robot dimentions 
	float mTrack;
	float mEncoderScaleFactor;

	//Motion variables
	float mDisplacementRight;
	float mDisplacementLeft;
	float mDisplacement;
	float mRotation;
	int mEncoderCountSecLimit;

	//Timing variables
	float mPeriod;
	int mCounter;
	bool mWaitForPeriod;
	void checkTimming();

	void speedRate2Counts(float speed, float rate, int *pCountSec);

	public:
		Robot(float period, float track, float encoderScaleFactor, char *pMotorInfo = 0, char *sensorInfo = 0);
		virtual ~Robot();
		virtual int readSensors() = 0;
		virtual void setActuators(char *pMotorSpeed) = 0;
		virtual void setActuators(float speed, float rate) = 0;
		virtual void setEncoderLimit(int pCountSecLimit);
		inline float getDisplacement() const {return mDisplacement;};
		inline float getAngle() const {return mRotation;};
		inline float getPeriod() const {return mPeriod;};
		inline float getCounter() const {return mCounter;};
		inline char *getName() {return mName;};
};

#endif
