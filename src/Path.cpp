/*
 * Path.cpp
 *
 *  Created on: Sep 24, 2012
 *      Author: alexander
 */

#include "Path.h"

Point Path::getCell(int index) const {
        /**
         * Возвращает координаты робота
	 * @param index переменная, определяющая шаг
	 * @return координаты робота с выбранным индексом
	 */
	return path[index];
}

void Path::addCell(const Point& c) {
    	/**
	 * Добавляет шаг к пути
	 * @param c точка, которая добавляется к пути
	 */
	path.push_back(c);
}

int Path::getSize() const {
    /**
     * Возвращает длину пути
     * @return длина пути
     */
    return path.size();
}
