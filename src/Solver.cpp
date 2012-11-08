/*
 * Solver.cpp
 *
 *  Created on: Oct 15, 2012
 *      Author: alexander
 */

#include "Solver.h"

/**
 * Конструктор класса Solver
 * Задает карту.
 * Обнуляет индекс текущей цели, количество собранных лямбд и наилучший результат.
 * Создает оптимальный путь для карты.
 * @param Field *f - карта.
 */
Solver::Solver(Field *f): lambdaRoute(), bestLambdaRoute(), snapshots()  {
	pField = f;
	currentGoalIndex = 0;
	createOptimalPath(pField);
	lambdasCollected = 0;
	bestLambdasCollected = 0;
	bestField = new Field(*pField);
}

/**
 * Поиск пути - отвечает за поиск наилучшего пути и выдачу результата.
 * @return результат.
 */
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
	while ((pField->getRobot()->getCoordinate() != finish)
			&& !SignalHandler::sigIntReceived()) {
		if (!t.empty()) {
			lambdasCollected++;
			if (lambdasCollected >= 20) {
				SolverSnapshot *s = snapshots.front();
				delete s;
				snapshots.pop_front();
			}
			lambdaRoute += t;
			createSnapshot(lambdaRoute);
			backtracksCount = 0;
		}
		goal = getNextGoal();
		while (goal == NULL && !SignalHandler::sigIntReceived()) { // если лямбды из оптимального пути закончились
			// но лямбды на карте еще есть => откат
			// если откаты не последовательные, то нужно удалить последний снэпшот
			if (backtracksCount == 0) {
				snapshots.pop_back();
			}
			if (snapshots.empty()) {  // откатываться некуда
				// попробуем собрать лямбды, которые раньше были недостижимы
				revisitLambdas();
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
		if (SignalHandler::sigIntReceived()) {
			if (lambdasCollected > bestLambdasCollected) {
				return lambdaRoute + "A";
			} else {
				return bestLambdaRoute + "A";
			}
		}
		mH.setGoal(goal->getCoordinate());
		AStar astar(pField, pField->getRobot(), &mH);
		t = astar.solve(&pField);
	}
	if (SignalHandler::sigIntReceived()) {
		if (lambdasCollected > bestLambdasCollected) {
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

/**
 * Создание промежуточного состояния карты.
 * @param string& deltaPath - текущий путь.
 */
void Solver::createSnapshot(const std::string& deltaPath) {
	SolverSnapshot *result = new SolverSnapshot(new Field(*pField),
												currentGoalIndex - 1,
												deltaPath);
	snapshots.push_back(result);
}

/**
 * Загрузка промежуточного состояния карты.
 */
void Solver::loadSnapshot() {
	SolverSnapshot* s = snapshots.back();
	*pField = *s->snapshot;
	lambdaRoute = s->delta;
	lambdasCollected--;
	currentGoalIndex = s->currentGoalIndex;
	optimalPath.deleteCell(currentGoalIndex + 1);
}

/**
 * Создание оптимального пути.
 * @param Field *f - карта.
 */
void Solver::createOptimalPath(Field *f) {
	NearestNeighbour nn(f);
	nn.createTour(f->getRobot()->getCoordinate());
	optimalPath = *nn.getTour();
	TwoOptOptimizer::optimize(&optimalPath);
}

/**
 * Откат к предыдущему состоянию.
 * Если текущий результат собранных лямбд больше, чем лучший результат, то текущий результат становится лучшим.
 */
void Solver::backtrack() {
	if (lambdasCollected > bestLambdasCollected) {
		bestLambdasCollected = lambdasCollected;
		bestLambdaRoute = lambdaRoute;
		delete bestField;
		bestField = new Field(*pField);
	}
	loadSnapshot();
}

/**
 * todo.
 * @return todo.
 */
std::string Solver::revisitLambdas() {
	createOptimalPath(bestField);
	int goalIndex = 0;

	Point finish = bestField->getLift()->getCoordinate();
	const FieldMember *goal = bestField->getXY(optimalPath.getCell(goalIndex++));
	ManhattanHeuristic mH(goal->getCoordinate());
	AStar astar(bestField, bestField->getRobot(), &mH);

	std::string t = astar.solve(&bestField);
	while (bestField->getRobot()->getCoordinate() != finish) {
		if (!t.empty()) {
			bestLambdaRoute += t;
		}
		if (goalIndex == optimalPath.getSize() ||
		    (t.empty() && (goal == bestField->getLift()))) {
			return bestLambdaRoute + "A";
		}
		goal = bestField->getXY(optimalPath.getCell(goalIndex++));
		mH.setGoal(goal->getCoordinate());
		AStar astar(bestField, bestField->getRobot(), &mH);
		t = astar.solve(&bestField);
	}
	return bestLambdaRoute + t;
}

/**
 * Деструктор класса Solver.
 * Удаляет промежуточные состояния карты.
 */
Solver::~Solver() {
	std::list<SolverSnapshot*>::iterator it = snapshots.begin();
	for (; it != snapshots.end(); it++) {
		delete *it;
	}
	delete bestField;
}
