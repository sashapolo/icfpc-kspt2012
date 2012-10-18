/*
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
#include "Field.h"
#include "FieldSim.h"
#include "AStarPoint.h"

using namespace std;
using namespace AStarNode;

class AStar {
protected:
	AStarPoint *start;
	const FieldSim fieldSim;

	priority_queue<AStarPoint*, vector<AStarPoint*>, AStarPointComparator> openedList;
	set<const AStarPoint*> closedList;

	bool isInClosedList(const AStarPoint*) const;
	void checkPoint(Point point, const AStarPoint& current, string move);
	void addNeighboursToOpenedList(const AStarPoint&);
	virtual int calculateHeuristics(const AStarPoint&) const;

public:

	AStar(const Field* pField, const FieldMember* s);
	const string solve();
	virtual ~AStar();
};

#endif /* ASTAR_H_ */
