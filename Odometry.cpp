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

#include <iostream>
#include <cmath>
#include "Odometry.h"
#include "MathFunctions.h"

using namespace std;

Odometry::Odometry(Robot *pSensors)
{
	mpSensors = pSensors;
	mPeriod = mpSensors->getPeriod();
	reset();
	cout << "Odometry is ready! T=" << mPeriod << " [s]" << endl;
}

void Odometry::reset()
{
	mX = 0.0;
	mY = 0.0;
	mHeading = 0.0;
}

void Odometry::getDisplacements()
{
	//Compute linear and angular displacements
	mDisplacement = mpSensors->getDisplacement();
	mRotation = mpSensors->getAngle();
}

void Odometry::updatePosition()
{
	getDisplacements();
	//Update positions
	mX += mDisplacement * cos(mHeading + mRotation/2.0);
	mY += mDisplacement * sin(mHeading + mRotation/2.0);

	//Update heading
	mHeading += mRotation;
	//mHeading = unwrapAngle(mHeading);
	mHeading = math_functions::unwrap(mHeading);

	//Convert from displacement to displacement/time
	mSpeed = mDisplacement / mPeriod;
	mRate = mRotation / mPeriod;
	cout << "POSITION: " << mX << " " << mY << " " << math_functions::rad2deg(mHeading) << " " << mSpeed << " " << math_functions::rad2deg(mRate) << endl;
}

