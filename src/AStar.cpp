/**
 * AStar.cpp
 *
 *  Created on: Sep 26, 2012
 *      Author: alexander
 */

#include "AStar.h"

#include <iostream>

AStar::AStar(Field* const pField,
			FieldMember* const s,
			FieldMember* const g)
				: fieldSim(), start(new AStarPoint(pField, s, 0, 1)), goal(g) {
	openedList.push(start);
}

AStar::~AStar() {
	delete start;
}


string AStar::solve(Field** pResultField) {
	AStarPoint *current = start;
	while (!openedList.empty()) {
		if (current->isGoalReached()) {
			*pResultField = new Field(*(current->getField()));
			return current->getPath();
		}
		openedList.pop();
		if (!isInClosedList(current)) {
			addNeighboursToOpenedList(*current);
			closedList.insert(current);
		}
		current = openedList.top();
	}
	return "";
}


void AStar::addNeighboursToOpenedList(const AStarPoint& current) {
	int x = current.getCell()->getCoordinate().x;
	int y = current.getCell()->getCoordinate().y;
	checkPoint(Point(x, y + 1), current, "D");
	checkPoint(Point(x, y - 1), current, "U");
	checkPoint(Point(x - 1, y), current, "L");
	checkPoint(Point(x + 1, y), current, "R");
}


void AStar::checkPoint(Point point, const AStarPoint& current, string move) {
	const FieldMember *tmp = current.getField()->getXY(point);
	if (tmp->isPassable()) {
        sSimResult res;
		Field* newField = fieldSim.calcRobotSteps(current.getField(), move, &res);
		int newCost = current.getGeneralCost() + tmp->getMetric();
		//TODO утечка?
		AStarPoint *result = new AStarPoint(newField,
										newField->getXY(point),
										newCost,
										0,
										current.getPath(), move);
		result->setHeuristics(calculateHeuristics(*result));
		if (!isInClosedList(result)) {
			openedList.push(result);
		}
	}
}


bool AStar::isInClosedList(AStarPoint* const pCurrent) const {
	set<AStarPoint*>::const_iterator it = find_if(closedList.begin(), closedList.end(),
			bind2nd(PointerComparatorEquals<AStarPoint*>(), pCurrent));
	return (it != closedList.end());
}



int AStar::calculateHeuristics(const AStarPoint& current) const {
	return current.getField()->getDistance(current.getCell()->getCoordinate(), goal->getCoordinate())
			* FieldMember::METRIC_NORMAL;
}
