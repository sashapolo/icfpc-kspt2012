/*
 * Solver.cpp
 *
 *  Created on: Oct 15, 2012
 *      Author: alexander
 */
#include "Solver.h"

string Solver::solve(Field* const pField) const {
	AStar astar(pField, pField->getRobot(), findNewGoal(pField));
	Field *pNewField = NULL;
	string result;
	string t = astar.solve(&pNewField);
	while (pNewField->getRobot() != pNewField->getLift()) {
		if (t.empty()) {
			delete pNewField;
			return result + "A";
		}
		result += t;
		AStar astar(pNewField, pNewField->getRobot(), findNewGoal(pNewField));
		//delete pNewField;
		t = astar.solve(&pNewField);
	}
	delete pNewField;
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

FieldMember* const Solver::findNewGoal(Field* const pField) const {
	if (pField->lambdaCacheEmpty()) {
		return pField->getLift();
	}
	list<FieldMember*>::const_iterator it = pField->getLambdaCacheIt();
	FieldMember *result = *it;
	Point start = pField->getRobot()->getCoordinate();
	int min = pField->getDistance((*it)->getCoordinate(), start);
	for (++it; it != pField->getLambdaCacheEnd(); it++) {
		int tmp = pField->getDistance((*it)->getCoordinate(), start);
		if (min > tmp) {
			min = tmp;
			result = *it;
		}
	}
	return result;
}
