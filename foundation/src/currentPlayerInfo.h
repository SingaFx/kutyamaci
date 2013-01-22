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
	int id;
	int poz; //-3..2
	int line; //last move 0, 1, 2 (c, r, x)

	double VPIP; //set in botlogic
	double PFR;
	double AF;
	int handnr;

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

	void setVPIP(double);
	double getVPIP();

	void setPFR(double);
	double getPFR();

	void setAF(double);
	double getAF();

	void setHandnr(int);
	int getHandnr();

	void setId(int);
	int getId();
};