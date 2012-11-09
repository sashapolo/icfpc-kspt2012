/*
 * Point.cpp
 *
 *  Created on: Nov 9, 2012
 *      Author: alexander
 */

#include "Point.h"

	Point::Point(int x, int y) {
        this->x = x;
        this->y = y;
    }

    void Point::set(int x,int y) {
        this->x = x;
        this->y = y;
    }

    Point& Point::operator + (const Point& other) {
        return Point(x + other.x, y + other.y);
    }

    Point& Point::operator - (const Point& other) {
        return Point(x - other.x,y - other.y);
    }

    void Point::operator += (const Point& other) {
        x += other.x;
        y += other.y;
    }
    void Point::operator -= (const Point& other) {
        x -= other.x;
        y -= other.y;
    }

    Point& Point::operator = (const Point& other)
    {
        x = other.x;
        y = other.y;
        return (*this);
    }

    bool Point::operator == (const Point& other) const {
        if ((x == other.x) && (y == other.y))
        	return true;
        return false;
    }

    bool Point::operator != (const Point& other) const {
        return !((*this) == other);
    }


    int Point::getDistance(const Point& to) const {
		int tx = x - to.x;
		int ty = y - to.y;
		return abs(tx) + abs(ty);
    }
