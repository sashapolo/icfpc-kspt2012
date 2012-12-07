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
	/**
	 * Вектор пути.
	 */
	std::vector<const Point*> path;
public:
	/**
	 * Деструктор класса Path.
	 * Очищает список пути.
	 */
	virtual ~Path();
	/**
	 * Возвращает длину пути.
	 * @return длина пути.
	 */
	int getSize() const {
		return path.size();
	}
	/**
	* Возвращает координаты точки,
	* в которой робот находился на заданном шаге.
 	* <div style="font-weight: bold">Индексация шагов начинается с нуля.</div>
	* @param index номер получаемого шага.
	* @return координаты робота на шаге с заданным номером.
	*/
	const Point* getCell(int index) const {
		return path[index];
	}
	/**
	* Добавляет шаг к пути.
 	* @param Point& c - точка, которая добавляется к пути.
 	*/
	void addCell(const Point& c) {
		path.push_back(new Point(c));
	}
	/**
	 * Проверка на отсутствие элементов в пути.
	 * @return true, если путь пуст, false в противном случае.
	 */
	bool isEmpty() const {
		return path.empty();
	}
	/**
	 * Получение значения длины пути.
	 * @return значение длины пути.
	 */
	int getDistance() const;
	/**
	* Обмен шагов робота местами.
 	* @param int i - 1 элемент.
	* @param int j - 2 элемент.
 	*/
	void swap(int i, int j);
	/**
	* Удаление шага робота.
 	* @param int - индекс удаляемого шага.
 	*/
	void deleteCell(int);
};

#endif /* PATH_H_ */
