/*
 * ManhattanHeuristic.cpp
 *
 *  Created on: Oct 28, 2012
 *      Author: alexander
 */

#include "ManhattanHeuristic.h"

/**
 * Подсчет Манхеттанской эвристики.
 * @param AStarPoint& a - точка.
 * @return значение.
 */
int ManhattanHeuristic::calculate(const AStarPoint& a) {
	Point start = a.getCell()->getCoordinate();
	return start.getDistance(*goal) * FieldMember::METRIC_NORMAL;
}
