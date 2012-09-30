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
    delete pCoordinate;
}

void FieldMember::setMetric(int metric) {
    this->metric = metric;
}