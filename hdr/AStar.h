/**
 * AStar.h
 *
 *  Created on: Sep 26, 2012
 *      Author: alexander
 */

#ifndef ASTAR_H_
#define ASTAR_H_

#include <algorithm>
#include <list>
#include "Field.h"
#include "AStarPoint.h"
#include "Path.h"

using namespace std;

/**
 * AStar.h
 *
 *  Created on: Sep 26, 2012
 *      Author: alexander
 */
class AStar {
private:
	AStarPoint *goal;
protected:
	AStarPoint *start;
	const Field *field;

	//ToDo: реализовать бинарную кучу для открытого списка
	list<AStarPoint> openedList; //открытый список отсортирован
	list<AStarPoint> closedList;

	void addToOpenedList(const AStarPoint&);
	bool isInClosedList(const AStarPoint&) const;
	bool isInOpenedList(const AStarPoint&) const;
	void checkPoint(const AStarPoint&, const AStarPoint&);
	virtual void addNeighboursToOpenedList(const AStarPoint&);
	Path constructPath(const AStarPoint*) const;
private:
	int calculateHeuristics(const AStarPoint&) const;

public:
	vector<Point> solve(const Point&, const Point&, const Field&);

	AStar(const Field* field, const FieldMember* s, const FieldMember* g = NULL) {
		this->field = field;
		start = new AStarPoint(s);
		if (goal != NULL) {
			goal = new AStarPoint(g);
		} else {
			goal = NULL;
		}
	}

	const Path solve();

	virtual ~AStar() {
		delete start;
		delete goal;
	}
};

#endif /* ASTAR_H_ */
