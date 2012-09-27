/*
 * Coordinate.cpp
 *
 *  Created on: Sep 24, 2012
 *      Author: alexander
 */

#include "Coordinate.h"

Coordinate::Coordinate(int x = 0, int y = 0) {
	if (x < 0 || y < 0) {
		throw BadCoorException();
	}
	this->x = x;
	this->y = y;
}

int Coordinate::getX() {
	return x;
}

int Coordinate::getY() {
	return y;
}

void Coordinate::setX(int a) {
	if (a < 0) {
		throw BadCoorException();
	}
	x = a;
}

void Coordinate::setY(int a) {
	if (a < 0) {
		throw BadCoorException();
	}
	y = a;
}
