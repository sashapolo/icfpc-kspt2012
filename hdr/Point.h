/*
 * Represents 2D point.
 * It is a value class - object is immutable.
 */

#ifndef POINT_H
#define	POINT_H

// ToDo: prohibit inheritance. Private constructor?
// ToDo: static factory with common values?
class Point {

public:

    // ToDo: easy to jumble x and y
    /*
     * Trivial constructor: makes point with given coordinates.
     *
     * param x X coordinate
     * param y Y coordinate
     */
    Point(int x, int y);
    /*
     * Copy constructor makes a copy of the object referenced by a given reference.
     *
     * param orig reference to the object being copied
     */
    Point(const Point& orig);
    virtual ~Point();

    /*
     * Get X coordinate of the point.
     */
    int getX() const;
    /*
     * Get Y coordinate of the point.
     */
    int getY() const;

    // Value class - immutable. No setters.

private:

    int x;
    int y;

};

#endif	/* POINT_H */