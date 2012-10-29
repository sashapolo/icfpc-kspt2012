/*
 * Solver.cpp
 *
 *  Created on: Oct 15, 2012
 *      Author: alexander
 */
#include "Solver.h"

string Solver::solve(Field* pField) const {
	FieldMember *goal = findNewGoal(pField);
	ManhattanHeuristic *mH = new ManhattanHeuristic(goal->getCoordinate());
	AStar astar(pField, pField->getRobot(), mH);
	Point finish = pField->getLift()->getCoordinate();
	Field *pNewField = NULL;
	string result;

	string t = astar.solve(&pNewField);
	while (pNewField->getRobot()->getCoordinate() != finish) {
		if (t.empty()) {
			if (pNewField->lambdaCacheEmpty()) {
				delete mH;
				return result + "A";
			} else {
			// TODO возможно, к этим лямбдам будет необходимо вернуться позднее
				deleteUnreachableLambda(pNewField, goal);
				goal = findNewGoal(pNewField);
			}
		}
		result += t;
		goal = findNewGoal(pNewField);
		mH->setGoal(goal->getCoordinate());
		AStar astar(pNewField, pNewField->getRobot(), mH);
		t = astar.solve(&pNewField);
	}
	result += t;
	delete mH;
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


FieldMember* Solver::findNewGoal(const Field* pField) const {
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


void Solver::deleteUnreachableLambda(Field* pField, const FieldMember* pLambda) const {
	list<FieldMember*>::iterator it = pField->getLambdaCacheIt();
	for (; it != pField->getLambdaCacheEnd(); it++) {
		if (*(*it) == *pLambda) {
			pField->deleteLambdaFromCache(it);
			return;
		}
	}
}
