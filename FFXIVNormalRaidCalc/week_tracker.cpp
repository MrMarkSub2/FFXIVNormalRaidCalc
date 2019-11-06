#include "stdafx.h"
#include "week_tracker.h"

#define DROPTABLE 5	// ShB droptable
//#define DROPTABLE 3		// HW and SB droptable

WeekTracker_t::WeekTracker_t(int weeknum)
	: m_floor1(FREETOKEN), m_floor2(FREETOKEN), m_floor3(FREETOKEN), m_floor4(FREETOKEN), m_weeknum(weeknum)
{
}

Token_t WeekTracker_t::getFloor(int floor) const
{
	switch (floor) {
	case 1:
		return m_floor1;
	case 2:
		return m_floor2;
	case 3:
		return m_floor3;
	case 4:
		return m_floor4;
	default:
		throw;
	}
}

bool WeekTracker_t::isFloorAvail(int floor) const
{
	return getFloor(floor) == FREETOKEN;
}

bool WeekTracker_t::isAnyFloorAvail() const
{
	return isFloorAvail(1) || isFloorAvail(2) || isFloorAvail(3) || isFloorAvail(4);
}

void WeekTracker_t::setFloor(int floor, Token_t token)
{
	switch (floor) {
	case 1:
		m_floor1 = token;
		break;
	case 2:
		m_floor2 = token;
		break;
	case 3:
		m_floor3 = token;
		break;
	case 4:
		m_floor4 = token;
		break;
	default:
		throw;
	}
}

int WeekTracker_t::weeknum() const
{
	return m_weeknum;
}

bool tokenDropsOnFloor(Token_t token, int floor)
{
#if DROPTABLE == 5
	switch (token) {
	case HEADTOKEN:
		return (floor == 1 || floor == 2 || floor == 3);
	case BODYTOKEN:
	case LEGSTOKEN:
	case ACCTOKEN:
		return (floor == 1 || floor == 2 || floor == 3 || floor == 4);
	case HANDSTOKEN:
		return (floor == 2 || floor == 3 || floor == 4);
	case WAISTTOKEN:
		return (floor == 1 || floor == 2);
	case FEETTOKEN:
		return (floor == 1 || floor == 2 || floor == 4);
	case USEDTOKEN:
		return true;
	}
#elif DROPTABLE == 3
	switch (token) {
	case HEADTOKEN:
		return (floor == 2 || floor == 3);
	case BODYTOKEN:
		return (floor == 4);
	case HANDSTOKEN:
	case LEGSTOKEN:
		return (floor == 3 || floor == 4);
	case WAISTTOKEN:
		return (floor == 1);
	case FEETTOKEN:
	case ACCTOKEN:
		return (floor == 1 || floor == 2);
	case USEDTOKEN:
		return true;
	}
#endif

	// Either an invalid droptable is defined, or an invalid token was passed here
	throw;
}
