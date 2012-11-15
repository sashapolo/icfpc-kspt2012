/*
 * ManualGoal.h
 *
 *  Created on: Nov 16, 2012
 *      Author: alexander
 */

#ifndef MANUALGOAL_H_
#define MANUALGOAL_H_

class ManualGoal: public AStarGoal {
private:
	const Point* goal;
public:
	ManualGoal(const Point *goal) {
		this->goal = goal;
	}

	virtual bool isGoalReached(const AStarPoint& state) const {
		return (*state.getField()->getRobot() == *goal);
	}

	void setGoal(const Point *goal) {
		this->goal = goal;
	}
};


#endif /* MANUALGOAL_H_ */
