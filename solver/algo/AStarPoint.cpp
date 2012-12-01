/*
 * AStarPoint.cpp
 *
 *  Created on: Oct 18, 2012
 *      Author: alexander
 */

#include "algo/AStar/AStarPoint.h"


AStarPoint::AStarPoint(const Field* pField,
					   const Point& cell,
					   int cost,
					   int heuristicsValue,
					   const std::string& path,
					   char move): cell(cell) {
	this->pField = pField;
	G = cost;
	H = heuristicsValue;
	pathCost = G + H;
	if (move == 0) {
		this->path = path;
	} else {
		this->path = path + move;
	}
}


AStarPoint::AStarPoint(const AStarPoint& orig): cell(orig.cell) {
	pField = new Field(*orig.pField);
	G = orig.G;
	H = orig.H;
	pathCost = orig.pathCost;
}


AStarPoint::~AStarPoint() {
	delete pField;
}


const Field* AStarPoint::getField() const {
	return pField;
}


const std::string& AStarPoint::getPath() const {
	return path;
}


int AStarPoint::getPathCost() const {
	return pathCost;
}


int AStarPoint::getGeneralCost() const {
	return G;
}


int AStarPoint::getHeuristics() const {
	return H;
}


const Point& AStarPoint::getCell() const {
	return cell;
}


bool AStarPoint::operator ==(const AStarPoint& x) const {
	return (*pField == *x.getField());
}


bool AStarPoint::operator >(const AStarPoint& x) const {
	return (pathCost > x.pathCost);
}

void AStarPoint::setHeuristics(int value) {
	H = value;
	pathCost = G + H;
}

bool AStarPoint::operator <(const AStarPoint& x) const {
	return (pathCost < x.pathCost);
}
