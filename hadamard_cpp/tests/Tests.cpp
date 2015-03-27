#include <iomanip> // for string alignment

#include "../../hadamard_cpp/src/hadamard_inverser.h"
#include "Tests.h"

#define for_row for (int row = 0; row < SIZE; ++row)
#define for_column for (int column = 0; column < SIZE; ++column)


void Tests::usualTest(std::ostream & ostream) {

    si r[SIZE][SIZE] = {
            { 17, 18, 191, 20 },
            { 21, 22, 231, 24 },
            { 251, 26, 27, 28 },
            { 29, 301, 31, 32 }
            };

    si p[SIZE][SIZE] = {
            { 1, 21, 34, 4 },
            { 5, 6, 71, 8 },
            { 91, 10, 11, 12 },
            { 13, 14, 15, 161 }
            };

    si c[SIZE][SIZE];

    HadamardInverser::transform(r, p, c);

    si c_rightAnswer[SIZE][SIZE] = {
            { 50, 16, -34, 20 },
            { -0, -54, -0, 18 },
            { -3, 16, -34, -16 },
            { -0, -18, 36, 18 }
            };

    check(
            ostream,
            matrixCompare(c, c_rightAnswer), 
            debug_details(r, p, c, c_rightAnswer)
            );
}

void Tests::compareToMatrixMultiply(std::ostream & ostream) {
    si r[SIZE][SIZE];
    si p[SIZE][SIZE];
    matrixRandomFill(r);
    matrixRandomFill(p);

    const si a[SIZE][SIZE] = {
            { 1, 1, 1, 1 },
            { 1, 1,-1,-1 },
            { 1,-1,-1, 1 },
            { 1,-1, 1,-1 }
            };

    // direct calc 1 / 16 * A * (R - P) * A

    si r_minus_p[SIZE][SIZE];
    matrixSubtraction(r, p, r_minus_p);

    si a_mul_r_minus_p[SIZE][SIZE];
    matrixMultiply(a, r_minus_p, a_mul_r_minus_p);

    si a_mul_r_minus_p_mul_a[SIZE][SIZE];
    matrixMultiply(a_mul_r_minus_p, a, a_mul_r_minus_p_mul_a);

    si matrixMultiplyResult[SIZE][SIZE];
    matrixDivByConst(a_mul_r_minus_p_mul_a, 16, matrixMultiplyResult);

    si hadamardTransformResult[SIZE][SIZE];
    HadamardInverser::transform(r, p, hadamardTransformResult);

    check(ostream, 
            matrixCompare(matrixMultiplyResult, hadamardTransformResult),
            debug_details(
                    r, 
                    p, 
                    hadamardTransformResult, 
                    matrixMultiplyResult
                    )
            );
}
void Tests::thereAndBackAgain(std::ostream & ostream) {
    // (C, P) -> R and then (P, R) -> C
    // R = A * C * A + P
    // new_C = HadamardInverser::transform(P, R)
    // matrixCompare(C, new_C)

    si c[SIZE][SIZE];
    si p[SIZE][SIZE];
    matrixRandomFill(c);
    matrixRandomFill(p);

    const si a[SIZE][SIZE] = {
            { 1, 1, 1, 1 },
            { 1, 1, -1, -1 },
            { 1, -1, -1, 1 },
            { 1, -1, 1, -1 }
            };

    si ca[SIZE][SIZE];
    matrixMultiply(c, a, ca);

    si aca[SIZE][SIZE];
    matrixMultiply(a, ca, aca);
    si r[SIZE][SIZE];
    matrixAddition(aca, p, r);

    si new_c[SIZE][SIZE];
    HadamardInverser::transform(r, p, new_c);

    check(ostream, matrixCompare(c, new_c), debug_details(r, p, new_c, c));
}

void Tests::speedTest(std::ostream & ostream, const int TRANSFORMS_COUNT) {

    si p[SIZE][SIZE] = {
            { 1, 21, 34, 4 },
            { 5, 6, 71, 8 },
            { 91, 10, 11, 12 },
            { 13, 14, 15, 161 }
            };

    si r[SIZE][SIZE] = {
            { 17, 18, 191, 20 },
            { 21, 22, 231, 24 },
            { 251, 26, 27, 28 },
            { 29, 301, 31, 32 }
            };
    si c[SIZE][SIZE];

    clock_t begin = clock();
    for (int i = 0; i < TRANSFORMS_COUNT; ++i) {
        HadamardInverser::transform(r, p, c);
    }
    clock_t end = clock();
    double time_in_sec = double(end - begin) / CLOCKS_PER_SEC;

    ostream << "Speed test:\t" << TRANSFORMS_COUNT
        << " transforms in " << time_in_sec << " sec" 
        << " (" << TRANSFORMS_COUNT / time_in_sec << " transforms in sec)"
        << std::endl;
    testsNum++;
}

void Tests::matrixRandomFill(si(&matrix)[SIZE][SIZE]) {
    srand((unsigned) time(0));
    for_row{
        for_column{
            matrix[row][column] = rand() % 256;
        }
    }
}

bool Tests::matrixCompare(
        const si(&left)[SIZE][SIZE],
        const si(&right)[SIZE][SIZE]
        ) {
    
    bool result = true;
    for_row {
        for_column {
            if (left[row][column] != right[row][column]) {
                result = false;
            }
        }
    }

    return result;
}

void Tests::matrixSubtraction(
        const si(&left)[SIZE][SIZE],
        const si(&right)[SIZE][SIZE],
        si(&result)[SIZE][SIZE]
        ) {
    for_row{
        for_column{
            result[row][column] = left[row][column] - right[row][column];
        }
    }
}

void Tests::matrixAddition(
        const si(&left)[SIZE][SIZE],
        const si(&right)[SIZE][SIZE],
        si(&result)[SIZE][SIZE]
        ) {
    for_row{
        for_column{
            result[row][column] = left[row][column] + right[row][column];
        }
    }
}

void Tests::matrixMultiply(
        const si(&left)[SIZE][SIZE],
        const si(&right)[SIZE][SIZE],
        si(&result)[SIZE][SIZE]
        ) {
    for_row {
        for_column {
            result[row][column] = 0;
            for (int i = 0; i < SIZE; ++i) {
                result[row][column] += left[row][i] * right[i][column];
            }
        }
    }
}

void Tests::matrixDivByConst(
        const si(&matrix)[SIZE][SIZE],
        const int divider,
        si(&result)[SIZE][SIZE]) {
    for_row{
        for_column{
            result[row][column] = 
                    (int) std::round(matrix[row][column] * 1.0 / divider);
        }
    }
}

std::string Tests::matrixPrint(const si(&matrix)[SIZE][SIZE]) {
    std::stringstream temp_stream;
    for_row{
        for_column{
            temp_stream << std::setfill(' ') << std::setw(4)
            << matrix[row][column] << " ";
        }
        temp_stream << "\n";
    }
    temp_stream << "\n";
    return temp_stream.str();
}

std::string Tests::debug_details(
    const si(&r)[SIZE][SIZE],
    const si(&p)[SIZE][SIZE],
    const si(&c)[SIZE][SIZE],
    const si(&c_rightAnswer)[SIZE][SIZE]
    ) {
    std::string debug_details;
    debug_details.reserve(500);
    debug_details.append("\nmatrix R:\n");
    debug_details.append(matrixPrint(r));
    debug_details.append("matrix P:\n");
    debug_details.append(matrixPrint(p));
    debug_details.append("matrix C:\n");
    debug_details.append(matrixPrint(c));
    debug_details.append("matrix C (right answer):\n");
    debug_details.append(matrixPrint(c_rightAnswer));
    return debug_details;
}