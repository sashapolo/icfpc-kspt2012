/*
 * Solver.cpp
 *
 *  Created on: Oct 15, 2012
 *      Author: alexander
 */
#include "Solver.h"

string Solver::solve(Field* const pField) const {
	FieldMember *goal = findNewGoal(pField);
	AStar astar(pField, pField->getRobot(), goal);
	Point finish = pField->getLift()->getCoordinate();
	Field *pNewField = NULL;
	string result;

	string t = astar.solve(&pNewField);
	while (pNewField->getRobot()->getCoordinate() != finish) {
		if (t.empty()) {
			if (pNewField->lambdaCacheEmpty()) {
				//delete pNewField;
				return result + "A";
			} else {
			// TODO возможно, к этим лямбдам будет необходимо вернуться позднее
				deleteUnreachableLambda(pNewField, goal);
				goal = findNewGoal(pNewField);
			}
		}
		result += t;
		AStar astar(pNewField, pNewField->getRobot(), findNewGoal(pNewField));
		//delete pNewField;
		t = astar.solve(&pNewField);
	}
	result += t;
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


void Solver::deleteUnreachableLambda(Field* pField, FieldMember* const pLambda) const {
	list<FieldMember*>::iterator it = pField->getLambdaCacheIt();
	for (; it != pField->getLambdaCacheEnd(); it++) {
		if (*(*it) == *pLambda) {
			pField->deleteLambdaFromCache(it);
			return;
		}
	}
}
