/*
 * NearestNeighbour.h
 *
 *  Created on: Nov 4, 2012
 *      Author: alexander
 */

#ifndef NEARESTNEIGHBOUR_H_
#define NEARESTNEIGHBOUR_H_

#include "stdinclude.h"

class NearestNeighbour {
private:
	Path path;
	const Field *pField;
	std::list<Point> goals;

	Point findNearestNeighbour(const Point&);
	void deleteFromGoals(const Point&);
public:
	NearestNeighbour(const Field *);
	void createTour(const Point&);
	const Path* getTour() const;
	virtual ~NearestNeighbour();
};

#endif /* NEARESTNEIGHBOUR_H_ */
