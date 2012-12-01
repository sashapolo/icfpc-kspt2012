/*
 * ManualGoal.h
 *
 *  Created on: Nov 16, 2012
 *      Author: alexander
 */

#ifndef MANUALGOAL_H_
#define MANUALGOAL_H_

#include "AStarGoal.h"

class ManualGoal: public AStarGoal {
private:
	/**
	 * Цель.
	 */
	const Point* goal;
public:
	/**
	 * Конструктор класса ManualGoal.
	 * @param const Point *goal - точка.
	 */
	ManualGoal(const Point *goal) {
		this->goal = goal;
	}
	/**
	 * Достигнута ли цель.
	 * @param const AStarPoint& state - цель.
	 * @return true, если достигнута; false - в протианом случае.
	 */
	virtual bool isGoalReached(const AStarPoint& state) const {
		return (*state.getField()->getRobot() == *goal);
	}

	/**
	 * Установка цели.
	 * @param const Point* goal - точка.
	 */
	void setGoal(const Point *goal) {
		this->goal = goal;
	}
};


#endif /* MANUALGOAL_H_ */
