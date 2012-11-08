#include "FieldSim.h"
#include "Logger.h"
#include "Solver.h"

#include "SolverTest.h"

CPPUNIT_TEST_SUITE_REGISTRATION(SolverTest);

SolverTest::SolverTest() {
}

SolverTest::~SolverTest() {
}

void SolverTest::setUp() {
}

void SolverTest::tearDown() {
}

void SolverTest::testSolverConsistency() {

    // Инициализация логгера
    HTMLLogger Logger;
    Logger.Init("log.html","MainLog");
    SetLogger(&Logger);

    // Загрузка поля
    Field* field = createField("res/maps/map9");
    if (!field) {
        puts("Map load error. See log.html for details.\n");
    }

    // Поиск пути для загруженной карты
    Solver s(field);
    std::string solution1 = s.solve();

    // Получение количества очков для найденного пути
    FieldSim simulator;
    sSimResult simResult1;
    simulator.calcRobotSteps(field, solution1, &simResult1);

    // Повторный поиск пути для загруженной карты
    std::string solution2 = s.solve();

    // Повторное получение количества очков для найденного пути
    sSimResult simResult2;
    simulator.calcRobotSteps(field, solution2, &simResult2);

    // Проверка того, что одинаковы пути и количество набранных очков.
    CPPUNIT_ASSERT(solution1 == solution2);
    CPPUNIT_ASSERT(simResult1.score == simResult2.score);

}

Field* SolverTest::createField(const std::string mapFileName) {

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

    file_buf = new char[file_size + 1];
    file.seekg(0, std::ios::beg);
    file.read(file_buf, file_size);
    if (file_buf[file_size -1] != '\n') {
        file_buf[file_size] = '\n';
    }
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
