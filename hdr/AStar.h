/*
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

class AStar {
private:
	AStarPoint *start;
	AStarPoint *goal;
	const Field *field;

	//ToDo: реализовать бинарную кучу для открытого списка
	list<AStarPoint> openedList; //открытый список отсортирован
	list<AStarPoint> closedList;

	void addToOpenedList(const AStarPoint&);
	bool isInClosedList(const AStarPoint&) const;
	bool isInOpenedList(const AStarPoint&) const;
	void checkPoint(const AStarPoint&, const AStarPoint&);
	void addNeighboursToOpenedList(const AStarPoint&);
	int calculateHeuristics(const AStarPoint&) const;
public:
	AStar(const Field* field, const FieldMember* s, const FieldMember* g) {
		this->field = field;
		start = new AStarPoint(s);
		goal = new AStarPoint(g);
	}

	const vector<Point> solve();

	~AStar() {
		delete start;
		delete goal;
	}
};

#endif /* ASTAR_H_ */
