#if defined ( WIN32 )
#define __func__ __FUNCTION__
#endif

#ifndef _TESTTEMPLATE_H_
#define _TESTTEMPLATE_H_

#include <sstream>

class TestTemplate {
protected:
	static int testsNum;
    static int testsFailed;

#define check(out,expr,debug_details) \
		    testsNum++; \
		    if (!expr) { \
			    error(out, #expr, debug_details, \
                        __func__, __FILE__, __LINE__); \
            } else out << "Test ok:\t" << #expr << std::endl;

    static void error(
            std::ostream & out, 
            const char * expr, 
            std::string & debug_details, 
            const char * func, 
            const char * filename, 
            const int line);
public:
	static void print_result(std::ostream & out, double time_in_sec);
	static void reset_counters();
};

#endif