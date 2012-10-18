/**
 * AStar.h
 *
 *  Created on: Sep 26, 2012
 *      Author: alexander
 */

#ifndef ASTAR_H_
#define ASTAR_H_

#include <list>
#include <iostream>
#include "Field.h"
#include "FieldMember.h"
#include "AStarPoint.h"

using namespace std;

/**
 * AStar.h
 *
 *  Created on: Sep 26, 2012
 *      Author: alexander
 */
class AStar {
private:

	//ToDo: реализовать бинарную кучу для открытого списка
	list<AStarPoint> openedList; //открытый список отсортирован
	list<AStarPoint> closedList;

	void addToOpenedList(const AStarPoint&);
	bool isInClosedList(const AStarPoint&) const;
	bool isInOpenedList(const AStarPoint&) const;
	void checkPoint(const AStarPoint&, const AStarPoint&);
	void addNeighboursToOpenedList(const AStarPoint&, const AStarPoint&, const Field&);
public:
       /**
        * AStar.h
        *
        *  Created on: Sep 26, 2012
        *      Author: alexander
        */
	vector<Point> solve(const Point&, const Point&, const Field&);
};

#endif /* ASTAR_H_ */
