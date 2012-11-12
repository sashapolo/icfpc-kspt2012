/*
 * ManhattanHeuristic.h
 *
 *  Created on: Oct 28, 2012
 *      Author: alexander
 */

#ifndef MANHATTANHEURISTIC_H_
#define MANHATTANHEURISTIC_H_

#include "algo/Heuristic.h"
#include "base/Field.h"

class ManhattanHeuristic: public Heuristic {
private:
	const Point *goal;
public:
	ManhattanHeuristic(const Point& g);

	/**
	 * Подсчет Манхеттенской эвристики.
	 * @param AStarPoint& a - точка.
	 * @return значение.
	 */
	virtual int calculate(const Point&) const;

	void setGoal(const Point& g);
};


#endif /* MANHATTANHEURISTIC_H_ */
