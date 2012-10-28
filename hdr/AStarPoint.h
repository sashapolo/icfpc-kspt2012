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
	const Field* pField;
	const FieldMember* pCell;
	int G;	// стоимость
	int H;  // эвристика
	string path;	// путь до этой точки

public:

	AStarPoint(const Field*, const FieldMember*,
			int cost = 0, int heuristicsValue = 0,
			string path = "", string move = "");
	AStarPoint(const AStarPoint& orig);

	bool isGoalReached() const;

	const Field* getField() const;

	string getPath() const;

	int getPathCost() const;

	int getGeneralCost() const;
	void setGeneralCost(int G);

	int getHeuristics() const;
	void setHeuristics(int H);

	const FieldMember* getCell() const;

	bool operator==(const AStarPoint& x) const;
	bool operator>(const AStarPoint& x) const;
};

#endif /* ASTARPOINT_H_ */
