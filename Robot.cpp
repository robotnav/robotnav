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
#include <sys/time.h>
#include "Robot.h"

using namespace std;

Robot::Robot(float period, float track, float encoderScaleFactor, char *motorInfo, char *sensorInfo)
{
	//This constructor does not do anything with the motor and sensor information.
	//That information is handled directly by each robot sub-class. This extra arguments are needed 
	//to allow class virtualization.
	
	//Initialize timing variables
	timeval time;
	gettimeofday(&time, 0);
	mStartTimeSec = time.tv_sec;
	mMeanTimeUsec = 0;
	mMaxTimeUsec = 0;
	mMinTimeUsec = 1000000; //Use an imposibly large value
	mCounter = 1;
	checkTimming(); //Initialize static time variables

	// Robot parameters
	mPeriod = period;
	mTrack = track;
	mEncoderScaleFactor = encoderScaleFactor;
	
	cout << "Robot ready!\n";
}

Robot::~Robot()
{
	cout << "Robot closed!\n";
}

void Robot::checkTimming()
{
	static int last_time_usec;
	timeval time;
	int time_diff_usec;
	gettimeofday(&time, NULL);
	time_diff_usec = time.tv_usec - last_time_usec;
	last_time_usec = time.tv_usec;
	if(time_diff_usec < 0)
		time_diff_usec += 1000000; //+1 sec

	// Compute timming statistics
	if(time_diff_usec < mMinTimeUsec)
		mMinTimeUsec = time_diff_usec;
	else if(time_diff_usec > mMaxTimeUsec)
		mMaxTimeUsec = time_diff_usec;
	mMeanTimeUsec = mMeanTimeUsec*(mCounter - 1) / mCounter + time_diff_usec / mCounter;
	
	// Display info
	cout << "TIME[" << mCounter << "] " << time.tv_sec - mStartTimeSec << " " << time_diff_usec << " " << mMeanTimeUsec << " " << mMinTimeUsec << " " << mMaxTimeUsec << endl;
	mCounter++;

};




