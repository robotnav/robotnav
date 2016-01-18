/*
 * Robot Navigation
 * www.robotnav.com
 *
 * (C) Copyright 2015 - 2016 Lauro Ojeda
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
#include <iostream>
#include <fstream>
#include <cstring>
#include <cstdlib>
#include <cmath>

#include "TicTacToe.h"

using namespace std;
const int MAX_BRUSH_DEPTH = 100;
const int GRID_SF = 100;
const int GRID_SIZE = 8;
// Coordinates X,Y of the lines
const float SHIFT = 100;
const float GRID[GRID_SIZE][2] = {{0,1},{3,1},{3,2},{0,2},{1,3},{1,0},{2,0},{2,3}};
enum {ROBOT_MOVE_STS = 10, HUMAN_MOVE_STS, CIRCLING_STS, GAME_OVER_STS};
enum {NO_MOVE = -1, PLAYING, ROBOT_WINS, HUMAN_WINS, NO_MOVES_LEFT, INVALID_MOVE};

const int MAX_MOVES = 4;
//Show all posible three in a row conbinations for any giving starting point
//the starting point is the actual row
const int WINING_MOVES[CELLS][MAX_MOVES * 2] = {{1,2,3,6,4,8,NO_MOVE,NO_MOVE}, //0
	{0,2,4,7,NO_MOVE,NO_MOVE,NO_MOVE,NO_MOVE},//1
	{0,1,4,6,5,8,NO_MOVE,NO_MOVE},//2
	{0,6,4,5,NO_MOVE,NO_MOVE,NO_MOVE,NO_MOVE},//3
	{0,8,1,7,2,6,3,5},//4
	{2,8,3,4,NO_MOVE,NO_MOVE,NO_MOVE,NO_MOVE},//5
	{0,3,2,4,7,8,NO_MOVE,NO_MOVE},//6
	{1,4,6,8,NO_MOVE,NO_MOVE,NO_MOVE,NO_MOVE},//7
	{0,4,2,5,6,7,NO_MOVE,NO_MOVE}}; //8

TicTacToe::TicTacToe(Odometry * pPos): Control(pPos)
{
	createGrid();
	mStatus = STANBY_STS;
	mGameStatus = PLAYING;
	mUsedCells = 0;
	mBrush = MAX_BRUSH_DEPTH;
	for(int i = 0; i < CELLS; i++)
		mCellStatus[i] = NO_MOVE;
	mRobotMove = rand() % 9;
	//mRobotMove = 1;
	mCellStatus[mRobotMove] = ROBOT_WINS;
	printStatus();
}

TicTacToe::~TicTacToe()
{
}

bool TicTacToe::drawGrid()
{
	static int s_grid_status = TURNING_STS; 
	mBrush = MAX_BRUSH_DEPTH;
	switch(s_grid_status)
	{
		case TURNING_STS:
			if(faceTarget()) 
				s_grid_status = MOVING_STS;
			break;
		case MOVING_STS:
			if(mpWaypointIndex % 2) //Draw only on odd numbers
				mBrush = 0;
			switch(freeHeading())
			{
				case FOUND_NEW_WAYPOINT:
					s_grid_status = TURNING_STS;
					break;
				case COMPLETED_WAYPOINT:
					mSpeed = 0;
					mRate = 0;
					s_grid_status = TURNING_STS;
					return true;
			}
			break;
	}
	return false;
}

bool TicTacToe::drawCircle()
{
	static int s_circle_status = TURNING_STS; 
	static int s_count_ellapsed = 0;
	mBrush = MAX_BRUSH_DEPTH;
	switch(s_circle_status)
	{
		case TURNING_STS:
			if(faceTarget()) 
				s_circle_status= MOVING_STS;
			break;
		case MOVING_STS:
			if(freeHeading() == COMPLETED_WAYPOINT)
				s_circle_status= CIRCLING_STS;
			break;
		case CIRCLING_STS:
			mBrush = 0; // Draw only during circling
			if(s_count_ellapsed > 200)
			{
				s_count_ellapsed = 0;
				s_circle_status = TURNING_STS;
				mSpeed = 0;
				mRate = 0;
				return true;
			}
			mSpeed = 20.0;
			mRate = 1.2;
			s_count_ellapsed ++;
			break;
	}
	return false;
}

void TicTacToe::printStatus()
{
	cout<< "XXX: Game Status = " << mGameStatus << endl ;
	for(int i = 2; i >= 0; i--)
	{
		cout<< "\nXXX: ";
		for(int j = 0; j < 3; j++)
			switch(mCellStatus[i*3 + j])
			{
				case ROBOT_WINS:
					cout<< "X ";
					break;
				case HUMAN_WINS:
					cout<< "0 ";
					break;
				case NO_MOVE:
					cout<< "- ";
					break;
			}
	}
	cout<< "\n";
}

void TicTacToe::getTarget(float &rSpeed,float &rRate, int& rBrush)
{
	switch(mStatus)
	{
		case STANBY_STS:
			// Do not update speed or rate
			return;
		case STARTING_STS:
			if(drawGrid())
			{
				createCross();
				mStatus = ROBOT_MOVE_STS;
			}
			break;
		case ROBOT_MOVE_STS:
			if(drawGrid())
				mStatus = STANBY_STS;
			break;
		case HUMAN_MOVE_STS:
			if(drawCircle())
			{
				robotMove();
				createCross();
				mStatus = ROBOT_MOVE_STS;
			}
			break;
		case GAME_OVER_STS:
			mSpeed = 0;
			mRate = 0;
			break;
	}
	rSpeed = mSpeed;
	rRate = mRate;
	rBrush = mBrush;
}

void TicTacToe::humanMove(int move)
{
	if(mUsedCells == CELLS) 
	{
		mGameStatus = NO_MOVES_LEFT;
		cout << "XXX: NO_MOVES_LEFT\n";
		return;
	}

	if(mCellStatus[move] != NO_MOVE)
	{
		mGameStatus = INVALID_MOVE;
		cout << "XXX: INVALID_MOVE\n";
		return;
	}

	mHumanMove = move;

	cout << "XXX Human Move = " << mHumanMove << endl;
	createCircle();
	mCellStatus[mHumanMove] = HUMAN_WINS;
	mUsedCells++;
	mStatus = HUMAN_MOVE_STS;
	//Follow previous move to increase chances of wining
	for(int i=0;i<MAX_MOVES;i++)
	{
		if(WINING_MOVES[mHumanMove][i*2]==NO_MOVE)
			break;
		if((mCellStatus[WINING_MOVES[mHumanMove][i*2]] == HUMAN_WINS) && (mCellStatus[WINING_MOVES[mHumanMove][i*2+1]] ==HUMAN_WINS))
			mGameStatus = HUMAN_WINS;
	}
	printStatus();
}

void TicTacToe::robotMove()
{
	int move = NO_MOVE;
	int number_of_hits = 0;
	int last_number_of_hits = -1; 
	int i;
	int j;
	bool is_a_win = false;

	if(mUsedCells==CELLS) 
	{
		mGameStatus = NO_MOVES_LEFT;
		printStatus();
		return;
	}
	mUsedCells++;


	//Follow previous move to increase chances of wining
	for(i=0;i<MAX_MOVES;i++)
	{
		if(WINING_MOVES[mRobotMove][i*2]==NO_MOVE)
			break;
		if((mCellStatus[WINING_MOVES[mRobotMove][i*2]]!=HUMAN_WINS) && (mCellStatus[WINING_MOVES[mRobotMove][i*2+1]]!=HUMAN_WINS))
		{
			number_of_hits = (mCellStatus[WINING_MOVES[mRobotMove][i*2]]==ROBOT_WINS) + (mCellStatus[WINING_MOVES[mRobotMove][i*2+1]]==ROBOT_WINS);
			if(number_of_hits>last_number_of_hits)
			{
				move = (mCellStatus[WINING_MOVES[mRobotMove][i*2]]==NO_MOVE) ? WINING_MOVES[mRobotMove][i*2] : WINING_MOVES[mRobotMove][i*2+1];
				last_number_of_hits = number_of_hits;
				if(last_number_of_hits)
					is_a_win=true;
			}
		}
	}
	cout << "XXX Robot 111 " << i << endl;

	//If it did not find a wining move, verify that human2 does not have one
	if(!is_a_win)
	{
		for(i=0;i<MAX_MOVES;i++)
		{
			if(WINING_MOVES[mHumanMove][i*2]==NO_MOVE)
				break;
			if(mCellStatus[WINING_MOVES[mHumanMove][i*2]]==HUMAN_WINS && mCellStatus[WINING_MOVES[mHumanMove][i*2 + 1]] == NO_MOVE)
				move = WINING_MOVES[mHumanMove][i*2 + 1];
			else if(mCellStatus[WINING_MOVES[mHumanMove][i*2 + 1]]==HUMAN_WINS && mCellStatus[WINING_MOVES[mHumanMove][i*2]] == NO_MOVE)
				move = WINING_MOVES[mHumanMove][i*2];
			else continue;
			break;
		}
		cout << "XXX Robot 222 " << i << endl;
	}	

	//If move was bloked and there is no risk of loosing, find a new row that has a wining chance
	if(move==NO_MOVE)
	{
		for(j=0;j<CELLS;j++)
		{
			if(mCellStatus[j] != NO_MOVE)
				continue;
			//Check if this move can make a win
			for(i=0;i<MAX_MOVES;i++)
			{
				if(WINING_MOVES[j][i*2] == NO_MOVE)
					break;
				if((mCellStatus[WINING_MOVES[j][i*2]]!=HUMAN_WINS) && (mCellStatus[WINING_MOVES[j][i*2+1]]!=HUMAN_WINS))
				{
					number_of_hits = (mCellStatus[WINING_MOVES[j][i*2]]==ROBOT_WINS) + (mCellStatus[WINING_MOVES[j][i*2+1]]==ROBOT_WINS);
					if(number_of_hits>last_number_of_hits)
					{
						move = j;
						last_number_of_hits = number_of_hits;
						if(last_number_of_hits>1)
							is_a_win=true;
					}
				}
			}
			if(move != NO_MOVE) break;
			move = j; //Even if it has no chance to win, store any valid move
		}
		cout << "XXX Robot 333 " << i << endl;
	}
	//Set new states
	mRobotMove = move;
	mCellStatus[mRobotMove] = ROBOT_WINS;
	cout << "XXX Robot Move = " << mRobotMove << endl;
	if(is_a_win)
		mGameStatus = ROBOT_WINS;
	printStatus();
}

void TicTacToe::createGrid()
{
	mWaypointLength = GRID_SIZE;
	mpWaypointIndex = 0;
	mpWaypoints = new float*[mWaypointLength];
	for(int i = 0; i< mWaypointLength; i++)
	{
		mpWaypoints[i] = new float[Y_AXIS + 1];
		mpWaypoints[i][X_AXIS] = GRID[i][X_AXIS] * GRID_SF + SHIFT;
		mpWaypoints[i][Y_AXIS] = GRID[i][Y_AXIS] * GRID_SF;
	}
}

void TicTacToe::createCross()
{
	int cross_center_x = mRobotMove%3;
	int cross_center_y = mRobotMove/3;
	mpWaypoints[0][X_AXIS] = cross_center_x + 0.25;
	mpWaypoints[0][Y_AXIS] = cross_center_y + 0.25;
	mpWaypoints[1][X_AXIS] = cross_center_x + 0.75;
	mpWaypoints[1][Y_AXIS] = cross_center_y + 0.75;
	mpWaypoints[2][X_AXIS] = cross_center_x + 0.25;
	mpWaypoints[2][Y_AXIS] = cross_center_y + 0.75;
	mpWaypoints[3][X_AXIS] = cross_center_x + 0.75;
	mpWaypoints[3][Y_AXIS] = cross_center_y + 0.25;
	mWaypointLength = 6;
	mpWaypointIndex = 0;
	for(int i = 0; i< mWaypointLength; i++)
	{
		mpWaypoints[i][X_AXIS] = mpWaypoints[i][X_AXIS] * GRID_SF + SHIFT;
		mpWaypoints[i][Y_AXIS] *= GRID_SF;
	}

	// Final point should be absolute 0, 0 (no SHIFT)
	mpWaypoints[4][X_AXIS] = 0;
	mpWaypoints[4][Y_AXIS] = 0;
	mpWaypoints[5][X_AXIS] = 0;
	mpWaypoints[5][Y_AXIS] = -10;
}

void TicTacToe::createCircle()
{
	int cross_center_x = mHumanMove%3;
	int cross_center_y = mHumanMove/3;
	mpWaypoints[0][X_AXIS] = (cross_center_x + 0.50) * GRID_SF + SHIFT;
	mpWaypoints[0][Y_AXIS] = (cross_center_y + 0.25) * GRID_SF;
	mWaypointLength = 1;
	mpWaypointIndex = 0;
}


