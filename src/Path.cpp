/*
 * Path.cpp
 *
 *  Created on: Sep 24, 2012
 *      Author: alexander
 */

#include "Path.h"

Point Path::getCell(int index) const {
	return path[index];
}

void Path::addCell(const Point& c) {
	path.push_back(c);
}

int Path::getSize() const {
    return path.size();
}
