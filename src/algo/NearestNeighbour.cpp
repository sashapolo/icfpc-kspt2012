/*
 * NearestNeighbour.cpp
 *
 *  Created on: Nov 4, 2012
 *      Author: alexander
 */

#include "algo/NearestNeighbour.h"


NearestNeighbour::NearestNeighbour(const Field *f): path() {
	pField = f;
	FieldCache::const_iterator it = pField->getLambdaCacheIt();
	for (; it != pField->getLambdaCacheEnd(); it++) {
		goals.push_back(*it);
	}
}


const Path& NearestNeighbour::getTour() const {
	return path;
}


void NearestNeighbour::createTour(const Point& start) {
	FieldCache::const_iterator it = pField->getLambdaCacheIt();
	FieldCache::const_iterator end = pField->getLambdaCacheEnd();

	const Point* current = findNearestNeighbour(start);
	for (; it != end; it++) {
		path.addCell(*current);
		current = findNearestNeighbour(*current);
	}
}


const Point* NearestNeighbour::findNearestNeighbour(const Point& point) {
	std::list<Point*>::const_iterator it = goals.begin();
	int min = point.getDistance(**it);
	Point *result = *it;

	for (++it; it != goals.end(); it++) {
		int tmp = point.getDistance(**it);
		if (min > tmp) {
			min = tmp;
			result = *it;
		}
	}
	deleteFromGoals(*result);
	return result;
}


void NearestNeighbour::deleteFromGoals(const Point& point) {
	std::list<Point*>::iterator it = goals.begin();
	for (; it != goals.end(); it++) {
		if (point == **it) {
			goals.erase(it);
			return;
		}
	}

}

