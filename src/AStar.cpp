/*
 * AStar.cpp
 *
 *  Created on: Sep 26, 2012
 *      Author: alexander
 */

#include "AStar.h"

// Реализация алгоритма А*
const Path AStar::solve() {
	addNeighboursToOpenedList(*start); // добавляем соседей в открытый список
	closedList.push_back(*start); //добавляем стартовую точку в закрытый список
	while (!isInOpenedList(*goal) && !openedList.empty()) {
		// Выбираем точку с наименьшей стоимостью пути
		AStarPoint current = openedList.front();
		openedList.pop_front(); // удаляем из открытого списка и добавляем в закрытый
		addNeighboursToOpenedList(current);
		closedList.push_back(current);
	}

	// формирование результата
	if (openedList.empty()) {
		return Path();
	}
	list<AStarPoint>::const_iterator it = std::find(openedList.begin(), openedList.end(), *goal);
	const AStarPoint *pCurrent = &(*it);
	return constructPath(pCurrent);
}


void AStar::addToOpenedList(const AStarPoint& param) {
	list<AStarPoint>::iterator it = openedList.begin();
	list<AStarPoint>::iterator end = openedList.end();
	int cost = param.getPathCost();
	for (; it != end; it++) {
		if (cost <= it->getPathCost()) {
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


void AStar::addNeighboursToOpenedList(const AStarPoint& current) {
	int x = current.getCell()->getCoordinate().x;
	int y = current.getCell()->getCoordinate().y;
	// ToDo: убрать здесь утечку
	AStarPoint *newPoint = new AStarPoint(current);
	AStarPoint down(field->getXY(Point(x, y + 1)), newPoint);
	down.setHeuristics(calculateHeuristics(down));
	checkPoint(down, *newPoint);
	AStarPoint up(field->getXY(Point(x, y - 1)), newPoint);
	up.setHeuristics(calculateHeuristics(down));
	checkPoint(up, *newPoint);
	AStarPoint right(field->getXY(Point(x + 1, y)), newPoint);
	right.setHeuristics(calculateHeuristics(down));
	checkPoint(right, *newPoint);
	AStarPoint left(field->getXY(Point(x - 1, y)), newPoint);
	left.setHeuristics(calculateHeuristics(down));
	checkPoint(left, *newPoint);
}


void AStar::checkPoint(const AStarPoint& point, const AStarPoint& current) {
	if (point.getCell()->isPassable() && !isInClosedList(point)) {
		list<AStarPoint>::iterator it = std::find(openedList.begin(), openedList.end(), point);
		if (it == openedList.end()) {
			addToOpenedList(point);
		} else if (it->getGeneralCost() < current.getGeneralCost() + FieldMember::METRIC_NORMAL) {
			AStarPoint newPoint = *it;
			newPoint.setParent(current);
			openedList.erase(it);
			addToOpenedList(newPoint);
		}
	}
}


int AStar::calculateHeuristics(const AStarPoint& current) const {
	return field->getDistance(current.getCell()->getCoordinate(), goal->getCell()->getCoordinate());
}


Path AStar::constructPath(const AStarPoint *pBegin) const {
	Path result;
	for (;pBegin != NULL; pBegin = pBegin->getParent()) {
		result.addCell(pBegin->getCell()->getCoordinate());
	}
	return result;
}
