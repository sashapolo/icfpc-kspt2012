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
    
    void Set(int X,int Y)
    {
        x=X;
        y=Y;
    }
    
    Point operator + (Point& other) {
        return Point(x+other.x,y+other.y);
    }
    Point operator - (Point& other) {
        return Point(x-other.x,y-other.y);
    }
    void operator += (Point& other) {
        x+=other.x;
        y+=other.y;
        //(*this)=(*this)+other;
    }
    void operator -= (Point& other) {
        x-=other.x;
        y-=other.y;
        //(*this)=(*this)-other;
    }
    
//    Point& operator = (Point& other)
//    {
//        x=other.x;
//        y=other.y;
//        return (*this);
//    }
    bool operator == (Point& other) {
        if((x==other.x) && (y==other.y)) return true;
        return false;
    }
    bool operator != (Point& other) {
        return !((*this)==other);
    }

    Point(int x, int y) {
        this->x = x;
        this->y = y;
    }
};

#endif	/* POINT_H */
