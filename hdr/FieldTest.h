#ifndef FIELDTEST_H
#define FIELDTEST_H

#include <cppunit/extensions/HelperMacros.h>

class FieldTest : public CPPUNIT_NS::TestFixture {
    CPPUNIT_TEST_SUITE(FieldTest);

    CPPUNIT_TEST(testMethod);
    CPPUNIT_TEST(testFailedMethod);

    CPPUNIT_TEST_SUITE_END();

public:
    FieldTest();
    virtual ~FieldTest();
    void setUp();
    void tearDown();

private:
    void testMethod();
    void testFailedMethod();
};

#endif	/* FIELDTEST_H */

