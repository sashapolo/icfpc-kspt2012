/**
 * Represents field.
 */

#include <string>

#include "Field.h"

using namespace std;

Field::Field(const string &ASCIIMap) {
    /*
     * Index used when filling vectors while reading the map.
     * At this stage vectors are variable length.
     * Further after determining the maximum length
     * all other vectors will be padded with empty cells.
     */
    int numOfVector = 0,       // Number of field line (Y coordinate)
        nestedVectorIndex = 0; // (X coordinate)
    // Reading the map
    for(int i = 0; i < ASCIIMap.length(); i++) {
        if(ASCIIMap[i] == '\n') {
            // Jump to the next line
            numOfVector++;
            nestedVectorIndex = 0;
        } else {
            switch (ASCIIMap[i]) {
                // ToDo: implement validation that there is only one robot in the map
                CellType cellType;
                case ('R'):    
                    cellType = ROBOT;
                    // ToDo: should be dynamically allocated?
                    field[numOfVector][nestedVectorIndex] = FieldMember(new Point(numOfVector, nestedVectorIndex), cellType);
                    break;
                case ('\\'):
                    cellType = LAMBDA;
                    // ToDo: Implement cache
                    // ToDo: should be dynamically allocated?
                    field[numOfVector][nestedVectorIndex] = FieldMember(new Point(numOfVector, nestedVectorIndex), cellType);
                    break;
                case ('#'):
                    cellType = WALL;
                    // ToDo: should be dynamically allocated?
                    field[numOfVector][nestedVectorIndex] = FieldMember(new Point(numOfVector, nestedVectorIndex), cellType);
                    break;
                case (' '):
                    cellType = EMPTY;
                    // ToDo: should be dynamically allocated?
                    field[numOfVector][nestedVectorIndex] = FieldMember(new Point(numOfVector, nestedVectorIndex), cellType);
                    break;
                case ('*'):
                    cellType = STONE;
                    // ToDo: Implement stone
                    // ToDo: should be dynamically allocated?
                    field[numOfVector][nestedVectorIndex] = FieldMember(new Point(numOfVector, nestedVectorIndex), cellType);
                    break;
                case ('L'):
                    cellType = CLOSED_LIFT;
                    // ToDo: should be dynamically allocated?
                    field[numOfVector][nestedVectorIndex] = FieldMember(new Point(numOfVector, nestedVectorIndex), cellType);
                    break;
                case ('O'):
                    cellType = OPENED_LIFT;
                    // ToDo: should be dynamically allocated?
                    field[numOfVector][nestedVectorIndex] = FieldMember(new Point(numOfVector, nestedVectorIndex), cellType);
                    break;
                default:
                    // ToDo: throw an exception
                    break;
                nestedVectorIndex++;
            }
        }
    }
    // Aligning the map
    // Searching the line with the maximum length
    int maxLen = 0;
    for(numOfVector = 0; numOfVector < field.size(); numOfVector) {
        if(maxLen < field[numOfVector].size() ) {
            maxLen = field[numOfVector].size();
        }
    }
    // Padding other lines with empty cells
    int sizeToPad, // Minor optimization: declaration is pulled out from cycle
        vectSize;  // Performance gain?
    for(numOfVector = 0; numOfVector < field.size(); numOfVector) {
        vectSize = field[numOfVector].size(); // Performance gain?
        if(vectSize != maxLen) {
            sizeToPad = maxLen - vectSize;
            for(int i = 0; i < sizeToPad; i++) {
                // ToDo: possible bug: +1?
                CellType cellType = WALL;
                field[numOfVector][vectSize + 1 + i] = FieldMember(new Point(numOfVector, nestedVectorIndex), cellType);
            }
        }
    }
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