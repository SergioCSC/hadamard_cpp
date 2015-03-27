#include <iostream>
#include <ctime>

#include "Tests.h"

void perform_tests(std::ostream & ostream, const int ITERATION_COUNT) {
    Tests::usualTest(ostream);
    Tests::compareToMatrixMultiply(ostream);
    Tests::thereAndBackAgain(ostream);
    Tests::speedTest(ostream, ITERATION_COUNT);

    //for (int i = 0; i < ITERATION_COUNT; ++i) {
    //    Tests::compareToMatrixMultiply(ostream);
    //    Tests::thereAndBackAgain(ostream);
    //}
}

int main() {
    static const int ITERATION_COUNT = 1000000;
    TestTemplate::reset_counters();
    clock_t begin = clock();
    perform_tests(std::cout, ITERATION_COUNT);
    clock_t end = clock();
    TestTemplate::print_result(
            std::cout, 
            ((double)(end - begin)) / CLOCKS_PER_SEC
            );
	return 0;
}