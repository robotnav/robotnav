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
#include <cmath>
#include <sys/time.h>
#include "Robot.h"
#include "MathFunctions.h"

const int MAX_ENCODER_CONT_SEC = 1000000; //The default limit used for the LEGO EV3 should be very large (infinite), this is only needed for high count encoders
using namespace std;

Robot::Robot(float period, float track, float encoderScaleFactor, char *pMotorInfo, char *sensorInfo)
{
	//This constructor does not do anything with the motor and sensor information.
	//That information is handled directly by each robot sub-class. This extra arguments are needed 
	//to allow class virtualization.
	
	// Robot parameters
	mPeriod = period;
	mTrack = track;
	mEncoderScaleFactor = encoderScaleFactor;
	mEncoderCountSecLimit = MAX_ENCODER_CONT_SEC;
	
	//Initialize timing variables
	mWaitForPeriod = true;
	mCounter = 1;
	gettimeofday(&mStartTimeSec, 0);
	checkTimming(); //Initialize static current_time variables

	cout << "Robot ready!\n";
}

Robot::~Robot()
{
	cout << "Robot closed!\n";
}

void Robot::checkTimming()
{
	static int s_mean_time_usec = 0;
	static int s_max_time_usec = 0;
	static int s_min_time_usec = SEC2USEC; //Use an imposibly large value
	static timeval s_last_time = mStartTimeSec;
	static int s_next_period_time_set_usec = (int)((float)(mPeriod * SEC2USEC));

	// Wait untill set period is met
	timeval current_time;
	int time_diff_usec;
	do{
		gettimeofday(&current_time, 0);
		time_diff_usec = SEC2USEC*(current_time.tv_sec - s_last_time.tv_sec) + (current_time.tv_usec - s_last_time.tv_usec);
	}while(time_diff_usec < s_next_period_time_set_usec && mWaitForPeriod);

	s_last_time = current_time;

	// Compute timming statistics
	if(time_diff_usec < s_min_time_usec)
		s_min_time_usec = time_diff_usec;
	else if(time_diff_usec > s_max_time_usec)
		s_max_time_usec = time_diff_usec;
	s_mean_time_usec = (int)((float)(s_mean_time_usec*(mCounter - 1) + time_diff_usec) / mCounter);
	
	// Display info
	cout << "TIME[" << mCounter << "/" << current_time.tv_sec - mStartTimeSec.tv_sec << "] T: " << time_diff_usec << " aveT: " << s_mean_time_usec << " minT: " << s_min_time_usec << " maxT: " << s_max_time_usec << endl;
	mCounter++;
}

void Robot::speedRate2Counts(float speed, float rate, int *pCountSec)
{
	//Compute left and right encoder counts per second
	float left_speed = (speed - rate * mTrack / 2.0) / mEncoderScaleFactor;
	float right_speed = (speed + rate * mTrack / 2.0) / mEncoderScaleFactor;
	
	//Round values to the clossest integer
	pCountSec[LEFT] = (left_speed > 0) ? (left_speed + .5) : (left_speed - 0.5);
	pCountSec[RIGHT] = (right_speed > 0) ? (right_speed + .5) : (right_speed - 0.5);

	//If there is some speed, even very tiny, we want to keep it. This prevents the robot from getting stuck.
	if(!pCountSec[LEFT] && left_speed) pCountSec[LEFT] = (left_speed > 0) ? 1 : -1;
	if(!pCountSec[RIGHT] && right_speed) pCountSec[RIGHT] = (right_speed > 0) ? 1 : -1;

	// Verify that the motor speed does not exceed the limitations of the encoder counter reader
	// this limit is set by the microcontroller that reads the encoder signals
	if((fabs(pCountSec[LEFT]) > mEncoderCountSecLimit || fabs(pCountSec[RIGHT]) > mEncoderCountSecLimit))
	{
		float speed_reduction = (fabs(pCountSec[LEFT]) > fabs(pCountSec[RIGHT])) ? fabs(pCountSec[LEFT]) : fabs(pCountSec[RIGHT]);
		speed_reduction /= mEncoderCountSecLimit;
		cout << "Max encoder speed exceeded by: " << speed_reduction << " Bef: "<<pCountSec[LEFT] << " " << pCountSec[RIGHT];
		pCountSec[LEFT] /= speed_reduction; 
		pCountSec[RIGHT] /= speed_reduction; 
		cout << " LIMIT: "<< mEncoderCountSecLimit ;
		cout << " Aft: "<<pCountSec[LEFT] << " " << pCountSec[RIGHT] << endl;
	}

	cout << "ROBOT: " << speed << " " << math_functions::rad2deg(rate) << " " << left_speed << "/" << (int)pCountSec[LEFT] << " " << right_speed << "/" << (int)pCountSec[RIGHT] << endl;

}
	
void Robot::setEncoderLimit(int pCountSecLimit)
{ // This is only useful is using a different encoder that has significantly larger counts per revolution
	// in this cases, the EV3 block may not be able to read the encoders
	mEncoderCountSecLimit = pCountSecLimit;
}

