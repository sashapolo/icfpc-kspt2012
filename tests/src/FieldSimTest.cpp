#include <cstring>

#include <cppunit/Asserter.h>
#include <cppunit/Message.h>
#include <cppunit/SourceLine.h>
#include <cppunit/TestAssert.h>

#include "base/FieldSim.h"

#include "FieldSimTest.h"

CPPUNIT_TEST_SUITE_REGISTRATION(FieldSimTest);

FieldSimTest::FieldSimTest() {
}

FieldSimTest::~FieldSimTest() {
}

void FieldSimTest::setUp() {
}

void FieldSimTest::tearDown() {
}

/**
 * Performs some checks on Field's constructor.
 */
void FieldSimTest::testElementalSteps() {
    testUpStep();
    testDownStep();
    testLeftStep();
    testRightStep();
}

void FieldSimTest::testUpStep() {
    char robotStep = 'U';
    const char* pathToMap = "tests/res/testmaps/ElementalStepsMap";
    const Field field(pathToMap);
    const Field *nextStepField = FieldSim::calcNextState(&field, robotStep);
    Point newPoint = Point(2, 1);
    CPPUNIT_ASSERT(*nextStepField->getRobot()==newPoint);
}

void FieldSimTest::testDownStep() {
    char robotStep = 'D';
    const char* pathToMap = "tests/res/testmaps/ElementalStepsMap";
    const Field field(pathToMap);
    const Field *nextStepField = FieldSim::calcNextState(&field, robotStep);
    Point newPoint = Point(2, 3);
    CPPUNIT_ASSERT(*nextStepField->getRobot()==newPoint);
}

void FieldSimTest::testLeftStep() {
    char robotStep = 'L';
    const char* pathToMap = "tests/res/testmaps/ElementalStepsMap";
    const Field field(pathToMap);
    const Field *nextStepField = FieldSim::calcNextState(&field, robotStep);
    Point newPoint = Point(1, 2);
    CPPUNIT_ASSERT(*nextStepField->getRobot()==newPoint);
}

void FieldSimTest::testRightStep() {
    char robotStep = 'R';
    const char* pathToMap = "tests/res/testmaps/ElementalStepsMap";
    const Field field(pathToMap);
    const Field *nextStepField = FieldSim::calcNextState(&field, robotStep);
    Point newPoint = Point(3, 2);
    CPPUNIT_ASSERT(*nextStepField->getRobot()==newPoint);
}


