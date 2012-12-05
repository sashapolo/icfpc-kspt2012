/*
 * NearestNeighbour.h
 *
 *  Created on: Nov 4, 2012
 *      Author: alexander
 */

#ifndef NEARESTNEIGHBOUR_H_
#define NEARESTNEIGHBOUR_H_

#include "../stdinclude.h"


class NearestNeighbour {
private:
	static const Point* findNearestNeighbour(const Point&, std::list<const Point*>*);
public:
	static Path* createTour(const Point&, const Field&);
};

#endif /* NEARESTNEIGHBOUR_H_ */
