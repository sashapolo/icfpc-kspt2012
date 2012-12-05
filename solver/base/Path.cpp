/*
 * Path.cpp
 *
 *  Created on: Sep 24, 2012
 *      Author: alexander
 */

#include "base/Path.h"

Path::~Path() {
	std::vector<const Point*>::iterator it = path.begin();
	std::vector<const Point*>::iterator end = path.end();
	for (; it != end; it++) {
		delete *it;
	}
	path.clear();
}

int Path::getDistance() const {
	int result = 0;
	for (unsigned int i = 0; i < path.size() - 1; i++) {
		result += path[i]->getDistance(*path[i+1]);
	}
	return result;
}


void Path::swap(int i, int j) {
	const Point *t = path[i];
	path[i] = path[j];
	path[j] = t;
}


void Path::deleteCell(int index) {
	delete path[index];
	path.erase(path.begin() + index);
}
