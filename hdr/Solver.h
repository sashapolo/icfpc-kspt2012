/*
 * Solver.h
 *
 *  Created on: Oct 15, 2012
 *      Author: alexander
 */

#ifndef SOLVER_H_
#define SOLVER_H_

//#include "Dijkstra.h"
//#include "FieldSim.h"
//
#include "AStar.h"
using namespace std;

class Solver {
private:
	string convertResultToString(const Path&) const;
	FieldMember* const findNewGoal(Field* const pField) const;
public:
	string solve(Field*) const;
};


#endif /* SOLVER_H_ */
