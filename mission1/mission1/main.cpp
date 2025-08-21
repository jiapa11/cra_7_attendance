#include <iostream>
#include "gmock/gmock.h"
#include "attendance_checker.h"

int main() {
	::testing::InitGoogleTest();
	return RUN_ALL_TESTS();

	AttendanceChecker checker;
	try {
		checker.Run();
	}
	catch (const std::exception& e) {
		std::cout << e.what() << std::endl;
		return -1;
	}

	return 0;	
}