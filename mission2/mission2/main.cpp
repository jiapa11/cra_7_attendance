#include <iostream>
#include "gmock/gmock.h"
#include "attendance_checker.h"

#define UNIT_TEST 1

int main() {
#if UNIT_TEST
	::testing::InitGoogleTest();
	return RUN_ALL_TESTS();
#else
	AttendanceChecker checker;
	try {
		checker.Run();
	}
	catch (const std::exception& e) {
		std::cout << e.what() << std::endl;
		return -1;
	}

	return 0;
#endif 	
}