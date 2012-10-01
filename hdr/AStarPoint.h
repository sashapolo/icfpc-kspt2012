/*
 * AStarPoint.h
 *
 *  Created on: Oct 1, 2012
 *      Author: alexander
 */

#ifndef ASTARPOINT_H_
#define ASTARPOINT_H_

#include <cstddef>

class AStarPoint {
private:
	const FieldMember *pCell;
	const AStarPoint *pParent;
	int G;	// стоимость
	int H;	// эвристика

public:
	AStarPoint(const FieldMember *cell, const AStarPoint *parent = NULL) {
		pCell = cell;
		pParent = parent;
		H = 0;
		if (parent) {
			G = parent->G + cell->getMetric();
		} else {
			G = 0;
		}
	}

	int getPathCost() const {
		return (G + H);
	}

	int getGeneralCost() const {
		return G;
	}

	void setParent(const AStarPoint& parent) {
		pParent = &parent;
		G = parent.G + pCell->getMetric();
	}

	const AStarPoint* getParent() const {
		return pParent;
	}

	const FieldMember* getCell() const {
		return pCell;
	}

	void setHeuristics(int value) {
		H = value;
	}

	bool operator== (const AStarPoint& x) {
		return (pCell == x.pCell);
	}
};

#endif /* ASTARPOINT_H_ */
