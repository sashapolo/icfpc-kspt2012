/*
 * ManhattanHeuristic.h
 *
 *  Created on: Oct 28, 2012
 *      Author: alexander
 */

#ifndef MANHATTANHEURISTIC_H_
#define MANHATTANHEURISTIC_H_

#include "Heuristic.h"

class ManhattanHeuristic: public Heuristic {
private:
	const Point *goal;
public:
	ManhattanHeuristic(const Point& g) {
		goal = &g;
	}

	virtual int calculate(const AStarPoint&);

	void setGoal(const Point& g) {
		goal = &g;
	}

	virtual ~ManhattanHeuristic() {};
};


#endif /* MANHATTANHEURISTIC_H_ */
