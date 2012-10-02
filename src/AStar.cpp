/*
 * AStar.cpp
 *
 *  Created on: Sep 26, 2012
 *      Author: alexander
 */

#include "AStar.h"
#include <algorithm>

// Реализация алгоритма А*
vector<Point> AStar::solve(const Point& s, const Point& f, const Field& field) {
	AStarPoint finish(field.getXY(f));
	AStarPoint start(field.getXY(s), NULL, &finish);
	addNeighboursToOpenedList(start, finish, field);
	closedList.push_back(start);

	while (!isInOpenedList(finish) || !openedList.empty()) {
		AStarPoint current = openedList.front();
		openedList.pop_front();
		addNeighboursToOpenedList(current, finish, field);
		closedList.push_back(current);
	}

	vector<Point> result;
	if (openedList.empty()) {
		return result;
	}
	const AStarPoint *pCurrent = &finish;
	while (!pCurrent) {
		result.push_back(*(pCurrent->getCell()->getCoordinate()));
		pCurrent = pCurrent->getParent();
	}
	return result;
}

void AStar::addToOpenedList(AStarPoint param) {
	list<AStarPoint>::iterator it = openedList.begin();
	list<AStarPoint>::iterator end = openedList.end();
	int cost = param.getPathCost();
	for (; it != end; it++) {
		if (it->getPathCost() >= cost) {
			openedList.insert(it, param);
			return;
		}
	}
	openedList.push_back(param);
}

bool AStar::isInClosedList(const AStarPoint& param) const {
	list<AStarPoint>::const_iterator it = std::find(closedList.begin(), closedList.end(), param);
	return (it != closedList.end());
}

bool AStar::isInOpenedList(const AStarPoint& param) const {
	list<AStarPoint>::const_iterator it = std::find(openedList.begin(), openedList.end(), param);
	return (it != openedList.end());
}

// ToDo: оптимизировать этот метод
void AStar::addNeighboursToOpenedList(const AStarPoint& parent, const AStarPoint& target, const Field& field) {
	int x = parent.getCell()->getCoordinate()->getX();
	int y = parent.getCell()->getCoordinate()->getY();
	AStarPoint up(field.getXY(Point(x, y + 1)), &parent, &target);
	checkPoint(up);
	AStarPoint down(field.getXY(Point(x, y - 1)), &parent, &target);
	checkPoint(down);
	AStarPoint right(field.getXY(Point(x + 1, y)), &parent, &target);
	checkPoint(right);
	AStarPoint left(field.getXY(Point(x - 1, y)), &parent, &target);
	checkPoint(left);
}

void AStar::checkPoint(const AStarPoint& point) {
	if (point.getCell()->isPassable() && !isInClosedList(point)) {
		list<AStarPoint>::const_iterator it = std::find(openedList.begin(), openedList.end(), point);
		if (it == openedList.end()) {
			addToOpenedList(point);
		} else {
			if (it->getGeneralCost() < point.getParent()->getGeneralCost() + METRIC_NORMAL) {
				AStarPoint newPoint = *it;
				newPoint.setParent(point);
				openedList.remove(*it);
				addToOpenedList(newPoint);
			}
		}
	}
}
