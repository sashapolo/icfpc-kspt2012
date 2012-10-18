/*
 * AStarPoint.cpp
 *
 *  Created on: Oct 18, 2012
 *      Author: alexander
 */

#include "AStarPoint.h"
using namespace AStarNode;

AStarPoint::AStarPoint(const Field *pField, const FieldMember *cell, int cost, int heuristicsValue, string path, string move) {
	this->pField = pField;
	pCell = cell;
	G = cost;
	H = heuristicsValue;
	this->path = path + move;
}

AStarPoint::AStarPoint(const AStarPoint& orig) {
	pField = orig.pField;
	pCell = orig.pCell;
	G = orig.G;
	H = orig.H;
}

bool AStarPoint::isGoalReached() const {
	//return pField->lambdaCacheEmpty();
	return (this->pCell->getCoordinate().x == 10 && this->pCell->getCoordinate().y == 3);
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

void AStarPoint::setHeuristics(int H) {
	this->H = H;
}

const FieldMember* AStarPoint::getCell() const {
	return pCell;
}

bool AStarPoint::operator== (const AStarPoint& x) const {
	return (pCell->getCoordinate() == x.getCell()->getCoordinate());
}

//bool AStarPoint::operator== (const AStarPoint* x) const {
//	return (pCell->getCoordinate() == x->getCell()->getCoordinate());
//}
//
//
//bool AStarPoint::operator()(const AStarPoint& a1, const AStarPoint& a2) {
//	return (a1.getPathCost() > a2.getPathCost());
//}
//
//bool AStarPoint::operator<(const AStarPoint* a) const {
//	return (this->getPathCost() < a->getPathCost());
//}
