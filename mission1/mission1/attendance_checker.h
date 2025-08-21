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

map<string, int> id1;
int id_cnt = 0;

//dat[사용자ID][요일]
int dat[100][100];
int points[100];
int grade[100];
string names[100];

int wed[100];
int weeken[100];

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

const string INPUT_DATA = "attendance_weekday_500.txt";
const int THRESHOLD_FOR_GOLD_LEVEL = 50;
const int THRESHOLD_FOR_SILVER_LEVEL = 30;

enum class Level {
	NORMAL = 0,
	GOLD = 1,
	SILVER = 2,
};

void GetInputAndParse() {
	ifstream fin{ INPUT_DATA }; //500개 데이터 입력
	for (int i = 0; i < 500; i++) {
		string name, day;
		fin >> name >> day;
		ParseLine({name, day});
	}
}

bool NeedToRemove(int i);

void Run() {
	GetInputAndParse();

	for (int i = 1; i <= id_cnt; i++) {
		if (dat[i][2] > 9) {
			points[i] += 10;
		}

		if (dat[i][5] + dat[i][6] > 9) {
			points[i] += 10;
		}

		if (points[i] >= THRESHOLD_FOR_GOLD_LEVEL) {
			// gold
			grade[i] = static_cast<int>(Level::GOLD);
		}
		else if (points[i] >= THRESHOLD_FOR_SILVER_LEVEL) {
			// silver
			grade[i] = static_cast<int>(Level::SILVER);
		}
		else {
			// normal
			grade[i] = static_cast<int>(Level::NORMAL);
		}

		cout << "NAME : " << names[i] << ", ";
		cout << "POINT : " << points[i] << ", ";
		cout << "GRADE : ";

		if (grade[i] == static_cast<int>(Level::GOLD)) {
			cout << "GOLD" << "\n";
		}
		else if (grade[i] == static_cast<int>(Level::SILVER)) {
			cout << "SILVER" << "\n";
		}
		else {
			cout << "NORMAL" << "\n";
		}
	}

	std::cout << "\n";
	std::cout << "Removed player\n";
	std::cout << "==============\n";
	for (int i = 1; i <= id_cnt; i++) {
		if (NeedToRemove(i)) {
			std::cout << names[i] << "\n";
		}
	}
}

bool NeedToRemove(int i)
{
	return grade[i] != static_cast<int>(Level::GOLD) && grade[i] != static_cast<int>(Level::SILVER) && wed[i] == 0 && weeken[i] == 0;
}
