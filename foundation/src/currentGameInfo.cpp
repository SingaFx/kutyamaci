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
