/*
 * Class represents a cell of the field.
 */

#ifndef FIELDMEMBER_H
#define FIELDMEMBER_H

#include <climits>

#include "Point.h"

enum CellType {CLOSED_LIFT, EARTH, EMPTY, LAMBDA, OPENED_LIFT, ROBOT, STONE, WALL};

char cellTypeToChar(CellType type);
CellType charToCellType(char c);


class FieldMember {
private:

    Point coordinate;
    CellType cellType;
    int metric;

public:

    const static int METRIC_NORMAL = 10;
    const static int METRIC_MEDIUM = 50;        //for bad places (metric - ?)
    const static int METRIC_INFINITY = INT_MAX;

    FieldMember(const Point& coordinate, CellType cellType);
    FieldMember(const Point& coordinate, CellType cellType, int metric): coordinate(coordinate) {
    	this->cellType = cellType;
    	this->metric = metric;
    }

    /*
     * Sets some metric used by algorithm.
     */
    void setMetric(int metric) {
    	this->metric = metric;
    }
    
    int getMetric() const {
        return this->metric;
    }
    
    void setDefaultMetric();

    bool isPassable() const {
    	return (metric == METRIC_NORMAL);
    }

    Point getCoordinate() const {
        return coordinate;
    };

    void setCoordinate(const Point &point) {
        this->coordinate = point;
    }

    CellType getType() const{
        return this->cellType;
    }
};

#endif	/* FIELDMEMBER_H */
