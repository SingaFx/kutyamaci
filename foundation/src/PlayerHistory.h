#ifndef PLAYERHISTORY_H
#define PLAYERHISTORY_H

#include "Action.h"
#include "Hand.h"
#include <string>
#include <vector>
using namespace std;

class PlayerHistory
{
private:

    string playerName;
	int seat;
	int position;
	double balance;
	bool handKnown;
	Hand hand;
	vector<Action> preflopAction;
	vector<Action> flopAction;
	vector<Action> turnAction;
	vector<Action> riverAction;

public:

   PlayerHistory();
   PlayerHistory(string, int , int, double, bool, Hand, vector<Action>, vector<Action>, vector<Action>, vector<Action>);

   void setPlayerName(string);
   const string&  getPlayerName() const;

   void setSeat(int);
   int getSeat() const;

   void setPosition(int);
   int  getPosition() const;

   void setBalance(double);
   double  getBalance() const;

   void setHandKnown(bool);
   bool isHandKnown() const;

   void setHand(Hand);
   const Hand&  getHand() const;

   void setPreflopAction(vector<Action>);
   const vector<Action>& getPreflopAction() const;
   vector<Action>& getPreflopAction();

   void setFlopAction(vector<Action>);
   const vector<Action>& getFlopAction() const;
   vector<Action>& getFlopAction();

   void setTurnAction(vector<Action>);
   const vector<Action>& getTurnAction() const;
   vector<Action>& getTurnAction();

   void setRiverAction(vector<Action>);
   const vector<Action>& getRiverAction() const;
   vector<Action>& getRiverAction();
};

#endif