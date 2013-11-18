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
const char INC_SPEED_COUNTS_SECOND = 5; // [count/sec]/10 - the kernel multiplies speed by 10
const float PERIOD = 0.1; //[sec]

// Available system status
enum {QUIT_PROGRAM, KEYBOARD_CONTROL};

int main()
{
	char motor_aux_info[] = {LEFT_MOTOR_PORT, RIGHT_MOTOR_PORT};
	Robot *p_robot = new Ev3(PERIOD, TRACK, ENCODER_SCALE_FACTOR, motor_aux_info); //Odomtery only
	//Robot *p_robot = new Xg1300lGyro(PERIOD, TRACK, ENCODER_SCALE_FACTOR, motor_aux_info, (char *)&XG1300L_GYRO_PORT); //Gyro Enhanced
	InputKeys *p_keyboard = new Keyboard;
	Odometry odometry(p_robot); 
	
	//Create and initialize speed variables
	char right_speed = 0;
	char left_speed = 0;

	//Define and initialize the system status
	int status = KEYBOARD_CONTROL;

	//Enter control loop
	while(status != QUIT_PROGRAM)
	{
		//Read sensors
		if(p_robot->readSensors() != DATA_READY)
			status = QUIT_PROGRAM;

		//Compute robot position
		odometry.updatePosition();

		//Define some interesting instructions
		switch(p_keyboard->getKey())
		{
		case MOVE_FORWARD:
			right_speed += INC_SPEED_COUNTS_SECOND;
			left_speed += INC_SPEED_COUNTS_SECOND;
			break;
		case MOVE_BACKWARDS:
			right_speed -= INC_SPEED_COUNTS_SECOND;
			left_speed -= INC_SPEED_COUNTS_SECOND;
			break;
		case TURN_LEFT:
			right_speed += INC_SPEED_COUNTS_SECOND;
			left_speed -= INC_SPEED_COUNTS_SECOND;
			break;
		case TURN_RIGHT:
			right_speed -= INC_SPEED_COUNTS_SECOND;
			left_speed += INC_SPEED_COUNTS_SECOND;
			break;
		case EXIT:
			status = QUIT_PROGRAM;
		case RESET:
			odometry.reset();
		case STOP_ROBOT: 
			right_speed = 0;
			left_speed = 0;
			break;
		}
		
		//Execute the instructions
		motor_aux_info[LEFT] = left_speed;
		motor_aux_info[RIGHT] = right_speed;
		p_robot->setActuators(motor_aux_info);
	}

	//Free used memory before exiting
	delete p_keyboard;
	delete p_robot;
	return 1;
}
