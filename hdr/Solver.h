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
	std::list<SolverSnapshot*> snapshots;
	std::list<const FieldMember*> markedLambdas;

	void createOptimalPath();
	const FieldMember* getNextGoal();
	void markUnreachableGoal(const FieldMember* pGoal);
	bool isMarked(const FieldMember* lambda) const;
	void createSnapshot(Field* s, std::string delta, const FieldMember* lambda);
	void loadSnapshot(bool);
	void optimize();
	void doTwoOpt(int start1, int end1, int start2, int end2);
public:
	Solver(Field*);
	std::string solve();
	virtual ~Solver();
};


#endif /* SOLVER_H_ */
