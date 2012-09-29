/*
 * Represents field.
 */

#ifndef FIELD_H
#define FIELD_H

#include <string>

#include "FieldMember.h"

using namespace std;

class Field {

public:

    /*
     * Trivial constructor: makes field cell of given coordinate with given cell type.
     *
     * param ASCIIMap Map in ASCII. See specification for details.
     */
    Field(string& ASCIIMap);
    /*
     * Copy constructor makes a copy of the object referenced by a given reference.
     *
     * param orig reference to the object being copied
     */
    Field(const Field& orig);
    virtual ~Field();

    /*
     * Returns cell with given coordinates.
     *
     * param pPoint pointer to the point object representing coordinates of the cell to retrieve
     */
    FieldMember getXY(Point *pPoint);

    
    // ToDo: Really point?
    /*
     * There are caches of Lambdas and Stones to provide some assistance to algorithm.
     * Deletes a Lambda with specified coordinates from the list.
     */
    // ToDo If given cell is not a lambda - throw exception?
    void deleteLambdaFromCache(Point *pPoint);
    /*
     * There are caches of Lambdas and Stones to provide some assistance to algorithm.
     * Deletes a Lambda with specified coordinates from the list.
     */
    void deleteStoneFromCache(Point *pPoint);

private:

};

#endif	/* FIELD_H */