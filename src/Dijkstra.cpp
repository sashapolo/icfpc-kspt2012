/*
 * Dijkstra.cpp
 *
 *  Created on: Oct 17, 2012
 *      Author: alexander
 */

#include "Dijkstra.h"

//Dijkstra temporally deprecated

//void Dijkstra::addNeighboursToOpenedList(const AStarPoint& current) {
//	int x = current.getCell()->getCoordinate().x;
//	int y = current.getCell()->getCoordinate().y;
//	// ToDo: убрать здесь утечку
//	AStarPoint *newPoint = new AStarPoint(current);
//	AStarPoint down(field->getXY(Point(x, y + 1)), newPoint);
//	checkPoint(down, *newPoint);
//	AStarPoint up(field->getXY(Point(x, y - 1)), newPoint);
//	checkPoint(up, *newPoint);
//	AStarPoint right(field->getXY(Point(x + 1, y)), newPoint);
//	checkPoint(right, *newPoint);
//	AStarPoint left(field->getXY(Point(x - 1, y)), newPoint);
//	checkPoint(left, *newPoint);
//}
//
//
//vector<Path> Dijkstra::findAllPaths() {
//	addNeighboursToOpenedList(*start); // добавляем соседей в открытый список
//	closedList.push_back(*start); //добавляем стартовую точку в закрытый список
//	while (!openedList.empty() && !goal.empty()) {
//		list<const FieldMember*>::iterator goalIt = goal.begin();
//		for (; goalIt != goal.end(); goalIt++) {
//			list<AStarPoint>::const_iterator it = std::find(openedList.begin(), openedList.end(), AStarPoint(*goalIt));
//			if (it != openedList.end()) {
//				result.push_back(constructPath(&(*it)));
//				goalIt = goal.erase(goalIt);
//			}
//		}
//		// Выбираем точку с наименьшей стоимостью пути
//		AStarPoint current = openedList.front();
//		openedList.pop_front(); // удаляем из открытого списка и добавляем в закрытый
//		addNeighboursToOpenedList(current);
//		closedList.push_back(current);
//	}
//	return result;
//}
//
//
//Path Dijkstra::findNearestPath() {
//	addNeighboursToOpenedList(*start); // добавляем соседей в открытый список
//	closedList.push_back(*start); //добавляем стартовую точку в закрытый список
//	while (!openedList.empty() && !goal.empty()) {
//		list<const FieldMember*>::iterator goalIt = goal.begin();
//		for (; goalIt != goal.end(); goalIt++) {
//			list<AStarPoint>::const_iterator it = std::find(openedList.begin(), openedList.end(), AStarPoint(*goalIt));
//			if (it != openedList.end()) {
//				return constructPath(&(*it));
//			}
//		}
//		// Выбираем точку с наименьшей стоимостью пути
//		AStarPoint current = openedList.front();
//		openedList.pop_front(); // удаляем из открытого списка и добавляем в закрытый
//		addNeighboursToOpenedList(current);
//		closedList.push_back(current);
//	}
//	return Path();
//}
