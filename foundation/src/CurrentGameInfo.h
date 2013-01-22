#pragma once
#include "Hand.h"
#include "currentPlayerInfo.h"
#include <vector>

using namespace std;

class CurrentGameInfo
{
private:
	int street; //0 preflop, 1 flop...
	double potcommon; //until this street (preflop - 0, flop - totalPreflop, ...)
	double bblind; //nagyvak merete -> 0.04
	double totalPot; //total potsize

	vector<CurrentPlayerInfo> opponentsInfo;
	CurrentPlayerInfo hero;

	double amountToCall;
	Hand hand; //own hand
	vector<Card> board;

	double handNumber;

public:

	CurrentGameInfo();
	CurrentGameInfo(double, double, double, vector<CurrentPlayerInfo>&, CurrentPlayerInfo&, double, Hand&, vector<Card>&);
	//~CurrentGameInfo();

	void setStreet(int);
	int getStreet(); 

	void setPotcommon(double);
	double getPotcommon();

	void setBblind(double);
	double getBblind();

	void setTotalPot(double);
	double getTotalPot();

	void setOpponentsInfo(vector<CurrentPlayerInfo>&);
	vector<CurrentPlayerInfo>& getOpponentsInfo();
	void addCurrentPlayerInfo(CurrentPlayerInfo&);
	

	void setHero(CurrentPlayerInfo&);
	CurrentPlayerInfo& getHero();

	void setAmountToCall(double);
	double getAmountToCall();

	void setHand(Hand&);
	Hand& getHand();

	void setBoard(vector<Card>&);
	vector<Card>& getBoard();

	CurrentPlayerInfo& getPlayerByName(string name);
	double getBiggestBet();
	CurrentPlayerInfo& getPlayerbyPosition(int position);
	CurrentPlayerInfo& getPlayerbyId(int id);

	void setHandNumber(double);
	double getHandNumber();
};