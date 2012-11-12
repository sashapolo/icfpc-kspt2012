/*
 * ManhattanHeuristic.cpp
 *
 *  Created on: Oct 28, 2012
 *      Author: alexander
 */

#include "algo/ManhattanHeuristic.h"

ManhattanHeuristic::ManhattanHeuristic(const Point& g) {
	goal = &g;
}

void ManhattanHeuristic::setGoal(const Point& g) {
	goal = &g;
}

int ManhattanHeuristic::calculate(const Point& start) const {
	return start.getDistance(*goal) * Field::METRIC_NORMAL;
}
