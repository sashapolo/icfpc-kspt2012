//============================================================================
// Name        : main.cpp
// Author      : Group №2
// Version     :
// Copyright   :
// Description : Supaplex analog for icfp contest
//============================================================================

#include <cstdlib>
#include <iostream>
#include "stdinclude.h"
#include "Solver.h"

using namespace std;

Field* createField(const string mapFileName) {
	ifstream file;
	char* file_buf;
	int file_size;

	file.open(mapFileName.c_str(), ifstream::in);

	if(!file.is_open()) {
		LOGERROR("Can't load map from \"%s\": can't open file", mapFileName.c_str());
		return NULL;
	};

	file.seekg (0, ios::end);
	file_size = file.tellg();
	if(file_size==0) {
		LOGERROR("Can't load map from \"%s\": file is empty", mapFileName.c_str());
		file.close();
		return NULL;
	}

	file_buf = new char[file_size];
	file.seekg(0, ios::beg);
	file.read(file_buf, file_size);
	file.close();
	Field *result;
	try {
		result = new Field(file_buf);
	} catch (Field::FieldParseException&) {
		LOGERROR("Can't load map from \"%s\": map is incorrect", mapFileName.c_str());
		file.close();
		delete [] file_buf;
		return NULL;
	}
	delete [] file_buf;
	LOGINFO("Map loaded from \"%s\"", mapFileName.c_str());
	return result;
}


int main(int argc, char** argv) {
    HTMLLogger Logger;
    Logger.Init("LOG.html","MainLog");
    SetLogger(&Logger);
    
    Field* field1 = createField("res/maps/test_field.txt");
    if (!field1) {
    	printf("Map load error! (See LOG.html)\n");
    	return 0;
    }
    Field* field2 = field1;
    
    Solver s;
    string result = s.solve(field1);
    FieldSim fieldSim;
	int nStep=0;
	sSimResult res; //Robot simulation result (end state, steps, lambdas, path)

	DrawField(field2,&res.path, nStep++);
	Field *newField = fieldSim.CalcRobotSteps(field2,result,&res);
	DrawField(newField,&res.path, nStep++);
	printf("NumSteps: %d, NumLambdas: %d, State: %s\n",res.stepsTaken,res.lambdaReceived,stateToStr(res.state));

    /*FieldSim fieldSim;
    int nStep=0;
    Field* oldField = field;
    sSimResult res; //Robot simulation result (end state, steps, lambdas, path)
    char inStr[128];

    printf("Controls:\n\tU - up\n\tD - down\n\tL - left\n\tR - right\n\tW - wait\n\tA - abort\nEnter - accept\n");
    DrawField(field,&res.path, nStep++);
    while(true)
    {
        scanf("%s",inStr);
        Field *newField = fieldSim.CalcRobotSteps(oldField,inStr,&res);
        DrawField(newField,&res.path, nStep++);
        printf("NumSteps: %d, NumLambdas: %d, State: %s\n",res.stepsTaken,res.lambdaReceived,stateToStr(res.state));
        oldField = newField;
    };*/
    return 0;
}
