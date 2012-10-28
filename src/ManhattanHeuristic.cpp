/*
 * ManhattanHeuristic.cpp
 *
 *  Created on: Oct 28, 2012
 *      Author: alexander
 */

#include "ManhattanHeuristic.h"

int ManhattanHeuristic::calculate(const AStarPoint& a) {
	Point start = a.getCell()->getCoordinate();
	return a.getField()->getDistance(start, *goal) * FieldMember::METRIC_NORMAL;
}
