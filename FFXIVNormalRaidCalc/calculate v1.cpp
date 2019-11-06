#include "stdafx.h"
#include <map>
#include "calculate.h"
#include "parts_needed.h"
#include "week_tracker.h"

// TODO
#include <iostream>
#include <string>

std::map<PartsNeeded_t, std::map<int, CALCRVAL_t> > g_calcCache;

int getWeekBitmask(const WeekTracker_t & week)
{
	int rval = 0;
	for (int floor = 1; floor <= 4; ++floor) {
		if (week.isFloorAvail(floor)) {
			rval += (1 << floor);
		}
	}

	return rval;
}

void writeToCache(const PartsNeeded_t& pn, const WeekTracker_t & week, CALCRVAL_t rval)
{
	int weekBitmask = getWeekBitmask(week);
	g_calcCache[pn].insert(std::make_pair(weekBitmask, rval)).second;
}

CALCRVAL_t readFromCache(const PartsNeeded_t& pn, const WeekTracker_t & week)
{
	int weekBitmask = getWeekBitmask(week);
	std::map<int, CALCRVAL_t>::iterator it = g_calcCache[pn].find(weekBitmask);

	if (it != g_calcCache[pn].end()) {
		return it->second;
	}
	else {
		return CUNKNOWN;
	}
}

CALCRVAL_t calculate(const PartsNeeded_t & pn, const WeekTracker_t & week, Token_t lastToken, const int maxweeks)
{
	// TODO: check if pn.token is 0... this means that we just set it to zero, which is a good thing
	// This will be part of BEST rval, where we track down how many items we can complete in week 1
	// TODO: remember that EVERY incrememt of Accessory counts as a completed part

	CALCRVAL_t cacheRval = readFromCache(pn, week);
	if (cacheRval != CUNKNOWN) {
		// cache hit!
		return cacheRval;
	}
	// cache miss, must calculate from here...

	if (pn.count() == 0) {
		// We found a path to a complete set!
		writeToCache(pn, week, CGOOD);
		return CGOOD;
	}
	
	if (!week.isAnyFloorAvail()) {
		// "token" was the final drop of this week
		// TODO: this should trip some sort of flag to determine how many drops we acquired in the first week

		if (week.weeknum() == maxweeks) {
			// we couldn't get every drop in the minimum number of weeks
			writeToCache(pn, week, CPOOR);
			return CPOOR;
		}
		else {
			// TODO: this is a slightly sloppy way to reset the week
			// start a new week, increment the weeknum counter
			WeekTracker_t newWeek(week.weeknum() + 1);
			CALCRVAL_t rval = calculate(pn, newWeek, USEDTOKEN, maxweeks);
			writeToCache(pn, newWeek, rval);
			return rval;
		}
	}

	for (int floor = 1; floor <= 4; ++floor) {
		if (week.isFloorAvail(floor)) {
			WeekTracker_t evalWeek(week);

			if (tokenDropsOnFloor(HEADTOKEN, floor) && pn.head() > 0) {
				PartsNeeded_t evalPN = pn.minusHead();
				evalWeek.setFloor(floor, HEADTOKEN);

				CALCRVAL_t rval = calculate(evalPN, evalWeek, HEADTOKEN, maxweeks);
				writeToCache(evalPN, evalWeek, rval);
				if (rval == CGOOD)
					return CGOOD;
			}

			if (tokenDropsOnFloor(BODYTOKEN, floor) && pn.body() > 0) {
				PartsNeeded_t evalPN = pn.minusBody();
				evalWeek.setFloor(floor, BODYTOKEN);

				CALCRVAL_t rval = calculate(evalPN, evalWeek, BODYTOKEN, maxweeks);
				writeToCache(evalPN, evalWeek, rval);
				if (rval == CGOOD)
					return CGOOD;
			}

			if (tokenDropsOnFloor(HANDSTOKEN, floor) && pn.hands() > 0) {
				PartsNeeded_t evalPN = pn.minusHands();
				evalWeek.setFloor(floor, HANDSTOKEN);

				CALCRVAL_t rval = calculate(evalPN, evalWeek, HANDSTOKEN, maxweeks);
				writeToCache(evalPN, evalWeek, rval);
				if (rval == CGOOD)
					return CGOOD;
			}

			if (tokenDropsOnFloor(WAISTTOKEN, floor) && pn.waist() > 0) {
				PartsNeeded_t evalPN = pn.minusWaist();
				evalWeek.setFloor(floor, WAISTTOKEN);

				CALCRVAL_t rval = calculate(evalPN, evalWeek, WAISTTOKEN, maxweeks);
				writeToCache(evalPN, evalWeek, rval);
				if (rval == CGOOD)
					return CGOOD;
			}

			if (tokenDropsOnFloor(LEGSTOKEN, floor) && pn.legs() > 0) {
				PartsNeeded_t evalPN = pn.minusLegs();
				evalWeek.setFloor(floor, LEGSTOKEN);

				CALCRVAL_t rval = calculate(evalPN, evalWeek, LEGSTOKEN, maxweeks);
				writeToCache(evalPN, evalWeek, rval);
				if (rval == CGOOD)
					return CGOOD;
			}

			if (tokenDropsOnFloor(FEETTOKEN, floor) && pn.feet() > 0) {
				PartsNeeded_t evalPN = pn.minusFeet();
				evalWeek.setFloor(floor, FEETTOKEN);

				CALCRVAL_t rval = calculate(evalPN, evalWeek, FEETTOKEN, maxweeks);
				writeToCache(evalPN, evalWeek, rval);
				if (rval == CGOOD)
					return CGOOD;
			}

			if (tokenDropsOnFloor(ACCTOKEN, floor) && pn.acc() > 0) {
				PartsNeeded_t evalPN = pn.minusAcc();
				evalWeek.setFloor(floor, ACCTOKEN);

				CALCRVAL_t rval = calculate(evalPN, evalWeek, ACCTOKEN, maxweeks);
				writeToCache(evalPN, evalWeek, rval);
				if (rval == CGOOD)
					return CGOOD;
			}
		}
	}

	// from the current state, we have a deadend where the entire subtree can't find a success state in time. So, this is the doomed timeline
	writeToCache(pn, week, CPOOR);
	return CPOOR;
}
