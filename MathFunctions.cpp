/*
 * Robot Navigation Program
 * www.robotnav.com
 *
 * (C) Copyright 2010 - 2016 Lauro Ojeda
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

#include "MathFunctions.h"

namespace math_functions
{
	float deg2rad(float degrees)
	{
		return degrees * PI / 180.0;
	}
	
	float rad2deg(float radians)
	{
		return radians * 180.0 / PI;
	}

	float unwrap(float radians)
	{
		while(radians < -PI)
			radians += 2.0 * PI;
		while(radians >= PI)
			radians -= 2.0 * PI;
		return radians; 
	}
}
