/*
 * AStarPoint.cpp
 *
 *  Created on: Oct 18, 2012
 *      Author: alexander
 */

#include "AStarPoint.h"

/**
 * todo
 * @param Field* field - поле.
 * @param FieldMember* cell - клетка поля.
 * @param int cost - вес точки.
 * @param int heuristicsValue - евристика.
 * @param string path - путь.
 * @param string move - шаг.
 * @return новый путь.
 */
AStarPoint::AStarPoint(const Field* field, const FieldMember* cell,
		int cost, int heuristicsValue, string path, string move):
				pField(field), pCell(cell) {
	G = cost;
	H = heuristicsValue;
	this->path = path + move;
}

/**
 * todo
 * @param AStarPoint& orig - todo.
 */
AStarPoint::AStarPoint(const AStarPoint& orig): pField(orig.pField), pCell(orig.pCell) {
	G = orig.G;
	H = orig.H;
}

/**
 * Достигнут ли финиш.
 * @return true, если достигнут; false в противном случае.
 */
bool AStarPoint::isGoalReached() const {
	//return pField->lambdaCacheEmpty();
	return H == 0;
}

/**
 * Получение поля.
 * @return поле.
 */
const Field* AStarPoint::getField() const {
	return pField;
}

/**
 * Получение пути.
 * @return путь.
 */
string AStarPoint::getPath() const {
	return path;
}

/**
 * Получение стоимости пути.
 * @return стоимоть пути.
 */
int AStarPoint::getPathCost() const {
	return G + H;
}

/**
 * Получение глобальной стоимости пути.
 * @return глобальную стоимоть пути.
 */
int AStarPoint::getGeneralCost() const {
	return G;
}

/**
 * Получение установка стоимости пути.
 * @param int G - стоимость.
 */
void AStarPoint::setGeneralCost(int G) {
	this->G = G;
}

/**
 * Получение эвристики.
 * @return эвристику.
 */
int AStarPoint::getHeuristics() const {
	return H;
}

/**
 * Получение установка эвристики.
 * @param int H - эвристика.
 */
void AStarPoint::setHeuristics(int H) {
	this->H = H;
}

/**
 * Получение ячейки.
 * @return ячейку.
 */
const FieldMember* AStarPoint::getCell() const {
	return pCell;
}

/**
 * Сравнение на равенство.
 * @param AStarPoint& x - ячейка поля.
 * @return true, если совпадают; false в противном случае.
 */
bool AStarPoint::operator== (const AStarPoint& x) const {
	return (pCell->getCoordinate() == x.getCell()->getCoordinate());
}

/**
 * Сравнение.
 * @param AStarPoint& x - ячейка поля.
 * @return true, если больше; false в противном случае.
 */
bool AStarPoint::operator>(const AStarPoint& x) const {
	return (this->getPathCost() > x.getPathCost());
}
