/*
 * AStarGoal.h
 *
 *  Created on: Nov 16, 2012
 *      Author: alexander
 */

#ifndef ASTARGOAL_H_
#define ASTARGOAL_H_

class AStarGoal {
public:
	virtual bool isGoalReached(const AStarPoint&) const = 0;
	virtual ~AStarGoal() {};
};


#endif /* ASTARGOAL_H_ */
