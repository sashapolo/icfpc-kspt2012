/*
 * Solver.h
 *
 *  Created on: Oct 15, 2012
 *      Author: alexander
 */

#ifndef SOLVER_H_
#define SOLVER_H_

#include "Point.h"
#include "Field.h"
#include "AStar.h"
using namespace std;

class Solver {
private:
	string convertResultToString(const vector<Point>&) const;
public:
	string solve(const Field* pField) {
		AStar astar(pField, pField->getRobot(), pField->getLift());
		return convertResultToString(astar.solve());
	}
};


#endif /* SOLVER_H_ */
