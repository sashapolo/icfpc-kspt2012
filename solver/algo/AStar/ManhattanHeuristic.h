/*
 * ManhattanHeuristic.h
 *
 *  Created on: Oct 28, 2012
 *      Author: alexander
 */

#ifndef MANHATTANHEURISTIC_H_
#define MANHATTANHEURISTIC_H_

#include "Heuristic.h"
#include "../../base/Point.h"
/**
 * Манхеттенская эвристика.<br />
 */
class ManhattanHeuristic: public Heuristic {
private:
	/**
	 * Цель.
	 */
	const Point *goal;
public:
	/**
	 * Конструктор класса ManhattanHeuristic.
	 * @param const Point* g - точка.
	 */
	ManhattanHeuristic(const Point* g): goal(g) {}

	/**
	 * Подсчет Манхеттенской эвристики.
	 * @param AStarPoint& - точка.
	 * @return значение.
	 */
	virtual int calculate(const AStarPoint& start) const {
		return start.getCell().getDistance(*goal) * Field::METRIC_NORMAL;
	}
	/**
	 * Установка цели.
	 * @param const Point* g - точка.
	 */
	void setGoal(const Point* g) {
		goal = g;
	}
};


#endif /* MANHATTANHEURISTIC_H_ */
