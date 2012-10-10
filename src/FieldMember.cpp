/*
 * Class represents a cell of the field.
 */

#include "FieldMember.h"

FieldMember::FieldMember(const Point& coordinate, CellType cellType): coordinate(coordinate) {
    this->cellType = cellType;
    switch (cellType) {
    case ROBOT:
    case STONE:
    case WALL:
    case CLOSED_LIFT:
    	this->metric = METRIC_INFINITY;
    	break;
    case EMPTY:
    case EARTH:
    case LAMBDA:
    case OPENED_LIFT:
    	this->metric = METRIC_NORMAL;
    	break;
    default:
    	this->metric = METRIC_NORMAL;  // просто так, на всякий случай
    	break;
    }
}

int FieldMember::getDistance(const FieldMember& to) const {
	int x = coordinate.x - to.getCoordinate().x;
	int y = coordinate.y - to.getCoordinate().y;
	x = (x < 0)? -x : x;
	y = (y < 0)? -y : y;
	return x + y;
}

