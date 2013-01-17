#pragma once
#include <string>

using namespace std;

class CurrentPlayerInfo
{
private:
	double stacksize; //starting stacksize
	double actualStacksize;
	double betsize; //currentbet(amennyi elotte van most)
	string name;
	int poz; //-3..2 -> 0..5
	int line; //last move 0, 1, 2
};