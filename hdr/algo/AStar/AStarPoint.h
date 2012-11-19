/*
 * AStarPoint.h
 *
 *  Created on: Oct 1, 2012
 *      Author: alexander
 */

#ifndef ASTARPOINT_H_
#define ASTARPOINT_H_

#include "base/Field.h"

class AStarPoint {
private:
	const Field* pField;
	Point cell;
	int G;	// стоимость
	int H;  // эвристика
	int pathCost;
	std::string path;	// путь до этой точки

public:

	AStarPoint(const Field* pField,
			   const Point& cell,
			   int cost = 0,
			   int heuristicsValue = 0,
			   const std::string& path = "",
			   char move = 0);
	AStarPoint(const AStarPoint& orig);
	virtual ~AStarPoint();

	/**
	 * Получение поля.
	 * @return поле.
	 */
	const Field* getField() const;
	/**
	 * Получение ячейки.
	 * @return ячейку.
	 */
	const Point& getCell() const;
	/**
	 * Получение пути.
	 * @return путь.
	 */
	const std::string& getPath() const;
	/**
	 * Получение стоимости пути.
	 * @return стоимоть пути.
	 */
	int getPathCost() const;
	/**
	 * Получение глобальной стоимости пути.
	 * @return глобальную стоимоть пути.
	 */
	int getGeneralCost() const;
	/**
	 * Получение эвристики.
	 * @return эвристику.
	 */
	int getHeuristics() const;

	void setHeuristics(int);

	/**
	 * Сравнение на равенство.
	 * @param AStarPoint& x - ячейка поля.
	 * @return true, если совпадают; false в противном случае.
	 */
	bool operator ==(const AStarPoint& x) const;
	/**
	 * Сравнение.
	 * @param AStarPoint& x - ячейка поля.
	 * @return true, если больше; false в противном случае.
	 */
	bool operator >(const AStarPoint& x) const;
	bool operator <(const AStarPoint& x) const;
};

#endif /* ASTARPOINT_H_ */
