/*
 * AStarPoint.cpp
 *
 *  Created on: Oct 18, 2012
 *      Author: alexander
 */

#include "AStarPoint.h"

AStarPoint::AStarPoint(const Field* field, const FieldMember* cell,
		int cost, int heuristicsValue, string path, string move):
				pField(field), pCell(cell) {
	G = cost;
	H = heuristicsValue;
	this->path = path + move;
}

AStarPoint::AStarPoint(const AStarPoint& orig): pField(orig.pField), pCell(orig.pCell) {
	G = orig.G;
	H = orig.H;
}

bool AStarPoint::isGoalReached() const {
	//return pField->lambdaCacheEmpty();
	return H == 0;
}

const Field* AStarPoint::getField() const {
	return pField;
}

string AStarPoint::getPath() const {
	return path;
}

int AStarPoint::getPathCost() const {
	return G + H;
}

int AStarPoint::getGeneralCost() const {
	return G;
}

void AStarPoint::setGeneralCost(int G) {
	this->G = G;
}

int AStarPoint::getHeuristics() const {
	return H;
}

void AStarPoint::setHeuristics(int H) {
	this->H = H;
}

const FieldMember* AStarPoint::getCell() const {
	return pCell;
}

bool AStarPoint::operator== (const AStarPoint& x) const {
	return (pCell->getCoordinate() == x.getCell()->getCoordinate());
}

bool AStarPoint::operator>(const AStarPoint& x) const {
	return (this->getPathCost() > x.getPathCost());
}
