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
	AStarPoint(const FieldMember *cell, const AStarPoint* parent = NULL, const AStarPoint* target = NULL) {
		pCell = cell;
		pParent = parent;
		if (target) {
			H = cell->getDistance(*target->pCell);
		} else {
			H = 0;
		}
		if (parent) {
			G = parent->G + METRIC_NORMAL;
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
		G = parent.G + METRIC_NORMAL;
	}

	const AStarPoint* getParent() const {
		return pParent;
	}

	const FieldMember* getCell() const {
		return pCell;
	}

	bool operator== (const AStarPoint& x) const{
		return (pCell == x.pCell);
	}
};

#endif /* ASTARPOINT_H_ */
