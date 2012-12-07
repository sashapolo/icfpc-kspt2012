/*
 * NearestNeighbour.cpp
 *
 *  Created on: Nov 4, 2012
 *      Author: alexander
 */

#include "algo/NearestNeighbour.h"


Path* NearestNeighbour::createTour(const Point& start, const Field& field) {
	Path *pPath = new Path();

	std::list<const Point*> goals;
	Field::FieldCache::const_iterator it = field.getLambdaCacheIt();
	Field::FieldCache::const_iterator end = field.getLambdaCacheEnd();
	for (; it != end; it++) {
		goals.push_back(*it);
	}

	const Point* current = findNearestNeighbour(start, &goals);
	pPath->addCell(*current);
	for (++it; it != end; it++) {
		if (goals.empty()) {
			break;
		}
		current = findNearestNeighbour(*current, &goals);
		pPath->addCell(*current);
	}
	return pPath;
}


const Point* NearestNeighbour::findNearestNeighbour(const Point& point,
													std::list<const Point*> *goals) {
	std::list<const Point*>::iterator it = goals->begin();
	std::list<const Point*>::iterator end = goals->end();
	int min = point.getDistance(*(*it));
	const Point *result = *it;

	for (++it; it != end; it++) {
		int tmp = point.getDistance(*(*it));
		if (min > tmp) {
			min = tmp;
			result = *it;
		}
	}

	for (it = goals->begin(); it != end; it++) {
		if (*result == *(*it)) {
			goals->erase(it);
			break;
		}
	}

	return result;
}
