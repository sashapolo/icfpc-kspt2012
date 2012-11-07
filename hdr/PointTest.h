#ifndef POINTTEST_H
#define	POINTTEST_H

#include <cppunit/extensions/HelperMacros.h>

class PointTest : public CPPUNIT_NS::TestFixture {

    CPPUNIT_TEST_SUITE(PointTest);

    CPPUNIT_TEST(testPlus);

    CPPUNIT_TEST_SUITE_END();

public:
    PointTest();
    virtual ~PointTest();
    void setUp();
    void tearDown();

private:
    void testPlus();
    /*
     * ToDo: дополнить для остальных комбинаций?
     * Двумерная точка, три знака координат: 2^3 = 8 вариантов.
     * Оператор бинарный, нужно перебрать сочетания всех вариантов:
     * Насколько понимаю, это 8! вариантов.
     */
    void testPlusRightResults();
    void testPlusRightResultsNeg();
    void testPlusRightResultsZero();
    void testPlusRightResultsPos();
};

#endif	/* POINTTEST_H */
