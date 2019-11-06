#include "stdafx.h"
#include <map>
#include <vector>
#include "calculate.h"
#include "parts_needed.h"
#include "week_tracker.h"

// TODO
#include <iostream>
#include <string>


//TODO: Just because week1floor1 = hands and week1floor1 = legs can reach the same week4 state, that does not imply that selecting week1floor1 is
// irrelevant to tomes. This should be map<partsneeeded, map<int, bool> > tracking ONLY if a path is feasible. Tomes should be determined ONLY by
// week1 selection
std::map<int, std::map<PartsNeeded_t, CALCRVAL_t> > g_calcCache;

//int getWeekBitmask(const WeekTracker_t & week)
//{
//	int rval = 0;
//	for (int floor = 1; floor <= 4; ++floor) {
//		if (week.isFloorAvail(floor)) {
//			rval += (1 << floor);
//		}
//	}
//
//	return rval;
//}

void writeToCache(const PartsNeeded_t& pn, const WeekTracker_t & week, CALCRVAL_t rval)
{
//	static int count = 0;
//	int weekBitmask = week.weeknum();
//	std::map<PartsNeeded_t, CALCRVAL_t>::iterator it = g_calcCache[week.weeknum()].find(pn);
//	if ((it == g_calcCache[week.weeknum()].end()) || (it->second < tomes)) {
//		g_calcCache[week.weeknum()][pn] = tomes;
//		++count;
//		return true;
//	}
//	return false;
	g_calcCache[week.weeknum()].insert(std::make_pair(pn, rval));
}

CALCRVAL_t readFromCache(const PartsNeeded_t& pn, const WeekTracker_t & week)
{
	std::map<PartsNeeded_t, CALCRVAL_t>::iterator it = g_calcCache[week.weeknum()].find(pn);

	if (it != g_calcCache[week.weeknum()].end()) {
		return it->second;
	}
	else {
		return CUNKNOWN;
	}
	/*int weekBitmask = week.weeknum();
	std::map<int, int>::iterator it = g_calcCache[pn].find(weekBitmask);

	if (it != g_calcCache[pn].end()) {
		return it->second;
	}
	else {
		return g_calcCacheMiss;
	}*/
	/*std::map<PartsNeeded_t, int>::iterator it = g_calcCache.find(pn);

	if (it != g_calcCache.end()) {
		return it->second;
	}
	else {
		return g_calcCacheMiss;
	}*/
}

std::vector<Token_t> getIterableTokenList()
{
	static std::vector<Token_t> rval;
	if (rval.empty()) {
		rval.push_back(HEADTOKEN);
		rval.push_back(BODYTOKEN);
		rval.push_back(HANDSTOKEN);
		rval.push_back(WAISTTOKEN);
		rval.push_back(LEGSTOKEN);
		rval.push_back(FEETTOKEN);
		rval.push_back(ACCTOKEN);
		rval.push_back(USEDTOKEN);
	}
	return rval;
}

bool tokenIsNeeded(const PartsNeeded_t & pn, Token_t t)
{
	switch (t) {
	case HEADTOKEN:
		return (pn.head() > 0);
	case BODYTOKEN:
		return (pn.body() > 0);
	case HANDSTOKEN:
		return (pn.hands() > 0);
	case WAISTTOKEN:
		return (pn.waist() > 0);
	case LEGSTOKEN:
		return (pn.legs() > 0);
	case FEETTOKEN:
		return (pn.feet() > 0);
	case ACCTOKEN:
		return (pn.acc() > 0);
	}

	return true;
}

PartsNeeded_t evalPNHelper(const PartsNeeded_t & pn, Token_t t)
{
	switch (t) {
	case HEADTOKEN:
		return pn.minusHead();
	case BODYTOKEN:
		return pn.minusBody();
	case HANDSTOKEN:
		return pn.minusHands();
	case WAISTTOKEN:
		return pn.minusWaist();
	case LEGSTOKEN:
		return pn.minusLegs();
	case FEETTOKEN:
		return pn.minusFeet();
	case ACCTOKEN:
		return pn.minusAcc();
	}

	return pn;
}

/*PartsNeeded_t evalPNHelper(const PartsNeeded_t & pn, Token_t floor1, Token_t floor2, Token_t floor3, Token_t floor4)
{
	return evalPNHelper(evalPNHelper(evalPNHelper(evalPNHelper(pn, floor1), floor2), floor3), floor4);
}*/

int calculate(const PartsNeeded_t & pn, const WeekTracker_t & week, Token_t lastToken, const int maxweeks, int valueToCache)
{
	// TODO: check if pn.token is 0... this means that we just set it to zero, which is a good thing
	// This will be part of BEST rval, where we track down how many items we can complete in week 1

	//CALCRVAL_t cacheRval = readFromCache(pn, week);
	//if (cacheRval != -1) {
		// cache hit!
	//	return cacheRval;
	//}
	// cache miss, must calculate from here...

	if (pn.count() == 0) {
		// We found a path to a complete set!
		writeToCache(pn, week, CGOOD);
		return valueToCache;
	}
	
	if (!week.isAnyFloorAvail()) {
		// "token" was the final drop of this week

		//if (week.weeknum() == maxweeks) {
		if (pn.count() > (maxweeks - week.weeknum()) * 4) {
			// we couldn't get every drop in the minimum number of weeks
			writeToCache(pn, week, CPOOR);
			return CPOOR;
		}
		else {
			if (week.weeknum() == 1) {
				// first week, how did we fare?
				valueToCache = pn.tomes();
			}

			const int cachedVal = readFromCache(pn, week);
			if (cachedVal == CUNKNOWN) {
				// start a new week, increment the weeknum counter
				WeekTracker_t newWeek(week.weeknum() + 1);
				int rval = calculate(pn, newWeek, USEDTOKEN, maxweeks, valueToCache);

				if (rval == CPOOR) {
					writeToCache(pn, week, CPOOR);
					return CPOOR;
				}
				else {
					writeToCache(pn, week, CGOOD);
					return rval;
				}
			}
			else if (cachedVal == CPOOR) {
				return CPOOR;
			}
			else {
				return valueToCache;
			}
		}
	}

	// TODO: Left off here, this fundamentally doesn't work. Above I'm only really checking/writing the cache. I'm probably not planning
	// to write to the cache for every partial week
	// Is that my answer? Do I need to instead generate a vector of possible weeks, keeping any locked floor, and permuting
	// every free floor as head/body/hands/waist/legs/feet/acc/used? (Including Used as an option helps for a final week 
	// where I need literally nothing from floor 2, a bye floor). That way, I'm only ever reading/writing caches of complete
	// weeks. 

	// put head/body/hands/waist/legs/feet/acc/used in a token array
	// floor 1 foreach token in array
	//   if floor1 already is this token, or if floor1 is avail and can drop this token
	//		floor 2 foreach token in array
	//	      if floor2 already is this token, or if floor1 is avail and can drop this token etc
	//          etc...
	//            read cache
	//            if the cache doesn't say this is a deadend, and the value we would write is > cacheval
	//              calculate this permuted week going into next week
	//              write to cache
	// return the max val from this crazy nested loop
	std::vector<Token_t> tokenList = getIterableTokenList();
	int maxval = CUNKNOWN;
	for (std::vector<Token_t>::iterator floor1 = tokenList.begin(); floor1 != tokenList.end(); ++floor1) {
		if ((*floor1 == week.getFloor(1)) || (week.isFloorAvail(1) && tokenDropsOnFloor(*floor1, 1) && tokenIsNeeded(pn, *floor1))) {
			PartsNeeded_t evalPN1 = (*floor1 == week.getFloor(1)) ? pn : evalPNHelper(pn, *floor1);
			for (std::vector<Token_t>::iterator floor2 = tokenList.begin(); floor2 != tokenList.end(); ++floor2) {
				if ((*floor2 == week.getFloor(2)) || (week.isFloorAvail(2) && tokenDropsOnFloor(*floor2, 2) && tokenIsNeeded(evalPN1, *floor2))) {
					PartsNeeded_t evalPN12 = (*floor2 == week.getFloor(2)) ? evalPN1 : evalPNHelper(evalPN1, *floor2);
					for (std::vector<Token_t>::iterator floor3 = tokenList.begin(); floor3 != tokenList.end(); ++floor3) {
						if ((*floor3 == week.getFloor(3)) || (week.isFloorAvail(3) && tokenDropsOnFloor(*floor3, 3) && tokenIsNeeded(evalPN12, *floor3))) {
							PartsNeeded_t evalPN123 = (*floor3 == week.getFloor(3)) ? evalPN12 : evalPNHelper(evalPN12, *floor3);
							for (std::vector<Token_t>::iterator floor4 = tokenList.begin(); floor4 != tokenList.end(); ++floor4) {
								if ((*floor4 == week.getFloor(4)) || (week.isFloorAvail(4) && tokenDropsOnFloor(*floor4, 4) && tokenIsNeeded(evalPN123, *floor4))) {
									WeekTracker_t evalWeek(week);
									evalWeek.setFloor(1, *floor1);
									evalWeek.setFloor(2, *floor2);
									evalWeek.setFloor(3, *floor3);
									evalWeek.setFloor(4, *floor4);

									PartsNeeded_t evalPN1234 = (*floor4 == week.getFloor(4)) ? evalPN123 : evalPNHelper(evalPN123, *floor4);

									int rval = calculate(evalPN1234, evalWeek, USEDTOKEN, maxweeks, valueToCache);
									if (rval > maxval)
										maxval = rval;
								}
							}
						}
					}
				}
			}
		}
	}
	return maxval;




	/*for (int floor = 1; floor <= 4; ++floor) {
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
	return CPOOR;*/
}
