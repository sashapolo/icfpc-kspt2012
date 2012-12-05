#include <cstdio>
#include <stdexcept>

#include "base/Path.h"

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
    CPPUNIT_ASSERT(!path.isEmpty());
    CPPUNIT_ASSERT(path.getSize() == numOfPoints );
    // ToDo: think how to use getCell()
    // ToDo: think how to use getDistance()
    // ToDo: think how to use swap()?
    // Ну что тут думать, вызываешь swap и вручную проверяешь,
    // что клетки поменялись местами

    /*
     * Мой капитан, как ты будешь делать это для случая с 1 одной точкой?
     * И я хотел добавить еще один тест для пустого пути. На нем как будешь swap'ить?
     * В том то и проблема, что этот метод - попытка написать универсальный тест
     * который бы издевался над путями разной длины (в том числе нулевой),
     * а длину пути можно было бы передавать аргументом.
     * Такой подход не совсем гладко ложится на граничные условия: пустой путь и одна точка.
     * Посмотри на закомментированный метод выше.
     */

    // ToDo: think how to use deleteCell()?
    // удаляешь клетку и смотришь, что ее больше нет (ваш КО)

    /*
     * Мой капитан, как будешь делать это для нулевого пути?
     * См. выше про универсальность метода.
     */

    /*
     * P.S. Не все так просто.
     * Не знаешь всего контекста - не лезь с репликами в стиле "Да что тут делать?!"
     */

}

// ToDo: этот метод, по хорошему, тоже нужно протестировать. Тест теста?
void PathTest::fillPathWithNPoints(Path &path, int numOfPoints) {

    // Проверка аргументов
    if(!path.isEmpty()) {
        throw std::invalid_argument("Path is empty.");
        return;
    }
    if(!(numOfPoints > 0)) {
        throw std::invalid_argument("Number of points is negative.");
        return;
    }

    // Заполнение пути
    bool right = false;    // Флаг для цикла, указывающий, вправо или вверх.
    // Счетчики координат. Необходимы из-за неизменяемости объектов класса Point
    int xCntr = 0;
    int yCntr = 0;
    for(int i = 0; i < numOfPoints; i++) {
        if(right) {
            Point point(xCntr, yCntr);
            path.addCell(point);
            xCntr++;
            right = false;
        } else {
            Point point(xCntr, yCntr);
            path.addCell(point);
            yCntr++;
            right = true;
        }
    }

}

void PathTest::testAddAndGet() {
    Path path;
    Point p0(0,0);
    Point p1(0,1);
    path.addCell(p0);
    // Предполагаем, что == для Point работает корректно
    CPPUNIT_ASSERT(*path.getCell(0) == p0);
    path.addCell(p1);
    CPPUNIT_ASSERT(*path.getCell(1) == p1);
}
