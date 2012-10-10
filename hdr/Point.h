/*
 * Represents 2D point.
 * It is a value class - object is immutable.
 */

#ifndef POINT_H
#define	POINT_H

class Point {
public:

    int x;
    int y;

    Point(int x, int y) {
        this->x = x;
        this->y = y;
    }
};

#endif	/* POINT_H */
