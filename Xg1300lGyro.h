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

#ifndef XGL1300_GYRO_H
#define XGL1300_GYRO_H

#include "Ev3.h"
#include "lms2012.h"

const float GYRO_NORMAL = -1.0;
const float GYRO_UPSIDEDOWN = 1.0;

class Xg1300lGyro : public Ev3
{
	IIC  *pIic;
	int mXglDevFile;
	int mGyroPort;
	float mRateSign;
	public:
		Xg1300lGyro(float period, float track, float encoderScaleFactor, char *pMotorInfo, char *sensorInfo, float rateSign = GYRO_NORMAL);
		virtual ~Xg1300lGyro();
		virtual int readSensors();
};

#endif
