#include "base/Point.h"

#include "PointTest.h"

CPPUNIT_TEST_SUITE_REGISTRATION(PointTest);

PointTest::PointTest() {
}

PointTest::~PointTest() {
}

void PointTest::setUp() {
}

void PointTest::tearDown() {
}

void PointTest::testPlus() {
    testPlusRightResults();
}

void PointTest::testMinus() {
    testMinusRightResults();
}

void PointTest::testPlusRightResults() {
    testPlusRightResultsNeg();
    testPlusRightResultsZero();
    testPlusRightResultsPos();
}

void PointTest::testMinusRightResults() {
    testMinusRightResultsNeg();
    testMinusRightResultsZero();
    testMinusRightResultsPos();
}

void PointTest::testPlusRightResultsNeg() {
    Point p1(-13, -42);
    Point p2(-42, -128);
    Point p3 = p1 + p2;
    CPPUNIT_ASSERT(p3.x == -55);
    CPPUNIT_ASSERT(p3.y == -170);
}

void PointTest::testPlusRightResultsZero() {
    Point p1(0, 0);
    Point p2(0, 0);
    Point p3 = p1 + p2;
    CPPUNIT_ASSERT(p3.x == 0);
    CPPUNIT_ASSERT(p3.y == 0);
}

void PointTest::testPlusRightResultsPos() {
    Point p1(13, 42);
    Point p2(42, 128);
    Point p3 = p1 + p2;
    CPPUNIT_ASSERT(p3.x == 55);
    CPPUNIT_ASSERT(p3.y == 170);
}

void PointTest::testMinusRightResultsNeg() {
    Point p1(-13, -42);
    Point p2(-42, -128);
    Point p3 = p1 - p2;
    CPPUNIT_ASSERT(p3.x == 29);
    CPPUNIT_ASSERT(p3.y == 86);
}

void PointTest::testMinusRightResultsZero() {
    Point p1(0, 0);
    Point p2(0, 0);
    Point p3 = p1 - p2;
    CPPUNIT_ASSERT(p3.x == 0);
    CPPUNIT_ASSERT(p3.y == 0);
}

void PointTest::testMinusRightResultsPos() {
    Point p1(13, 42);
    Point p2(42, 128);
    Point p3 = p1 - p2;
    CPPUNIT_ASSERT(p3.x == -29);
    CPPUNIT_ASSERT(p3.y == -86);
}
