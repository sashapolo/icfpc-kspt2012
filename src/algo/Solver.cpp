/*
 * Solver.cpp
 *
 *  Created on: Oct 15, 2012
 *      Author: alexander
 */

#include "algo/Solver.h"


Solver::Solver(Field *f): lambdaRoute(), bestLambdaRoute(), snapshots(),
						  shittyLambdas() {
	pField = new Field(*f);
	nextGoalIndex = 0;
	optimalPath = NULL;
	createOptimalPath(pField);
	lambdasCollected = 0;
	score = 0;
	bestScore = 0;
}

std::string Solver::solve() {
	int backtracksCount = 0;
	int lambdasCollectedOld = 0;

	// инициализация переменных для А*
	const Point finish = *pField->getLift();
	const Point *goal = getNextGoal();
	ManhattanHeuristic mH(goal);
	ManualGoal mG(goal);
	AStar astar(pField, &mH, &mG, shittyLambdas);

	// сохраняем стартовое состояние
	createSnapshot();

	std::string t = astar.solve(&pField);
	while ((*pField->getRobot() != finish) && !SignalHandler::sigIntReceived()) {
		if (!t.empty()) {
			// если были откаты, то необходимо заново проложить маршрут
			if (backtracksCount != 0) {
				createOptimalPath(pField);
				nextGoalIndex = 0;
				clearShittyList();
				backtracksCount = 0;
			}
			lambdasCollected++;
			if (snapshots.size() >= SNAPSHOT_DEPTH) {
				delete snapshots.front();
				snapshots.pop_front();
			}
			score += 25 * (lambdasCollected - lambdasCollectedOld) - t.size();
			lambdasCollectedOld = lambdasCollected;
			lambdaRoute += t;
			createSnapshot();
		}
		goal = getNextGoal();
		while (goal == NULL && !SignalHandler::sigIntReceived()) {
			// если лямбды из оптимального пути закончились,
			// но лямбды на карте еще есть => откат.
			// если откаты не последовательные, то нужно очистить список помеченных лямбд
			if (backtracksCount == 0) {
				delete snapshots.back();
				snapshots.pop_back();
			}
			if (snapshots.empty()) {  // откатываться некуда
				return bestLambdaRoute + "A";
			}
			shittyLambdas.push_back(new Point(*pField->getRobot()));
			backtrack();
			lambdasCollectedOld = lambdasCollected;
			goal = getNextGoal();
			backtracksCount++;
		}
		if (SignalHandler::sigIntReceived()) {
			break;
		}
		mH.setGoal(goal);
		mG.setGoal(goal);
		AStar astar(pField, &mH, &mG, shittyLambdas);
		t = astar.solve(&pField);
	}
	if (SignalHandler::sigIntReceived()) {
		if (score > bestScore) {
			return lambdaRoute + "A";
		} else {
			return bestLambdaRoute + "A";
		}
	}
	return lambdaRoute + t;
}

/**
 * Получение следующией лямбы из оптимального пути.
 * @return следующая лямбда.
 */
const Point* Solver::getNextGoal() {
	while (nextGoalIndex < optimalPath->getSize()) {
		const Point *result = optimalPath->getCell(nextGoalIndex);
		nextGoalIndex++;
		if (pField->getXY(*result) != LAMBDA) {
			lambdasCollected++;
		} else if (!isShitty(result)) {
			return result;
		}
	}
	return pField->getXY(*pField->getLift()) == OPENED_LIFT ? pField->getLift() : NULL;
}

/**
 * Создание промежуточного состояния карты.
 * @param string& deltaPath - текущий путь.
 */
void Solver::createSnapshot() {
	if (score > bestScore) {
		bestScore = score;
		bestLambdaRoute = lambdaRoute;
	}
	SolverSnapshot *result = new SolverSnapshot(pField, lambdasCollected, nextGoalIndex,
												score, lambdaRoute);
	snapshots.push_back(result);
}

/**
 * Загрузка промежуточного состояния карты.
 */
void Solver::loadSnapshot() {
	SolverSnapshot* s = snapshots.back();
	pField = new Field(*s->snapshot);
	lambdaRoute = s->delta;
	lambdasCollected = s->lambdasCollected;
	nextGoalIndex = s->nextGoalIndex;
	score = s->score;
	delete s;
	snapshots.pop_back();
}

/**
 * Создание оптимального пути.
 * @param Field *f - карта.
 */
void Solver::createOptimalPath(Field *f) {
	if (optimalPath != NULL) {
		delete optimalPath;
	}
	optimalPath = NearestNeighbour::createTour(*f->getRobot(), *pField);
	TwoOptOptimizer::optimize(optimalPath);
}

/**
 * Откат к предыдущему состоянию.
 * Если текущий результат собранных лямбд больше, чем лучший результат, то текущий результат становится лучшим.
 */
void Solver::backtrack() {
	if (score > bestScore) {
		bestScore = score;
		bestLambdaRoute = lambdaRoute;
	}
	loadSnapshot();
}

bool Solver::isShitty(const Point* point) {
	std::list<const Point*>::const_iterator it = find_if(shittyLambdas.begin(),
				shittyLambdas.end(),
				bind2nd(Comparators::PointerComparatorEquals<const Point*>(), point));
	return (it != shittyLambdas.end());
}

void Solver::clearShittyList() {
	std::list<const Point*>::iterator it = shittyLambdas.begin();
	while (it != shittyLambdas.end()) {
		delete *it;
		it = shittyLambdas.erase(it);
	}
}

Solver::~Solver() {
	delete optimalPath;

	std::list<SolverSnapshot*>::iterator it1 = snapshots.begin();
	std::list<SolverSnapshot*>::iterator end1 = snapshots.end();
	for (; it1 != end1; it1++) {
		delete *it1;
	}

	clearShittyList();
}
