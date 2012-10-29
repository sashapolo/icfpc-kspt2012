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

using namespace std;

class Solver {
private:
	string convertResultToString(const Path&) const;
	FieldMember* findNewGoal(const Field* pField) const;
	void deleteUnreachableLambda(Field* pField, const FieldMember* pLambda) const;
public:
	string solve(Field*) const;
};


#endif /* SOLVER_H_ */
