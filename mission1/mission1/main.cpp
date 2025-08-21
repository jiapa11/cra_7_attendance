#include <iostream>
#include "gmock/gmock.h"
#include "attendance_checker.h"

int main() {
	//::testing::InitGoogleTest();
	//return RUN_ALL_TESTS();

	AttendanceChecker checker;
	checker.Run();
	//Run();
}