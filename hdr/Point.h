/*
 * Represents 2D point.
 * It is a value class - object is immutable.
 */

#ifndef POINT_H
#define	POINT_H

/**
 * Двумерная точка.<br />
 * Описывает координаты двумерной точки.
 */

class Point {
public:

    /**
     * Координата x.
     */
    int x;

    /**
     * Координата y.
     */
    int y;
    
    void Set(int X,int Y)
    {
        /**
         * Присваивает точке заданные координаты.
         * @param int X - координата x
         * @param int Y - координата y
         */
        x=X;
        y=Y;
    }
    
    Point operator + (Point other) {
        /**
         * Изменяет координаты точки(сложение).
         * @param other - точка
         * @return новые координаты точки
         */
        return Point(x+other.x,y+other.y);
    }
    Point operator - (Point other) {
        /**
         * Изменяет координаты точки(вычитание).
         * @param Point other - точка
         * @return новые координаты точки
         */
        return Point(x-other.x,y-other.y);
    }
    void operator += (Point other) {
        /**
         * Изменяет координаты текущей точки(сложение).
         * @param Point other - точка
         */
        x+=other.x;
        y+=other.y;
    }
    void operator -= (Point other) {
        /**
         * Изменяет координаты текущей точки(вычитание).
         * @param Point other - точка
         */
        x-=other.x;
        y-=other.y;
    }
    
//    Point& operator = (Point& other)
//    {
//        x=other.x;
//        y=other.y;
//        return (*this);
//    }
    bool operator == (Point other) const {
        /**
         * Сравнение на равенство.
         * @param Point other - точка
         * @return если координаты совпадают, то возвращает true. Если не совпадают - false.
         */
        if((x==other.x) && (y==other.y)) return true;
        return false;
    }
    bool operator != (Point other) const {
        /**
         * Сравнение на неравенство.
         * @param Point other - точка
         * @return если координаты не совпадают, то возвращает true. Если совпадают - false.
         */
        return !((*this)==other);
    }

    Point(int x, int y) {
        /**
         * Объект точка
         * @param int x - координата x
         * @param int y - координата y
         */
        this->x = x;
        this->y = y;
    }
};

#endif	/* POINT_H */
