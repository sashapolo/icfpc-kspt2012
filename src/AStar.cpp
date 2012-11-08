/**
 * AStar.cpp
 *
 *  Created on: Sep 26, 2012
 *      Author: alexander
 */

#include "AStar.h"


AStar::AStar(Field* pField, FieldMember* s, Heuristic* h)
				: fieldSim(), start(new AStarPoint(pField, s, 0, 1)) {
	this->h = h;
	openedList.push(start);
}

AStar::~AStar() {
	delete start;
	while (!openedList.empty()) {
		AStarPoint *result = openedList.top();
		delete result;
		openedList.pop();
	}
}

/**
 * Поиск пути.
 * @param Field** pResultField - состояние карты после прохождения пути.
 * @return путь поиска лямбд.
 */
std::string AStar::solve(Field** pResultField) {
	AStarPoint *current = start;
	while (!openedList.empty() && !SignalHandler::sigIntReceived()) {
		if (current->isGoalReached()) {
			*pResultField = current->getField();
			return current->getPath();
		}
		openedList.pop();
		if (!isInClosedList(current)) {
			addNeighboursToOpenedList(*current);
			closedList.push_front(current);
		}
		current = openedList.top();

	}
	return "";
}

    /**
     * Добавление соседей текущей точки.
     * @param AStarPoint& current - текущая точка.
     */
void AStar::addNeighboursToOpenedList(const AStarPoint& current) {
	int x = current.getCell()->getCoordinate().x;
	int y = current.getCell()->getCoordinate().y;
	checkPoint(Point(x, y), current, "W");
	checkPoint(Point(x, y + 1), current, "D");
	checkPoint(Point(x, y - 1), current, "U");
	checkPoint(Point(x - 1, y), current, "L");
	checkPoint(Point(x + 1, y), current, "R");
}

    /**
     * todo
     * @param Point point - анализируемая точка.
     * @param AStarPoint& current - текущая точка.
     * @param string move - ход робота.
     */
void AStar::checkPoint(Point point, const AStarPoint& current, std::string move) {
	const FieldMember *tmp = current.getField()->getXY(point);
	if (tmp->isPassable()) {
        sSimResult res;
		Field* newField = fieldSim.calcRobotSteps(current.getField(), move, &res);
		int newCost = current.getGeneralCost() + tmp->getMetric();
		AStarPoint *result = new AStarPoint(newField,
										newField->getXY(point),
										newCost,
										0,
										current.getPath(), move);
		result->setHeuristics(h->calculate(*result));
		if (!isInClosedList(result)) {
			openedList.push(result);
		} else {
			delete result;
		}
	}
}


/**
 * Поиск возможности хода.
 * @param AStarPoint* pCurrent - текущая точка
 * @return true, если текущая точка не является последней допустимой; false - в протианом случае.
 */
bool AStar::isInClosedList(AStarPoint* pCurrent) const {
	AStarClosedList::const_iterator it = find_if(closedList.begin(), closedList.end(),
			bind2nd(Comparators::PointerComparatorEquals<AStarPoint*>(), pCurrent));
	return (it != closedList.end());
}
