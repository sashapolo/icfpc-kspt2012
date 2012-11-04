/*
 * Solver.cpp
 *
 *  Created on: Oct 15, 2012
 *      Author: alexander
 */

#include "Solver.h"

Solver::Solver(Field *f): snapshots(), markedLambdas() {
	pField = f;
	currentGoalIndex = 0;
	createOptimalPath();
}


std::string Solver::solve() {
	// флаг, говорящий о том, что откаты происходят несколько раз подряд =>
	// нужно все собранные на этом шаге лямбды заносить как помеченные
	bool isSequentialBacktracking = false;
	string bestResult;
	int mostLambdasCollected = 0;
	int currentLambdasCollected = 0;
	int backtracksCount = 0;

	// инициализация переменных для А*
	Point finish = pField->getLift()->getCoordinate();
	const FieldMember *goal = getNextGoal();
	ManhattanHeuristic mH(goal->getCoordinate());
	AStar astar(pField, pField->getRobot(), &mH);
	string result;

	// сохраняем стартовое состояние
	createSnapshot(pField, result, goal);

	string t = astar.solve(&pField);
	while (pField->getRobot()->getCoordinate() != finish) {
		if (!t.empty()) {
			createSnapshot(pField, result, goal);
			result += t;
		}
		goal = getNextGoal();
		if (goal == NULL || (t.empty() && goal == pField->getLift())) {
			return result + "A";
		}
		mH.setGoal(goal->getCoordinate());
		AStar astar(pField, pField->getRobot(), &mH);
		t = astar.solve(&pField);
	}
	return result + t;
}


const FieldMember* Solver::getNextGoal() {
	FieldMember *result;
	do {
		if (currentGoalIndex == optimalPath.getSize()) {
			return pField->lambdaCacheEmpty() ? pField->getLift() : NULL;
		}
		result = pField->getXY(optimalPath.getCell(currentGoalIndex));
		currentGoalIndex++;
	} while (result->getType() != LAMBDA);
	return result;
}


void Solver::markUnreachableGoal(const FieldMember* pGoal) {
	markedLambdas.push_back(pGoal);
}


bool Solver::isMarked(const FieldMember* lambda) const {
	list<const FieldMember*>::const_iterator it = markedLambdas.begin();
	for (; it != markedLambdas.end(); it++) {
		if (*(*it) == *lambda) {
			return true;
		}
	}
	return false;
}


void Solver::createSnapshot(Field* s, std::string delta, const FieldMember* lambda) {
	SolverSnapshot *result = new SolverSnapshot(new Field(*s), delta, lambda);
	snapshots.push_back(result);
}


void Solver::loadSnapshot(bool isSequentialBacktracking) {
	if (!isSequentialBacktracking) {
		markedLambdas.clear();
	} else if (markedLambdas.size() == 2) {
		markedLambdas.pop_front();
	}
	SolverSnapshot* s = snapshots.back();
	pField = s->snapshot;
	markedLambdas.push_back(s->goal);
	snapshots.pop_back();
}


void Solver::createOptimalPath() {
	NearestNeighbour nn(pField);
	nn.createTour(pField->getRobot()->getCoordinate());
	optimalPath = *nn.getTour();
}


Solver::~Solver() {
	std::list<SolverSnapshot*>::iterator it = snapshots.begin();
	for (; it != snapshots.end(); it++) {
		delete *it;
	}
}
