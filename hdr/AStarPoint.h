/*
 * AStarPoint.h
 *
 *  Created on: Oct 1, 2012
 *      Author: alexander
 */

#ifndef ASTARPOINT_H_
#define ASTARPOINT_H_

#include "Field.h"

namespace AStarNode {
	class AStarPoint {
	private:
		const Field *pField;
		const FieldMember *pCell;
		int G;	// стоимость
		int H;  // эвристика
		string path;	// путь до этой точки

	public:

		AStarPoint(const Field *pField, const FieldMember *cell, int cost = 0, int heuristicsValue = 0, string path = "", string move = "");
		AStarPoint(const AStarPoint& orig);

		bool isGoalReached() const;

		const Field* getField() const;

		string getPath() const;

		int getPathCost() const;

		int getGeneralCost() const;
		void setGeneralCost(int G);

		void setHeuristics(int H);

		const FieldMember* getCell() const;

		bool operator== (const AStarPoint& x) const;
	//	bool operator== (const AStarPoint* x) const;
	//	bool operator()(const AStarPoint& a1, const AStarPoint& a2);
	//	bool operator<(const AStarPoint* a) const;
	};

	struct AStarPointComparator: public binary_function< AStarPoint*,AStarPoint*,bool > {
		bool operator()(const AStarPoint* pL, const AStarPoint* pR) const {
			return (pL->getPathCost() > pR->getPathCost());
		}
	};

	template <class T>
	struct Comparator: public binary_function<T,T,bool> {
		bool operator()(const T& l, const T& r) const {
			return (*l == *r);
		}
	};
}

#endif /* ASTARPOINT_H_ */
