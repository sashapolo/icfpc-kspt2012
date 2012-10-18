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

class Path {
	std::vector<Point> path;
public:
    int getSize() const;
	Point getCell(int) const;
	void addCell(const Point&);
};

#endif /* PATH_H_ */
