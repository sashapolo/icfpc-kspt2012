/* 
 * File:   FieldSim.cpp
 * Author: kirill
 * 
 * Created on October 1, 2012, 9:05 PM
 */
#include "stdinclude.h"
#include "FieldSim.h"

/////////////////////////////////////////////
void DrawField(Field* pField, int nStep)
{
    CellType cell;
    char cellsymb;
    
    printf("\nStep: %d\n\t",nStep);
    for(int i=pField->getSize().second-1;i>=0;i--)
    {
        for(int j=0;j<pField->getSize().first;j++)
        {
            cell=pField->getCellType(Point(j,i));
            switch(cell)
            {
                case ROBOT: cellsymb='R'; break;
                case LAMBDA: cellsymb='\\'; break;
                case ROCK: cellsymb='*'; break;
                case WALL: cellsymb='#'; break;
                case EMPTY: cellsymb=' '; break;
                case CLOSED_LIFT: cellsymb='L'; break;
                case OPEN_LIFT: cellsymb='L'; break;
                case EARTH: cellsymb='.'; break;
                default: cellsymb='U';
            }
            printf("%c",cellsymb);
        }
        printf("\n\t");
    }
}
/////////////////////////////////////////////

FieldSim::FieldSim() {
}

FieldSim::FieldSim(const FieldSim& orig) {
}

FieldSim::~FieldSim() {
}

void FieldSim::CalcNextState(Field* pField)
{
    Field NewField=(*pField);
    for(int y=0;y<pField->getSize().second;y++)
    {
        for(int x=0;x<pField->getSize().first;x++)
        {
            CellType cell=pField->getCellType(Point(x,y));
            
            if(cell==ROCK)
            {
                if(pField->getCellType(Point(x,y-1))==EMPTY)
                {
                    NewField.swap(Point(x,y),Point(x,y-1));
                }
                else if((pField->getCellType(Point(x,y-1))==ROCK))
                {

                    if((pField->getCellType(Point(x+1,y))==EMPTY) &&
                        (pField->getCellType(Point(x+1,y-1))==EMPTY))
                    {
                        NewField.swap(Point(x,y),Point(x+1,y-1));
                    }
                    else if((pField->getCellType(Point(x-1,y))==EMPTY) &&
                        (pField->getCellType(Point(x-1,y-1))==EMPTY))
                    {
                        NewField.swap(Point(x,y),Point(x-1,y-1));
                    }
                }
                else if((pField->getCellType(Point(x,y-1))==LAMBDA) &&
                        (pField->getCellType(Point(x+1,y))==EMPTY) &&
                        (pField->getCellType(Point(x+1,y-1))==EMPTY))
                {
                    NewField.swap(Point(x,y),Point(x-1,y-1));
                }
                ///+Add CLOSED_LIFT Reaction/////
            }
        }
    }
    
    (*pField)=NewField;
    
}