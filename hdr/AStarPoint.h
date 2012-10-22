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

	template <class T>
	struct PointerComparatorMore: public binary_function<T, T, bool> {
		bool operator()(const T& pL, const T& pR) const {
			return (*pL > *pR);
		}
	};

	template <class T>
	struct PointerComparatorEquals: public binary_function<T, T, bool> {
		bool operator()(const T& l, const T& r) const {
			return (*l == *r);
		}
	};
}

#endif /* ASTARPOINT_H_ */
