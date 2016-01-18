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
#ifndef TIC_TAC_TOE 
#define TIC_TAC_TOE 

#include "Control.h"
using namespace std;

const int CELLS = 9;
class TicTacToe: public Control
{
	protected:
		int mGameStatus;
		int mBrush;
		int mRobotMove;
		int mHumanMove;
		int mUsedCells;
		int mCellStatus[CELLS];

	public:
		TicTacToe(Odometry *pPos);
		virtual ~TicTacToe();
		virtual void getTarget(float &rSpeed, float &rRate, int &rBrush);
		bool drawGrid();
		bool drawCircle();
		void createGrid();
		void createCross();
		void createCircle();
		void robotMove();
		void humanMove(int move);
		void printStatus();
};
#endif
