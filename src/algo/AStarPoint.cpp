/*
 * AStarPoint.cpp
 *
 *  Created on: Oct 18, 2012
 *      Author: alexander
 */

#include "algo/AStarPoint.h"


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


void AStarPoint::setGeneralCost(int G) {
	this->G = G;
	pathCost = G + H;
}


int AStarPoint::getHeuristics() const {
	return H;
}


void AStarPoint::setHeuristics(int H) {
	this->H = H;
	pathCost = G + H;
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


bool AStarPoint::operator <(const AStarPoint& x) const {
	return (pathCost < x.pathCost);
}
