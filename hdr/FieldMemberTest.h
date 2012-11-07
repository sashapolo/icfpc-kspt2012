#ifndef FIELDMEMBERTEST_H
#define	FIELDMEMBERTEST_H

#include <cppunit/extensions/HelperMacros.h>

/**
 * Инкапсулирует тесты для класса FieldMember.
 */
class FieldMemberTest : public CPPUNIT_NS::TestFixture {

    CPPUNIT_TEST_SUITE(FieldMemberTest);

    CPPUNIT_TEST(testEqualsOperator);

    CPPUNIT_TEST_SUITE_END();

public:
    FieldMemberTest();
    virtual ~FieldMemberTest();
    void setUp();
    void tearDown();

private:

    /**
     * <p>Проверяет следующие свойства оператора ==, переопределенного для FieldMember:</p>
     * <p>* Рефлексивность<p/>
     * <p>  Для любого a, неравного NULL, a == a должно вычисляться в true.</p>
     * <p>* Симметричность</p>
     * <p>
     *      Для любых a и b, неравных NULL,
     *      a == b должно вычисляться в true тогда и только тогда,
     *      когда b == a вычисляется в true.
     * </p>
     * <p>* Транзитивность</p>
     * <p>
     *      Для любых a, b, c, неравных NULL,
     *      если a == b вычисляется в true и b == c вычисляется в true,
     *      то a == c также должно вычисляться в true.
     * </p>
     * <p>* Консистентность</p>
     * <p>
     *      Для любых a и b, неравных NULL, при неизменном состоянии объектов
     *      a == b постоянно должно вычисляться в одно и то же значение.
     * </p>
     */
    void testEqualsOperator();
    /**
     * Вспомогательная функция для проверки рефлексивности.
     */
    void testEqualsOperatorReflexivity();
    /**
     * Вспомогательная функция для проверки симметричности.
     */
    void testEqualsOperatorSymmetry();
    /**
     * Вспомогательная функция для проверки симметричности для случая равных объектов.
     */
    void testEqualsOperatorSymmetryEqObj();
    /**
     * Вспомогательная функция для проверки симметричности для случая неравных объектов.
     */
    void testEqualsOperatorSymmetryUneqObj();
    /**
     * Вспомогательная функция для проверки транзитивности.
     */
    void testEqualsOperatorTransitivity();
    /**
     * Вспомогательная функция для проверки транзитивности для случая равных объектов.
     */
    void testEqualsOperatorTransitivityEqObj();
    /**
     * Вспомогательная функция для проверки транзитивности для случая неравных объектов.
     */
    void testEqualsOperatorTransitivityUneqObj();
    /**
     * Вспомогательная функция для проверки консистентности.
     */
    void testEqualsOperatorConsistency();

};

#endif	/* FIELDMEMBERTEST_H */
