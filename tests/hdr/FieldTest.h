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

    /**
     * Воспроизводит ситуации, при которых генерируется FieldParseException.
     */
    void testConstructorFieldParseException();

    /**
     * Проверяет, что FieldParseException генерируется,
     * если задано неверное имя файла.
     * 
     * Asserts:
     * * FieldParseException при пустой строке в качестве имени файла.
     */
    void testConstructorWrongFileName();

    /**
     * Проверяет, что FieldParseException генерируется,
     * если задан пустой файл.
     * 
     * Asserts:
     * * FieldParseException, если файл пуст.
     */
    void testConstructorEmptyFile();

    /**
     * Проверяет, что FieldParseException генерируется,
     * если в файле карты присутствует пустая строчка.
     * 
     * Asserts:
     * * FieldParseException, если в файле присутствует пустая строка.
     */
    void testConstructorEmptyLine();

    /**
     * Проверяет, что FieldParseException генерируется,
     * если в загружаемой карте более одного робота.
     * 
     * Asserts:
     * * FieldParseException, если в файле присутствует более одного робота.
     */
    void testConstructorMoreThanOneRobot();

    /**
     * Проверяет, что FieldParseException генерируется,
     * если в загружаемой карте более одного закрытого лифта.
     * 
     * Asserts:
     * * FieldParseException, если в файле присутствует более одного закрытого лифта.
     */
    void testConstructorMoreThanOneClosedLifts();

    /**
     * Проверяет, что FieldParseException генерируется,
     * если в загружаемой карте присутствует открытый лифт.
     * 
     * Asserts:
     * * FieldParseException, если в файле присутствует открытый лифт.
     */
    void testConstructorContainsOpenedLift();

    /**
     * Проверяет, что FieldParseException генерируется,
     * если в файле встречаются незнакомые символы.
     * 
     * Asserts:
     * * FieldParseException при встрече символа A
     */
    void testConstructorIncorrectSymbols();

    /**
     * Проверяет, что FieldParseException генерируется,
     * если на карте нет робота.
     * 
     * Asserts:
     * * FieldParseException если на карте нет робота.
     */
    void testConstructorNoRobot();

    /**
     * Проверяет, что FieldParseException генерируется,
     * если на карте нет закрытого лифта.
     * 
     * Asserts:
     * * FieldParseException если на карте нет закрытого лифта.
     */
    void testConstructorNoLift();

};

#endif	/* FIELDTEST_H */
