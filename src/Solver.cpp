/*
 * Solver.cpp
 *
 *  Created on: Oct 15, 2012
 *      Author: alexander
 */

#include "Solver.h"

string Solver::solve(Field* pField) {
	const FieldMember *goal = findNewGoal(pField);
	ManhattanHeuristic *mH = new ManhattanHeuristic(goal->getCoordinate());
	AStar astar(pField, pField->getRobot(), mH);
	Point finish = pField->getLift()->getCoordinate();
	Field *pNewField = NULL;
	string result;

	string t = astar.solve(&pNewField);
	while (pNewField->getRobot()->getCoordinate() != finish) {
		createSnapshot(pNewField, result, goal);
		if (t.empty()) {
			if (pNewField->lambdaCacheEmpty()) {  // если остался только лифт
				delete mH;
				return result + "A";
			} else {
			// TODO возможно, к этим лямбдам будет необходимо вернуться позднее
				markUnreachableLambda(goal);
			}
		} else {
			result += t;
		}
		goal = findNewGoal(pNewField);
		while (goal == NULL) {
			if (snapshots.empty()) {  // больше некуда откатиться
				delete mH;
				return result + "A";
			} else {  // откат к предыдущей лямбде
				SolverSnapshot *ss = loadSnapshot();
				pNewField = ss->snapshot;
				result = ss->deltaPath;
				goal = findNewGoal(pNewField);
			}
		}
		mH->setGoal(goal->getCoordinate());
		AStar astar(pNewField, pNewField->getRobot(), mH);
		t = astar.solve(&pNewField);
	}
	delete mH;
	return result + t;
}


const FieldMember* Solver::findNewGoal(const Field* pField) const {
	if (pField->lambdaCacheEmpty()) {
		return pField->getLift();
	}
	list<FieldMember*>::const_iterator it = pField->getLambdaCacheIt();
	FieldMember *result = *it;
	Point start = pField->getRobot()->getCoordinate();
	int min = pField->getDistance((*it)->getCoordinate(), start);
	for (++it; it != pField->getLambdaCacheEnd(); it++) {
		int tmp = pField->getDistance((*it)->getCoordinate(), start);
		if (min > tmp && !isMarked(*it)) {
			min = tmp;
			result = *it;
		}
	}
	return isMarked(result)? NULL : result;
}


void Solver::markUnreachableLambda(const FieldMember* pLambda) {
	markedLambdas.push_back(pLambda);
}


bool Solver::isMarked(const FieldMember* lambda) const {
	list<const FieldMember*>::const_iterator it = std::find(markedLambdas.begin(),
													markedLambdas.end(),
													lambda);
	return (it != markedLambdas.end());
}


void Solver::createSnapshot(Field* s, std::string delta, const FieldMember* lambda) {
	SolverSnapshot *result = new SolverSnapshot(s, delta, lambda);
	snapshots.push_back(result);
}


SolverSnapshot* Solver::loadSnapshot() {
	markedLambdas.clear();
	SolverSnapshot* result = snapshots.back();
	snapshots.pop_back();
	markedLambdas.push_back(result->collectedLambda);
	return result;
}


Solver::~Solver() {
//TODO удалить массив снэпшотов
}
