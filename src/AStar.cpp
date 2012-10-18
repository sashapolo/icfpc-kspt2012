/*
 * AStar.cpp
 *
 *  Created on: Sep 26, 2012
 *      Author: alexander
 */

#include "AStar.h"
// Debug
#include <iostream>

AStar::AStar(const Field* pField, const FieldMember* s): fieldSim() {
	start = new AStarPoint(pField, s);
	openedList.push(*start);
}

AStar::~AStar() {
	delete start;
}

// Реализация алгоритма А*
const string AStar::solve() {
	const AStarPoint *current = start;
	while (!openedList.empty() && !current->isGoalReached()) {
		openedList.pop();
		if (!isInClosedList(*current)) {
			addNeighboursToOpenedList(*current);
			closedList.insert(*current);
		}
		current = &(openedList.top());
	}
	return current->getPath();
}


void AStar::addNeighboursToOpenedList(const AStarPoint& current) {
	int x = current.getCell()->getCoordinate().x;
	int y = current.getCell()->getCoordinate().y;
	checkPoint(Point(x, y+1), current, "D");
	checkPoint(Point(x, y-1), current, "U");
	checkPoint(Point(x-1, y), current, "L");
	checkPoint(Point(x+1, y), current, "R");
	// Debug
	cout<<"Point: "<<current.getCell()->getCoordinate().x<<";"<<current.getCell()->getCoordinate().y<<endl;
	cout<<"Field size: "<<current.getField()->getSize().first<<";"<<current.getField()->getSize().second<<endl;
}


void AStar::checkPoint(Point point, const AStarPoint& current, string move) {
	const FieldMember *tmp = current.getField()->getXY(point);
	if (tmp->isPassable()) {
		sSimResult res;
		Field* newField = fieldSim.CalcRobotSteps(current.getField(), move, &res);
		int newCost = current.getGeneralCost() + FieldMember::METRIC_NORMAL;
		AStarPoint result(newField, newField->getXY(point), newCost, 0, current.getPath(), move);
		result.setHeuristics(calculateHeuristics(result));
		openedList.push(result);
		// Debug
		cout<<"Point: "<<current.getCell()->getCoordinate().x<<";"<<current.getCell()->getCoordinate().y<<endl;
		cout<<"Field size: "<<current.getField()->getSize().first<<";"<<current.getField()->getSize().second<<endl;
	}
}


bool AStar::isInClosedList(const AStarPoint& current) const {
	return (closedList.find(current) != closedList.end());
}


int AStar::calculateHeuristics(const AStarPoint& current) const {
	list<FieldMember*>::const_iterator it = current.getField()->getLambdaCacheIt();
	int min = current.getField()->getDistance((*it++)->getCoordinate(), current.getCell()->getCoordinate());
	for (; it != current.getField()->getLambdaCacheEnd(); it++) {
		int tmp = current.getField()->getDistance((*it++)->getCoordinate(), current.getCell()->getCoordinate());
		if (min > tmp) {
			min = tmp;
		}
	}
	return min * 10;
}
