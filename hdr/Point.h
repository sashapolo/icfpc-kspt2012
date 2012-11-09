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
    
    /**
	 * Объект точка
	 * @param int x - координата x
	 * @param int y - координата y
	 */
	Point(int x, int y);

    /**
	 * Присваивает точке заданные координаты.
	 * @param int x - координата x
	 * @param int y - координата y
	 */
    void set(int x,int y);
    
    /**
	 * Изменяет координаты точки(сложение).
	 * @param other - точка
	 * @return новые координаты точки
	 */
    Point& operator + (const Point& other);

    /**
	 * Изменяет координаты точки(вычитание).
	 * @param Point other - точка
	 * @return новые координаты точки
	 */
    Point& operator - (const Point& other);

    /**
	 * Изменяет координаты текущей точки(сложение).
	 * @param Point other - точка
	 */
    void operator += (const Point& other);

    /**
     * Изменяет координаты текущей точки(вычитание).
     * @param Point other - точка
     */
    void operator -= (const Point& other);

    Point& operator = (const Point& other);

    /**
	 * Сравнение на равенство.
	 * @param Point other - точка
	 * @return если координаты совпадают, то возвращает true. Если не совпадают - false.
	 */
    bool operator == (const Point& other) const;

    /**
	 * Сравнение на неравенство.
	 * @param Point other - точка
	 * @return если координаты не совпадают, то возвращает true. Если совпадают - false.
	 */
    bool operator != (const Point& other) const;
    
    int getDistance(const Point& to) const;
};

#endif	/* POINT_H */
