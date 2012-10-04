/**
 * Represents field.
 */

#ifndef FIELD_H
#define FIELD_H

#include <string>
#include <vector>

#include "FieldMember.h"

using namespace std;

class Field {

public:

    /**
     * Trivial constructor: makes field cell of given coordinate with given cell type.
     * Lines in the map being passed are of variable length.
     * Horizontal size of the map is determined by the line of the maximum length.
     * All other lines are padded with empty cells.
     *
     * param ASCIIMap Map in ASCII. See specification for details.
     */
    Field(const string &ASCIIMap);
    /**
     * Copy constructor makes a copy of the object referenced by a given reference.
     *
     * param orig reference to the object being copied
     */
    Field(const Field& orig);
    virtual ~Field();

    // ToDo: Really point?
    /**
     * There are caches of Lambdas and Stones to provide some assistance to algorithm.
     * Deletes a Lambda with specified coordinates from the list.
     */
    // ToDo If given cell is not a lambda - throw exception?
    void deleteLambdaFromCache(const Point &rPoint);
    /**
     * There are caches of Lambdas and Stones to provide some assistance to algorithm.
     * Deletes a Lambda with specified coordinates from the list.
     */
    // ToDo If given cell is not a stone - throw exception?
    void deleteStoneFromCache(const Point &rPoint);

    /**
     * Returns cell with given coordinates.
     *
     * param pPoint pointer to the point object representing coordinates of the cell to retrieve
     */
    FieldMember* getXY(const Point &rPoint);

    pair<int, int> getSize();

    // ToDo: write in doc comments that Points are immutable.
    /**
     * Swaps contents of the two cells.
     *
     * @param rCell1 coordinates of the first cell
     * @param rCell2 coordinates of the second cell
     */
    void swap(const Point &rCell1, const Point &rCell2);

    /**
     * Returns type of the cell with given coordinates.
     *
     * @param rPoint
     * @return 
     */
    CellType getCellType(const Point &rPoint);

    /**
     * Writes given field member into position, specified by coordinates contained in field member.
     *
     * @param rFieldMember
     */
    // ToDo: need in throwing any exceptions?
    void setFieldMember(FieldMember fieldMember);

private:

    vector< vector<FieldMember> > field;

};

#endif	/* FIELD_H */