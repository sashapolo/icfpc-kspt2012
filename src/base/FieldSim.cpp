/* 
 * File:   FieldSim.cpp
 * Author: kirill
 * 
 * Created on October 1, 2012, 9:05 PM
 */

#include "base/FieldSim.h"

/////////////////////////////////////////////
void FieldSim::drawField(const Field* pField, int nStep) {
    std::cout << "\nStep: " << nStep << "\n\t";
    //print field
    for (int i = 0; i < pField->getSize().second; i++) {
        for (int j = 0; j < pField->getSize().first; j++) {
            std::cout << cellTypeToChar((CellType)pField->getXY(j, i));
        }
        std::cout << "\n\t";
    }
}


void FieldSim::drawStepByStep(const Field* pField, const std::string& path) {
	int lambdasCollected = 0;
	int score = 0;
	std::string state = "Started";
	int lambdaCount = pField->getLambdaCount();
	int numOfSteps = 0;

	drawField(pField, 0);
	if (path.length() > 0) {
		Field *oldField = new Field(*pField);
		for (unsigned int i = 0; i < path.length(); i++) {
			Field *newField = calcNextState(oldField, path[i]);
			drawField(newField, i);
			delete oldField;
			oldField = newField;
			if (lambdaCount < newField->getLambdaCount()) {
				lambdasCollected++;
				lambdaCount--;
				score += 25;
				state = "Collecting a lambda";
			} else if (!newField->isRobotAlive()) {
				score = 0;
				state = "Dead";
				break;
			} else if (*newField->getLift() == *newField->getRobot()) {
				score += 50 * lambdasCollected;
				state = "Finished";
			} else if (path[i] == 'A') {
				score += 25 * lambdasCollected;
				state = "Aborted";
			} else {
				state = "Moving";
			}
			numOfSteps++;
		}
		delete oldField;
	}
	std::cout << "Score: " << score << " NumSteps: " << numOfSteps
			  << " NumLambdas: " << lambdasCollected << " LC: "
			  << pField->getLambdaCount() << " State " << state << "\n";
}


Field* FieldSim::calcNextState(const Field* pField, char step) {
	return calcNextFieldState(calcRobotStep(pField, step));
}


Field* FieldSim::calcNextFieldState(Field* pField) {
    if(!pField->isRobotAlive()) {
        return pField;
    }
    
    Field *newField = new Field(*pField);

    for (int y = pField->getSize().second - 2; y >= 0; y--)
    {
        for (int x = 0; x < pField->getSize().first - 1; x++)
        {
            char cell = pField->getXY(x, y);
            if (cell == STONE) {
                if (pField->getXY(x, y+1) == EMPTY) {
                    newField->write(x, y, EMPTY);
                    newField->write(x, y+1, STONE);
                } else if ((pField->getXY(x, y+1) == STONE)) {
                    if ((pField->getXY(x+1, y) == EMPTY) &&
                        (pField->getXY(x+1, y+1) == EMPTY)) {
                        newField->write(x, y, EMPTY);
                        newField->write(x+1, y+1, STONE);
                    } else if ((pField->getXY(x-1, y) == EMPTY) &&
                    		   (pField->getXY(x-1,y+1) == EMPTY)) {
                        newField->write(x, y, EMPTY);
                        newField->write(x-1, y+1, STONE);
                    }
                } else if((pField->getXY(x, y+1) == LAMBDA) &&
                          (pField->getXY(x+1, y) == EMPTY) &&
                          (pField->getXY(x+1, y+1) == EMPTY)) {
                    newField->write(x, y, EMPTY);
                    newField->write(x+1, y+1, STONE);
                }
            }
        }
    }
    
    if (newField->lambdaCacheEmpty() && newField->isLiftClosed()) {
        newField->write(*newField->getLift(), OPENED_LIFT);
    }

    //test for robot destruction
    Point destroyCrd = *newField->getRobot() + Point(0, -1);
    if ((newField->getXY(destroyCrd) == STONE) &&
       (pField->getXY(destroyCrd) != STONE)) {
        newField->killRobot();
    }
    
    delete pField;
    return newField;
}


Field* FieldSim::calcRobotStep(const Field* pField, char step) {
	Field *newField = new Field(*pField);
	if (!pField->isRobotAlive()) {
		LOGERROR("Can't calculate: robot must be alive");
		return newField;
	}
	Point *nextPos;

	switch(step) {
		case 'U':
			nextPos = new Point(0, -1);
			break;
		case 'D':
			nextPos = new Point(0, 1);
			break;
		case 'L':
			nextPos = new Point(-1, 0);
			break;
		case 'R':
			nextPos = new Point(1, 0);
			break;
		case 'W':
			nextPos = new Point(0, 0);
			break;
		case 'A':
			return newField;
		default:
			LOGERROR("Wrong path symbol: \'%c\'", step);
			return newField;
	}

	Point absNextPos = *newField->getRobot() + *nextPos;

	//step solve
	switch (newField->getXY(absNextPos)) {
		case STONE:
			if ((step == 'L') || (step == 'R')) {
				if (newField->getXY(absNextPos + *nextPos) == EMPTY) {
					newField->swap(absNextPos + *nextPos, absNextPos);
					newField->swap(absNextPos, *newField->getRobot());
				} else {
					LOGWARNING("Wrong robot step from(%d,%d) to (%d,%d)[*]",
								newField->getRobot()->x,
								newField->getRobot()->y,
								absNextPos.x,
								absNextPos.y);
					delete nextPos;
					return newField;
				}
			} else {
				LOGWARNING("Wrong robot step from(%d,%d) to (%d,%d)[*]",
							newField->getRobot()->x,
							newField->getRobot()->y,
							absNextPos.x,
							absNextPos.y);
				delete nextPos;
				return newField;
			}
			break;
		case EMPTY:
			newField->swap(absNextPos, *newField->getRobot());
			break;
		case EARTH:
		case LAMBDA:
		case OPENED_LIFT:
			newField->write(*newField->getRobot(), EMPTY);
			newField->write(absNextPos, ROBOT);
			break;
		default:
			LOGWARNING("Wrong robot step from(%d,%d) to (%d,%d)[%c]",
					   newField->getRobot()->x,
					   newField->getRobot()->y,
					   absNextPos.x,
					   absNextPos.y,
					   cellTypeToChar((CellType)newField->getXY(absNextPos)));
			delete nextPos;
			return newField;
	}
	delete nextPos;
	return newField;
}
