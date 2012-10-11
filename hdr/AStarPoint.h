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
			G = parent->G + FieldMember::METRIC_NORMAL;
		} else {
			G = 0;
		}
	}

	AStarPoint(const AStarPoint& orig) {
		pCell = orig.pCell;
		pParent = orig.pParent;
		G = orig.G;
		H = orig.H;
	}

	int getPathCost() const {
		return (G + H);
	}

	int getGeneralCost() const {
		return G;
	}

	// при задании нового родителя необходимо пересчитать метрику G
	void setParent(const AStarPoint& parent) {
		pParent = &parent;
		G = parent.G + FieldMember::METRIC_NORMAL;
	}

	const AStarPoint* getParent() const {
		return pParent;
	}

	const FieldMember* getCell() const {
		return pCell;
	}

	bool operator== (const AStarPoint& x) const{
		return (pCell == x.pCell); // равны, если указывают на один и тот же FieldMember
	}

/*	virtual ~AStarPoint() {
		delete pParent;
	}*/
};

#endif /* ASTARPOINT_H_ */
