/*
 * Path.cpp
 *
 *  Created on: Sep 24, 2012
 *      Author: alexander
 */

#include "Path.h"

Path::Path(): path() {

}

Point Path::getCell(int index) const {
        /**
         * Возвращает координаты робота
	 * @param int index - переменная, определяющая шаг
	 * @return координаты робота с выбранным индексом
	 */
	return path[index];
}

void Path::addCell(const Point& c) {
    	/**
	 * Добавляет шаг к пути
	 * @param Point& c - точка, которая добавляется к пути
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

bool Path::isEmpty() const {
	return path.empty();
}

int Path::getDistance() const {
	int result = 0;
	for (unsigned int i = 0; i < path.size() - 1; i++) {
		result += path[i].getDistance(path[i+1]);
	}
	return result;
}

void Path::swap(int i, int j) {
	Point t = path[i];
	path[i] = path[j];
	path[j] = t;
}

void Path::deleteCell(int index) {
	path.erase(path.begin() + index);
}
