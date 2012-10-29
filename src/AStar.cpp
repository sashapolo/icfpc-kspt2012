/**
 * AStar.cpp
 *
 *  Created on: Sep 26, 2012
 *      Author: alexander
 */

#include "AStar.h"

#include <iostream>

AStar::AStar(const Field* pField, FieldMember* s, Heuristic* h)
				: fieldSim(), start(new AStarPoint(pField, s, 0, 1)) {
	this->h = h;
	openedList.push(start);
}

AStar::~AStar() {
	delete start;
}


string AStar::solve(Field** pResultField) {
    /**
     * Поиск пути.
     * @param pResultField - todo
     * @return путь поиска лямбд
     */
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
    /**
     * Добавление следующего хода в путь.
     * @param AStarPoint& current - текущая точка
     */
	int x = current.getCell()->getCoordinate().x;
	int y = current.getCell()->getCoordinate().y;
	checkPoint(Point(x, y + 1), current, "D");
	checkPoint(Point(x, y - 1), current, "U");
	checkPoint(Point(x - 1, y), current, "L");
	checkPoint(Point(x + 1, y), current, "R");
}


void AStar::checkPoint(Point point, const AStarPoint& current, string move) {
    /**
     * todo
     * @param Point point - точка назначения(???)
     * @param AStarPoint& current - текущая точка
     * @param string move - строка последовательности ходов
     */
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
		result->setHeuristics(h->calculate(*result));
		if (!isInClosedList(result)) {
			openedList.push(result);
		}
	}
}


bool AStar::isInClosedList(const AStarPoint* pCurrent) const {
    /**
     * Поиск возможности хода.
     * @param AStarPoint* pCurrent - текущая точка
     * @return true, если текущая точка не является последней допустимой; false - в протианом случае.
     */
	set<AStarPoint*>::const_iterator it = find_if(closedList.begin(), closedList.end(),
			bind2nd(Comparators::PointerComparatorEquals<AStarPoint*>(), pCurrent));
	return (it != closedList.end());
}
