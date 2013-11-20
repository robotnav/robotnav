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


#ifndef CONTROL
#define CONTROL 

#include "Odometry.h"

const float FACE_NEXT_WAYPOINT = 100.0;

class Control
{
	protected:
	Odometry *mpOdometry;
	float** mpWaypoints;
	int mWaypointLength;
	int mpWaypointIndex;
	float mSpeed;
	float mRate;
	int mStatus;
	float mPeriod;

	enum {STANBY_STS, STARTING_STS, TURNING_STS, FACING_ZERO_STS, MOVING_STS};
	enum {MOVING_TO_WAYPOINT, FOUND_NEW_WAYPOINT, COMPLETED_WAYPOINT};

	void cmpTargetDirDist(float &rRelativeDistance, float &rRelativeAngle);
	virtual int freeHeading();
	bool faceTarget(float targetAngle = FACE_NEXT_WAYPOINT);
	void createWaypoints();
public :
	void enable();
	void reset();
	void getTargetSpeedRate(float &rSpeed, float &rRate);
	Control(Odometry *pOdometry);
	virtual ~Control();
};
#endif	
