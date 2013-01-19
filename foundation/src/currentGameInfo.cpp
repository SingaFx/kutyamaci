#include "CurrentGameInfo.h"

CurrentGameInfo::CurrentGameInfo()
{
}

CurrentGameInfo::CurrentGameInfo(double	potcommon, double bblind, double totalPot, vector<CurrentPlayerInfo>& opponentsInfo, CurrentPlayerInfo& hero, double amountToCall, Hand& hand, vector<Card>& board)
	: potcommon(potcommon)
	, bblind(bblind)
	, totalPot(totalPot)
	, opponentsInfo(opponentsInfo)
	, hero(hero)
	, amountToCall(amountToCall)
	, hand(hand)
	, board(board)
{
}

void CurrentGameInfo::setStreet(int street)
{
	this->street = street;
}
int CurrentGameInfo::getStreet()
{
	return street;
}

void CurrentGameInfo::setPotcommon(double potcommon)
{
	this->potcommon = potcommon;
}
double CurrentGameInfo::getPotcommon()
{
	return potcommon;
}

void CurrentGameInfo::setBblind(double bblind)
{
	this->bblind = bblind;
}
double CurrentGameInfo::getBblind()
{
	return bblind;
}

void CurrentGameInfo::setTotalPot(double totalPot)
{
	this->totalPot = totalPot;
}
double CurrentGameInfo::getTotalPot()
{
	return totalPot;
}

void CurrentGameInfo::setOpponentsInfo(vector<CurrentPlayerInfo>& opponentsInfo)
{
	this->opponentsInfo = opponentsInfo;
}
vector<CurrentPlayerInfo>& CurrentGameInfo::getOpponentsInfo()
{
	return opponentsInfo;
}

void CurrentGameInfo::setHero(CurrentPlayerInfo& hero)
{
	this->hero = hero;
}
CurrentPlayerInfo& CurrentGameInfo::getHero()
{
	return hero;
}

void CurrentGameInfo::setAmountToCall(double amountToCall)
{
	this->amountToCall = amountToCall;
}
double CurrentGameInfo::getAmountToCall()
{
	return amountToCall;
}

void CurrentGameInfo::setHand(Hand& hand)
{
	this->hand = hand;
}
Hand& CurrentGameInfo::getHand()
{
	return hand;
}

void CurrentGameInfo::setBoard(vector<Card>& board)
{
	this->board = board;
}
vector<Card>& CurrentGameInfo::getBoard()
{
	return board;
}

CurrentPlayerInfo& CurrentGameInfo::getPlayerByName(string name)
{
	for (int i = 0; i < opponentsInfo.size(); ++i)
	{
		if (opponentsInfo[i].getName() == name) return opponentsInfo[i];
	}
}

double CurrentGameInfo::getBiggestBet()
{
	double max = 0;
	for (int i = 0; i < opponentsInfo.size(); ++i)
	{
		if (opponentsInfo[i].getBetsize() > max) max = opponentsInfo[i].getBetsize();
	}

	return max;
}