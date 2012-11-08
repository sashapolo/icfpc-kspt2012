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
#include "SignalHandler.h"

typedef std::priority_queue<AStarPoint*,
						    std::vector<AStarPoint*>,
						    Comparators::PointerComparatorMore<AStarPoint*> >
		AStarOpenedList;
typedef std::list<AStarPoint*> AStarClosedList;

/**
 * А*.<br />
 * Описывает алгоритм поиска А*.
 */

class AStar {
private:
	const FieldSim fieldSim;
	AStarPoint *start;
	Heuristic *h;

	AStarOpenedList openedList;
	AStarClosedList closedList;

	bool isInClosedList(AStarPoint*) const;
	void checkPoint(Point point, const AStarPoint& current, std::string move);
	void addNeighboursToOpenedList(const AStarPoint&);

public:
	AStar(const Field*, FieldMember*, Heuristic*);
	std::string solve(Field**);
	virtual ~AStar();
};

#endif /* ASTAR_H_ */
