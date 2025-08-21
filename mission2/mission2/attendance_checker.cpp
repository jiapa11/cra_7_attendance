#include "attendance_checker.h"

void AttendanceChecker::Run() {
	Init();
	Print();
}

void AttendanceChecker::Init() {
	GetInputFromFile();
	UpdatePoints();
	AssignGradeToPlayers();
	UpdateRemovedPlayersList();
}

void AttendanceChecker::Print() {
	PrintAllPlayers();
	PrintRemovedPlayers();
}

void AttendanceChecker::GetInputFromFile() {
	ifstream fin{ INPUT_DATA };
	for (int i = 0; i < NUM_OF_LINES_FOR_RAW_DATA; i++) {
		string name, day;
		fin >> name >> day;
		raw_data.push_back({ name, day });
	}
}

void AttendanceChecker::UpdateBasicPoints() {
	for (const Attendance& attendance : raw_data) {
		UpdateAttendance(attendance);
	}
}

void AttendanceChecker::UpdateBonusPoints() {
	for (PlayerInfo& player : players) {
		int wednesday_attendance = player.attendance_per_day[WEDNESDAY];
		if (wednesday_attendance >= THRESHOLD_FOR_WEDNESDAY_ATTENDANCE_BONUS_POINT) {
			player.point += WEDNESDAY_ATTENDANCE_BONUS_POINT;
		}

		int weekend_attendance = player.attendance_per_day[SATURDAY] + player.attendance_per_day[SUNDAY];
		if (weekend_attendance >= THRESHOLD_FOR_WEEKEND_ATTENDANCE_BONUS_POINT) {
			player.point += WEEKEND_ATTENDANCE_BONUS_POINT;
		}
	}
}

void AttendanceChecker::UpdatePoints() {
	UpdateBasicPoints();
	UpdateBonusPoints();
}

bool AttendanceChecker::IsNewPlayer(string name) {
	if (name_to_id_map.count(name) == 0) return true;
	return false;
}

PlayerInfo* AttendanceChecker::RegisterAndGetNewPlayer(string name) {
	PlayerInfo new_player{};
	name_to_id_map.insert({ name, ++id_cnt });
	new_player.name = name;
	players.push_back(new_player);
	return &(players[id_cnt - 1]);
}

PlayerInfo* AttendanceChecker::GetExistingPlayer(string name) {
	int id = name_to_id_map[name];
	return &(players[id - 1]);
}

void AttendanceChecker::UpdateAttendance(Attendance record) {
	string name = record.name;
	string day = record.day;

	PlayerInfo* player = nullptr;
	if (IsNewPlayer(name)) {
		player = RegisterAndGetNewPlayer(name);
	}
	else player = GetExistingPlayer(name);

	player->attendance_per_day[GetDayIndex(day)] += 1;
	player->point += GetAddPoint(day);
}

void AttendanceChecker::AssignGradeToPlayers() {
	for (PlayerInfo& player : players) {
		if (player.point >= THRESHOLD_FOR_GOLD_LEVEL) player.grade = GOLD;
		else if (player.point >= THRESHOLD_FOR_SILVER_LEVEL) player.grade = SILVER;
		else player.grade = NORMAL;

		player.grade_string = ToString(player.grade);
	}
}

bool AttendanceChecker::NeedToRemove(PlayerInfo player) {
	if (player.grade == GOLD) return false;
	if (player.grade == SILVER) return false;
	if (player.attendance_per_day[WEDNESDAY] != 0) return false;
	if (player.attendance_per_day[SATURDAY] != 0 || player.attendance_per_day[SUNDAY] != 0) return false;

	return true;
}

void AttendanceChecker::UpdateRemovedPlayersList() {
	for (PlayerInfo& player : players) {
		if (NeedToRemove(player)) removed_players.push_back(player);
	}
}

void AttendanceChecker::PrintAllPlayers() {
	for (auto player : players) {
		cout << "NAME : " << player.name << ", ";
		cout << "POINT : " << player.point << ", ";
		cout << "GRADE : " << ToString(player.grade) << "\n";
	}
}

void AttendanceChecker::PrintRemovedPlayers() {
	std::cout << "\n";
	std::cout << "Removed player\n";
	std::cout << "==============\n";

	for (auto removed_player : removed_players) {
		std::cout << removed_player.name << "\n";
	}
}