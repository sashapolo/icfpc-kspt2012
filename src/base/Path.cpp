/*
 * Path.cpp
 *
 *  Created on: Sep 24, 2012
 *      Author: alexander
 */

#include "base/Path.h"

Path::Path(): path() {
}

Path::~Path() {
	std::vector<Point*>::iterator it = path.begin();
	std::vector<Point*>::iterator end = path.end();
	for (; it != end; it++) {
		delete *it;
	}
}


const Point* Path::getCell(int index) const {
    return path[index];
}


void Path::addCell(const Point& c) {
    /*
     * ToDo проверка того,
     * что добавляемая точка находится рядом с последней добавленной.
     */
    path.push_back(new Point(c));
}


int Path::getSize() const {
    return path.size();
}


bool Path::isEmpty() const {
	return path.empty();
}


int Path::getDistance() const {
	int result = 0;
	for (unsigned int i = 0; i < path.size() - 1; i++) {
		result += path[i]->getDistance(*path[i+1]);
	}
	return result;
}


void Path::swap(int i, int j) {
	Point *t = path[i];
	path[i] = path[j];
	path[j] = t;
}


void Path::deleteCell(int index) {
	delete path[index];
	path.erase(path.begin() + index);
}
