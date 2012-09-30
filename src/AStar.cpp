/*
 * AStar.cpp
 *
 *  Created on: Sep 26, 2012
 *      Author: alexander
 */

#include "AStar.h"

void AStar::addToOpenedList(const AStarPoint& p) {
	list<AStarPoint>::iterator it = openedList.begin();
	list<AStarPoint>::iterator end = openedList.end();
	for (; it != end; it++) {
		if (it->cell->getMetric() >= p.cell->getMetric()) {
			openedList.insert(it, p);
			return;
		}
	}
	openedList.push_back(p);
}

// Реализация алгоритма А*
Path AStar::solve(const Point& s, const Point& finish, const Field& field) {
	AStarPoint start;
	start.cell = field.getXY(s);
	start.parent = NULL;
	addToOpenedList(start);

	field.getXY(Point(s.getX(), s.getY() + 1));
	field.getXY(Point(s.getX(), s.getY() - 1));
	field.getXY(Point(s.getX() + 1, s.getY()));
	field.getXY(Point(s.getX() - 1, s.getY()));

	openedList.remove(start);


}
