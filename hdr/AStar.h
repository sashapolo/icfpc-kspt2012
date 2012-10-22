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

using namespace std;
using namespace AStarNode;

class AStar {
protected:
	const FieldSim fieldSim;
	AStarPoint *const start;
	FieldMember *const goal;

	priority_queue<AStarPoint*, vector<AStarPoint*>, PointerComparatorMore<AStarPoint*> > openedList;
	set<AStarPoint*> closedList;

	bool isInClosedList(AStarPoint* const) const;
	void checkPoint(Point point, const AStarPoint& current, string move);
	void addNeighboursToOpenedList(const AStarPoint&);
	virtual int calculateHeuristics(const AStarPoint&) const;

public:
	AStar(Field* const, FieldMember* const, FieldMember* const);
	string solve(Field**);
	virtual ~AStar();
};

#endif /* ASTAR_H_ */
