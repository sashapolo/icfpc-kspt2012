/* 
 * File:   FieldSim.cpp
 * Author: kirill
 * 
 * Created on October 1, 2012, 9:05 PM
 */

#include <vector>

#include "base/FieldSim.h"

/////////////////////////////////////////////
void FieldSim::drawField(const Field* pField, int nStep) {
    std::cout << "\nStep: " << nStep << "\n\t";
    //print field
    for (int i = 0; i < pField->getSize().second; i++) {
        for (int j = 0; j < pField->getSize().first; j++) {
            std::cout << pField->getXY(j, i);
        }
        std::cout << "\n\t";
    }
}


void FieldSim::drawStepByStep(const Field* pField, const std::string& path) {
	int lambdasCollected = 0;
	int score = 0;
	std::string state = "In progress";
	int lambdaCount = pField->getLambdaCount();
	int numOfSteps = 0;

	drawField(pField, 0);
	if (path.length() > 0) {
		const Field *oldField = new Field(*pField);
		for (unsigned int i = 0; i < path.length(); i++) {
			const Field *newField = calcNextState(oldField, path[i]);
			delete oldField;
			oldField = newField;
			if (!newField->isRobotAlive()) {
				score = 0;
				state = "Dead";
				break;
			} else if (*newField->getLift() == *newField->getRobot()) {
				score += 50 * lambdasCollected - 1;
				state = "Finished";
				drawField(newField, i + 1);
				break;
			} else if (lambdaCount > newField->getLambdaCount()) {
				lambdasCollected++;
				lambdaCount--;
				score += 25;
			} else if (path[i] == 'A') {
				score += 25 * lambdasCollected;
				state = "Aborted";
				break;
			}
			drawField(newField, i + 1);
			numOfSteps++;
			score--;
		}
		delete oldField;
		std::cout << "Score: " << score << " NumSteps: " << numOfSteps
				  << " NumLambdas: " << lambdasCollected << " LC: "
				  << pField->getLambdaCount() << " State " << state << "\n";
	}
}


const Field* FieldSim::calcNextState(const Field* pField, char step) {
	return calcNextFieldState(calcRobotStep(pField, step));
}

const Field* FieldSim::calcNextStateEx(const Field* pField, char step, sSimResult* pResult) {
	pResult->stepsTaken++;
	int lambdaCount = pField->getLambdaCount();
	Point liftPos = *(pField->getLift());
	pResult->state = ES_NONE;

	Field* ret = calcNextFieldState(
			calcRobotStep(pField, step, &(pResult->Changes)),
			&(pResult->Changes));
        pResult->score--;
	if (ret->getLambdaCount() < lambdaCount) {
		pResult->lambdaReceived++;
		pResult->state = ES_EAT_LAMBDA;
		pResult->score += 25;
	}

	if (step == 'A') {
		pResult->state = ES_ABORTED;
		pResult->score += pResult->lambdaReceived * 25;
	}
	if (*(ret->getRobot()) == liftPos) {
		pResult->state = ES_FINISHED;
		pResult->score += pResult->lambdaReceived * 50;
	}
	if (!ret->isRobotAlive())
		pResult->state = ES_DESTROYED;

	return ret;
}


Field* FieldSim::calcNextFieldState(Field* pField, FieldChanges* pChanges) {
    if(!pField->isRobotAlive()) {
        return pField;
    }
    
    Field *newField = new Field(*pField);

	for (int y = pField->getSize().second - 2; y >= 0; y--) {
		for (int x = 0; x < pField->getSize().first - 1; x++) {
			char cell = pField->getXY(x, y);
			if (cell == STONE) {
				if (pField->getXY(x, y + 1) == EMPTY) {
					if (pChanges)
						pChanges->push_back(sSimChange(CH_MOVE, STONE, Point(x, y),
											Point(x, y + 1)));
					newField->write(x, y, EMPTY);
					newField->write(x, y + 1, STONE);
				} else if ((pField->getXY(x, y + 1) == STONE)) {
					if ((pField->getXY(x + 1, y) == EMPTY)
							&& (pField->getXY(x + 1, y + 1) == EMPTY)) {
						if (pChanges)
							pChanges->push_back(sSimChange(CH_MOVE, STONE, Point(x, y),
												Point(x + 1, y + 1)));
						newField->write(x, y, EMPTY);
						newField->write(x + 1, y + 1, STONE);
					} else if ((pField->getXY(x - 1, y) == EMPTY)
							&& (pField->getXY(x - 1, y + 1) == EMPTY)) {
						if (pChanges)
							pChanges->push_back(sSimChange(CH_MOVE, STONE, Point(x, y),
												Point(x - 1, y + 1)));
						newField->write(x, y, EMPTY);
						newField->write(x - 1, y + 1, STONE);
					}
				} else if ((pField->getXY(x, y + 1) == LAMBDA)
						&& (pField->getXY(x + 1, y) == EMPTY)
						&& (pField->getXY(x + 1, y + 1) == EMPTY)) {
					if (pChanges)
						pChanges->push_back(sSimChange(CH_MOVE, STONE, Point(x, y),
											Point(x + 1, y + 1)));
					newField->write(x, y, EMPTY);
					newField->write(x + 1, y + 1, STONE);
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


Field* FieldSim::calcRobotStep(const Field* pField, char step,FieldChanges* pChanges) {
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
				if (pChanges) {
                                        pChanges->push_back(
							sSimChange(CH_MOVE, STONE, absNextPos,
									absNextPos + *nextPos));
					pChanges->push_back(
							sSimChange(CH_MOVE, ROBOT, *newField->getRobot(),
									absNextPos));
					
				}
				newField->swap(absNextPos + *nextPos, absNextPos);
				newField->swap(absNextPos, *newField->getRobot());
			} else {
				LOGWARNING("Wrong robot step from(%d,%d) to (%d,%d)[*]",
						newField->getRobot()->x, newField->getRobot()->y,
						absNextPos.x, absNextPos.y);
				delete nextPos;
				return newField;
			}
		} else {
			LOGWARNING("Wrong robot step from(%d,%d) to (%d,%d)[*]",
					newField->getRobot()->x, newField->getRobot()->y,
					absNextPos.x, absNextPos.y);
			delete nextPos;
			return newField;
		}
		break;
	case EMPTY:
		if (pChanges)
			pChanges->push_back(sSimChange(CH_MOVE, ROBOT, *newField->getRobot(),
								absNextPos));
		newField->swap(absNextPos, *newField->getRobot());
		break;
	case EARTH:
		if (pChanges) {
			pChanges->push_back(sSimChange(CH_DESTROY, EARTH, absNextPos, absNextPos));
                        pChanges->push_back(sSimChange(CH_MOVE, ROBOT, *newField->getRobot(),
								absNextPos));
                }
		newField->write(*newField->getRobot(), EMPTY);
		newField->write(absNextPos, ROBOT);
		break;
	case LAMBDA:
		if (pChanges) {
			pChanges->push_back(sSimChange(CH_DESTROY, LAMBDA, absNextPos, absNextPos));
                        pChanges->push_back(sSimChange(CH_MOVE, ROBOT, *newField->getRobot(),
								absNextPos));
                }
		newField->write(*newField->getRobot(), EMPTY);
		newField->write(absNextPos, ROBOT);
		break;
	case OPENED_LIFT:
		if (pChanges)
			pChanges->push_back(sSimChange(CH_MOVE, ROBOT, *newField->getRobot(),
								absNextPos));
		newField->write(*newField->getRobot(), EMPTY);
		newField->write(absNextPos, ROBOT);
		break;
	default:
		LOGWARNING("Wrong robot step from(%d,%d) to (%d,%d)[%c]",
				newField->getRobot()->x, newField->getRobot()->y, absNextPos.x,
				absNextPos.y,
				cellTypeToChar((CellType) newField->getXY(absNextPos)));
		delete nextPos;
		return newField;
	}
	delete nextPos;
	return newField;
}
