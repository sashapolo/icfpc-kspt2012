/*
 * Heuristic.h
 *
 *  Created on: Oct 28, 2012
 *      Author: alexander
 */

#ifndef HEURISTIC_H_
#define HEURISTIC_H_

#include "base/Point.h"

class Heuristic {
public:
	virtual int calculate(const Point&) const = 0;
	virtual ~Heuristic() {};
};


#endif /* HEURISTIC_H_ */
