#ifndef _TESTS_H_
#define _TESTS_H_

#include "TestTemplate.h"

class Tests : public TestTemplate
{
public:
    typedef short int si;
    static const int SIZE = 4;
    
    static void usualTest(std::ostream & ostream);
    static void compareToMatrixMultiply(std::ostream & ostream);
    static void thereAndBackAgain(std::ostream & ostream);
    static void speedTest(std::ostream & ostream, const int TRANSFORMS_COUNT);

private:
    static void matrixRandomFill(si(&matrix)[SIZE][SIZE]);
    static bool matrixCompare(
            const si (&left)[SIZE][SIZE],
            const si (&right)[SIZE][SIZE]
            );
    static void matrixSubtraction(
            const si(&left)[SIZE][SIZE],
            const si(&right)[SIZE][SIZE],
            si(&result)[SIZE][SIZE]
            );
    static void matrixAddition(
            const si(&left)[SIZE][SIZE],
            const si(&right)[SIZE][SIZE],
            si(&result)[SIZE][SIZE]
            );
    static void matrixMultiply(
            const si(&left)[SIZE][SIZE],
            const si(&right)[SIZE][SIZE],
            si(&result)[SIZE][SIZE]
            );
    static void matrixDivByConst(
            const si(&matrix)[SIZE][SIZE],
            const int divider,
            si(&result)[SIZE][SIZE]);
    static std::string matrixPrint(const si(&matrix)[SIZE][SIZE]);
    static std::string debug_details(
            const si(&r)[SIZE][SIZE],
            const si(&p)[SIZE][SIZE],
            const si(&c)[SIZE][SIZE],
            const si(&c_right_answer)[SIZE][SIZE]
            );
};

#endif

