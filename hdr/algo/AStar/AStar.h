/**
 * AStar.h
 *
 *  Created on: Sep 26, 2012
 *      Author: alexander
 */

#ifndef ASTAR_H_
#define ASTAR_H_

#include "stdinclude.h"
#include "algo/AStar/AStarPoint.h"
#include "algo/AStar/AStarGoal.h"
#include "algo/AStar/Heuristic.h"


typedef std::priority_queue<AStarPoint*,
						    std::vector<AStarPoint*>,
						    Comparators::PointerComparatorMore<AStarPoint*> >
		AStarOpenedList;
typedef std::list<AStarPoint*> AStarClosedList;

/**
 * А*.<br />
 * Описывает алгоритм поиска А*.
 */

class AStar {
private:
	const Heuristic *h;
	const AStarGoal *goal;
	AStarOpenedList openedList;
	AStarClosedList closedList;

	/**
	 * Поиск возможности хода.
	 * @param AStarPoint* pCurrent - текущая точка
	 * @return true, если текущая точка не является последней допустимой; false - в протианом случае.
	 */
	bool isInClosedList(const AStarPoint&) const;
	/**
	 * todo
	 * @param Point point - анализируемая точка.
	 * @param AStarPoint& current - текущая точка.
	 * @param string move - ход робота.
	 */
	void checkPoint(const Point& point,
					const AStarPoint& current,
					char move);
	/**
	 * Добавление соседей текущей точки.
	 * @param AStarPoint& current - текущая точка.
	 */
	void addNeighboursToOpenedList(const AStarPoint&);
	bool isGoalReached(const AStarPoint&);

public:
	AStar(const Field*, const Heuristic*, const AStarGoal*);
	~AStar();
	/**
	 * Поиск пути.
	 * @param Field** pResultField - состояние карты после прохождения пути.
	 * @return путь поиска лямбд.
	 */
	std::string solve(Field** pResultField);
};

#endif /* ASTAR_H_ */
