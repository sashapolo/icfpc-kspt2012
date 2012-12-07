/*
 * AStarPoint.h
 *
 *  Created on: Oct 1, 2012
 *      Author: alexander
 */

#ifndef ASTARPOINT_H_
#define ASTARPOINT_H_

#include "../../base/Field.h"

/**
 * А* Point.<br />
 * Описывает точку, используемую в алгоритме А*.
 */
class AStarPoint {
private:
	/**
	 * Карта.
	 */
	const Field* pField;
	/**
	 * Клетка карты.
	 */
	Point cell;
	/**
	 * Стоимость.
	 */
	int G;
	/**
	 * Значение эвристики.
	 */
	int H;
	/**
	 * Стоимость пути.
	 */
	int pathCost;
	/**
	 * Путь до точки.
	 */
	std::string path;

public:
	/**
	 * Конструктор класса AStarPoint
	 * @param Field* field - карта.
	 * @param FieldMember* cell - клетка карты.
	 * @param int cost - вес точки.
	 * @param int heuristicsValue - значение эвристики.
	 * @param string path - путь.
	 * @param string move - шаг.
	 */
	AStarPoint(const Field* pField, const Point& cell, int cost = 0,
			int heuristicsValue = 0, const std::string& path = "",
			char move = 0);
	/**
	 * todo
	 * @param AStarPoint& orig - todo.
	 */
	AStarPoint(const AStarPoint& orig);
	/**
	 * Деструктор класса AStarPoint.
	 */
	virtual ~AStarPoint() {
		delete pField;
	}

	/**
	 * Получение поля.
	 * @return поле.
	 */
	const Field* getField() const {
		return pField;
	}
	/**
	 * Получение ячейки.
	 * @return ячейку.
	 */
	const Point& getCell() const {
		return cell;
	}
	/**
	 * Получение пути.
	 * @return путь.
	 */
	const std::string& getPath() const {
		return path;
	}
	/**
	 * Получение стоимости пути.
	 * @return стоимоть пути.
	 */
	int getPathCost() const {
		return pathCost;
	}
	/**
	 * Получение глобальной стоимости пути.
	 * @return глобальную стоимоть пути.
	 */
	int getGeneralCost() const {
		return G;
	}
	/**
	 * Получение эвристики.
	 * @return эвристику.
	 */
	int getHeuristics() const {
		return H;
	}
	/**
	 * Установка значения эвристики.
	 * @param int - значение эвристики.
	 */
	void setHeuristics(int value) {
		H = value;
		pathCost = G + H;
	}

	/**
	 * Сравнение на равенство.
	 * @param AStarPoint& x - ячейка поля.
	 * @return true, если совпадают; false в противном случае.
	 */
	bool operator ==(const AStarPoint& x) const {
		return (*pField == *x.getField());
	}
	/**
	 * Сравнение (больше).
	 * @param AStarPoint& x - клетка карты.
	 * @return true, если больше; false в противном случае.
	 */
	bool operator >(const AStarPoint& x) const {
		return (pathCost > x.pathCost);
	}
	/**
	 * Сравнение (меньше).
	 * @param AStarPoint& x - клетка карты.
	 * @return true, если меньше; false в противном случае.
	 */
	bool operator <(const AStarPoint& x) const {
		return (pathCost < x.pathCost);
	}
};

#endif /* ASTARPOINT_H_ */
