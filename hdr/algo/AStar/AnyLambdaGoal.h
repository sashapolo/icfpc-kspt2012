/*
 * AnyLambdaGoal.h
 *
 *  Created on: Nov 16, 2012
 *      Author: alexander
 */

#ifndef ANYLAMBDAGOAL_H_
#define ANYLAMBDAGOAL_H_

class AnyLambdaGoal: public AStarGoal {
public:
	virtual bool isGoalReached(const AStarPoint& state) const {
		FieldCache::const_iterator it = state.getField()->getLambdaCacheIt();
		FieldCache::const_iterator end = state.getField()->getLambdaCacheEnd();
		for (; it != end; it++) {
			if (**it == *state.getField()->getRobot()) {
				return true;
			}
		}
		return false;
	}
};


#endif /* ANYLAMBDAGOAL_H_ */
