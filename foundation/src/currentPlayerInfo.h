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
	int line; //last move 0, 1, 2 (c, r, x)

public:

	CurrentPlayerInfo();
	CurrentPlayerInfo(double, double, double, string&, int, int);

	void setStacksize(double);
	double getStacksize();

	void setActualStacksize(double);
	double getActualStacksize();

	void setBetsize(double);
	double getBetsize();

	void setName(string&);
	string& getName();

	void setPoz(int);
	int getPoz();

	void setLine(int);
	int getLine();
};