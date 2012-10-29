#include <string>

#include <cppunit/Asserter.h>
#include <cppunit/Message.h>
#include <cppunit/SourceLine.h>
#include <cppunit/TestAssert.h>

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

/**
 * Performs some checks on Field's constructor.
 */
void FieldTest::testConstructor() {
    // Simplest test ever
    string simpleTestMap("##\n##\n");
    Field field(simpleTestMap);
    CellType cellType = WALL;
    CPPUNIT_ASSERT(field.getXY(*new Point(0,0))->getType() == cellType);
    CPPUNIT_ASSERT(field.getXY(*new Point(0,1))->getType() == cellType);
    CPPUNIT_ASSERT(field.getXY(*new Point(1,0))->getType() == cellType);
    CPPUNIT_ASSERT(field.getXY(*new Point(1,1))->getType() == cellType);
}
