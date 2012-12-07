//============================================================================
// Name        : main.cpp
// Author      : Group 2
// Version     :
// Copyright   :
// Description : Supaplex analog for icfp contest
//============================================================================


#include "stdinclude.h"
#include "algo/Solver.h"


/**
 * Главная функция.
 * @param int argc - аргумент командной строки.
 * @param char** argv - аргумент командной строки.
 * @return результат запуска.
 */
int main(int argc, char** argv) {
	// установка обработчика SIGINT
	SignalHandler::setupSignalHandler();

	HTMLLogger Logger;
	Logger.Init("LOG.html","MainLog");
	SetLogger(&Logger);

	try {
		Field field(std::cin);
		//Field field("../../maps-without-extensions/map213");
		Solver s(&field);
		std::string result = s.solve();
		std::cout<<result<<"\n";
		//FieldSim::drawStepByStep(&field, result);
	} catch (const Field::FieldParseException&) {
		//std::cout << "Map load error! (See LOG.html)\n";
		std::cout << "A\n";
		return 0;
	}

//	    Field *oldField = field;
//		FieldSim fieldSim;
//		sSimResult res;
//		int nStep = 0;
//		char inStr[100];
//	    printf("Controls:\n\tU - up\n\tD - down\n\tL - left\n\tR - right\n\tW - wait\n\tA - abort\nEnter - accept\n");
//	    drawField(field, &res.path, nStep++);
//	    while(true)
//	    {
//	        scanf("%s",inStr);
//	        Field *newField = fieldSim.calcRobotSteps(oldField,inStr,&res);
//	        drawField(newField,&res.path, nStep++);
//	        printf("Score: %d, NumSteps: %d, NumLambdas: %d, LC: %d, State: %s\n",res.score,res.stepsTaken,res.lambdaReceived,newField->getLambdaCount(),stateToStr(res.state));
//	        oldField = newField;
//	    };
    return 0;
}
