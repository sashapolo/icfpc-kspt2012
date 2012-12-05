/*
 * AStarPoint.cpp
 *
 *  Created on: Oct 18, 2012
 *      Author: alexander
 */

#include "AStarPoint.h"


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
