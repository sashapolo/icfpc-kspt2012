/* 
 * File:   FieldSim.h
 * Author: kirill
 *
 * Created on October 1, 2012, 9:05 PM
 */

#ifndef FIELDSIM_H
#define	FIELDSIM_H

#include "stdinclude.h"

void DrawField(Field* pField, Path* pPath=NULL, int nStep=0);

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

struct sSimResult
{
    sSimResult()
    {
        lambdaReceived=0;
        stepsTaken=0;
        state=ES_NONE;
    }
          
    int lambdaReceived;
    int stepsTaken;
    eEndState state;
    Path path;
};

const char* stateToStr(eEndState state);

class FieldSim {
public:
    FieldSim();
    FieldSim(const FieldSim& orig);
    virtual ~FieldSim();
    
    Field* CalcNextState(Field* pField, bool* pRobotDestroyed=NULL) const;
    Field* CalcRobotSteps(const Field* pField, std::string Steps, sSimResult* pResult, bool bBrakeWhenWrongStep=false) const;
private:

};

#endif	/* FIELDSIM_H */

