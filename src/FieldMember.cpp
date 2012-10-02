/*
 * Class represents a cell of the field.
 */

#include "FieldMember.h"

FieldMember::FieldMember(const Point *pCoordinate, CellType cellType) {
    this->pCoordinate = pCoordinate;
    this->cellType = cellType;
}

FieldMember::FieldMember(const FieldMember& orig) {
    this->cellType = orig.cellType;
    this->pCoordinate = orig.pCoordinate;
}

// ToDo: what if object pointed to is shared?
FieldMember::~FieldMember() {

}

bool FieldMember::isPassable() const {
    switch (cellType) {
        case EARTH:
        case WALL:
        case CLOSED_LIFT:
            return false;
        default:
            return true;
    }
}

int FieldMember::getDistance(const FieldMember& to) const {
	int x = pCoordinate->getX() - to.getCoordinate()->getX();
	int y = pCoordinate->getY() - to.getCoordinate()->getY();
	x = (x < 0)? -x : x;
	y = (y < 0)? -y : y;
	return x + y;
}

bool FieldMember::operator==(const FieldMember& param) const {
	return (cellType == param.cellType && *pCoordinate == *param.pCoordinate);
}
