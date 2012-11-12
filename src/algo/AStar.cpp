/**
 * AStar.cpp
 *
 *  Created on: Sep 26, 2012
 *      Author: alexander
 */

#include "algo/AStar.h"


AStar::AStar(const Field* pField, const Point* start, const Heuristic* h) {
	this->start = new AStarPoint(new Field(*pField), *start, 0, h->calculate(*start));
	this->h = h;
	openedList.push(this->start);
}


AStar::~AStar() {
	while (!openedList.empty()) {
		delete openedList.top();
		openedList.pop();
	}
	AStarClosedList::iterator it = closedList.begin();
	AStarClosedList::iterator end = closedList.end();
	for (; it != end; it++) {
		delete *it;
	}
}


bool AStar::isGoalReached(const AStarPoint& current) {
	return current.getHeuristics() == 0;
}


std::string AStar::solve(Field** pResultField) {
	AStarPoint *current = start;
	while (!openedList.empty() && !SignalHandler::sigIntReceived()) {
		if (isGoalReached(*current)) {
			*pResultField = new Field(*current->getField());
			return current->getPath();
		}
		openedList.pop();
		if (!isInClosedList(*current)) {
			addNeighboursToOpenedList(*current);
			closedList.push_front(current);
		}
		current = openedList.top();
	}
	return "";
}


void AStar::addNeighboursToOpenedList(const AStarPoint& current) {
	int x = current.getCell().x;
	int y = current.getCell().y;
	checkPoint(Point(x, y), current, 'W');
	checkPoint(Point(x, y + 1), current, 'D');
	checkPoint(Point(x, y - 1), current, 'U');
	checkPoint(Point(x - 1, y), current, 'L');
	checkPoint(Point(x + 1, y), current, 'R');
}


void AStar::checkPoint(const Point& point,
					   const AStarPoint& current,
					   char move) {
	if (current.getField()->isPassable(point)) {
		Field* newField = FieldSim::calcNextState(current.getField(), move);
		int newCost = current.getGeneralCost() + Field::METRIC_NORMAL;
		AStarPoint *result = new AStarPoint(newField,
											point,
											newCost,
											h->calculate(point),
											current.getPath(),
											move);
		if (!isInClosedList(*result)) {
			openedList.push(result);
		} else {
			delete result;
		}
	}
}


bool AStar::isInClosedList(const AStarPoint& current) const {
	AStarClosedList::const_iterator it = find_if(closedList.begin(),
			closedList.end(),
			bind2nd(Comparators::PointerComparatorEquals<AStarPoint*>(), &current));
	return (it != closedList.end());
}
