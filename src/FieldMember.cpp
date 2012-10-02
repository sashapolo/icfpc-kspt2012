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

void FieldMember::setMetric(int metric) {
    this->metric = metric;
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