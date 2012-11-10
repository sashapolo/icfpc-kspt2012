/* 
 * File:   FieldSim.h
 * Author: kirill
 *
 * Created on October 1, 2012, 9:05 PM
 */

#ifndef FIELDSIM_H
#define	FIELDSIM_H

#include "stdinclude.h"

void drawField(const Field* pField, Path* pPath=0, int nStep=0);

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
    CH_DESTROY
};

struct sSimChange
{
    sSimChange() : pos1(0,0), pos2(0,0)
    {
        nStep=0;
        nChange=0;
        changeType=CH_NONE;
        cellType1=EMPTY;
        cellType2=EMPTY;
    }
    
    sSimChange(int nStep_,int nChange_,eChangeType changeType_,CellType cellType1_,CellType cellType2_,Point pos1_,Point pos2_)
    {
        nStep=nStep_;
        nChange=nChange_;
        changeType=changeType_;
        cellType1=cellType1_;
        cellType2=cellType2_;
        pos1=pos1_;
        pos2=pos2_;
    }
    int nStep;
    int nChange;
    eChangeType changeType;
    CellType cellType1;
    CellType cellType2;
    Point pos1;
    Point pos2;
};

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
    Path path;
    
    std::vector<sSimChange> Changes;
};

const char* stateToStr(eEndState state);

class FieldSim {
public:
    FieldSim();
    FieldSim(const FieldSim& orig);
    virtual ~FieldSim();
    
    Field* calcNextState(Field* pField, bool* pRobotDestroyed=NULL, std::vector<sSimChange>* pChanges=NULL) const;
    Field* calcRobotSteps(const Field* pField, std::string Steps, sSimResult* pResult, bool bBrakeWhenWrongStep=false) const;
private:

};

#endif	/* FIELDSIM_H */

