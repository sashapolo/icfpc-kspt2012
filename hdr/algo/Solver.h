/*
 * Solver.h
 *
 *  Created on: Oct 15, 2012
 *      Author: alexander
 */

#ifndef SOLVER_H_
#define SOLVER_H_

#include "stdinclude.h"
#include "algo/AStar/AStar.h"
#include "algo/AStar/ManualGoal.h"
#include "algo/AStar/ManhattanHeuristic.h"
#include "algo/SolverSnapshot.h"
#include "algo/NearestNeighbour.h"
#include "algo/TwoOptOptimizer.h"

class Solver {
private:
	Field *pField;
	//Field *bestField;
	Path *optimalPath;
	int currentGoalIndex;
	std::string lambdaRoute;
	//std::string bestLambdaRoute;
	//int lambdasCollected;
	//int score;
	//int bestScore;
	//std::list<SolverSnapshot*> snapshots;

	void createOptimalPath(Field *);
	const Point* getNextGoal();
//	void createSnapshot(const std::string&);
//	void loadSnapshot();
//	std::string revisitLambdas();
//	void backtrack();
public:
	/**
	 * Конструктор класса Solver
	 * Задает карту.
	 * Обнуляет индекс текущей цели, количество собранных лямбд и наилучший результат.
	 * Создает оптимальный путь для карты.
	 * @param Field *f - карта.
	 */
	Solver(Field*);
	/**
	 * Деструктор класса Solver.
	 * Удаляет промежуточные состояния карты.
	 */
	virtual ~Solver();
	/**
	 * Поиск пути - отвечает за поиск наилучшего пути и выдачу результата.
	 * @return результат.
	 */
	std::string solve();
};


#endif /* SOLVER_H_ */
