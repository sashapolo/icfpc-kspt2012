//============================================================================
// Name        : main.cpp
// Author      : Group ���2
// Version     :
// Copyright   :
// Description : Supaplex analog for icfp contest
//============================================================================


#include <cstdlib>
#include <iostream>
#include <fstream>

#include "Solver.h"


/**
 * Создание поля.
 * @param string mapFileName - имя файла карты.
 * @return результат создания поля.
 */
Field* createField(const std::string mapFileName) {
	std::ifstream file;
	char* file_buf;
	int file_size;

	file.open(mapFileName.c_str(), std::ifstream::in);

	if(!file.is_open()) {
		LOGERROR("Can't load map from \"%s\": can't open file", mapFileName.c_str());
		return NULL;
	};

	file.seekg (0, std::ios::end);
	file_size = file.tellg();
	if(file_size==0) {
		LOGERROR("Can't load map from \"%s\": file is empty", mapFileName.c_str());
		file.close();
		return NULL;
	}

	file_buf = new char[file_size];
	file.seekg(0, std::ios::beg);
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


/**
 * Пошаговая отрисовка.
 * @param Field* pField - поле.
 * @param string path - путь робота.
 */
void drawStepByStep(Field* const pField, const std::string path) {
	FieldSim fieldSim;
	sSimResult res;
	int nStep = 0;
	drawField(pField, &res.path, nStep++);
	std::string t;
	t += path[0];
	Field *newField = fieldSim.calcRobotSteps(pField, t, &res);
	for (unsigned int i = 1; i < path.size(); i++) {
		drawField(newField, &res.path, nStep++);
		std::string t;
		t += path[i];
		newField = fieldSim.calcRobotSteps(newField, t, &res);
	}
	printf("Score: %d, NumSteps: %d, NumLambdas: %d, LC: %d, State: %s\n",
			res.score,
			res.stepsTaken,
			res.lambdaReceived,
			newField->getLambdaCount(),
			stateToStr(res.state));
}

/**
 * Главная функция.
 * @param int argc - иаргумент командной строки.
 * @param char** argv - аргумент командной строки.
 * @return результат запускад.
 */
int main(int argc, char** argv) {
    if (argc != 2) {
    	printf("Usage: solver <map path>\n");
    	return 1;
    }

	HTMLLogger Logger;
    Logger.Init("LOG.html","MainLog");
    SetLogger(&Logger);

    Field* field = createField(argv[1]);
	if (!field) {
		printf("Map load error! (See LOG.html)\n");
		return 0;
	}
	Solver s(field);
	std::string result = s.solve();
	std::cout<<result<<'\n';
	drawStepByStep(field, result);

//	Field *oldField = field;
//	FieldSim fieldSim;
//	sSimResult res;
//	int nStep = 0;
//	char inStr[100];
//    printf("Controls:\n\tU - up\n\tD - down\n\tL - left\n\tR - right\n\tW - wait\n\tA - abort\nEnter - accept\n");
//    drawField(field, &res.path, nStep++);
//    while(true)
//    {
//        scanf("%s",inStr);
//        Field *newField = fieldSim.calcRobotSteps(oldField,inStr,&res);
//        drawField(newField,&res.path, nStep++);
//        printf("Score: %d, NumSteps: %d, NumLambdas: %d, LC: %d, State: %s\n",res.score,res.stepsTaken,res.lambdaReceived,newField->getLambdaCount(),stateToStr(res.state));
//        oldField = newField;
//    };
    return 0;
}
