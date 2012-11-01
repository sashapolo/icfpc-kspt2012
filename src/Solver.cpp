/*
 * Solver.cpp
 *
 *  Created on: Oct 15, 2012
 *      Author: alexander
 */

#include "Solver.h"

#include <iostream>

Solver::Solver(): snapshots(), markedLambdas() {
}

string Solver::solve(Field* pField) {
	// флаг, говорящий о том, что откаты происходят несколько раз подряд =>
	// нужно все собранные на этом шаге лямбды заносить как помеченные
	bool isSequentialBacktracking = false;
	string bestResult;
	int mostLambdasCollected = 0;
	int currentLambdasCollected = 0;
	int backtracksCount = 0;

	const FieldMember *goal = findNewGoal(pField);
	ManhattanHeuristic *mH = new ManhattanHeuristic(goal->getCoordinate());
	AStar astar(pField, pField->getRobot(), mH);
	Point finish = pField->getLift()->getCoordinate();
	Field *pNewField = NULL;
	string result;

	createSnapshot(pField, result, goal); // сохраняем стартовое состояние

	string t = astar.solve(&pNewField);
	while (pNewField->getRobot()->getCoordinate() != finish) {
		if (t.empty()) {
			markUnreachableGoal(goal);
		} else {
			result += t;
			currentLambdasCollected++;
			markedLambdas.clear();
		}
		goal = findNewGoal(pNewField);
		if (goal != NULL) {
			createSnapshot(pNewField, result, goal);
		} else { // если некуда идти - откат
			while (goal == NULL) {
				if (snapshots.empty()) {  // больше некуда откатиться
					delete mH;
					return bestResult + "A";
				} else {  // откат к предыдущей лямбде
					if (backtracksCount == 20) {
						delete mH;
						return bestResult + "A";
					}
					backtracksCount++;
					std::cout<<backtracksCount<<std::endl;
					if (!isSequentialBacktracking) {
						snapshots.pop_back(); // удаляем текущий снэпшот
						if (snapshots.empty()) {  // больше некуда откатиться
							delete mH;
							return bestResult + "A";
						}
					}
					if (mostLambdasCollected == 0) {  // запоминаем лучший результат
						mostLambdasCollected = currentLambdasCollected;
						bestResult = result;
					} else if (currentLambdasCollected > mostLambdasCollected) {
						mostLambdasCollected = currentLambdasCollected;
						bestResult = result;
					}
					currentLambdasCollected = 0;
					SolverSnapshot *ss = loadSnapshot(isSequentialBacktracking);
					pNewField = ss->snapshot;
					result = ss->deltaPath;
					goal = findNewGoal(pNewField);
					isSequentialBacktracking = true;
				}
			}
			isSequentialBacktracking = false;
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
		return isMarked(pField->getLift()) ? NULL : pField->getLift();
	}

	list<FieldMember*>::const_iterator it = pField->getLambdaCacheIt();
	FieldMember *result = isMarked(*it) ? NULL : *it;
	Point start = pField->getRobot()->getCoordinate();
	int min = pField->getDistance((*it)->getCoordinate(), start);

	for (++it; it != pField->getLambdaCacheEnd(); it++) {
		int tmp = pField->getDistance((*it)->getCoordinate(), start);
		if (min > tmp && !isMarked(*it)) {
			min = tmp;
			result = *it;
		} else if (result == NULL && !isMarked(*it)) {
			min = tmp;
			result = *it;
		}
	}
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


SolverSnapshot* Solver::loadSnapshot(bool isSequentialBacktracking) {
	if (!isSequentialBacktracking) {
		markedLambdas.clear();
	} else if (markedLambdas.size() == 2) {
		markedLambdas.pop_front();
	}
	SolverSnapshot* result = snapshots.back();
	snapshots.pop_back();
	markedLambdas.push_back(result->goal);
	return result;
}


Solver::~Solver() {
	std::list<SolverSnapshot*>::iterator it = snapshots.begin();
	for (; it != snapshots.end(); it++) {
		delete *it;
	}
}
