//============================================================================
// Name        : main.cpp
// Author      : Group №2
// Version     :
// Copyright   :
// Description : Supaplex analog for icfp contest
//============================================================================

#include <cstdlib>
#include <iostream>
//#include <term.h>
#include "stdinclude.h"
#include "AStar.h"

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
	Field *result = new Field(file_buf);
	delete [] file_buf;
	LOGINFO("Map loaded from \"%s\"", mapFileName.c_str());
	return result;
}

string convert(const vector<Point>& coordinates) {
	string result;
	for (int i = coordinates.size() - 2; i >= 0; i--) {
		if (coordinates[i + 1].x - coordinates[i].x == 1) {
			result += 'L';
		} else if (coordinates[i + 1].x - coordinates[i].x == -1) {
			result += 'R';
		} else if (coordinates[i + 1].y - coordinates[i].y == 1) {
			result += 'U';
		} else if (coordinates[i + 1].y - coordinates[i].y == -1) {
			result += 'D';
		} else {
			result += 'W';
		}
	}
	return result;
}

int main(int argc, char** argv) {
    HTMLLogger Logger;
    Logger.Init("LOG.html","MainLog");
    SetLogger(&Logger);
    
    Field* field = createField("res/maps/test_field.txt");
    if (!field) {
    	printf("Map load error! (See LOG.html)\n");
    	return 0;
    }
    
    AStar astar;
    vector<Point> result = astar.solve(field->getRobot()->getCoordinate(), field->getLift()->getCoordinate(), *field);
    cout << convert(result) << endl;
    /*FieldSim fieldSim;
  
    int nStep=0;
    
    DrawField(field, nStep++);
    Field* oldField = field;
    while(true)
    {
        Field *newField = fieldSim.CalcNextState(oldField);
        DrawField(newField, nStep++);
        oldField = newField;
        getchar();
    };*/
    return 0;
}
