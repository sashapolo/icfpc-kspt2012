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

class Solver {
private:
	Field *pField;
	Path optimalPath;
	int currentGoalIndex;
	std::string lambdaRoute;
	std::string bestLambdaRoute;
	int lambdasCollected;
	int bestLambdasCollected;
	std::list<SolverSnapshot*> snapshots;
	std::vector<Point> markedLambdas;

	void createOptimalPath();
	const FieldMember* getNextGoal();
	void createSnapshot(const std::string&);
	void loadSnapshot();
	void optimize();
	void doTwoOpt(int start1, int end1, int start2, int end2);
	void backtrack();
public:
	Solver(Field*);
	std::string solve();
	virtual ~Solver();
};


#endif /* SOLVER_H_ */
