#pragma once

#include "AbstractBotLogic.h"
#include "bayesUserFunctions.H"
#include "database.h"
#include "bayesDecision.h"
#include "simpleDecision.h"

class PlusEVBotLogic : public AbstractBotLogic //implements interface
{
private:
	static const int patternsNeeded = 10;
	BayesUserPreflop preflop;
	BayesUserFlop flop;
	BayesUserTurn turn;
	BayesUserRiver river;
	Database* database;

	CurrentPlayerInfo& setStandardPlayerType(CurrentPlayerInfo&, double);
	void extendGameInfo(CurrentGameInfo&);

public:
	PlusEVBotLogic();
	virtual ~PlusEVBotLogic();

    virtual PlayerRange& calculateRange(string name, CurrentGameInfo&, PlayerRange&);
    virtual Action makeDecision(CurrentGameInfo&, vector<PlayerRange>&);
};