#pragma once
#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <map>
#include <algorithm>

using namespace std;

struct Attendance {
	string name, day;
};

struct PlayerInfo {
	string name;
	int point;
	string level;
};

struct Player {
	int day[7];
	int point;
	int grade;
	string name;
};

class AttendanceChecker {
public:
	void Run() {
		Init();
		Print();
	}

	void Init() {
		GetInput();
		Parse();
		AssignLevel();
	}

	void Print() {
		PrintAllPlayers();
		PrintRemovedPlayers();
	}

	int GetNumLinesOfRawData() {
		return raw_data.size();
	}

	vector<PlayerInfo> GetAllPlayersInfo() {
		vector<PlayerInfo> v;
		for (int i = 1; i <= id_cnt; i++) {
			v.push_back({ names[i], points[i], GetLevelString(grade[i]) });
		}
		return v;
	}

	vector<string> GetRemovedPlayers() {
		vector<string> v;
		for (int i = 1; i <= id_cnt; i++) {
			if (NeedToRemove(i)) {
				v.push_back(names[i]);
			}
		}
		return v;
	}



	int GetDayIndex(string day) {
		if (day == "monday") return MONDAY;
		if (day == "tuesday") return TUESDAY;
		if (day == "wednesday") return WEDNESDAY;
		if (day == "thursday") return THURSDAY;
		if (day == "friday") return FRIDAY;
		if (day == "saturday") return SATURDAY;
		if (day == "sunday") return SUNDAY;

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

	int AssignAndGetID(string name) {
		if (name_to_id_map.count(name) == 0) {
			name_to_id_map.insert({ name, ++id_cnt });
			names[id_cnt] = name;
		}
		return name_to_id_map[name];
	}

	bool IsNewPlayer(string name) {
		if (name_to_id_map.count(name) == 0) return true;
		return false;
	}

	void ParseLine(Attendance record) {
		string name = record.name;
		string day = record.day;

		Player player{};

		//if (IsNewPlayer(name)) {
		//	Player new_player{};
		//	new_player.name = name;
		//	players.push_back(new_player);
		//	player = players[id_cnt];
		//}
		//else {
		//	name_to_id_map[name];
		//}

		int id = AssignAndGetID(name);
		int day_index = GetDayIndex(day);
		if (day_index == -1) {
			cout << "ERROR" << " invalid day" << endl;
			while (1);
		}

		dat[id][day_index] += 1; //사용자ID별 요일 데이터에 1씩 증가
		points[id] += GetAddPoint(day);

		Player new_player{};
		new_player.name = name;
		new_player.day[day_index] += 1;
		new_player.point = GetAddPoint(day);
		players.push_back(new_player);
	}

private:
	const int NUM_OF_LINES_FOR_RAW_DATA = 500;
	vector<Attendance> raw_data;

	map<string, int> name_to_id_map;
	int id_cnt = 0;

	vector<Player> players;
	vector<Player> removed_players;


	int dat[100][100]; 	//dat[사용자ID][요일]
	int points[100];
	int grade[100];
	string names[100];

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

	void GetInput() {
		ifstream fin{ INPUT_DATA }; //500개 데이터 입력
		for (int i = 0; i < NUM_OF_LINES_FOR_RAW_DATA; i++) {
			string name, day;
			fin >> name >> day;
			raw_data.push_back({ name, day });
		}
	}

	void Parse() {
		for (const Attendance& attendance : raw_data) {
			ParseLine(attendance);
		}
	}

	void AssignLevel() {
		for (int i = 1; i <= id_cnt; i++) {
			if (dat[i][WEDNESDAY] >= 10) {
				points[i] += 10;
			}

			if (dat[i][SATURDAY] + dat[i][SUNDAY] >= 10) {
				points[i] += 10;
			}

			if (points[i] >= THRESHOLD_FOR_GOLD_LEVEL) {
				grade[i] = GOLD;
			}
			else if (points[i] >= THRESHOLD_FOR_SILVER_LEVEL) {
				grade[i] = SILVER;
			}
			else {
				grade[i] = NORMAL;
			}
		}
	}

	string GetLevelString(int level) {
		if (level == 1) return "GOLD";
		if (level == 2) return "SILVER";

		return "NORMAL";
	}

	void PrintAllPlayers() {
		for (int i = 1; i <= id_cnt; i++) {
			cout << "NAME : " << names[i] << ", ";
			cout << "POINT : " << points[i] << ", ";
			cout << "GRADE : ";

			if (grade[i] == GOLD) {
				cout << "GOLD" << "\n";
			}
			else if (grade[i] == SILVER) {
				cout << "SILVER" << "\n";
			}
			else {
				cout << "NORMAL" << "\n";
			}
		}
	}

	void PrintRemovedPlayers() {
		std::cout << "\n";
		std::cout << "Removed player\n";
		std::cout << "==============\n";
		for (int i = 1; i <= id_cnt; i++) {
			if (NeedToRemove(i)) {
				std::cout << names[i] << "\n";
			}
		}
	}

	bool NeedToRemove(int i) {
		if (grade[i] == GOLD) return false;
		if (grade[i] == SILVER) return false;
		if (dat[i][WEDNESDAY] != 0) return false;
		if (dat[i][SATURDAY] != 0 || dat[i][SUNDAY] != 0) return false;

		return true;		
	}
};
