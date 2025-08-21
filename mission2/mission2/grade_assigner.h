#pragma once

namespace GradeString{
	const std::string NORMAL = "NORMAL";
	const std::string GOLD = "GOLD";
	const std::string SILVER = "SILVER";
}

class Grade {
public:
	Grade() = default;
	virtual std::string GetGradeString() = 0;
};

class NormalGrade : public Grade {
	std::string GetGradeString() override {
		return GradeString::NORMAL;
	}
};

class GoldGrade : public Grade {
	std::string GetGradeString() override {
		return GradeString::GOLD;
	}
};

class SilverGrade : public Grade {
	std::string GetGradeString() override {
		return GradeString::SILVER;
	}
};

class GradeFactory {
public:
	static std::unique_ptr<Grade> GetGrade(int point) {
		if (point >= THRESHOLD_FOR_GOLD) return std::make_unique<GoldGrade>();
		else if (point >= THRESHOLD_FOR_SILVER) return std::make_unique<SilverGrade>();
		return std::make_unique<NormalGrade>();
	}

	static const int THRESHOLD_FOR_GOLD = 50;
	static const int THRESHOLD_FOR_SILVER = 30;
};

