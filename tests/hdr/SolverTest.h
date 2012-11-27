#ifndef SOLVERTEST_H
#define	SOLVERTEST_H

#include <cppunit/extensions/HelperMacros.h>

/**
 * Инкапсулирует тесты для класса Solver.
 */
class SolverTest : public CPPUNIT_NS::TestFixture {

    CPPUNIT_TEST_SUITE(SolverTest);

    CPPUNIT_TEST(testSolverConsistency);

    CPPUNIT_TEST_SUITE_END();

public:
    SolverTest();
    virtual ~SolverTest();
    void setUp();
    void tearDown();

private:

    /**
     * Проверяет консистентность решателя:
     * для одной и той же карты он должен постоянно выдавать один и тот же путь
     * с одинаковым количеством набранных очков.
     *
     * Проверяемые утверждения:
     * 1) При повторном решении карты получен тот же путь.
     * 2) При повторном решении карты набрано такое же количество очков.
     */
    void testSolverConsistency();
};

#endif	/* SOLVERTEST_H */
