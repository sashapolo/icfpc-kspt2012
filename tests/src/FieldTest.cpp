#include <cstring>

#include <cppunit/Asserter.h>
#include <cppunit/Message.h>
#include <cppunit/SourceLine.h>
#include <cppunit/TestAssert.h>

#include "base/Field.h"

#include "FieldTest.h"

CPPUNIT_TEST_SUITE_REGISTRATION(FieldTest);

FieldTest::FieldTest() {
}

FieldTest::~FieldTest() {
}

void FieldTest::setUp() {
}

void FieldTest::tearDown() {
}

/**
 * Performs some checks on Field's constructor.
 */
void FieldTest::testConstructor() {
    testConstructorRightResults();
}

void FieldTest::testConstructorRightResults() {

    // Простая маленькая корректно сформированная карта, содержащая все типы символов.
    const char* pathToMap = "tests/res/testmaps/OrdinaryCorrectMap";
    Field field(pathToMap);

    /*
     * Сравнение того,
     * как карта считалась в объект с теми символами,
     * которые мы сами считываем из файла.
     */

    // Проверяем, что совпадают размеры
    int curLen; // Длина считываемой строки
    int maxLen = 0;
    int numOfLines = 0;
    std::ifstream mapFile(pathToMap);
    std::string line;
    while(std::getline(mapFile, line)) {

        // Нахождение размера по X
        // Поиск максимальной длины
        curLen = line.length();
        if(curLen > maxLen) {
            maxLen = curLen;
        }

        // Поэлементное сравнение считанной карты и файла
        std::string::iterator itr    = line.begin();
        std::string::iterator endItr = line.end();
        int i = 0;
        for(; itr != endItr; itr++) {
            CPPUNIT_ASSERT( *itr == field.getXY(i++, numOfLines) );
        }

        // Нахождение размера по Y
        numOfLines++;

    }

    std::pair<int, int> fieldSize = field.getSize();
    CPPUNIT_ASSERT(fieldSize.first == maxLen);
    CPPUNIT_ASSERT(fieldSize.second == numOfLines);

}
