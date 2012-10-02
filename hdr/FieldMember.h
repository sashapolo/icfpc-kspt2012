/*
 * Class represents a cell of the field.
 */

#ifndef FIELDMEMBER_H
#define FIELDMEMBER_H

#include "Point.h"

#define METRIC_NORMAL 10
#define METRIC_INFINITY 100

enum CellType {CLOSED_LIFT, EARTH, EMPTY, LAMBDA, OPEN_LIFT, ROBOT, ROCK, WALL};

class FieldMember {

public:

    /*
     * Trivial constructor: makes field cell of given coordinate with given cell type.
     *
     * param pCoordinate pointer to the coordinate object. Pointer is just assigned to internal pointer because object to which it points is immutable.
     */
    FieldMember(const Point *pCoordinate, CellType cellType);
    /*
     * Copy constructor makes a copy of the object referenced by a given reference.
     *
     * param orig reference to the object being copied
     */
    FieldMember(const FieldMember& orig);
    virtual ~FieldMember();

    /*
     * Sets some metric used by algorithm.
     */
    void setMetric(int metric);

    bool isPassable() const;

    const Point* getCoordinate() const {
        return pCoordinate;
    };

    void setCoordinate(const Point &point) {
        this->pCoordinate = &point;
    }

    CellType getType() const{
        return this->cellType;
    }

    int getDistance(const FieldMember&) const;

    bool operator==(const FieldMember&) const;
private:

    // Referenced object is immutable
    const Point* pCoordinate;
    CellType cellType;

};

#endif	/* FIELDMEMBER_H */
