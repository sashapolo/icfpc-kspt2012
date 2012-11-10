#include <cstdio>

#include "Path.h"

#include "PathTest.h"

CPPUNIT_TEST_SUITE_REGISTRATION(PathTest);

PathTest::PathTest() {
}

PathTest::~PathTest() {
}

void PathTest::setUp() {
}

void PathTest::tearDown() {
}

// ToDo: think how to generify this properly.

//void PathTest::testOneElemPath() {
//    Path path;
//    CPPUNIT_ASSERT(path.isEmpty() );
//    Point p1(0,0);
//    path.addCell(p1);
//    CPPUNIT_ASSERT(!path.isEmpty() );
//    CPPUNIT_ASSERT(path.getSize() == 1);
//    CPPUNIT_ASSERT(path.getCell() == p1 );
//    // ToDo: use getDistance();
//    path.deleteCell(0);
//    CPPUNIT_ASSERT(path.isEmpty() );
//    CPPUNIT_ASSERT(path.getSize() == 0);
//}

void PathTest::testPath() {
    testNPointPath(1);
    testNPointPath(2);
    testNPointPath(3);
    testNPointPath(42);
    testNPointPath(1000000);
}

void PathTest::testNPointPath(int numOfPoints) {
    Path path;
    CPPUNIT_ASSERT(path.isEmpty() );
    fillPathWithNPoints(path, numOfPoints);
    CPPUNIT_ASSERT(!path.isEmpty() );
    CPPUNIT_ASSERT(path.getSize() == numOfPoints );
    // ToDo: think how to use getCell()
    // ToDo: think how to use getDistance()
    // ToDo: think how to use swap()?
    // ToDo: think how to use deleteCell()?
}

// ToDo: этот метод, по хорошему, тоже нужно протестировать. Тест теста?
void PathTest::fillPathWithNPoints(Path &path, int numOfPoints) {

    // Проверка аргументов
    if(!path.isEmpty()) {
        // ToDo: throw an exception
        /*
         * Аналог IllegalArgumentException. Навскидку что должен хранить:
         * * имя аргумента, для которого не прошла проверка
         * * его тип
         * * сообщение об ошибке
         */
        return;
    }
    if(!(numOfPoints > 0)) {
        // ToDo: throw an exception
        return;
    }

    // Заполнение пути
    // На первом шаге координаты точки не должны измениться
    Point p(0,0);
    path.addCell(p);
    bool right = true;    // Флаг для цикла, указывающий, вправо или вверх.
    // Мелкая оптимизация: вычитание на каждой итерации избыточно.
    int cycleBoundary = (numOfPoints-1);
    for(int i = 0; i < cycleBoundary; i++) {
        if(right) {
            p.x += 1;
            path.addCell(p);
            right = false;
        } else {
            p.y += 1;
            path.addCell(p);
            right = true;
        }
    }

}