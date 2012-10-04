//============================================================================
// Name        : main.cpp
// Author      : Group №2
// Version     :
// Copyright   :
// Description : Supaplex analog for icfp contest
//============================================================================

#include <cstdlib>
#include <term.h>
#include "stdinclude.h"

using namespace std;

int main(int argc, char** argv) {

    HTMLLogger Logger;
    Logger.Init("LOG.html","MainLog");
    SetLogger(&Logger);
    
    Field field;
    if(!field.load("res/maps/test_field.txt")) {printf("Map load error! (See LOG.html)\n"); return 0;}
    
    FieldSim fieldSim;
  
    int nStep=0;
    
    DrawField(&field,nStep++);
    while(true)
    {
        fieldSim.CalcNextState(&field);
        DrawField(&field,nStep++);
        getchar();
    };
    
    return 0;
}