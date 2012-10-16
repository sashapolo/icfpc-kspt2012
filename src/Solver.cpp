/*
 * Solver.cpp
 *
 *  Created on: Oct 15, 2012
 *      Author: alexander
 */

#include "Solver.h"

vector<Path> Solver::solve(const Field* pField) const {
//		AStar astar(pField, pField->getRobot(), pField->getLift());
	Dijkstra dj(pField, pField->getRobot());
	list<FieldMember*>::const_iterator it = pField->getLambdaCacheIt();
	for (; it != pField->getLambdaCacheEnd(); it++) {
		dj.addGoal(*it);
	}
	vector<Path> result = dj.findAllPaths();
	//return convertResultToString(result[0]);
	return result;
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


