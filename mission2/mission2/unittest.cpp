#include "gmock/gmock.h"
#include "attendance_checker.h"

class Fixture : public ::testing::Test {
protected:
	AttendanceChecker checker;
	struct PlayerInfoForPrint {
		string name;
		int point;
		string grade;

		bool operator==(const PlayerInfoForPrint& other) const {
			return name == other.name && point == other.point && grade == other.grade;
		}
	};

	void SetUp() override {
		checker.Init();
	}

	void TearDown() override {

	}
};

TEST_F(Fixture, Cleared) {
	checker.Clear();
	EXPECT_EQ(true, checker.IsCleared());
}

TEST_F(Fixture, NoExceptionWhileInit) {
	EXPECT_NO_THROW(checker.Clear());
	EXPECT_NO_THROW(checker.Init());
}

TEST_F(Fixture, Get500Lines) {
	EXPECT_EQ(500, checker.GetNumLinesOfRawData());
}

TEST_F(Fixture, comparator) {
	std::vector<PlayerInfoForPrint> expected;
	expected.push_back({ "Umar", 48, "SILVER" });
	std::vector<PlayerInfoForPrint> actual;
	actual.push_back({ "Daisy", 45, "SILVER" });

	EXPECT_NE(expected, actual);
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

	std::vector<PlayerInfo> v = checker.GetAllPlayersInfo();

	std::vector<PlayerInfoForPrint> actual;
	for (auto e : v) {
		actual.push_back({e.name, e.point, e.grade});
	}

	EXPECT_EQ(expected, actual);
}

TEST_F(Fixture, IdentifyRemovedPlayers) {
	vector<string> expected = { "Bob", "Zane" };
	vector<PlayerInfo> v = checker.GetRemovedPlayers();
	std::vector<string> actual;
	for (auto e : v) {
		actual.push_back({ e.name });
	}

	EXPECT_EQ(expected, actual);
}

TEST_F(Fixture, NoExceptionWhileRun) {
	EXPECT_NO_THROW(checker.Clear());
	EXPECT_NO_THROW(checker.Run());
}

TEST_F(Fixture, NoExceptionWhilePrinting) {
	EXPECT_NO_THROW(checker.Print());
}

TEST_F(Fixture, BonusPoints) {
	PlayerInfo player;
	player.attendance_per_day[2] = 11;

	EXPECT_EQ(10, checker.GetBonusPoints(player));

	player.attendance_per_day[5] = 0;
	player.attendance_per_day[6] = 11;
	EXPECT_EQ(20, checker.GetBonusPoints(player));
}