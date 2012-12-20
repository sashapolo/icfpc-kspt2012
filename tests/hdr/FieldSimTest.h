/* 
 * File:   FieldSimTest.h
 * Author: xeenon
 *
 * Created on 17 Декабрь 2012 г., 2:01
 */

#ifndef FIELDSIMTEST_H
#define	FIELDSIMTEST_H

#include <cppunit/extensions/HelperMacros.h>

/**
 * Инкапсулирует тесты для класса FieldSim.
 */
class FieldSimTest : public CPPUNIT_NS::TestFixture {

    CPPUNIT_TEST_SUITE(FieldSimTest);

    CPPUNIT_TEST(testElementalSteps);

    CPPUNIT_TEST_SUITE_END();

public:
    FieldSimTest();
    virtual ~FieldSimTest();
    void setUp();
    void tearDown();

private:
    
    /**
     * Метод, агрегирующий тесты элементархных перемещений робота.
     */
    void testElementalSteps();
    /**
     * Проверяет правильность хода робота вверх.
     */
    void testUpStep();
    /**
     * Проверяет правильность хода робота вниз.
     */
    void testDownStep();
    /**
     * Проверяет правильность хода робота влево.
     */
    void testLeftStep();
    /**
     * Проверяет правильность хода робота вправо.
     */
    void testRightStep();
    
};


#endif	/* FIELDSIMTEST_H */

