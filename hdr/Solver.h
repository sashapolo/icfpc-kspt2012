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

class Solver {
private:
	std::list<SolverSnapshot*> snapshots;
	std::list<const FieldMember*> markedLambdas;

	const FieldMember* findNewGoal(const Field* pField) const;
	void markUnreachableGoal(const FieldMember* pGoal);
	bool isMarked(const FieldMember* lambda) const;
	void createSnapshot(Field* s, std::string delta, const FieldMember* lambda);
	SolverSnapshot* loadSnapshot(bool);
public:
	Solver();
	string solve(Field*);
	virtual ~Solver();
};


#endif /* SOLVER_H_ */
