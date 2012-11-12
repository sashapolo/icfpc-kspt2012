/*
 * Path.h
 *
 *  Created on: Sep 24, 2012
 *      Author: alexander
 */

#ifndef PATH_H_
#define PATH_H_

#include <vector>
#include "Point.h"

/**
 * Путь робота.<br />
 * Содержит все ходы робота.<br />
 * Если была команда Wait, то будет повторно записана координата предыдущего шага.
 */
class Path {
	std::vector<Point*> path;
public:
	Path();
	virtual ~Path();
	/**
	 * Возвращает длину пути
	 * @return длина пути
	 */
    int getSize() const;
    /**
    * Возвращает координаты точки,
    * в которой робот находился на заданном шаге.
    * <div style="font-weight: bold">Индексация шагов начинается с нуля.</div>
    * @param index номер получаемого шага
    * @return координаты робота на шаге с заданным номером
    */
    const Point* getCell(int) const;
    /**
	 * Добавляет шаг к пути
	 * @param Point& c - точка, которая добавляется к пути
	 */
    void addCell(const Point&);
    bool isEmpty() const;
    int getDistance() const;
    void swap(int i, int j);
    void deleteCell(int);
};

#endif /* PATH_H_ */
