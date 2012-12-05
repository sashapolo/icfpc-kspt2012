/**
 * AStar.h
 *
 *  Created on: Sep 26, 2012
 *      Author: alexander
 */

#ifndef ASTAR_H_
#define ASTAR_H_

#include "../../stdinclude.h"
#include "AStarPoint.h"
#include "AStarGoal.h"
#include "Heuristic.h"

/**
 * А*.<br />
 * Описывает алгоритм поиска А*.
 */

class AStar {
public:
	typedef std::priority_queue<AStarPoint*,
							    std::vector<AStarPoint*>,
							    Comparators::PointerComparatorMore<AStarPoint*> >
			AStarOpenedList;
	typedef std::list<AStarPoint*> AStarClosedList;
private:
	/**
     	 * Значение эвристики.
     	 */
	const Heuristic *h;
	/**
     	 * Цель.
     	 */
	const AStarGoal *goal;
	/**
     	 * Открытый список лямбд.
     	 */
	AStarOpenedList openedList;
	/**
     	 * Закрытый список лямбд.
     	 */
	AStarClosedList closedList;


	/**
	 * Кэш "плохих" лямбд. Лямбда считается плохой, если в процессе решения
	 * выяснилось, что эта лямбда приводит к ловушке.
	 */
	const std::list<const Point*>& shittyLambdas;
	/**
	 * todo.
	 * @param const Point& - текущая точка.
	 * @return todo.
	 */
	bool isShitty(const Point&);

	// подхак: ограничение, на количество перебираемых состояний
	// помогает от зацикливаний при недостижимости цели.
	// Вычисляется в зависимости от размера карты
	unsigned int stateCheckLimit;

	/**
	 * Поиск возможности хода.
	 * @param AStarPoint* pCurrent - текущая точка.
	 * @return true, если текущая точка не является последней допустимой; false - в протианом случае.
	 */
	bool isInClosedList(const AStarPoint&) const;
	/**
	 * todo
	 * @param Point point - анализируемая точка.
	 * @param AStarPoint& current - текущая точка.
	 * @param string move - ход робота.
	 */
	void checkPoint(const Point& point, const AStarPoint& current, char move);
	/**
	 * Добавление соседей текущей точки.
	 * @param AStarPoint& current - текущая точка.
	 */
	void addNeighboursToOpenedList(const AStarPoint&);
	/**
	 * Достигнута ли цель.
	 * @param const AStarPoint& - цель.
	 * @return true, если достигнута; false - в протианом случае.
	 */
	bool isGoalReached(const AStarPoint& current) const {
		return current.getHeuristics() == 0;
	}

public:
	/**
	 * Конструктор класса AStar.
	 * @param Field* pField - карта.
	 * @param FieldMember* s - элемент карты.
	 * @param Heuristic* h - значение эвристики.
	 */
	AStar(const Field*, const Heuristic*, const AStarGoal*,
		  const std::list<const Point*>&);
	/**
	 * Деструктор класса AStar.
	 * Удаляет точку старта и очищает результат.
	 */
	~AStar();
	/**
	 * Поиск пути.
	 * @param Field** pResultField - состояние карты после прохождения пути.
	 * @return путь поиска лямбд.
	 */
	std::string solve(const Field** pResultField = NULL);
};

#endif /* ASTAR_H_ */
