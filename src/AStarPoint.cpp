/*
 * AStarPoint.cpp
 *
 *  Created on: Oct 18, 2012
 *      Author: alexander
 */

#include "AStarPoint.h"

/**
 * todo
 * @param Field* field - ��������.
 * @param FieldMember* cell - ������������ ��������.
 * @param int cost - ������ ����������.
 * @param int heuristicsValue - ������������������.
 * @param string path - ��������.
 * @param string move - ������.
 * @return ���������� ��������.
 */
AStarPoint::AStarPoint(const Field* field, const FieldMember* cell,
		int cost, int heuristicsValue, std::string path, std::string move):
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
 * ������������������ ���� ����������.
 * @return true, �������� ������������������; false �� ������������������ ������������.
 */
bool AStarPoint::isGoalReached() const {
	//return pField->lambdaCacheEmpty();
	return H == 0;
}

/**
 * ������������������ ��������.
 * @return ��������.
 */
const Field* AStarPoint::getField() const {
	return pField;
}

/**
 * ������������������ ��������.
 * @return ��������.
 */
std::string AStarPoint::getPath() const {
	return path;
}

/**
 * ������������������ ������������������ ��������.
 * @return ���������������� ��������.
 */
int AStarPoint::getPathCost() const {
	return G + H;
}

/**
 * ������������������ �������������������� ������������������ ��������.
 * @return �������������������� ���������������� ��������.
 */
int AStarPoint::getGeneralCost() const {
	return G;
}

/**
 * ������������������ ������������������ ������������������ ��������.
 * @param int G - ������������������.
 */
void AStarPoint::setGeneralCost(int G) {
	this->G = G;
}

/**
 * ������������������ ������������������.
 * @return ������������������.
 */
int AStarPoint::getHeuristics() const {
	return H;
}

/**
 * ������������������ ������������������ ������������������.
 * @param int H - ������������������.
 */
void AStarPoint::setHeuristics(int H) {
	this->H = H;
}

/**
 * ������������������ ������������.
 * @return ������������.
 */
const FieldMember* AStarPoint::getCell() const {
	return pCell;
}

/**
 * ������������������ ���� ������������������.
 * @param AStarPoint& x - ������������ ��������.
 * @return true, �������� ������������������; false �� ������������������ ������������.
 */
bool AStarPoint::operator== (const AStarPoint& x) const {
	return (pCell->getCoordinate() == x.getCell()->getCoordinate());
}

/**
 * ������������������.
 * @param AStarPoint& x - ������������ ��������.
 * @return true, �������� ������������; false �� ������������������ ������������.
 */
bool AStarPoint::operator>(const AStarPoint& x) const {
	return (this->getPathCost() > x.getPathCost());
}
