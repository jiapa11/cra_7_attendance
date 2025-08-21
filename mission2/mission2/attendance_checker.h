#pragma once
#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <map>
#include <algorithm>
#include "grade_assigner.h"

using namespace std;

struct PlayerInfo {
	string name;
	int point;
	string grade;
	int attendance_per_day[7];	
};

class AttendanceChecker {
public:
	struct Attendance {
		string name, day;
	};

	AttendanceChecker() = default;

	~AttendanceChecker() = default;

	void Run();

	void Init();

	bool IsCleared();

	void Clear();

	void Print();

	size_t GetNumLinesOfRawData() { 
		return raw_data.size(); 
	}

	vector<PlayerInfo> GetAllPlayersInfo() { return players; }

	vector<PlayerInfo> GetRemovedPlayers() { return removed_players; }
	
	int GetBonusPoints(PlayerInfo& player);

private:
	const int NUM_OF_LINES_FOR_RAW_DATA = 500;
	vector<Attendance> raw_data;

	map<string, int> name_to_id_map;
	int id_cnt = 0;

	vector<PlayerInfo> players;
	vector<PlayerInfo> removed_players;

	const string INPUT_DATA = "attendance_weekday_500.txt";

	const static int MONDAY = 0;
	const static int TUESDAY = 1;
	const static int WEDNESDAY = 2;
	const static int THURSDAY = 3;
	const static int FRIDAY = 4;
	const static int SATURDAY = 5;
	const static int SUNDAY = 6;
	const static int INVALID_INPUT = -1;

	const static int THRESHOLD_FOR_WEDNESDAY_ATTENDANCE_BONUS_POINT = 10;
	const static int WEDNESDAY_ATTENDANCE_BONUS_POINT = 10;
	const static int THRESHOLD_FOR_WEEKEND_ATTENDANCE_BONUS_POINT = 10;
	const static int WEEKEND_ATTENDANCE_BONUS_POINT = 10;

	void GetInputFromFile();

	void UpdateBasicPoints();

	void UpdateBonusPoints();

	void UpdatePoints();

	bool IsNewPlayer(string name);

	PlayerInfo* RegisterAndGetNewPlayer(string name);

	PlayerInfo* GetExistingPlayer(string name);

	void UpdateAttendance(Attendance record);

	void AssignGradeToPlayers();

	bool NeedToRemove(PlayerInfo player);

	void UpdateRemovedPlayersList();

	string ToString(int level) {
		if (level == 1) return "GOLD";
		if (level == 2) return "SILVER";

		return "NORMAL";
	}

	void PrintAllPlayers();

	void PrintRemovedPlayers();

	int GetDayIndex(string day) {
		if (day == "monday") return MONDAY;
		if (day == "tuesday") return TUESDAY;
		if (day == "wednesday") return WEDNESDAY;
		if (day == "thursday") return THURSDAY;
		if (day == "friday") return FRIDAY;
		if (day == "saturday") return SATURDAY;
		if (day == "sunday") return SUNDAY;

		return 0;
	}

	int GetAddPoint(string day) {
		if (day == "monday") return 1;
		if (day == "tuesday") return 1;
		if (day == "wednesday") return 3;
		if (day == "thursday") return 1;
		if (day == "friday") return 1;
		if (day == "saturday") return 2;
		if (day == "sunday") return 2;

		return 0;
	}
};
