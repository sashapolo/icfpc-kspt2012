/*
 * NullHeuristic.h
 *
 *  Created on: Nov 16, 2012
 *      Author: alexander
 */

#ifndef NULLHEURISTIC_H_
#define NULLHEURISTIC_H_

#include "Heuristic.h"

class NullHeuristic: public Heuristic {
public:
	/**
	 * Подсчет todo.
	 * @param AStarPoint& state - точка.
	 * @return значение.
	 */
	virtual int calculate(const AStarPoint& state) const {
		return 0;
	}
};


#endif /* NULLHEURISTIC_H_ */
