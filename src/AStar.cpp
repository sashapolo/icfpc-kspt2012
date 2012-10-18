/**
 * AStar.cpp
 *
 *  Created on: Sep 26, 2012
 *      Author: alexander
 */

#include "AStar.h"

#include <iostream>

AStar::AStar(const Field* pField, const FieldMember* s): fieldSim() {
	start = new AStarPoint(pField, s, 0, 1);
	goal = NULL;
	openedList.push(start);
}

AStar::~AStar() {
	delete start;
}

// Реализация алгоритма А*
const string AStar::solve() {
	if (!findNewGoal(start->getField())) {
		return "";
	}
	AStarPoint *current = start;
	while (!openedList.empty()) {
		if (current->isGoalReached()) {
			AStar astar(current->getField(), current->getCell());
			return current->getPath() + astar.solve();
		}
		openedList.pop();
		if (!isInClosedList(current)) {
			addNeighboursToOpenedList(*current);
			closedList.insert(current);
		}
		current = openedList.top();
	}
	return "A";
}


void AStar::addNeighboursToOpenedList(const AStarPoint& current) {
	int x = current.getCell()->getCoordinate().x;
	int y = current.getCell()->getCoordinate().y;
	checkPoint(Point(x, y+1), current, "D");
	checkPoint(Point(x, y-1), current, "U");
	checkPoint(Point(x-1, y), current, "L");
	checkPoint(Point(x+1, y), current, "R");
}


void AStar::checkPoint(Point point, const AStarPoint& current, string move) {
	const FieldMember *tmp = current.getField()->getXY(point);
	if (tmp->isPassable()) {
		sSimResult res;
		Field* newField = fieldSim.CalcRobotSteps(current.getField(), move, &res);
		int newCost = current.getGeneralCost() + tmp->getMetric();
		// утечка?
		AStarPoint *result = new AStarPoint(newField, newField->getXY(point), newCost, 0, current.getPath(), move);
		result->setHeuristics(calculateHeuristics(*result));
		if (!isInClosedList(result)) {
			openedList.push(result);
		}
	}
}


bool AStar::isInClosedList(const AStarPoint* pCurrent) const {
	set<AStarPoint*>::const_iterator it = find_if(closedList.begin(), closedList.end(),
			bind2nd(Comparator<const AStarPoint*>(), pCurrent));
	return (it != closedList.end());
}



int AStar::calculateHeuristics(const AStarPoint& current) const {
//	list<FieldMember*>::const_iterator it = current.getField()->getLambdaCacheIt();
//	int min = current.getField()->getDistance((*it++)->getCoordinate(), current.getCell()->getCoordinate());
//	for (; it != current.getField()->getLambdaCacheEnd(); it++) {
//		int tmp = current.getField()->getDistance((*it)->getCoordinate(), current.getCell()->getCoordinate());
//		if (min > tmp) {
//			min = tmp;
//		}
//	}
//	return min * 10;
	return current.getField()->getDistance(current.getCell()->getCoordinate(), goal->getCoordinate()) * 10;
}

bool AStar::findNewGoal(const Field* pField) {
	if (pField->lambdaCacheEmpty()) {
		return false;
	}
	list<FieldMember*>::const_iterator it = pField->getLambdaCacheIt();
	Point start = pField->getRobot()->getCoordinate();
	goal = *it;
	int min = pField->getDistance((*it++)->getCoordinate(), start);
	for (; it != pField->getLambdaCacheEnd(); it++) {
		int tmp = pField->getDistance((*it)->getCoordinate(), start);
		if (min > tmp) {
			min = tmp;
			goal = *it;
		}
	}
	return true;
}
