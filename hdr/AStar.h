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
	AStarPoint *start;
	const FieldSim fieldSim;
	const FieldMember *goal;

	priority_queue<AStarPoint*, vector<AStarPoint*>, AStarPointComparator> openedList;
	set<AStarPoint*> closedList;

	bool isInClosedList(const AStarPoint*) const;
	void checkPoint(Point point, const AStarPoint& current, string move);
	void addNeighboursToOpenedList(const AStarPoint&);
	virtual int calculateHeuristics(const AStarPoint&) const;
	bool findNewGoal(const Field* pField);

public:
	vector<Point> solve(const Point&, const Point&, const Field&);

	AStar(const Field* pField, const FieldMember* s);
	const string solve();
	virtual ~AStar();
};

#endif /* ASTAR_H_ */
