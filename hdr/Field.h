/**
 * Represents field.
 */

#ifndef FIELD_H
#define FIELD_H

#include <string>
#include <vector>
#include <list>

#include "FieldMember.h"

typedef std::list<Point*> FieldCache;


/**
 * Represents field.
 */
class Field {
private:
	CellType field[][];
	int xSize, ySize;
	FieldCache lambdaCache, stoneCache;
    Point robot, lift;

public:
    enum CellType { CLOSED_LIFT,
    			    OPENED_LIFT,
    			    EARTH,
    			    EMPTY,
    			    LAMBDA,
    			    ROBOT,
    			    STONE,
    			    WALL };

    // ислючение, говорящее о неправильности задания карты
    struct FieldParseException {};

    /**
     * Trivial constructor: makes field cell of given coordinate with given cell type.
     * Lines in the map being passed are of variable length.
     * Horizontal size of the map is determined by the line of the maximum length.
     * All other lines are padded with empty cells.
     *
     * param ASCIIMap Map in ASCII. See specification for details.
     */
    Field(const std::string &ASCIIMap);
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
    FieldCache::const_iterator getLambdaCacheIt() const;
    FieldCache::iterator getLambdaCacheIt();
    FieldCache::const_iterator getLambdaCacheEnd() const;
    bool lambdaCacheEmpty() const;
    FieldCache::iterator deleteLambdaFromCache(FieldCache::iterator it);

    FieldCache::const_iterator getStoneCacheIt() const;
    FieldCache::iterator getStoneCacheIt();
    FieldCache::const_iterator getStoneCacheEnd() const;
    FieldCache::iterator deleteStoneFromCache(FieldCache::iterator it);


    void write(const Point& xy, CellType type);

    /**
     * Returns cell with given coordinates.
     *
     * param pPoint pointer to the point object representing coordinates of the cell to retrieve
     */
    CellType getXY(const Point &point) const;

    std::pair<int, int>& getSize() const;

    /**
     * Swaps contents of the two cells.
     *
     * @param rCell1 coordinates of the first cell
     * @param rCell2 coordinates of the second cell
     */
    void swap(const Point &Cell1, const Point &Cell2);

    const Point& getRobot() const;
    Point& getRobot();

    const Point& getLift() const;

    bool isRobotAlive() const;
    
    int getLambdaCount() const;
    int getStoneCount() const;

    Field& operator=(const Field&);
    bool operator==(const Field&) const;
};

#endif	/* FIELD_H */
