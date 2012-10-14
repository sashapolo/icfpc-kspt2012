/*
 * Path.cpp
 *
 *  Created on: Sep 24, 2012
 *      Author: alexander
 */

#include "Path.h"

Point Path::getCell(int index) {
	return path[index];
}

void Path::addCell(const Point& c) {
	path.push_back(c);
}

int Path::getSize()
{
    return path.size();
}