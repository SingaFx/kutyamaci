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