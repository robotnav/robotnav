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

#ifndef NAMESPACE_MATH_FUNCTIONS_H 
#define NAMESPACE_MATH_FUNCTIONS_H

const float PI = 3.14159265;
namespace math_functions
{
	float deg2rad(float degrees);
	float rad2deg(float radians);
	float unwrap(float radians);
}

#endif
