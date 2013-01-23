#pragma once
#include "Action.h"
#include "rangeFunctions.h"
#include "currentGameInfo.h"
#include "bayesUserFunctions.h"
#include "globalConst.h"

class BayesDecision
{
private:
public:
	//static double calculateEV(
	static double calculateEQ(vector<PlayerRange>&, vector<Card>&, Hand&);
	static PlayerRange getCallRaiseRange(double, PlayerRange&, CurrentGameInfo&, BayesUserPreflop&, BayesUserFlop&, BayesUserTurn&, BayesUserRiver&);

	static char calculateDecision(CurrentGameInfo&, vector<PlayerRange>&, BayesUserPreflop&, BayesUserFlop&, BayesUserTurn&, BayesUserRiver&);
	static double calculateBetSize(CurrentGameInfo&, vector<PlayerRange>&, BayesUserPreflop&, BayesUserFlop&, BayesUserTurn&, BayesUserRiver&);

	static double modifyValue(double, double);

	static double modifyFEbyBetSize(int, CurrentPlayerInfo&, double, double, double, double);
	static double modifyEQbyRelativePosition(CurrentGameInfo& gameInfo, double EQ);

public:
	//range-t kap
	static Action makeDecision(CurrentGameInfo&, vector<PlayerRange>&, BayesUserPreflop&, BayesUserFlop&, BayesUserTurn&, BayesUserRiver&);
};