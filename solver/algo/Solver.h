/*
 * Solver.h
 *
 *  Created on: Oct 15, 2012
 *      Author: alexander
 */

#ifndef SOLVER_H_
#define SOLVER_H_

#include "../stdinclude.h"
#include "AStar/AStar.h"
#include "AStar/ManualGoal.h"
#include "AStar/ManhattanHeuristic.h"
#include "SolverSnapshot.h"
#include "NearestNeighbour.h"
#include "TwoOptOptimizer.h"

class Solver {
private:
	static const unsigned int SNAPSHOT_DEPTH = 20;

	const Field *pField;
	Path *optimalPath;
	int nextGoalIndex;

	std::string lambdaRoute;
	std::string bestLambdaRoute;

	int lambdasCollected;
	int score;
	int bestScore;

	std::list<SolverSnapshot*> snapshots;
	std::list<const Point*> shittyLambdas;

	bool isShitty(const Point*);
	void clearShittyList();

	void createOptimalPath(const Field*);
	const Point* getNextGoal();

	void createSnapshot();
	void loadSnapshot();
	void backtrack();
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
