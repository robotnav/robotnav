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

#ifndef ROBOT_H
#define ROBOT_H

const int DATA_READY = 1;
enum {LEFT, RIGHT};

class Robot
{
	protected:
	//Robot dimentions 
	float mTrack;
	float mEncoderScaleFactor;

	//Motion variables
	float mDisplacementRight;
	float mDisplacementLeft;
	float mAngle;

	//Timing variables
	float mPeriod;
	int mCounter;
	int mMeanTimeUsec;
	int mMaxTimeUsec;
	int mMinTimeUsec;
	int mStartTimeSec;
	virtual void checkTimming();

	public:
		Robot(float period, float track, float encoderScaleFactor, char *motorInfo = 0, char *sensorInfo = 0);
		virtual ~Robot();
		virtual int readSensors() = 0;
		virtual void setActuators(char *motorSpeed) = 0;
		inline float getEncoders(char side){return(side == LEFT) ? mDisplacementLeft : mDisplacementRight;};
		inline float getTrack(){return mTrack;};
		inline float getAngle(){return mAngle;};
		inline float getPeriod(){return mPeriod;};
};

#endif
