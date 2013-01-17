#pragma once
#include "Hand.h"
#include "currentPlayerInfo.h"
#include <vector>

using namespace std;

class CurrentGameInfo
{
private:
	double potcommon; //until this street (preflop - 0, flop - totalPreflop, ...)
	double bblind; //nagyvak merete -> 0.04
	double totalPot; //total potsize

	vector<CurrentPlayerInfo> opponentsInfo;
	CurrentPlayerInfo hero;

	double amountToCall;
	Hand hand; //own hand
};