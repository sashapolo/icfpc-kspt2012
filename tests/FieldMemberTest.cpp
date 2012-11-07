#include <cstdio>

#include "FieldMember.h"

#include "FieldMemberTest.h"

CPPUNIT_TEST_SUITE_REGISTRATION(FieldMemberTest);

FieldMemberTest::FieldMemberTest() {
}

FieldMemberTest::~FieldMemberTest() {
}

void FieldMemberTest::setUp() {
}

void FieldMemberTest::tearDown() {
}

void FieldMemberTest::testEqualsOperator() {
    testEqualsOperatorReflexivity();
    testEqualsOperatorSymmetry();
}

void FieldMemberTest::testEqualsOperatorReflexivity() {
    Point coord(1, 1);
    FieldMember fm(coord, WALL);
    CPPUNIT_ASSERT(fm == fm);
}

void FieldMemberTest::testEqualsOperatorSymmetry() {
    testEqualsOperatorSymmetryEqObj();
    testEqualsOperatorSymmetryUneqObj();
}

void FieldMemberTest::testEqualsOperatorSymmetryEqObj() {
    Point coord(1, 1);
    FieldMember fm1(coord, WALL);
    FieldMember fm2(coord, WALL);
    // && - both should be true
    CPPUNIT_ASSERT( (fm1 == fm2) && ((fm2 == fm1)) );
}

void FieldMemberTest::testEqualsOperatorSymmetryUneqObj() {
    Point coord1(1, 1);
    Point coord2(2, 2);
    FieldMember fm1(coord1, WALL);
    FieldMember fm2(coord2, STONE);
    // || - both should be false
    CPPUNIT_ASSERT( !( (fm1 == fm2) || (fm2 == fm1) ) );
}

void FieldMemberTest::testEqualsOperatorTransitivity() {
    testEqualsOperatorTransitivityEqObj();
    testEqualsOperatorTransitivityUneqObj();
}

void FieldMemberTest::testEqualsOperatorTransitivityEqObj() {
    Point coord(1, 1);
    FieldMember fm1(coord, WALL);
    FieldMember fm2(coord, WALL);
    FieldMember fm3(coord, WALL);
    // && - all should be true
    CPPUNIT_ASSERT( (fm1 == fm2) && (fm2 == fm3) && (fm1 == fm3) );
}

void FieldMemberTest::testEqualsOperatorTransitivityUneqObj() {
    Point coord1(1, 1);
    Point coord2(2, 2);
    Point coord3(3, 3);
    FieldMember fm1(coord1, WALL);
    FieldMember fm2(coord2, STONE);
    FieldMember fm3(coord3, LAMBDA);
    // || - all should be true
    CPPUNIT_ASSERT( !( (fm1 == fm2) || (fm2 == fm3) || (fm1 == fm3) ) );
}

void FieldMemberTest::testEqualsOperatorConsistency() {
    Point coord(1, 1);
    FieldMember fm1(coord, WALL);
    FieldMember fm2(coord, WALL);
    // State of the objects is changed: results should be the same
    CPPUNIT_ASSERT(fm1 == fm2);
    CPPUNIT_ASSERT(fm1 == fm2);
    CPPUNIT_ASSERT(fm1 == fm2);
}