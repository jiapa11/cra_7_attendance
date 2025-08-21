#pragma once
#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <map>
#include <algorithm>

using namespace std;

struct PlayerInfoForPrint {
	string name;
	int point;
	string level;
};

struct PlayerInfo {
	string name;
	int point;
	int grade;
	string level;
	int attendance_per_day[7];	
};

class AttendanceChecker {
public:
	struct Attendance {
		string name, day;
	};


	void Run() {
		Init();
		Print();
	}

	void Init() {
		GetInputFromFile();
		ParseInput();
		AssignLevelToPlayers();
		UpdateRemovedPlayers();
	}

	void Print() {
		PrintAllPlayers();
		PrintRemovedPlayers();
	}

	int GetNumLinesOfRawData() {
		return raw_data.size();
	}

	vector<PlayerInfoForPrint> GetAllPlayersInfo() {
		vector<PlayerInfoForPrint> v;
		for (auto player : players) {
			v.push_back({ player.name, player.point, GetLevelString(player.grade) });
		}
		return v;
	}

	vector<string> GetRemovedPlayers() {
		vector<string> v;
		for (auto removed_player : removed_players) {
			v.push_back(removed_player.name);
		}
		return v;
	}

	void ParseLine(Attendance record) {
		string name = record.name;
		string day = record.day;

		PlayerInfo *player = nullptr;
		if (IsNewPlayer(name)) {
			player = RegisterAndGetNewPlayer(name);
		}
		else {
			player = GetExistingPlayer(name);
		}

		player->attendance_per_day[GetDayIndex(day)] += 1;
		player->point += GetAddPoint(day);
	}

private:
	const int NUM_OF_LINES_FOR_RAW_DATA = 500;
	vector<Attendance> raw_data;

	map<string, int> name_to_id_map;
	int id_cnt = 0;

	vector<PlayerInfo> players;
	vector<PlayerInfo> removed_players;

	const string INPUT_DATA = "attendance_weekday_500.txt";
	const int THRESHOLD_FOR_GOLD_LEVEL = 50;
	const int THRESHOLD_FOR_SILVER_LEVEL = 30;

	const int NORMAL = 0;
	const int GOLD = 1;
	const int SILVER = 2;

	const int MONDAY = 0;
	const int TUESDAY = 1;
	const int WEDNESDAY = 2;
	const int THURSDAY = 3;
	const int FRIDAY = 4;
	const int SATURDAY = 5;
	const int SUNDAY = 6;
	const int INVALID_INPUT = -1;

	void GetInputFromFile() {
		ifstream fin{ INPUT_DATA }; //500개 데이터 입력
		for (int i = 0; i < NUM_OF_LINES_FOR_RAW_DATA; i++) {
			string name, day;
			fin >> name >> day;
			raw_data.push_back({ name, day });
		}
	}

	void ParseInput() {
		for (const Attendance& attendance : raw_data) {
			ParseLine(attendance);
		}
	}

	void AssignLevelToPlayers() {
		for (PlayerInfo& player : players) {
			if (player.attendance_per_day[WEDNESDAY] >= 10) player.point += 10;
			if ((player.attendance_per_day[SATURDAY] + player.attendance_per_day[SUNDAY]) >= 10) player.point += 10;

			if (player.point >= THRESHOLD_FOR_GOLD_LEVEL) player.grade = GOLD;
			else if (player.point >= THRESHOLD_FOR_SILVER_LEVEL) player.grade = SILVER;
			else player.grade = NORMAL;
		}
	}

	void UpdateRemovedPlayers() {
		for (PlayerInfo& player : players) {
			if (NeedToRemove(player)) removed_players.push_back(player);
		}
	}

	string GetLevelString(int level) {
		if (level == 1) return "GOLD";
		if (level == 2) return "SILVER";

		return "NORMAL";
	}

	void PrintAllPlayers() {
		for (auto player : players) {
			cout << "NAME : " << player.name << ", ";
			cout << "POINT : " << player.point << ", ";
			cout << "GRADE : " << GetLevelString(player.grade) << "\n";
		}
	}

	void PrintRemovedPlayers() {
		std::cout << "\n";
		std::cout << "Removed player\n";
		std::cout << "==============\n";

		for (auto removed_player : removed_players) {
			std::cout << removed_player.name << "\n";
		}
	}


	bool NeedToRemove(PlayerInfo player) {
		if (player.grade == GOLD) return false;
		if (player.grade == SILVER) return false;
		if (player.attendance_per_day[WEDNESDAY] != 0) return false;
		if (player.attendance_per_day[SATURDAY] != 0 || player.attendance_per_day[SUNDAY] != 0) return false;

		return true;
	}

	int GetDayIndex(string day) {
		if (day == "monday") return MONDAY;
		if (day == "tuesday") return TUESDAY;
		if (day == "wednesday") return WEDNESDAY;
		if (day == "thursday") return THURSDAY;
		if (day == "friday") return FRIDAY;
		if (day == "saturday") return SATURDAY;
		if (day == "sunday") return SUNDAY;

		throw std::runtime_error("invalid day input: " + day);

		return INVALID_INPUT;
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

	bool IsNewPlayer(string name) {
		if (name_to_id_map.count(name) == 0) return true;
		return false;
	}

	PlayerInfo* RegisterAndGetNewPlayer(string name) {
		PlayerInfo new_player{};
		name_to_id_map.insert({ name, ++id_cnt });
		new_player.name = name;
		players.push_back(new_player);
		return &(players[id_cnt - 1]);
	}

	PlayerInfo* GetExistingPlayer(string name) {
		int id = name_to_id_map[name];
		return &(players[id - 1]);
	}
};
