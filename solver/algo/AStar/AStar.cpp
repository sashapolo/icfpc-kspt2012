/**
 * AStar.cpp
 *
 *  Created on: Sep 26, 2012
 *      Author: alexander
 */

#include "algo/AStar/AStar.h"

AStar::AStar(const Field* pField, const Heuristic* h, const AStarGoal* g,
		const std::list<const Point*> &shittyLambdas) :
		shittyLambdas(shittyLambdas) {
	AStarPoint *start = new AStarPoint(new Field(*pField), *pField->getRobot(), 1, 0);
	openedList.push(start);
	this->h = h;
	this->goal = g;
	this->stateCheckLimit = pField->getSize().first * pField->getSize().second * 5;
}

AStar::~AStar() {
	while (!openedList.empty()) {
		delete openedList.top();
		openedList.pop();
	}
	AStarClosedList::iterator it = closedList.begin();
	while (it != closedList.end()) {
		delete *it;
		it = closedList.erase(it);
	}
}

bool AStar::isShitty(const Point& point) {
	std::list<const Point*>::const_iterator it = find_if(shittyLambdas.begin(),
			shittyLambdas.end(),
			bind2nd(Comparators::PointerComparatorEquals<const Point*>(),
					&point));
	return (it != shittyLambdas.end());
}

std::string AStar::solve(const Field** pResultField) {
	AStarPoint *current = openedList.top();
	while (!openedList.empty() && !SignalHandler::sigIntReceived()) {
		if (goal->isGoalReached(*current)) {
			if (pResultField != NULL) {
				*pResultField = new Field(*current->getField());
			}
			return current->getPath();
		}
		openedList.pop();
		if (!isInClosedList(*current)) {
			addNeighboursToOpenedList(*current);
			closedList.push_front(current);
		} else {
			delete current;
		}
		if (closedList.size() >= stateCheckLimit) {
			return "";
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

void AStar::checkPoint(const Point& point, const AStarPoint& current,
		char move) {
	if (current.getField()->isPassable(point)) {
		if (!(current.getField()->getXY(point) == LAMBDA && isShitty(point))) {
			const Field* newField = FieldSim::calcNextState(current.getField(),
					move);
			if (!newField->isRobotAlive()) {
				delete newField;
				return;
			}
			int newCost = current.getGeneralCost() + Field::METRIC_NORMAL;
			AStarPoint *result = new AStarPoint(newField, point, newCost, 0,
					current.getPath(), move);
			if (!isInClosedList(*result)) {
				result->setHeuristics(h->calculate(*result));
				openedList.push(result);
			} else {
				delete result;
			}
		}
	}
}

bool AStar::isInClosedList(const AStarPoint& current) const {
	AStarClosedList::const_iterator it = find_if(closedList.begin(),
			closedList.end(),
			bind2nd(Comparators::PointerComparatorEquals<AStarPoint*>(),
					&current));
	return (it != closedList.end());
}
