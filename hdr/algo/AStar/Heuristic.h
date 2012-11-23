/*
 * Heuristic.h
 *
 *  Created on: Oct 28, 2012
 *      Author: alexander
 */

#ifndef HEURISTIC_H_
#define HEURISTIC_H_

#include "algo/AStar/AStarPoint.h"
/**
 * Эвристика.<br />
 */
class Heuristic {
public:
	/**
	 * Подсчет эвристики.
	 * @param AStarPoint& - точка.
	 * @return значение.
	 */
	virtual int calculate(const AStarPoint&) const = 0;
	/**
	 * Деструктор класса Heuristic.
	 */
	virtual ~Heuristic() {};
};


#endif /* HEURISTIC_H_ */
