/* 
 * File:   FieldSim.cpp
 * Author: kirill
 * 
 * Created on October 1, 2012, 9:05 PM
 */
#include "stdinclude.h"
#include "FieldSim.h"
#include "Path.h"

/////////////////////////////////////////////
void DrawField(Field* pField, Path* pPath,  int nStep)
{
    CellType cell;
    char cellsymb;
    int metric=FieldMember::METRIC_NORMAL;
    int pathPoint=-1;
    
    printf("\nStep: %d (original, ",nStep); 
    if(pPath) printf("path, ");
    printf("metric)\n\t");
    
    for(int i=0; i < pField->getSize().second; i++)
    {
        for(int j=0; j < pField->getSize().first; j++) //print field
        {
            cell=pField->getCellType(Point(j,i));
            printf("%c",cellTypeToChar(cell));
        }
        
        if(pPath) //print path
        {
            printf("    ");
            
            for(int j=0; j < pField->getSize().first; j++)
            {
                pathPoint=-1;
                for(int z=0;z<pPath->getSize();z++)
                {
                    if(pPath->getCell(z)==Point(j,i)) pathPoint=z;
                }
                
                if(pathPoint!=-1) printf("%d",pathPoint%10);
                else 
                {
                    cell=pField->getCellType(Point(j,i));
                    printf("%c",cellTypeToChar(cell));
                }  
            }
        }
        
        printf("    ");
        
        for(int j=0; j < pField->getSize().first; j++) //print metric
        {
            metric=pField->getXY(Point(j,i))->getMetric();
            cell=pField->getCellType(Point(j,i));
            
            switch(metric)
            {
                case FieldMember::METRIC_NORMAL: cellsymb=cellTypeToChar(cell); break;
                case FieldMember::METRIC_MEDIUM: cellsymb='?'; break;
                case FieldMember::METRIC_INFINITY: cellsymb='!'; break;
                default: cellsymb='X'; break;
            }
            printf("%c",cellsymb);
        }
        
        printf("\n\t");
    }
}

const char* stateToStr(eEndState state)
{
    switch(state)
    {
        case ES_ABORTED:        return "Aborted";
        case ES_DESTROYED:      return "Destroyed";
        case ES_EAT_LAMBDA:     return "Eat lambda";
        case ES_ERROR:          return "Error";
        case ES_FINISHED:       return "Finished";
        case ES_NONE:           return "None";
        case ES_WRONG_STEP:     return "Wrong step";
    }
    return "Unknown";
}
/////////////////////////////////////////////

FieldSim::FieldSim() {
}

FieldSim::FieldSim(const FieldSim& orig) {
}

FieldSim::~FieldSim() {
}

Field* FieldSim::CalcNextState(Field* pField, bool* pRobotDestroyed)
{
    Field *NewField = new Field(*pField);
    if(!pField->isRobotAlive())
    {
        LOGERROR("Can't simulate: robot must be alive");
        return pField;
    }
    
    
    for(int y = pField->getSize().second - 2; y >= 0; y--)
    {
        for(int x=0;x<pField->getSize().first - 1;x++)
        {
            CellType cell=pField->getCellType(Point(x,y));
            
            NewField->getXY(Point(x,y))->setDefaultMetric();
            if(cell==STONE)
            {
                if(pField->getCellType(Point(x,y+1))==EMPTY)
                {
                    NewField->write(Point(x,y),EMPTY);
                    NewField->write(Point(x,y+1),STONE);
                    NewField->getXY(Point(x,y+1))->setMetric(FieldMember::METRIC_NORMAL);
                }
                else if((pField->getCellType(Point(x,y+1))==STONE))
                {
                    if((pField->getCellType(Point(x+1,y))==EMPTY) &&
                        (pField->getCellType(Point(x+1,y+1))==EMPTY))
                    {
                        NewField->write(Point(x,y),EMPTY);
                        NewField->write(Point(x+1,y+1),STONE);
                        NewField->getXY(Point(x+1,y+1))->setMetric(FieldMember::METRIC_NORMAL);
                    }
                    else if((pField->getCellType(Point(x-1,y))==EMPTY) &&
                        (pField->getCellType(Point(x-1,y+1))==EMPTY))
                    {
                        NewField->write(Point(x,y),EMPTY);
                        NewField->write(Point(x-1,y+1),STONE);
                        NewField->getXY(Point(x-1,y+1))->setMetric(FieldMember::METRIC_NORMAL);
                    }
                }
                else if((pField->getCellType(Point(x,y+1))==LAMBDA) &&
                        (pField->getCellType(Point(x+1,y))==EMPTY) &&
                        (pField->getCellType(Point(x+1,y+1))==EMPTY))
                {
                    NewField->write(Point(x,y),EMPTY);
                    NewField->write(Point(x+1,y+1),STONE);
                    NewField->getXY(Point(x+1,y+1))->setMetric(FieldMember::METRIC_NORMAL);
                }
            }
        }
    }
    
    if((pField->getLambdaCount()==0) && (NewField->getLift()->getType()==CLOSED_LIFT))
    {
        NewField->write(NewField->getLift()->getCoordinate(),OPENED_LIFT);
    }
    
    
    
    //Calculate stone metric (need simple next step sim)
    list<FieldMember*>::iterator it=NewField->getStoneCacheIt();
    int x,y;
    while(it!=NewField->getStoneCacheEnd())
    {
        x=(*it)->getCoordinate().x;
        y=(*it)->getCoordinate().y;
        
        if(NewField->getCellType(Point(x,y+1))==EMPTY)
        {
            NewField->getXY(Point(x,y+1))->setMetric(FieldMember::METRIC_INFINITY);
            NewField->getXY(Point(x,y+2))->setMetric(FieldMember::METRIC_INFINITY);
        }
        else if((NewField->getCellType(Point(x,y+1))==STONE))
        {
            if((NewField->getCellType(Point(x+1,y))==EMPTY) &&
                (NewField->getCellType(Point(x+1,y+1))==EMPTY))
            {
                NewField->getXY(Point(x+1,y+1))->setMetric(FieldMember::METRIC_INFINITY);
                NewField->getXY(Point(x+1,y+2))->setMetric(FieldMember::METRIC_INFINITY);
            }
            else if((NewField->getCellType(Point(x-1,y))==EMPTY) &&
                (NewField->getCellType(Point(x-1,y+1))==EMPTY))
            {
                NewField->getXY(Point(x-1,y+1))->setMetric(FieldMember::METRIC_INFINITY);
                NewField->getXY(Point(x-1,y+2))->setMetric(FieldMember::METRIC_INFINITY);
            }
        }
        else if((NewField->getCellType(Point(x,y+1))==LAMBDA) &&
                        (NewField->getCellType(Point(x+1,y))==EMPTY) &&
                        (NewField->getCellType(Point(x+1,y+1))==EMPTY))
        {
            NewField->getXY(Point(x+1,y+1))->setMetric(FieldMember::METRIC_INFINITY);
            NewField->getXY(Point(x+1,y+2))->setMetric(FieldMember::METRIC_INFINITY);
        }
        
        it++;
    }
    
    Point DestroyCrd=pField->getRobot()->getCoordinate()+Point(0,-1); //test to robot destruction
    if((NewField->getCellType(DestroyCrd)==STONE) && 
            (pField->getCellType(DestroyCrd)!=STONE)) 
    {
        if(pRobotDestroyed) (*pRobotDestroyed)=true;
    }
    else
    {
        if(pRobotDestroyed) (*pRobotDestroyed)=false;
    }
    
    return NewField;
}

 Field* FieldSim::CalcRobotSteps(Field* pField, std::string Steps, sSimResult* pResult, bool bBrakeWhenWrongStep)
 {
     if(!pField->isRobotAlive()) {
         pResult->state=ES_ERROR;
         LOGERROR("Can't calculate: robot must be alive");
         return pField;
     };
     
     bool bDestroyed=false;
     
     Point NextPos(0,0);
     Point AbsNextPos(0,0);
     for(unsigned int i=0;i<Steps.size();i++)
     {
         switch(Steps[i])
         {
             case 'U': NextPos.Set(0,-1); break;
             case 'D': NextPos.Set(0,1); break;
             case 'L': NextPos.Set(-1,0); break;
             case 'R': NextPos.Set(1,0); break;
             case 'W': NextPos.Set(0,0); break;
             case 'A': 
                 pResult->state=ES_ABORTED;
                 pResult->stepsTaken++;
                 return pField;
             default: 
                 pResult->state=ES_ERROR;
                 LOGERROR("Wrong path symbol: \'%c\'",Steps[i]);
                 return pField;
         };
         
         AbsNextPos=pField->getRobot()->getCoordinate()+NextPos;
         pResult->stepsTaken++;
         pResult->state=ES_NONE;
         switch(pField->getCellType(AbsNextPos)) //step solve
         {
             case ROBOT: break;
             case STONE: 
                 if((Steps[i]=='L') || (Steps[i]=='R'))
                 {
                     if(pField->getCellType(AbsNextPos+NextPos)==EMPTY) \
                     {
                         pField->swap(AbsNextPos+NextPos,AbsNextPos);
                         pField->swap(AbsNextPos,pField->getRobot()->getCoordinate());
                         pResult->path.addCell(AbsNextPos);
                     }
                     else 
                     {
                         pResult->state=ES_WRONG_STEP;
                         pResult->path.addCell(pField->getRobot()->getCoordinate());
                         LOGWARNING("Wrong robot step from(%d,%d) to (%d,%d)[*]",
                                pField->getRobot()->getCoordinate().x,pField->getRobot()->getCoordinate().y,AbsNextPos.x,AbsNextPos.y);
                         if(bBrakeWhenWrongStep) 
                         {
                             return pField;
                         }
                     }
                         
                 } else {
                     pResult->state=ES_WRONG_STEP;
                     pResult->path.addCell(pField->getRobot()->getCoordinate());
                     LOGWARNING("Wrong robot step from(%d,%d) to (%d,%d)[*]",
                         pField->getRobot()->getCoordinate().x,pField->getRobot()->getCoordinate().y,AbsNextPos.x,AbsNextPos.y);
                     if(bBrakeWhenWrongStep) 
                     {
                         return pField;
                     }
                 }
                 break;
             case EMPTY: 
                 pField->swap(AbsNextPos,pField->getRobot()->getCoordinate()); 
                 pResult->path.addCell(AbsNextPos);
                 break;
             case EARTH: 
                 //FieldMember tmp=(*pField->getRobot());
                 pField->write(pField->getRobot()->getCoordinate(),EMPTY);
                 pField->write(AbsNextPos,ROBOT);
                 pResult->path.addCell(AbsNextPos);
                 break;
             case LAMBDA:
                 pField->write(pField->getRobot()->getCoordinate(),EMPTY);
                 pField->write(AbsNextPos,ROBOT);
                 pResult->path.addCell(AbsNextPos);
                 pResult->lambdaReceived++;
                 pResult->state=ES_EAT_LAMBDA;
                 break;
             case OPENED_LIFT: 
                 pResult->state=ES_FINISHED;
                 return pField;
             default:
                 pResult->state=ES_WRONG_STEP;
                 pResult->path.addCell(pField->getRobot()->getCoordinate());
                 LOGWARNING("Wrong robot step from(%d,%d) to (%d,%d)[%c]",
                         pField->getRobot()->getCoordinate().x,pField->getRobot()->getCoordinate().y,AbsNextPos.x,AbsNextPos.y,
                         cellTypeToChar(pField->getCellType(AbsNextPos)));
                 if(bBrakeWhenWrongStep) 
                 {
                     return pField;
                 }
                 break;
         }
         
         pField=CalcNextState(pField,&bDestroyed);
         if(bDestroyed)
         {
             pResult->state=ES_DESTROYED;
             return pField;
         }
     }
     
     return pField;
 }
