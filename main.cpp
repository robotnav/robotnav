 /*
	* Robot Navigation
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
const char LEFT_MOTOR_PORT = 1;
const char RIGHT_MOTOR_PORT = 4;

//XGL1300L port, as shown in Ev3 brick labels
const char XG1300L_GYRO_PORT = 'A';

//Platform measurments
const float TRACK = 118.0; // [mm]
const int COUNTS_PER_REVOLUTION = 360; // [count/rev]
const float WHEEL_DIAMETER = 43.0; // [mm]
const float ENCOD_SCALE_FACTOR = PI * WHEEL_DIAMETER / COUNTS_PER_REVOLUTION; // [mm/count]

// Runtime constants
const float INC_SPEED_COUNTS_SECOND = 5.0; // [%]
const float PERIOD = .1; //[sec]

// Available system status
enum {QUIT_PROGRAM, KEYBOARD_CONTROL};

int main()
{
	char motor_aux_info[] = {LEFT_MOTOR_PORT, RIGHT_MOTOR_PORT};
	Robot *p_robot = new Xg1300lGyro(PERIOD, TRACK, ENCOD_SCALE_FACTOR, motor_aux_info, (char *)&XG1300L_GYRO_PORT);
	InputKeys *p_keyboard = new Keyboard;
	Odometry odometry(p_robot); 
	
	//Create and initialize speed variables
	int right_speed = 0;
	int left_speed = 0;

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

		//Dtermine some interesting instructions
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
		
		//Excute the instructions
		motor_aux_info[LEFT] = (char)left_speed;
		motor_aux_info[RIGHT] = (char)right_speed;
		p_robot->setActuators(motor_aux_info);
	}

	//Free used memory before exiting
	delete p_keyboard;
	delete p_robot;
	return 1;
}
