// *****************************************************************************
// Waits for return of score calculation to the simulator ...
// *****************************************************************************

#include "base/FieldSim.h"
#include "Logger.h"
#include "algo/Solver.h"

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
    try {
    	Field field("res/maps/map1");
		// Поиск пути для загруженной карты
		Solver s1(&field);
		std::string solution1 = s1.solve();

		// Получение количества очков для найденного пути
		sSimResult simResult1;
		const Field *oldField = new Field(field);
		for (unsigned int i = 0; i < solution1.size(); i++) {
			const Field *newField = FieldSim::calcNextStateEx(oldField, solution1[i], &simResult1);
			delete oldField;
			oldField = newField;
		}
		delete oldField;

		// Повторный поиск пути для загруженной карты
		Solver s2(&field);
		std::string solution2 = s2.solve();

		// Повторное получение количества очков для найденного пути
		sSimResult simResult2;
		oldField = new Field(field);
		for (unsigned int i = 0; i < solution2.size(); i++) {
			const Field *newField = FieldSim::calcNextStateEx(oldField, solution2[i], &simResult2);
			delete oldField;
			oldField = newField;
		}
		delete oldField;

		// Проверка того, что одинаковы пути и количество набранных очков.
		CPPUNIT_ASSERT(solution1 == solution2);
		CPPUNIT_ASSERT(simResult1.score == simResult2.score);
    } catch (const Field::FieldParseException&) {
		std::cout << "Map load error. See log.html for details.\n";
	}
}


