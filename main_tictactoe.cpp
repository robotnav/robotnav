/*
 * Robot Navigation
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
 *
 */

#include "Odometry.h"
#include "Ev3.h"
#include "Xg1300lGyro.h"
#include "LegoGyro.h"
#include "TicTacToe.h"
#include "Keyboard.h"
#include "GridCell.h"
#include "MathFunctions.h"

//Left and right motor ports, as shown in EV3 brick labels
const char LEFT_MOTOR_PORT = 'D';
const char RIGHT_MOTOR_PORT = 'B';
const char MARKER_MOTOR_PORT = 'C';

//XGL1300L port, as shown in EV3 brick labels
const char XG1300L_GYRO_PORT = 1;
const char LEGO_GYRO_PORT = 4;
const char IR_PORT = 4;
const char MOTOR_PORTS[] =  {LEFT_MOTOR_PORT, RIGHT_MOTOR_PORT};

//Platform measurements
const float TRACK = 110.0; //[mm]
const int COUNTS_REVOLUTION = 360; //[count/rev]
const float WHEEL_DIAMETER = 43.0; //[mm]
const float ENCODER_SCALE_FACTOR = PI * WHEEL_DIAMETER / COUNTS_REVOLUTION; //[mm/count]

//Runtime constants
const float INC_SPEED_MM_SECOND = 50.0; //[mm/sec]
const float INC_RATE_RAD_SECOND = math_functions::deg2rad(10.0); //[rad/sec]
const float PERIOD = 1/50.0; //[sec]

//Available system status
const int PENCIL_INC = 100;

// Position estimation mode
#define XGL_GYRO
// User input mode
#define INPUT_KEYBOARD

int main()
{
#if defined INPUT_KEYBOARD
	Keyboard user_input;
#elif defined INPUT_BUTTONS
	GridCell user_input;
#else
	#error
#endif
#if defined NO_GYRO
	Ev3 robot(PERIOD, TRACK, ENCODER_SCALE_FACTOR, (char *)MOTOR_PORTS); //Odometry only
#elif defined LEGO_GYRO
	LegoGyro robot(PERIOD, TRACK, ENCODER_SCALE_FACTOR, (char *)MOTOR_PORTS, (char *)&LEGO_GYRO_PORT); //Gyro Enhanced
#elif defined XGL_GYRO
	Xg1300lGyro robot(PERIOD, TRACK, ENCODER_SCALE_FACTOR, (char *)MOTOR_PORTS, (char *)&XG1300L_GYRO_PORT); //Gyro Enhanced
#else
#error
#endif
	Odometry odometry(&robot); 
	TicTacToe control(&odometry);

	//Create and initialize speed variables
	float speed = 0;
	float rate = 0;
	int marker = PENCIL_INC; 
	bool quit_program = false;
	char key;

	robot.setExtraMotor(MARKER_MOTOR_PORT);

	//Enter main loop
	while(!quit_program)
	{
		//Read sensors
		robot.readSensors();

		//Compute position
		odometry.updatePosition();

		//Control instructions
		switch(key = user_input.getKey())
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
			case MARKER_UP:
				marker += PENCIL_INC;
				break;
			case MARKER_DOWN:
				marker -= PENCIL_INC;
				break;
			case '0': case '1': case '2':
			case '3': case '4': case '5':
			case '6': case '7': case '8':
				control.humanMove(key - '0');
				break;
			case EXIT:
				quit_program = true;
			case RESET:
				odometry.reset();
			case STOP_ROBOT: 
				speed = 0;
				rate = 0;
				control.disable();
				break;
		}
		//High level control instructions
		control.getTarget(speed, rate, marker);
		//Execute the instructions
		robot.setActuators(speed, rate);
		robot.setMarker(marker);
	}
	return 1;
}

