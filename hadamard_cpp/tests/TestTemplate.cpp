#include "TestTemplate.h"

int TestTemplate::testsFailed = 0;
int TestTemplate::testsNum = 0;

void TestTemplate::error(
        std::ostream & out,
        const char * expr, 
        std::string & debug_details,
        const char * func, 
        const char * filename, 
        const int line) {
	testsFailed++;
    out << "Failed test: " << expr
            << "\tfunction: " << func
            << "\tfile: " << filename
            << "\tline: " << line
            << std::endl
            << debug_details
            << std::endl;
}

void TestTemplate::print_result(std::ostream & out, double time_in_sec) {
    out << std::endl
            << "Total tests:\t" << testsNum << std::endl
            << "Failed tests:\t" << testsFailed << std::endl
            << "All tests time in seconds: " << time_in_sec << std::endl;
}

void TestTemplate::reset_counters() {
	testsNum = 0;
	testsFailed = 0;
}