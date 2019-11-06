#pragma once

enum Token_t {
	HEADTOKEN,
	BODYTOKEN,
	HANDSTOKEN,
	WAISTTOKEN,
	LEGSTOKEN,
	FEETTOKEN,
	ACCTOKEN,

	FREETOKEN, // can roll anything
	USEDTOKEN  // it's already been used for the week, I just forget what
};

bool tokenDropsOnFloor(Token_t token, int floor);

class WeekTracker_t {
public:
	WeekTracker_t(int weeknum = 1);

	Token_t getFloor(int floor) const;
	bool isFloorAvail(int floor) const;
	bool isAnyFloorAvail() const;
	void setFloor(int floor, Token_t token);

	int weeknum() const;

private:
	Token_t m_floor1, m_floor2, m_floor3, m_floor4;
	int m_weeknum;
};