#ifndef _HADAMARD_INVERSER_H_
#define _HADAMARD_INVERSER_H_

#define for_row for (int row = 0; row < SIZE; ++row)
#define for_column for (int column = 0; column < SIZE; ++column)

/**
 * This class solves problem of calculation inverse Hadamard transform:
 * (P, R) -> C, assuming C is a 16-bit signed integer, P and R are 
 * 8-bit unsigned integers, and 16-bit representation is enough to 
 * hold any intermediate result.
 * Hadamard transform is (C,P) -> R function: 
 * R = A * C * A + P
 * where where * denotes matrix multiplication, R, C and P are 4x4 
 * matrixes (matrices dimensions are fixed and will never be changed). 
 * Matrix A is a constant, i.e. all its values are constants and 
 * will never be changed.
 * A[4][4] = {
 *		{1, 1, 1, 1},
 *		{1, 1,-1,-1},
 *		{1,-1,-1, 1},
 *		{1,-1, 1,-1}
 *	};
 */



class HadamardInverser {
public:
    static const int SIZE = 4;
    typedef short int si;

    /**
     * perform inverse transform (P, R) -> C
     * by formula C = 1/16 * A * (R - P) * A
     * with Fast Walsh-Hadamard Tranform (Sequency ordered)
     * (or FWHT(w) shortly) optimization.
     * This optimization allows fast multiplication
     * both row to A and A to column.
     * (N * log N = 8 additions and substractions, N is size of matrix,
     * e.g N == 4, against N * (N - 1) = 12 operations in case of 
     * direct multiplication). 
     * Optimization contains in lineMulByHadamard method.
     */

    inline static void transform(
            const si (&r)[SIZE][SIZE],
            const si (&p)[SIZE][SIZE],
            si (&result)[SIZE][SIZE]) {

        // calc R - P matrix
        substitute(r, p, result);


        // calc (R - P) * A
        for_row {
            lineMulByHadamard(
                    result[row][0],
                    result[row][1],
                    result[row][2],
                    result[row][3]);
        }

        // calc A * (R - P) * A
        for_column {
            lineMulByHadamard(
                    result[0][column],
                    result[1][column],
                    result[2][column],
                    result[3][column]);
        }

        // calc 1/16 * A * (R - P) * A
        divMatrixBy16(result);
    }

private:

    /** 
     * perform "row * A" or "A * column" operation, 
     * these actions has the same result
     * (with an accuracy to result transposition):
     * (x0,x1,x2,x3) -> 
     * (x0+x1+x2+x3,x0+x1-x2-x3,x0-x1-x2+x3,x0-x1+x2-x3)
     * this method uses FWHT(w) algorithm, 
     * precalculated for 4*4 matrix size special case.
     */
    inline static void lineMulByHadamard(
            si &x0,
            si &x1,
            si &x2,
            si &x3) {

        si temp[SIZE];

        temp[0] = x0 + x1;
        temp[1] = x2 + x3;
        temp[2] = x0 - x1;
        temp[3] = x2 - x3;

        x0 = temp[0] + temp[1];
        x1 = temp[0] - temp[1];
        x2 = temp[2] - temp[3];
        x3 = temp[2] + temp[3];
    }

    inline static void substitute(
            const si (&left)[SIZE][SIZE],
            const si (&right)[SIZE][SIZE],
            si (&result)[SIZE][SIZE]) {
        for_row {
            for_column {
                result[row][column] = left[row][column] - right[row][column];
            }
        }
    }

    inline static void divMatrixBy16(si (&matrix)[SIZE][SIZE]) {
        for_row {
            for_column {                
                matrix[row][column] = (matrix[row][column] / 16)
                    + (matrix[row][column] % 16) / 8;
                // it is significantly faster than
                // matrix[row][column] = 
                //      ((int)std::round(matrix[row][column] * 1.0 / 16));
                // even slightly faster, but wrong for negative numbers:
                // matrix[row][column] = (matrix[row][column] >> 4)
                //                    + ((matrix[row][column] & 8) >> 3);
            }
        }
    }
};

#endif