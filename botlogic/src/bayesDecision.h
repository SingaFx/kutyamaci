#pragma once
#include "Action.h"
#include "rangeFunctions.h"
#include "currentGameInfo.h"
#include "bayesUserFunctions.h"

class BayesDecision
{
private:
	//static double calculateEV(
	static double calculateEQ(PlayerRange&, vector<Card>&, Hand&);

	static char calculateDecision(CurrentGameInfo&, vector<PlayerRange>&, BayesUserPreflop&, BayesUserFlop&, BayesUserTurn&, BayesUserRiver&);
	static double calculateBetSize(CurrentGameInfo&, vector<PlayerRange>&, BayesUserPreflop&, BayesUserFlop&, BayesUserTurn&, BayesUserRiver&);

public:
	//range-t kap
	static Action makeDecision(CurrentGameInfo&, vector<PlayerRange>&, BayesUserPreflop&, BayesUserFlop&, BayesUserTurn&, BayesUserRiver&);
};