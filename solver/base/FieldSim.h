/*
 * File:   FieldSim.h
 * Author: kirill
 *
 * Created on October 1, 2012, 9:05 PM
 */

#ifndef FIELDSIM_H
#define	FIELDSIM_H

#include <iostream>
#include "Field.h"
#include "Point.h"
#include "Path.h"

enum eEndState
{
    ES_NONE, //Никакого окончательного состояния
    ES_EAT_LAMBDA,  //Робот поглотил лямбду
    ES_DESTROYED,  //Робот уничтожен
    ES_FINISHED,   //Робот зашел в лифт
    ES_ABORTED,     //Сигнал аборта
    ES_WRONG_STEP,   //Неправильный ход (в стену и т.п.)
    ES_ERROR         //Ошибка
};

enum eChangeType
{
    CH_NONE,
    CH_MOVE,
    CH_DESTROY,
    CH_LIFT_OPEN
};

struct sSimChange
{
    sSimChange() : pos1(0,0), pos2(0,0)
    {
        changeType=CH_NONE;
        cellType=EMPTY;
    }

    sSimChange(eChangeType changeType_,CellType cellType_,Point pos1_,Point pos2_)
    {
        changeType=changeType_;
        cellType=cellType_;
        pos1=pos1_;
        pos2=pos2_;
    }

    eChangeType changeType;
    CellType cellType;
    Point pos1;
    Point pos2;
};

typedef std::vector<sSimChange> FieldChanges;
struct sSimResult
{
    sSimResult()
    {
        lambdaReceived=0;
        stepsTaken=0;
        state=ES_NONE;
        score=0;
    }

    int lambdaReceived;
    int stepsTaken;
    int score;
    eEndState state;

    FieldChanges Changes;
};

class FieldSim {
private:
	/**
	 * Подсчет нового шага робота.
	 * @param Field* pField - карта.
	 * @param string path - путь робота.
	 * @param FieldChanges* pChanges - изменение карты.
	 * @return новое положение робота.
	 */
	static Field* calcRobotStep(const Field* pField, char step,FieldChanges* pChanges=0);
	/**
	 * Подсчет следующего состояния карты.
	 * @param Field* pField - карта.
	 * @param FieldChanges* pChanges - изменение карты.
	 * @return новое состояние карты.
	 */
	static Field* calcNextFieldState(Field* pField,FieldChanges* pChanges=0);
	/**
	 * Отрисовка карты.
	 * @param int nStep - шаг.
	 */
	static void drawField(const Field* pField, int nStep = 0);
public:
	/**
	 * Пошаговая отрисовка.
	 * @param Field* pField - карта.
	 * @param string& path - путь робота.
	 */
	static void drawStepByStep(const Field* pField, const std::string& path = "");
	/**
	 * Подсчет следующего состояния.
	 * @param Field* pField - карта.
	 * @param char step - ход.
	 * @return значение поля на следующем шаге.
	 */
	static const Field* calcNextState(const Field* pField, char step);
	/**
	 * Подсчет следующего состояния.
	 * @param Field* pField - карта.
	 * @param char step - ход.
	 * @param sSimResult* pResult - результат симуляции.
	 * @return значение поля на следующем шаге.
	 */
	static const Field* calcNextStateEx(const Field* pField, char step, sSimResult* pResult);
};

#endif	/* FIELDSIM_H */

