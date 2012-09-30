/*
 * AStar.h
 *
 *  Created on: Sep 26, 2012
 *      Author: alexander
 */

#ifndef ASTAR_H_
#define ASTAR_H_

#include <list>
#include "Path.h"
#include "Field.h"
#include "FieldMember.h"
#include "Algorithm.h"

class AStar: public Algorithm {
private:
	struct AStarPoint {
		FieldMember *cell;
		AStarPoint *parent;

		bool operator== (const AStarPoint& x) {
			return (this->cell == x.cell);
		};
	};

	//ToDo: реализовать бинарную кучу для открытого списка
	//открытый список отсортирован
	std::list<AStarPoint> openedList;
	std::list<AStarPoint> closedList;


public:
	Path solve(const Point&, const Point&, const Field&);
	void addToOpenedList(const AStarPoint&);
	~AStar();
};

#endif /* ASTAR_H_ */
