/*
 * AStarGoal.h
 *
 *  Created on: Nov 16, 2012
 *      Author: alexander
 */

#ifndef ASTARGOAL_H_
#define ASTARGOAL_H_

#include "AStarPoint.h"

/**
 * А* Цель.<br />
 */
class AStarGoal {
public:
	/**
	 * Достигнута ли цель.
	 * Обнуление достижения.
	 */
	virtual bool isGoalReached(const AStarPoint&) const = 0;
	/**
	 * Деструктор класса AStarGoal.
	 */
	virtual ~AStarGoal() {};
};


#endif /* ASTARGOAL_H_ */
