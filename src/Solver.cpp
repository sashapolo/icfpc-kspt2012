/*
 * Solver.cpp
 *
 *  Created on: Oct 15, 2012
 *      Author: alexander
 */
#include "Solver.h"

string Solver::solve(Field* pField) const {
	AStar astar(pField, pField->getRobot());
	return astar.solve();
}

string Solver::convertResultToString(const Path& coordinates) const {
	string result;
	for (int i = coordinates.getSize() - 2; i >= 0; i--) {
		if (coordinates.getCell(i + 1).x - coordinates.getCell(i).x == 1) {
			result += 'L';
		} else if (coordinates.getCell(i + 1).x - coordinates.getCell(i).x == -1) {
			result += 'R';
		} else if (coordinates.getCell(i + 1).y - coordinates.getCell(i).y == 1) {
			result += 'U';
		} else if (coordinates.getCell(i + 1).y - coordinates.getCell(i).y == -1) {
			result += 'D';
		} else {
			result += 'W';
		}
	}
	return result;
}


