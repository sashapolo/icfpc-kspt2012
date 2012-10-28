/*
 * Heuristic.h
 *
 *  Created on: Oct 28, 2012
 *      Author: alexander
 */

#ifndef HEURISTIC_H_
#define HEURISTIC_H_

#include "AStarPoint.h"

class Heuristic {
public:
	virtual int calculate(const AStarPoint&) = 0;
	virtual ~Heuristic() {};
};


#endif /* HEURISTIC_H_ */
