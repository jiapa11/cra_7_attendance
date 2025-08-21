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

bool AttendanceChecker::IsCleared() {
	if (raw_data.size() != 0) return false;
	if (name_to_id_map.size() != 0) return false;
	if (id_cnt != 0) return false;
	if (players.size() != 0) return false;
	if (removed_players.size() != 0) return false;
	return true;
}

void AttendanceChecker::Clear() {
	raw_data.clear();
	name_to_id_map.clear();
	id_cnt = 0;
	players.clear();
	removed_players.clear();
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

int AttendanceChecker::GetBonusPoints(PlayerInfo &player) {
	int bonus_points = 0;
	
	int wednesday_attendance = player.attendance_per_day[WEDNESDAY];
	if (wednesday_attendance >= THRESHOLD_FOR_WEDNESDAY_ATTENDANCE_BONUS_POINT) {
		bonus_points += WEDNESDAY_ATTENDANCE_BONUS_POINT;
	}

	int weekend_attendance = player.attendance_per_day[SATURDAY] + player.attendance_per_day[SUNDAY];
	if (weekend_attendance >= THRESHOLD_FOR_WEEKEND_ATTENDANCE_BONUS_POINT) {
		bonus_points += WEEKEND_ATTENDANCE_BONUS_POINT;
	}
	
	return bonus_points;
}

void AttendanceChecker::UpdateBonusPoints() {
	for (PlayerInfo& player : players) {
		player.point += GetBonusPoints(player);
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

	if (player) {
		player->attendance_per_day[GetDayIndex(day)] += 1;
		player->point += GetAddPoint(day);
	}
}

void AttendanceChecker::AssignGradeToPlayers() {
	for (PlayerInfo& player : players) {
		Grade* grade = GradeFactory::GetGrade(player.point);
		player.grade = grade->GetGradeString();
		free(grade);
	}
}

bool AttendanceChecker::NeedToRemove(PlayerInfo player) {
	if (player.grade == "GOLD") return false;
	if (player.grade == "SILVER") return false;
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
		cout << "GRADE : " << player.grade << "\n";
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