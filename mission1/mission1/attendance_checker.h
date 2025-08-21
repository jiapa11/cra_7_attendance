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

class AttendanceChecker {
public:
	const int NUM_OF_LINES_FOR_RAW_DATA = 500;
	vector<Attendance> raw_data;

	void GetInput() {
		ifstream fin{ INPUT_DATA }; //500개 데이터 입력
		for (int i = 0; i < NUM_OF_LINES_FOR_RAW_DATA; i++) {
			string name, day;
			fin >> name >> day;
			raw_data.push_back({ name, day });
		}
	}

	int GetNumLinesOfRawData() {
		return raw_data.size();
	}

	void Parse() {
		for (const Attendance &attendance : raw_data) {
			ParseLine(attendance);
		}
	}

	string GetLevelString(int level) {
		if (level == 1) return "GOLD";
		if (level == 2) return "SILVER";

		return "NORMAL";
	}

	void AssignLevel() {
		for (int i = 1; i <= id_cnt; i++) {
			if (dat[i][2] > 9) {
				points[i] += 10;
			}

			if (dat[i][5] + dat[i][6] > 9) {
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

	vector<PlayerInfo> GetAllPlayersInfo() {
		vector<PlayerInfo> v;
		for (int i = 1; i <= id_cnt; i++) {
			v.push_back({names[i], points[i], GetLevelString(grade[i])});
		}
		return v;
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

	void Run() {
		GetInput();
		Parse();
		AssignLevel();
		PrintAllPlayers();
		PrintRemovedPlayers();
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
		return grade[i] != GOLD && grade[i] != SILVER && wed[i] == 0 && weeken[i] == 0;
	}

	void ParseLine(Attendance record/*string w, string wk*/) {
		string w = record.name;
		string wk = record.day;

		//ID 부여
		if (id1.count(w) == 0) {
			id1.insert({ w, ++id_cnt });

			if (w == "Daisy") {
				int debug = 1;
			}

			names[id_cnt] = w;
		}
		int id2 = id1[w];

		//디버깅용
		if (w == "Daisy") {
			int debug = 1;
		}


		int add_point = 0;
		int index = 0;
		if (wk == "monday") {
			index = 0;
			add_point++;
		}
		if (wk == "tuesday") {
			index = 1;
			add_point++;
		}
		if (wk == "wednesday") {
			index = 2;
			add_point += 3;
			wed[id2] += 1;
		}
		if (wk == "thursday") {
			index = 3;
			add_point++;
		}
		if (wk == "friday") {
			index = 4;
			add_point++;
		}
		if (wk == "saturday") {
			index = 5;
			add_point += 2;
			weeken[id2] += 1;
		}
		if (wk == "sunday") {
			index = 6;
			add_point += 2;
			weeken[id2] += 1;
		}

		//사용자ID별 요일 데이터에 1씩 증가
		dat[id2][index] += 1;
		points[id2] += add_point;
	}

private:
	map<string, int> id1;
	int id_cnt = 0;

	//dat[사용자ID][요일]
	int dat[100][100];
	int points[100];
	int grade[100];
	string names[100];

	int wed[100];
	int weeken[100];

	const string INPUT_DATA = "attendance_weekday_500.txt";
	const int THRESHOLD_FOR_GOLD_LEVEL = 50;
	const int THRESHOLD_FOR_SILVER_LEVEL = 30;

	const int NORMAL = 0;
	const int GOLD = 1;
	const int SILVER = 2;
};
