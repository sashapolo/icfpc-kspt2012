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

void FieldMemberTest::equalsOperatorTest() {
    equalsOperatorReflexivityTest();
    equalsOperatorSymmetryTest();
}

void FieldMemberTest::equalsOperatorReflexivityTest() {
    Point coord(1, 1);
    FieldMember fm(coord, WALL);
    CPPUNIT_ASSERT(fm == fm);
}

void FieldMemberTest::equalsOperatorSymmetryTest() {
    equalsOperatorSymmetryEqObjTest();
    equalsOperatorSymmetryUneqObjTest();
}

void FieldMemberTest::equalsOperatorSymmetryEqObjTest() {
    Point coord(1, 1);
    FieldMember fm1(coord, WALL);
    FieldMember fm2(coord, WALL);
    // && - both should be true
    CPPUNIT_ASSERT( (fm1 == fm2) && ((fm2 == fm1)) );
}

void FieldMemberTest::equalsOperatorSymmetryUneqObjTest() {
    Point coord1(1, 1);
    Point coord2(2, 2);
    FieldMember fm1(coord1, WALL);
    FieldMember fm2(coord2, STONE);
    // || - both should be false
    CPPUNIT_ASSERT( !( (fm1 == fm2) || (fm2 == fm1) ) );
}

void FieldMemberTest::equalsOperatorTransitivityTest() {
    equalsOperatorTransitivityEqObjTest();
    equalsOperatorTransitivityUneqObjTest();
}

void FieldMemberTest::equalsOperatorTransitivityEqObjTest() {
    Point coord(1, 1);
    FieldMember fm1(coord, WALL);
    FieldMember fm2(coord, WALL);
    FieldMember fm3(coord, WALL);
    // && - all should be true
    CPPUNIT_ASSERT( (fm1 == fm2) && (fm2 == fm3) && (fm1 == fm3) );
}

void FieldMemberTest::equalsOperatorTransitivityUneqObjTest() {
    Point coord1(1, 1);
    Point coord2(2, 2);
    Point coord3(3, 3);
    FieldMember fm1(coord1, WALL);
    FieldMember fm2(coord2, STONE);
    FieldMember fm3(coord3, LAMBDA);
    // || - all should be true
    CPPUNIT_ASSERT( !( (fm1 == fm2) || (fm2 == fm3) || (fm1 == fm3) ) );
}

void FieldMemberTest::equalsOperatorConsistencyTest() {
    Point coord(1, 1);
    FieldMember fm1(coord, WALL);
    FieldMember fm2(coord, WALL);
    // State of the objects is changed: results should be the same
    CPPUNIT_ASSERT(fm1 == fm2);
    CPPUNIT_ASSERT(fm1 == fm2);
    CPPUNIT_ASSERT(fm1 == fm2);
}