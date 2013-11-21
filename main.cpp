 /*
	* Robot Navigation
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
	*
	*/

#include "Ev3.h"
#include "Xg1300lGyro.h"
#include "Odometry.h"
#include "Control.h"
#include "InputKeys.h"
#include "Keyboard.h"
#include "MathFunctions.h"

//Left and right motor ports, as shown in EV3 brick labels
const char LEFT_MOTOR_PORT = 'D';
const char RIGHT_MOTOR_PORT = 'A';

//XGL1300L port, as shown in EV3 brick labels
const char XG1300L_GYRO_PORT = 1;

//Platform measurements
const float TRACK = 110.0; // [mm]
const int COUNTS_REVOLUTION = 360; // [count/rev]
const float WHEEL_DIAMETER = 43.0; // [mm]
const float ENCODER_SCALE_FACTOR = PI * WHEEL_DIAMETER / COUNTS_REVOLUTION; // [mm/count]

// Runtime constant
const float INC_SPEED_MM_SECOND = 10.0; //[mm/sec]
const float INC_RATE_RAD_SECOND = math_functions::deg2rad(10.0); //[rad/sec]
const float PERIOD = 0.1; //[sec]

int main()
{
	char motor_aux_info[] = {LEFT_MOTOR_PORT, RIGHT_MOTOR_PORT};
	//Robot *p_robot = new Ev3(PERIOD, TRACK, ENCODER_SCALE_FACTOR, motor_aux_info); //Odomtery only
	Robot *p_robot = new Xg1300lGyro(PERIOD, TRACK, ENCODER_SCALE_FACTOR, motor_aux_info, (char *)&XG1300L_GYRO_PORT); //Gyro Enhanced
	Odometry odometry(p_robot); 
	Control control(&odometry);
	InputKeys *p_keyboard = new Keyboard;
	
	//Create and initialize speed variables
	float speed = 0;
	float rate = 0;
	bool quit_program = false;

	//Enter main loop
	while(!quit_program)
	{
		//Read sensors
		p_robot->readSensors();

		//Compute position
		odometry.updatePosition();

		//Control instructions
		//User defined instructions using keyboard
		switch(p_keyboard->getKey())
		{
		case MOVE_FORWARD:
			speed += INC_SPEED_MM_SECOND;
			break;
		case MOVE_BACKWARDS:
			speed -= INC_SPEED_MM_SECOND;
			break;
		case TURN_LEFT:
			rate += INC_RATE_RAD_SECOND;
			break;
		case TURN_RIGHT:
			rate -= INC_RATE_RAD_SECOND;
			break;
		case ENABLE_CONTROL:
			control.enable();
			break;
		case EXIT:
			quit_program = true;
		case RESET:
			odometry.reset();
			control.reset();
		case STOP_ROBOT: 
			speed = 0;
			rate = 0;
			break;
		}
		//High level control instructions
		control.getTargetSpeedRate(speed, rate);
		
		//Execute the instructions
		p_robot->setActuators(speed, rate);
	}

	//Free used memory before exiting
	delete p_keyboard;
	delete p_robot;
	return 1;
}
