/*
 * Solver.cpp
 *
 *  Created on: Oct 15, 2012
 *      Author: alexander
 */
#include "stdinclude.h"
#include "Solver.h"

string Solver::solve(Field* pField) const {
	string result;
	/*FieldSim fieldSim;
	sSimResult simRes;
	Dijkstra *dj = new Dijkstra(pField, pField->getRobot());
	list<FieldMember*>::const_iterator it = pField->getLambdaCacheIt();
	for (; it != pField->getLambdaCacheEnd(); it++) {
		dj->addGoal(*it);
	}
	string t = convertResultToString(dj->findNearestPath());
	delete dj;
	Field *oldField = pField;

	while (!t.empty()) {
		result += t;
		Field *newField = fieldSim.CalcRobotSteps(oldField, t, &simRes);
		dj = new Dijkstra(newField, newField->getRobot());
		it = pField->getLambdaCacheIt();
		for (; it != pField->getLambdaCacheEnd(); it++) {
			dj->addGoal(*it);
		}
		t = convertResultToString(dj->findNearestPath());
		delete dj;
		oldField = newField;
	}
	if (oldField->getLambdaCount() == 0) { // если все собрали - идем в лифт
		dj = new Dijkstra(oldField, oldField->getRobot());
		dj->addGoal(oldField->getLift());
		result += convertResultToString(dj->findNearestPath());
		delete dj;
	} else {
		result += "A";
	}*/
        
	return result;
}

string Solver::convertResultToString(const Path& coordinates) const {
	string result;
	for (int i = coordinates.getSize() - 2; i >= 0; i--) {
		if (coordinates.getCell(i + 1).x - coordinates.getCell(i).x == 1) {
			result += 'L';
		} else if (coordinates.getCell(i + 1).x - coordinates.getCell(i).x == -1) {
			result += 'R';
		} else if (coordinates.getCell(i + 1).y - coordinates.getCell(i).y == 1) {
			result += 'U';
		} else if (coordinates.getCell(i + 1).y - coordinates.getCell(i).y == -1) {
			result += 'D';
		} else {
			result += 'W';
		}
	}
	return result;
}


