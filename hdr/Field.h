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

class Field {
private:
    vector< vector<FieldMember*> > field;
    list<FieldMember*> lambdaCache;
    list<FieldMember*> stoneCache;
    FieldMember* pRobot;
    FieldMember* pLift;

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

    virtual ~Field() {
    	for (unsigned int i = 0; i < field.size(); i++) {
    		for (unsigned int j = 0; j < field[i].size(); j++) {
    			delete field[i][j];
    		}
    	}
    }

    /**
     * There are caches of Lambdas and Stones to provide some assistance to algorithm.
     */
    list<FieldMember*>::const_iterator getLambdaCacheIt() const{
    	return lambdaCache.begin();
    }
    list<FieldMember*>::iterator getLambdaCacheIt() {
    	return lambdaCache.begin();
    }

    list<FieldMember*>::const_iterator getStoneCacheIt() const{
    	return stoneCache.begin();
    }
    list<FieldMember*>::iterator getStoneCacheIt() {
    	return stoneCache.begin();
    }

    void deleteLambdaFromCache(list<FieldMember*>::iterator it) {
    	lambdaCache.erase(it);
    }
    void deleteStoneFromCache(list<FieldMember*>::iterator it) {
    	stoneCache.erase(it);
    }

    /**
     * Returns cell with given coordinates.
     *
     * param pPoint pointer to the point object representing coordinates of the cell to retrieve
     */
    const FieldMember* getXY(const Point &point) const {
        return field.at(point.y).at(point.x);
    }
    FieldMember* getXY(const Point &point) {
        return field.at(point.y).at(point.x);
    }


    pair<int, int> getSize() {
        if (field.size() == 0) {
        	return make_pair(0,0);
        }
        return make_pair(field.size(), field[0].size());
    }

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
    CellType getCellType(const Point &point) const {
        return getXY(point)->getType();
    }

    /**
     * Writes given field member into position, specified by coordinates contained in field member.
     *
     * @param rFieldMember
     */
    // ToDo: need in throwing any exceptions?
    void setFieldMember(const FieldMember& fieldMember) {
        int x = fieldMember.getCoordinate().x;
        int y = fieldMember.getCoordinate().y;
        *(field.at(y).at(x)) = fieldMember;
    }

    const FieldMember* getRobot() const {
    	return pRobot;
    }
    FieldMember* getRobot() {
		return pRobot;
    }

    const FieldMember* getLift() const {
		return pLift;
	}

    bool isRobotAlive() const {
    	return pRobot;
    }

    Field& operator=(const Field&);
};

#endif	/* FIELD_H */
