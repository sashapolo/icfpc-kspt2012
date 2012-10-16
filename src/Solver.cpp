/*
 * Solver.cpp
 *
 *  Created on: Oct 15, 2012
 *      Author: alexander
 */

#include "Solver.h"

string Solver::convertResultToString(const vector<Point>& coordinates) const {
	string result;
	for (int i = coordinates.size() - 2; i >= 0; i--) {
		if (coordinates[i + 1].x - coordinates[i].x == 1) {
			result += 'L';
		} else if (coordinates[i + 1].x - coordinates[i].x == -1) {
			result += 'R';
		} else if (coordinates[i + 1].y - coordinates[i].y == 1) {
			result += 'U';
		} else if (coordinates[i + 1].y - coordinates[i].y == -1) {
			result += 'D';
		} else {
			result += 'W';
		}
	}
	return result;
}


