/*
 * Path.cpp
 *
 *  Created on: Sep 24, 2012
 *      Author: alexander
 */

#include "Path.h"

Coordinate Path::getCell(int index) {
	return path[index];
}

void Path::addCell(const Coordinate& c) {
	path.push_back(c);
}
