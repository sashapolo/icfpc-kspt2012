/*
 * AStarPoint.h
 *
 *  Created on: Oct 1, 2012
 *      Author: alexander
 */

#ifndef ASTARPOINT_H_
#define ASTARPOINT_H_

#include "Field.h"

class AStarPoint {
private:
	Field* const pField;
	FieldMember* const pCell;
	int G;	// стоимость
	int H;  // эвристика
	string path;	// путь до этой точки

public:

	AStarPoint(Field *const, FieldMember *const,
			int cost = 0, int heuristicsValue = 0,
			string path = "", string move = "");
	AStarPoint(const AStarPoint& orig);

	bool isGoalReached() const;

	Field* const getField() const;

	string getPath() const;

	int getPathCost() const;

	int getGeneralCost() const;
	void setGeneralCost(int G);

	int getHeuristics() const;
	void setHeuristics(int H);

	FieldMember* const getCell() const;

	bool operator==(const AStarPoint& x) const;
	bool operator>(const AStarPoint& x) const;
};

#endif /* ASTARPOINT_H_ */
