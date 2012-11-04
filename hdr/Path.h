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
	std::vector<Point> path;
public:
	Path();
    int getSize() const;
	Point getCell(int) const;
	void addCell(const Point&);
	bool isEmpty() const;
};

#endif /* PATH_H_ */
