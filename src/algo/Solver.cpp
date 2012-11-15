/*
 * Solver.cpp
 *
 *  Created on: Oct 15, 2012
 *      Author: alexander
 */

#include "algo/Solver.h"


Solver::Solver(Field *f): lambdaRoute() {
	//, bestLambdaRoute(), snapshots()  {
	pField = f;
	currentGoalIndex = 0;
	createOptimalPath(pField);
//	lambdasCollected = 0;
//	score = 0;
//	bestScore = 0;
//	bestField = new Field(*pField);
}


std::string Solver::solve() {
	//int backtracksCount = 0;
	//int lambdasCollectedOld = 0;
	// инициализация переменных для А*
	const Point *finish = pField->getLift();
	const Point *goal = getNextGoal();
	ManhattanHeuristic mH(goal);
	ManualGoal mG(goal);
	AStar astar(pField, &mH, &mG);

	// сохраняем стартовое состояние
	//createSnapshot("");

	std::string t = astar.solve(&pField);
	while ((pField->getRobot() != finish) && !SignalHandler::sigIntReceived()) {
		if (!t.empty()) {
//			lambdasCollected++;
//			score += 25 * (lambdasCollected - lambdasCollectedOld);
//			score -= t.size();
//			lambdasCollectedOld = lambdasCollected;
//			if (lambdasCollected >= 20) {
//				SolverSnapshot *s = snapshots.front();
//				delete s;
//				snapshots.pop_front();
//			}
			lambdaRoute += t;
//			createSnapshot(lambdaRoute);
//			backtracksCount = 0;
//		} else if (goal == pField->getLift()) {
//			return lambdaRoute + "A";
		} else {
			return lambdaRoute + "A";
		}
		goal = getNextGoal();
		while (goal == NULL && !SignalHandler::sigIntReceived()) { // если лямбды из оптимального пути закончились
//			// но лямбды на карте еще есть => откат
//			// если откаты не последовательные, то нужно удалить последний снэпшот
//			if (backtracksCount == 0) {
//				snapshots.pop_back();
//			}
//			if (snapshots.empty()) {  // откатываться некуда
//				// попробуем собрать лямбды, которые раньше были недостижимы
//				return revisitLambdas();
//			}
//			backtrack();
//			if (backtracksCount != 0) {
//				snapshots.pop_back();
//			}
//			goal = getNextGoal();
//			backtracksCount++;
			return lambdaRoute;
		}
		if (SignalHandler::sigIntReceived()) {
			//if (score > bestScore) {
				return lambdaRoute + "A";
			//} else {
			//	return bestLambdaRoute + "A";
			//}
		}
		mH.setGoal(goal);
		mG.setGoal(goal);
		AStar astar(pField, &mH, &mG);
		t = astar.solve(&pField);
	}
	if (SignalHandler::sigIntReceived()) {
//		if (score > bestScore) {
			return lambdaRoute + "A";
//		} else {
//			return bestLambdaRoute + "A";
//		}
	}
	return lambdaRoute + t;
}

/**
 * Получение следующией лямбы из оптимального пути.
 * @return следующая лямбда.
 */
const Point* Solver::getNextGoal() {
	while (currentGoalIndex < optimalPath->getSize()) {
		const Point *result = optimalPath->getCell(currentGoalIndex);
		if (pField->getXY(*result) != LAMBDA) {
//			lambdasCollected++;
//			optimalPath->deleteCell(currentGoalIndex);
			currentGoalIndex++;
		} else {
			return result;
		}
	}
	return pField->getXY(*pField->getLift()) == OPENED_LIFT ? pField->getLift() : NULL;
}

/**
 * Создание промежуточного состояния карты.
 * @param string& deltaPath - текущий путь.
 */
//void Solver::createSnapshot(const std::string& deltaPath) {
//	if (score > bestScore) {
//		bestScore = score;
//		bestLambdaRoute = lambdaRoute;
//		delete bestField;
//		bestField = new Field(*pField);
//	}
//	SolverSnapshot *result = new SolverSnapshot(new Field(*pField),
//												currentGoalIndex - 1,
//												deltaPath);
//	snapshots.push_back(result);
//}

/**
 * Загрузка промежуточного состояния карты.
 */
//void Solver::loadSnapshot() {
//	SolverSnapshot* s = snapshots.back();
//	*pField = *s->snapshot;
//	lambdaRoute = s->delta;
//	lambdasCollected--;
//	currentGoalIndex = s->currentGoalIndex;
//	optimalPath.deleteCell(currentGoalIndex + 1);
//}

/**
 * Создание оптимального пути.
 * @param Field *f - карта.
 */
void Solver::createOptimalPath(Field *f) {
	optimalPath = NearestNeighbour::createTour(*f->getRobot(), *pField);
	TwoOptOptimizer::optimize(optimalPath);
}

/**
 * Откат к предыдущему состоянию.
 * Если текущий результат собранных лямбд больше, чем лучший результат, то текущий результат становится лучшим.
 */
//void Solver::backtrack() {
//	if (score > bestScore) {
//		bestScore = score;
//		bestLambdaRoute = lambdaRoute;
//		delete bestField;
//		bestField = new Field(*pField);
//	}
//	loadSnapshot();
//}

/**
 * todo.
 * @return todo.
 */
//std::string Solver::revisitLambdas() {
//	createOptimalPath(bestField);
//	int goalIndex = 0;
//
//	Point finish = bestField->getLift()->getCoordinate();
//	const FieldMember *goal = bestField->getXY(optimalPath.getCell(goalIndex++));
//	ManhattanHeuristic mH(goal->getCoordinate());
//	AStar astar(bestField, bestField->getRobot(), &mH);
//
//	std::string t = astar.solve(&bestField);
//	while (bestField->getRobot()->getCoordinate() != finish) {
//		if (!t.empty()) {
//			bestLambdaRoute += t;
//		} else if (goal->getType() == OPENED_LIFT) {
//			return bestLambdaRoute += "A";
//		}
//		if (goalIndex == optimalPath.getSize()) {
//			if (bestField->lambdaCacheEmpty()) {
//				goal = bestField->getLift();
//			} else {
//				return bestLambdaRoute + "A";
//			}
//		} else {
//			goal = bestField->getXY(optimalPath.getCell(goalIndex++));
//		}
//		mH.setGoal(goal->getCoordinate());
//		AStar astar(bestField, bestField->getRobot(), &mH);
//		t = astar.solve(&bestField);
//	}
//	return bestLambdaRoute + t;
//}


Solver::~Solver() {
	delete optimalPath;
//	std::list<SolverSnapshot*>::iterator it = snapshots.begin();
//	for (; it != snapshots.end(); it++) {
//		delete *it;
//	}
//	delete bestField;
}