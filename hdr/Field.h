/**
 * Represents field.
 */

#ifndef FIELD_H
#define FIELD_H

#include <string>
#include <vector>
#include <list>

#include "FieldMember.h"

using namespace std;

/**
 * Represents field.
 */
class Field {
private:
    vector< vector<FieldMember*> > field;
    list<FieldMember*> lambdaCache;
    list<FieldMember*> stoneCache;
    FieldMember* pRobot;
    FieldMember* pLift;

public:
    struct FieldParseException {}; // ислючение, говорящее о неправильности задания карты
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

    /**
     * There are caches of Lambdas and Stones to provide some assistance to algorithm.
     */
    list<FieldMember*>::const_iterator getLambdaCacheIt() const;
    list<FieldMember*>::iterator getLambdaCacheIt();
    list<FieldMember*>::const_iterator getLambdaCacheEnd() const;
    bool lambdaCacheEmpty() const;
    list<FieldMember*>::iterator deleteLambdaFromCache(list<FieldMember*>::iterator it);

    list<FieldMember*>::const_iterator getStoneCacheIt() const;
    list<FieldMember*>::iterator getStoneCacheIt();
    list<FieldMember*>::const_iterator getStoneCacheEnd() const;
    list<FieldMember*>::iterator deleteStoneFromCache(list<FieldMember*>::iterator it);
    

    void write(Point xy, CellType type);

    /**
     * Returns cell with given coordinates.
     *
     * param pPoint pointer to the point object representing coordinates of the cell to retrieve
     */
    FieldMember* const getXY(const Point &point) const;
    FieldMember* getXY(const Point &point);

    pair<int, int> getSize() const;

    /**
     * Swaps contents of the two cells.
     *
     * @param rCell1 coordinates of the first cell
     * @param rCell2 coordinates of the second cell
     */
    void swap(const Point &Cell1, const Point &Cell2);

    /**
     * Returns type of the cell with given coordinates.
     *
     * @param rPoint
     * @return 
     */
    CellType getCellType(const Point &point) const;

    /**
     * Writes given field member into position, specified by coordinates contained in field member.
     *
     * @param rFieldMember
     */
    void setFieldMember(const FieldMember& fieldMember);

    FieldMember* const getRobot() const;
    FieldMember* getRobot();

    FieldMember* const getLift() const;

    bool isRobotAlive() const;
    
    int getLambdaCount() const;
    int getStoneCount() const;

    Field& operator=(const Field&);
    bool operator==(const Field&) const;

    int getDistance(const Point& from, const Point& to) const;
};

#endif	/* FIELD_H */
