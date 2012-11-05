/*
 * Solver.cpp
 *
 *  Created on: Oct 15, 2012
 *      Author: alexander
 */

#include "Solver.h"

Solver::Solver(Field *f): lambdaRoute(), bestLambdaRoute(), snapshots()  {
	pField = f;
	currentGoalIndex = 0;
	createOptimalPath();
	markedLambdas.reserve(optimalPath.getSize());
	lambdasCollected = 0;
	bestLambdasCollected = 0;
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
				return bestLambdaRoute + "A";
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
	markedLambdas.push_back(optimalPath.getCell(currentGoalIndex + 1));
	optimalPath.deleteCell(currentGoalIndex + 1);
}


void Solver::createOptimalPath() {
	NearestNeighbour nn(pField);
	nn.createTour(pField->getRobot()->getCoordinate());
	optimalPath = *nn.getTour();
	optimize();
}


// Оптимизирует путь при помощи эвристики 2-opt
void Solver::optimize() {
	// перебираем все возможные ребра в пути
	for (int i = 0; i < optimalPath.getSize() - 3; i++) {
		for (int j = i + 3; j < optimalPath.getSize() - 1; j++) {
			doTwoOpt(i, i+1, j, j+1);
		}
	}
}


void Solver::doTwoOpt(int start1, int end1, int start2, int end2) {
	if (start2 == start1 || start2 == end1 || end2 == start1 || end2 == end1) {
		return;
	}
	int oldDistance = optimalPath.getDistance();
	optimalPath.swap(end1, start2);
	int newDistance = optimalPath.getDistance();
	if (newDistance >= oldDistance) {
		optimalPath.swap(end1, start2);
	}
}


void Solver::backtrack() {
	if (lambdasCollected > bestLambdasCollected) {
		bestLambdasCollected = lambdasCollected;
		bestLambdaRoute = lambdaRoute;
	}
	loadSnapshot();
}


Solver::~Solver() {
	std::list<SolverSnapshot*>::iterator it = snapshots.begin();
	for (; it != snapshots.end(); it++) {
		delete *it;
	}
}
