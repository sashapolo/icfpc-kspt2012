/*
 * Represents 2D point.
 * It is a value class - object is immutable.
 */

#ifndef POINT_H
#define	POINT_H

#include <stdlib.h>

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
	 * Объект точка - установка нулевого значения.
	 */
	Point(void);
	/**
	 * Объект точка - установка значения.
	 * @param int x - координата x.
	 * @param int y - координата y.
	 */
	Point(int x, int y);
	/**
	 * Объект точка - установка значения.
	 * @param const Point& orig - точка.
	 */
	Point(const Point& orig);

	/**
	 * Изменяет координаты точки(сложение).
	 * @param other - точка.
	 * @return новые координаты точки.
	 */
	Point operator +(const Point& other) const;

	/**
	 * Изменяет координаты точки(вычитание).
	 * @param Point other - точка.
	 * @return новые координаты точки.
	 */
	Point operator -(const Point& other) const;

	/**
	 * Сравнение на равенство.
	 * @param Point other - точка.
	 * @return если координаты совпадают, то возвращает true. Если не совпадают - false.
	 */
	bool operator ==(const Point& other) const;

	/**
	 * Сравнение на неравенство.
	 * @param Point other - точка.
	 * @return если координаты не совпадают, то возвращает true. Если совпадают - false.
	 */
	bool operator !=(const Point& other) const;
	/**
	 * Расчет дистанции до точки.
	 * Вычисляется как сумма модулей разностей между соответствующими координатами двух точек.
	 * @param const Point& to - точка.
	 * @return расстояние до точки.
	 */
	int getDistance(const Point& to) const;
};

#endif	/* POINT_H */
