#include <string>

#include "Field.h"
#include "FieldMember.h"
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

void FieldTest::testMethod() {
    // Simplest test ever
    string simpleTestMap("##\n##\n");
    Field field(simpleTestMap);
    CellType cellType = WALL;
    CPPUNIT_ASSERT(field.getXY(*new Point(0,0))->getType() == cellType);
    CPPUNIT_ASSERT(field.getXY(*new Point(0,1))->getType() == cellType);
    CPPUNIT_ASSERT(field.getXY(*new Point(1,0))->getType() == cellType);
    CPPUNIT_ASSERT(field.getXY(*new Point(1,1))->getType() == cellType);
}

void FieldTest::testFailedMethod() {
    CPPUNIT_ASSERT(false);
}

