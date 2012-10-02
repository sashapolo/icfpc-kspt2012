/*
 * AStar.h
 *
 *  Created on: Sep 26, 2012
 *      Author: alexander
 */

#ifndef ASTAR_H_
#define ASTAR_H_

#include <list>
#include "Field.h"
#include "FieldMember.h"
#include "AStarPoint.h"
#include "Algorithm.h"

using namespace std;

class AStar: public Algorithm {
private:

	//ToDo: реализовать бинарную кучу для открытого списка
	//открытый список отсортирован
	list<AStarPoint> openedList;
	list<AStarPoint> closedList;

	void addToOpenedList(AStarPoint);
	bool isInClosedList(const AStarPoint&) const;
	bool isInOpenedList(const AStarPoint&) const;
	void checkPoint(const AStarPoint&);
	void addNeighboursToOpenedList(const AStarPoint&, const AStarPoint&, const Field&);

public:
	vector<Point> solve(const Point&, const Point&, const Field&);
};

#endif /* ASTAR_H_ */
