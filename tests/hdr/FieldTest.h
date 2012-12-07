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

    /**
     * Метод, агрегирующий тесты конструкторов класса Field.
     */
    void testConstructor();

    /**
     * Проверяет правильность работы конструктора на правильных данных.
     * 
     * Asserts:
     * * Длина горизонтали считанной карты равна длине наибольшей строки файла
     * * Длина вертикали считанной карты равна количеству строк в файле
     * * Содержимое каждого поля карты совпадает с символом из файла (все строки в файле должны быть одинаковой длины)
     */
    void testConstructorRightResults();

};

#endif	/* FIELDTEST_H */

