#include "currentPlayerInfo.h"

CurrentPlayerInfo::CurrentPlayerInfo()
{
}

CurrentPlayerInfo::CurrentPlayerInfo(double stacksize, double actualStacksize, double betsize, string& name, int poz, int line) 
	: stacksize(stacksize)
	, actualStacksize(actualStacksize)
	, betsize(betsize)
	, name(name)
	, poz(poz)
	, line(line)
	, VPIP(0)
{
}

void CurrentPlayerInfo::setStacksize(double stacksize)
{
	this->stacksize = stacksize;
}
double CurrentPlayerInfo::getStacksize()
{
	return stacksize;
}

void CurrentPlayerInfo::setActualStacksize(double actualStacksize)
{
	this->actualStacksize = actualStacksize;
}
double CurrentPlayerInfo::getActualStacksize()
{
	return actualStacksize;
}

void CurrentPlayerInfo::setBetsize(double betsize)
{
	this->betsize = betsize;
}
double CurrentPlayerInfo::getBetsize()
{
	return betsize;
}

void CurrentPlayerInfo::setName(string& name)
{
	this->name = name;
}
string& CurrentPlayerInfo::getName()
{
	return name;
}

void CurrentPlayerInfo::setPoz(int poz)
{
	this->poz = poz;
}
int CurrentPlayerInfo::getPoz()
{
	return poz;
}

void CurrentPlayerInfo::setLine(int line)
{
	this->line = line;
}
int CurrentPlayerInfo::getLine()
{
	return line;
}

void CurrentPlayerInfo::setVPIP(double VPIP)
{
	this->VPIP = VPIP;
}
double CurrentPlayerInfo::getVPIP()
{
	return VPIP;
}

void CurrentPlayerInfo::setPFR(double PFR)
{
	this->PFR = PFR;
}
double CurrentPlayerInfo::getPFR()
{
	return PFR;
}

void CurrentPlayerInfo::setAF(double AF)
{
	this->AF = AF;
}
double CurrentPlayerInfo::getAF()
{
	return AF;
}

void CurrentPlayerInfo::setHandnr(int handnr)
{
	this->handnr = handnr;
}
int CurrentPlayerInfo::getHandnr()
{
	return handnr;
}

void CurrentPlayerInfo::setId(int id)
{
	this->id = id;
}
int CurrentPlayerInfo::getId()
{
	return id;
}