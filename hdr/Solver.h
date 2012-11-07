/*
 * Solver.h
 *
 *  Created on: Oct 15, 2012
 *      Author: alexander
 */

#ifndef SOLVER_H_
#define SOLVER_H_

#include "AStar.h"
#include "ManhattanHeuristic.h"
#include "SolverSnapshot.h"
#include "NearestNeighbour.h"
#include "TwoOptOptimizer.h"

class Solver {
private:
	Field *pField;
	Field *bestField;
	Path optimalPath;
	int currentGoalIndex;
	std::string lambdaRoute;
	std::string bestLambdaRoute;
	int lambdasCollected;
	int bestLambdasCollected;
	std::list<SolverSnapshot*> snapshots;

	void createOptimalPath(Field *);
	const FieldMember* getNextGoal();
	void createSnapshot(const std::string&);
	void loadSnapshot();
	void backtrack();
	std::string revisitLambdas();
public:
	Solver(Field*);
	std::string solve();
	virtual ~Solver();
};


#endif /* SOLVER_H_ */
