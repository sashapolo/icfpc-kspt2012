/*
 * Solver.cpp
 *
 *  Created on: Oct 15, 2012
 *      Author: alexander
 */

#include "Solver.h"

#include <iostream>
Solver::Solver(Field *f): lambdaRoute(), bestLambdaRoute(), snapshots()  {
	pField = f;
	currentGoalIndex = 0;
	createOptimalPath(pField);
	lambdasCollected = 0;
	bestLambdasCollected = 0;
	bestField = new Field(*pField);
}


std::string Solver::solve() {
	int backtracksCount = 0;
	// инициализация переменных для А*
	Point finish = pField->getLift()->getCoordinate();
	const FieldMember *goal = getNextGoal();
	ManhattanHeuristic mH(goal->getCoordinate());
	AStar astar(pField, pField->getRobot(), &mH);

	// сохраняем стартовое состояние
	createSnapshot("");

	std::string t = astar.solve(&pField);
	while (pField->getRobot()->getCoordinate() != finish) {
		if (!t.empty()) {
			lambdasCollected++;
			if (lambdasCollected >= 35) {
				SolverSnapshot *s = snapshots.front();
				delete s;
				snapshots.pop_front();
			}
			lambdaRoute += t;
			createSnapshot(lambdaRoute);
			backtracksCount = 0;
		}
		goal = getNextGoal();
		while (goal == NULL) { // если лямбды из оптимального пути закончились
			// но лямбды на карте еще есть => откат
			// если откаты не последовательные, то нужно удалить последний снэпшот
			if (backtracksCount == 0) {
				snapshots.pop_back();
			}
			if (snapshots.empty()) {  // откатываться некуда
				// попробуем собрать лямбды, которые раньше были недостижимы
				return bestLambdaRoute + revisitLambdas();
			}
			backtrack();
			if (backtracksCount != 0) {
				snapshots.pop_back();
			}
			goal = getNextGoal();
			backtracksCount++;
		}
		if (t.empty() && (goal == pField->getLift())) {
			return lambdaRoute + "A";
		}
		mH.setGoal(goal->getCoordinate());
		AStar astar(pField, pField->getRobot(), &mH);
		t = astar.solve(&pField);
	}
	return lambdaRoute + t;
}


const FieldMember* Solver::getNextGoal() {
	FieldMember *result;
	do {
		if (currentGoalIndex >= optimalPath.getSize()) {
			return pField->lambdaCacheEmpty() ? pField->getLift() : NULL;
		}
		result = pField->getXY(optimalPath.getCell(currentGoalIndex));
		if (result->getType() != LAMBDA) {
			lambdasCollected++;
			optimalPath.deleteCell(currentGoalIndex);
		} else {
			currentGoalIndex++;
		}
	} while (result->getType() != LAMBDA);
	return result;
}


void Solver::createSnapshot(const std::string& deltaPath) {
	SolverSnapshot *result = new SolverSnapshot(new Field(*pField),
												currentGoalIndex - 1,
												deltaPath);
	snapshots.push_back(result);
}


void Solver::loadSnapshot() {
	SolverSnapshot* s = snapshots.back();
	*pField = *s->snapshot;
	lambdaRoute = s->delta;
	lambdasCollected--;
	currentGoalIndex = s->currentGoalIndex;
	optimalPath.deleteCell(currentGoalIndex + 1);
}


void Solver::createOptimalPath(Field *f) {
	NearestNeighbour nn(f);
	nn.createTour(f->getRobot()->getCoordinate());
	optimalPath = *nn.getTour();
	TwoOptOptimizer::optimize(&optimalPath);
}


void Solver::backtrack() {
	if (lambdasCollected > bestLambdasCollected) {
		bestLambdasCollected = lambdasCollected;
		bestLambdaRoute = lambdaRoute;
		delete bestField;
		bestField = new Field(*pField);
	}
	loadSnapshot();
}


std::string Solver::revisitLambdas() {
	lambdaRoute = "";
	createOptimalPath(bestField);
	int goalIndex = 0;

	Point finish = bestField->getLift()->getCoordinate();
	const FieldMember *goal = bestField->getXY(optimalPath.getCell(goalIndex++));
	ManhattanHeuristic mH(goal->getCoordinate());
	AStar astar(bestField, bestField->getRobot(), &mH);

	std::string t = astar.solve(&bestField);
	while (bestField->getRobot()->getCoordinate() != finish) {
		if (!t.empty()) {
			lambdaRoute += t;
		}
		if (goalIndex == optimalPath.getSize() ||
		    (t.empty() && (goal == bestField->getLift()))) {
			return lambdaRoute + "A";
		}
		goal = bestField->getXY(optimalPath.getCell(goalIndex++));
		mH.setGoal(goal->getCoordinate());
		AStar astar(bestField, bestField->getRobot(), &mH);
		t = astar.solve(&bestField);
	}
	return lambdaRoute + t;
}


Solver::~Solver() {
	std::list<SolverSnapshot*>::iterator it = snapshots.begin();
	for (; it != snapshots.end(); it++) {
		delete *it;
	}
	delete bestField;
}
