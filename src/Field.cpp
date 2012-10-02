/*
 * Represents field.
 */

#include <string>

#include "Field.h"

using namespace std;

Field::Field(const string &ASCIIMap) {
    
}

Field::Field(const Field& orig) {
}

Field::~Field() {
}

void Field::deleteLambdaFromCache(const Point &rPoint) {
    
}

void Field::deleteStoneFromCache(const Point &rPoint) {
    
}

FieldMember* Field::getXY(const Point &point) {
    return &field.at(point.getY()).at(point.getX());
}

void Field::swap(const Point &cell1, const Point &cell2) {
    FieldMember* tmp1 = getXY(cell1);
    tmp1->setCoordinate(cell2);
    FieldMember* tmp2 = getXY(cell2);
    tmp2->setCoordinate(cell1);
    setFieldMember(*tmp1);
    setFieldMember(*tmp2);
}

CellType Field::getCellType(const Point &point) {
    return getXY(point)->getType();
}

void Field::setFieldMember(FieldMember fieldMember) {
    int x = fieldMember.getCoordinate()->getX();
    int y = fieldMember.getCoordinate()->getY();
    field.at(y).at(x) = fieldMember;
}