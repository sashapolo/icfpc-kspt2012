/* 
 * File:   FieldSim.h
 * Author: kirill
 *
 * Created on October 1, 2012, 9:05 PM
 */

#ifndef FIELDSIM_H
#define	FIELDSIM_H

#include <iostream>
#include "base/Field.h"

class FieldSim {
private:
	static Field* calcRobotStep(const Field* pField, char step);
	static Field* calcNextFieldState(Field* pField);
	static void drawField(const Field* pField, int nStep = 0);
public:
	/**
	 * Пошаговая отрисовка.
	 * @param Field* pField - поле.
	 * @param string path - путь робота.
	 */
	static void drawStepByStep(const Field* pField, const std::string& path = "");
	static const Field* calcNextState(const Field* pField, char step);
};

#endif	/* FIELDSIM_H */

