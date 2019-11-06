#pragma once

#include "week_tracker.h"

enum CALCRVAL_t {
	CGOOD,
	CPOOR = -1,
	CUNKNOWN = -2,
	CNOTAPPLICABLE = -3
};

// rval used when there's no path to completion in the minimum number of weeks
// can't use 0 since 0 is a legit value to cache
//const int g_calcNoPath = -1;
// rval used when we don't know if there's a path or not
//const int g_calcCacheMiss = -2;
// rval when not applicable
//const int g_calcNA = -3;

class PartsNeeded_t;

int calculate(const PartsNeeded_t& pn, const WeekTracker_t& week, Token_t lastToken, const int maxweeks, int valueToCache);