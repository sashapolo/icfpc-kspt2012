/*
 * ManhattanHeuristic.cpp
 *
 *  Created on: Oct 28, 2012
 *      Author: alexander
 */

#include "algo/AStar/ManhattanHeuristic.h"

ManhattanHeuristic::ManhattanHeuristic(const Point* g) {
	goal = g;
}

void ManhattanHeuristic::setGoal(const Point* g) {
	goal = g;
}

int ManhattanHeuristic::calculate(const AStarPoint& start) const {
	return start.getCell().getDistance(*goal) * Field::METRIC_NORMAL;
}
