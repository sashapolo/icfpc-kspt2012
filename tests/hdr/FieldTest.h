#ifndef FIELDTEST_H
#define FIELDTEST_H

#include <cppunit/extensions/HelperMacros.h>

/**
 * Инкапсулирует тесты для класса Field.
 */
class FieldTest : public CPPUNIT_NS::TestFixture {

    CPPUNIT_TEST_SUITE(FieldTest);

    CPPUNIT_TEST(testConstructor);

    CPPUNIT_TEST_SUITE_END();

public:
    FieldTest();
    virtual ~FieldTest();
    void setUp();
    void tearDown();

private:

    void testConstructor();

};

#endif	/* FIELDTEST_H */

