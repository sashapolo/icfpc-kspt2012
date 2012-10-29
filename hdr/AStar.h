/**
 * AStar.h
 *
 *  Created on: Sep 26, 2012
 *      Author: alexander
 */

#ifndef ASTAR_H_
#define ASTAR_H_

#include <queue>
#include <set>
#include <algorithm>
#include "AStarPoint.h"
#include "FieldSim.h"
#include "Heuristic.h"
#include "Comparators.h"

using namespace std;

/**
 * А*.<br />
 * Описывает алгоритм поиска А*.
 */

class AStar {
private:
	const FieldSim fieldSim;
	AStarPoint *start;
	Heuristic *h;

	priority_queue<AStarPoint*, vector<AStarPoint*>,
					Comparators::PointerComparatorMore<AStarPoint*> > openedList;
	set<AStarPoint*> closedList;

	bool isInClosedList(const AStarPoint*) const;
	void checkPoint(Point point, const AStarPoint& current, string move);
	void addNeighboursToOpenedList(const AStarPoint&);

public:
	AStar(const Field*, FieldMember*, Heuristic*);
	string solve(Field**);
	virtual ~AStar();
};

#endif /* ASTAR_H_ */
