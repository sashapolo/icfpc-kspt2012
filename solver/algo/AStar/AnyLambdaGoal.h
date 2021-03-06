/*
 * AnyLambdaGoal.h
 *
 *  Created on: Nov 16, 2012
 *      Author: alexander
 */

#ifndef ANYLAMBDAGOAL_H_
#define ANYLAMBDAGOAL_H_

#include "AStarGoal.h"

class AnyLambdaGoal: public AStarGoal {
public:
	/**
	 * Достигнута ли цель.
	 * @param const AStarPoint& state - todo.
	 * @return true, если цель достигнута; false - в противном случае.
	 */
	virtual bool isGoalReached(const AStarPoint& state) const {
		Field::FieldCache::const_iterator it = state.getField()->getLambdaCacheIt();
		Field::FieldCache::const_iterator end = state.getField()->getLambdaCacheEnd();
		for (; it != end; it++) {
			if (**it == *state.getField()->getRobot()) {
				return true;
			}
		}
		return false;
	}
};


#endif /* ANYLAMBDAGOAL_H_ */
