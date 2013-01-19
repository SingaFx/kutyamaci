#pragma once

#include "AbstractBotLogic.h"
#include "bayesUserFunctions.H"
#include "database.h"
#include "bayesDecision.h"
#include "simpleDecision.h"
#include "globalConst.h"

class PlusEVBotLogic : AbstractBotLogic //implements interface
{
private:
	BayesUserPreflop preflop;
	BayesUserFlop flop;
	BayesUserTurn turn;
	BayesUserRiver river;
	Database* database;

	CurrentPlayerInfo& setStandardPlayerType(CurrentPlayerInfo&, double);
	void extendGameInfo(CurrentGameInfo&);

public:
	PlusEVBotLogic();
	~PlusEVBotLogic();

    PlayerRange& calculateRange(string name, CurrentGameInfo&, PlayerRange&);
    Action makeDecision(CurrentGameInfo&, vector<PlayerRange>&);
};