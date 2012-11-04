/*
 * NearestNeighbour.cpp
 *
 *  Created on: Nov 4, 2012
 *      Author: alexander
 */

#include "NearestNeighbour.h"

NearestNeighbour::NearestNeighbour(const Field *f): tour() {
	pField = f;
	std::list<FieldMember*>::const_iterator it = pField->getLambdaCacheIt();
	for (; it != pField->getLambdaCacheEnd(); it++) {
		goals.push_back((*it)->getCoordinate());
	}
}

const Path* NearestNeighbour::getTour() const {
	return &tour;
}


NearestNeighbour::~NearestNeighbour() {
}


void NearestNeighbour::createTour(const Point& start) {
	std::list<FieldMember*>::const_iterator it = pField->getLambdaCacheIt();
	std::list<FieldMember*>::const_iterator end = pField->getLambdaCacheEnd();

	Point current = start;
	for (; it != end; it++) {
		current = findNearestNeighbour(current);
		tour.addCell(current);
	}
}


Point NearestNeighbour::findNearestNeighbour(const Point& point) {
	std::list<Point>::const_iterator it = goals.begin();
	int min = pField->getDistance(point, *it);
	Point result = *it;

	for (++it; it != goals.end(); it++) {
		int tmp = pField->getDistance(point, *it);
		if (min > tmp) {
			min = tmp;
			result = *it;
		}
	}
	deleteFromGoals(result);
	return result;
}


void NearestNeighbour::deleteFromGoals(const Point& point) {
	std::list<Point>::iterator it = goals.begin();
	for (; it != goals.end(); it++) {
		if (point == *it) {
			goals.erase(it);
			return;
		}
	}

}

