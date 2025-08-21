#include "gmock/gmock.h"
#include "attendance_checker.h"

class Fixture : public ::testing::Test {
protected:
	AttendanceChecker checker;

	void SetUp() override {
		checker.Init();

	}

	void TearDown() override {

	}
};

TEST_F(Fixture, Get500Lines) {
	EXPECT_EQ(500, checker.GetNumLinesOfRawData());
}

TEST_F(Fixture, AllPlayersPointCorrect) {
	std::vector<PlayerInfoForPrint> expected;

	expected.push_back({ "Umar", 48, "SILVER"});
	expected.push_back({ "Daisy", 45, "SILVER" });
	expected.push_back({ "Alice", 61, "GOLD" });
	expected.push_back({ "Xena", 91, "GOLD" });
	expected.push_back({ "Ian", 23, "NORMAL" });
	expected.push_back({ "Hannah", 127, "GOLD" });
	expected.push_back({ "Ethan", 44, "SILVER" });
	expected.push_back({ "Vera", 22, "NORMAL" });
	expected.push_back({ "Rachel", 54, "GOLD" });
	expected.push_back({ "Charlie", 58, "GOLD" });
	expected.push_back({ "Steve", 38, "SILVER" });
	expected.push_back({ "Nina", 79, "GOLD" });
	expected.push_back({ "Bob", 8, "NORMAL" });
	expected.push_back({ "George", 42, "SILVER" });
	expected.push_back({ "Quinn", 6, "NORMAL" });
	expected.push_back({ "Tina", 24, "NORMAL" });
	expected.push_back({ "Will", 36, "SILVER" });
	expected.push_back({ "Oscar", 13, "NORMAL" });
	expected.push_back({ "Zane", 1, "NORMAL" });

	std::vector<PlayerInfoForPrint> actual = checker.GetAllPlayersInfo();
	ASSERT_EQ(expected.size(), actual.size());

	for (int i = 0; i < expected.size(); i++) {
		ASSERT_EQ(expected[i].name, actual[i].name);
		ASSERT_EQ(expected[i].point, actual[i].point);
		ASSERT_EQ(expected[i].level, actual[i].level);
	}
}

TEST_F(Fixture, IdentifyRemovedPlayers) {
	vector<string> expected = { "Bob", "Zane" };
	vector<string> actual = checker.GetRemovedPlayers();
	ASSERT_EQ(expected.size(), actual.size());

	for (int i = 0; i < expected.size(); i++) {
		ASSERT_EQ(expected[i], actual[i]);
	}
}