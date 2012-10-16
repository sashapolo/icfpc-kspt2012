/*
 * Solver.h
 *
 *  Created on: Oct 15, 2012
 *      Author: alexander
 */

#ifndef SOLVER_H_
#define SOLVER_H_

#include "Dijkstra.h"
using namespace std;

class Solver {
private:

public:
	string convertResultToString(const Path&) const;
	vector<Path> solve(const Field*) const;
};


#endif /* SOLVER_H_ */
