/*
 * Point.cpp
 *
 *  Created on: Nov 9, 2012
 *      Author: alexander
 */

#include "base/Point.h"

Point::Point(void): x(0), y(0){
    
}

Point::Point(int x, int y): x(x), y(y) {
}


Point::Point(const Point& orig): x(orig.x), y(orig.y) {
}


Point Point::operator +(const Point& other) const {
	return Point(x + other.x, y + other.y);
}


Point Point::operator -(const Point& other) const {
	return Point(x - other.x, y - other.y);
}


bool Point::operator ==(const Point& other) const {
	return ((x == other.x) && (y == other.y));
}


bool Point::operator !=(const Point& other) const {
	return !(*this == other);
}


int Point::getDistance(const Point& to) const {
	int tx = x - to.x;
	int ty = y - to.y;
	return abs(tx) + abs(ty);
}
