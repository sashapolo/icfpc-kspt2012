/*
 * Class represents a cell of the field.
 */

#include "FieldMember.h"

char cellTypeToChar(CellType type) {
    switch(type)
    {
        case ROBOT: return 'R';
        case STONE: return '*';
        case WALL:  return '#';
        case CLOSED_LIFT: return 'L';
        case EMPTY:     return ' ';
        case EARTH:     return '.';
        case LAMBDA:    return '\\';
        case OPENED_LIFT: return 'O';
    }
    return 'U';
};

CellType charToCellType(char c) {
    switch(c)
    {
        case 'R':  return ROBOT;
        case '*':  return STONE;
        case '#':  return WALL;
        case 'L':  return CLOSED_LIFT;
        case ' ':  return EMPTY;
        case '.':  return EARTH;
        case '\\': return LAMBDA;
        case 'O':  return OPENED_LIFT;
    }
    return EMPTY;
};

FieldMember::FieldMember(const Point& coordinate, CellType cellType): coordinate(coordinate) {
    this->cellType = cellType;
    setDefaultMetric();
}

int FieldMember::getDistance(const FieldMember& to) const {
	int x = coordinate.x - to.getCoordinate().x;
	int y = coordinate.y - to.getCoordinate().y;
	x = (x < 0)? -x : x;
	y = (y < 0)? -y : y;
	return x + y;
}

void FieldMember::setDefaultMetric()
{
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