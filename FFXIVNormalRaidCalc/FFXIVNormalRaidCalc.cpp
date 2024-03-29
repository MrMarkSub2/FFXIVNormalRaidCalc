#include "stdafx.h"
#include "calculate.h"
#include "parts_needed.h"
#include "week_tracker.h"
#include <iostream>
#include <string>
#include <Windows.h>

std::string tokenToStr(Token_t t) {
	switch (t) {
	case HEADTOKEN:
		return "Head";
	case BODYTOKEN:
		return "Body";
	case HANDSTOKEN:
		return "Hands";
	case WAISTTOKEN:
		return "Waist";
	case LEGSTOKEN:
		return "Legs";
	case FEETTOKEN:
		return "Feet";
	case ACCTOKEN:
		return "Acc";
	case FREETOKEN:
		return "FREE";
	case USEDTOKEN:
		return "USED";
	default:
		throw;
	}
}

Token_t strToToken(const std::string& str)
{
	if (str == "head")
		return HEADTOKEN;
	if (str == "body")
		return BODYTOKEN;
	if (str == "hands")
		return HANDSTOKEN;
	if (str == "waist")
		return WAISTTOKEN;
	if (str == "legs")
		return LEGSTOKEN;
	if (str == "feet")
		return FEETTOKEN;
	if (str == "acc")
		return ACCTOKEN;
	if (str == "free")
		return FREETOKEN;
	if (str == "used")
		return USEDTOKEN;

	throw;
}

PartsNeeded_t inputPartsNeeded()
{
	int head, body, hands, waist, legs, feet, acc;
	bool valid;
	do {
		valid = true;
		std::cout << std::endl << "Input # of parts needed, space separated (Head Body Hands Waist Legs Feet Acc): ";
		std::cin >> head >> body >> hands >> waist >> legs >> feet >> acc;

		if (head < 0 || head > 2) {
			std::cout << "ERROR: Needed head tokens must be between 0 and 2" << std::endl;
			valid = false;
		}
		if (body < 0 || body > 4) {
			std::cout << "ERROR: Needed body tokens must be between 0 and 4" << std::endl;
			valid = false;
		}
		if (hands < 0 || hands > 2) {
			std::cout << "ERROR: Needed hands tokens must be between 0 and 2" << std::endl;
			valid = false;
		}
		if (waist < 0 || waist > 1) {
			std::cout << "ERROR: Needed waist tokens must be between 0 and 1" << std::endl;
			valid = false;
		}
		if (legs < 0 || legs > 4) {
			std::cout << "ERROR: Needed legs tokens must be between 0 and 4" << std::endl;
			valid = false;
		}
		if (feet < 0 || feet > 2) {
			std::cout << "ERROR: Needed feet tokens must be between 0 and 2" << std::endl;
			valid = false;
		}
		if (acc < 0 || acc > 4) {
			std::cout << "ERROR: Needed acc tokens must be between 0 and 4" << std::endl;
			valid = false;
		}
	} while (!valid);
	return PartsNeeded_t(head, body, hands, waist, legs, feet, acc);
}

void print(const PartsNeeded_t& pn, const WeekTracker_t& week) 
{
	std::cout << std::string(79, '-') << std::endl;

	std::cout << "Head - " << pn.head() << "\tBody - " << pn.body() << "\tHands - " << pn.hands() << "\tWaist - " << pn.waist()
		<< "\tLegs - " << pn.legs() << "\tFeet - " << pn.feet() << "\tAcc - " << pn.acc() << std::endl;

	std::cout << "Floor 1 - " << tokenToStr(week.getFloor(1)) << "\tFloor 2 - " << tokenToStr(week.getFloor(2))
		<< "\tFloor 3 - " << tokenToStr(week.getFloor(3)) << "\tFloor 4 - " << tokenToStr(week.getFloor(4)) << std::endl;

	std::cout << std::string(79, '-') << std::endl << std::endl;
}

void setFloorCommand(PartsNeeded_t& pn, WeekTracker_t& week)
{
	int floor;
	std::cin >> floor;

	std::string tokenStr;
	std::cin >> tokenStr;

	if (floor < 1 || floor > 4) {
		std::cout << "ERROR: Floor must be between 1 and 4" << std::endl;
		return;
	}

	const Token_t oldToken = week.getFloor(floor);
	const Token_t newToken = strToToken(tokenStr);

	if (oldToken == newToken) // no change
		return;

	if (newToken != FREETOKEN && newToken != USEDTOKEN && !tokenDropsOnFloor(newToken, floor))  {
		std::cout << "ERROR: Cannot select " << tokenToStr(newToken) << ", not available on this floor!" << std::endl;
		return;
	}

	PartsNeeded_t new_pn(pn);

	switch (oldToken) {
	case HEADTOKEN:
		new_pn = new_pn.addHead();
		break;
	case BODYTOKEN:
		new_pn = new_pn.addBody();
		break;
	case HANDSTOKEN:
		new_pn = new_pn.addHands();
		break;
	case WAISTTOKEN:
		new_pn = new_pn.addWaist();
		break;
	case LEGSTOKEN:
		new_pn = new_pn.addLegs();
		break;
	case FEETTOKEN:
		new_pn = new_pn.addFeet();
		break;
	case ACCTOKEN:
		new_pn = new_pn.addAcc();
		break;
	}

	switch (newToken) {
	case HEADTOKEN:
		if (new_pn.head() == 0) {
			std::cout << "Cannot select head, already completed!" << std::endl << std::endl;
			return;
		}

		new_pn = new_pn.minusHead();
		break;
	case BODYTOKEN:
		if (new_pn.body() == 0) {
			std::cout << "Cannot select body, already completed!" << std::endl << std::endl;
			return;
		}

		new_pn = new_pn.minusBody();
		break;
	case HANDSTOKEN:
		if (new_pn.hands() == 0) {
			std::cout << "Cannot select hands, already completed!" << std::endl << std::endl;
			return;
		}

		new_pn = new_pn.minusHands();
		break;
	case WAISTTOKEN:
		if (new_pn.waist() == 0) {
			std::cout << "Cannot select waist, already completed!" << std::endl << std::endl;
			return;
		}

		new_pn = new_pn.minusWaist();
		break;
	case LEGSTOKEN:
		if (new_pn.legs() == 0) {
			std::cout << "Cannot select legs, already completed!" << std::endl << std::endl;
			return;
		}

		new_pn = new_pn.minusLegs();
		break;
	case FEETTOKEN:
		if (new_pn.feet() == 0) {
			std::cout << "Cannot select feet, already completed!" << std::endl << std::endl;
			return;
		}

		new_pn = new_pn.minusFeet();
		break;
	case ACCTOKEN:
		if (new_pn.acc() == 0) {
			std::cout << "Cannot select acc, already completed!" << std::endl << std::endl;
			return;
		}

		new_pn = new_pn.minusAcc();
		break;
	}

	week.setFloor(floor, newToken);
	pn = new_pn;
}

void calcCommandHelper(const PartsNeeded_t& pn, const WeekTracker_t& week, Token_t token, int floor, int maxweeks)
{
	std::cout << tokenToStr(token) << " - ";
	if (tokenDropsOnFloor(token, floor)) {
		int rval;
		WeekTracker_t evalWeek(week);
		evalWeek.setFloor(floor, token);

		switch (token) {
		case HEADTOKEN:
			rval = pn.head() > 0 ? calculate(pn.minusHead(), evalWeek, token, maxweeks, CUNKNOWN) : CNOTAPPLICABLE;
			break;
		case BODYTOKEN:
			rval = pn.body() > 0 ? calculate(pn.minusBody(), evalWeek, token, maxweeks, CUNKNOWN) : CNOTAPPLICABLE;
			break;
		case HANDSTOKEN:
			rval = pn.hands() > 0 ? calculate(pn.minusHands(), evalWeek, token, maxweeks, CUNKNOWN) : CNOTAPPLICABLE;
			break;
		case WAISTTOKEN:
			rval = pn.waist() > 0 ? calculate(pn.minusWaist(), evalWeek, token, maxweeks, CUNKNOWN) : CNOTAPPLICABLE;
			break;
		case LEGSTOKEN:
			rval = pn.legs() > 0 ? calculate(pn.minusLegs(), evalWeek, token, maxweeks, CUNKNOWN) : CNOTAPPLICABLE;
			break;
		case FEETTOKEN:
			rval = pn.feet() > 0 ? calculate(pn.minusFeet(), evalWeek, token, maxweeks, CUNKNOWN) : CNOTAPPLICABLE;
			break;
		case ACCTOKEN:
			rval = pn.acc() > 0 ? calculate(pn.minusAcc(), evalWeek, token, maxweeks, CUNKNOWN) : CNOTAPPLICABLE;
			break;
		}

		HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);

		switch (rval) {
		case CPOOR:
			SetConsoleTextAttribute(hConsole, 4);
			std::cout << "Poor" << std::endl;
			break;
		case CUNKNOWN:
			SetConsoleTextAttribute(hConsole, 4);
			std::cout << "MISS?" << std::endl;
			break;
		case CNOTAPPLICABLE:
			SetConsoleTextAttribute(hConsole, 8);
			std::cout << "N/A" << std::endl;
			break;
		default:
			SetConsoleTextAttribute(hConsole, 10);
			// TODO: this ought only show the NET GAIN of rval on the first week, subtract from week 0
			std::cout << "Good\t" << std::string((rval - pn.tomes())/100, '*') << std::endl;
			break;
		}
		SetConsoleTextAttribute(hConsole, 7);
	}
	else {
		HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
		SetConsoleTextAttribute(hConsole, 8);
		std::cout << "N/A" << std::endl;
		SetConsoleTextAttribute(hConsole, 7);
	}
}

void calcCommand(const PartsNeeded_t& pn, const WeekTracker_t& week)
{
	int floor;
	std::cin >> floor;

	if (floor < 1 || floor > 4) {
		std::cout << "ERROR: Floor must be between 1 and 4" << std::endl;
		return;
	}

	if (!week.isFloorAvail(floor)) {
		std::cout << "ERROR: Cannot calc on a floor already claimed" << std::endl;
		return;
	}

	int tokensThisWeek = 0;
	if (!week.isFloorAvail(1)) ++tokensThisWeek;
	if (!week.isFloorAvail(2)) ++tokensThisWeek;
	if (!week.isFloorAvail(3)) ++tokensThisWeek;
	if (!week.isFloorAvail(4)) ++tokensThisWeek;

	int maxweeks = static_cast<int>(ceil((double)(pn.count() + tokensThisWeek) / 4.0));

	std::cout << "Floor " << floor << " Recommendations:" << std::endl;
	calcCommandHelper(pn, week, HEADTOKEN, floor, maxweeks);
	calcCommandHelper(pn, week, BODYTOKEN, floor, maxweeks);
	calcCommandHelper(pn, week, HANDSTOKEN, floor, maxweeks);
	calcCommandHelper(pn, week, WAISTTOKEN, floor, maxweeks);
	calcCommandHelper(pn, week, LEGSTOKEN, floor, maxweeks);
	calcCommandHelper(pn, week, FEETTOKEN, floor, maxweeks);
	calcCommandHelper(pn, week, ACCTOKEN, floor, maxweeks);

	std::cout << std::endl;
}

void queryCommand(PartsNeeded_t& pn, WeekTracker_t& week)
{
	std::string command;

	std::cout << "?) ";
	std::cin >> command;

	// TODO: case-sensitive
	if (command == "set")
		setFloorCommand(pn, week);
	// if (command == "need")
	//   pn = inputPartsNeeded();
	if (command == "calc")
		calcCommand(pn, week);
	if (command == "quit")
		exit(0);
}

int main()
{
	PartsNeeded_t partsNeeded = inputPartsNeeded();
	WeekTracker_t currentWeek;
	while (true) {
		print(partsNeeded, currentWeek);
		queryCommand(partsNeeded, currentWeek);		// show best drops for floor N
	}
}

// recursion loop
	// partsNeeded pn
	// add part as an optimal part to choose if there exists AT LEAST ONE path to get all remaining parts in 
	//    ceil(count/4) weeks, taking into account currently selected as locked choices
	// if we haven't explored down this branch yet, recurse with pn.minusPart
	// save exploration to cache for quicker future lookups