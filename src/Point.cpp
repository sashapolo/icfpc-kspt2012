/*
 * Represents 2D point.
 * It is a value class - object is immutable.
 */

#include "Point.h"

Point::Point(int x, int y) {
    this->x = x;
    this->y = y;
}

Point::Point(const Point& orig) {
    this->x = orig.x;
    this->y = orig.y;
}

Point::~Point() {
}

int Point::getX() const {
    return this->x;
}

int Point::getY() const {
    return this->y;
}