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
	static double calculateEQ(vector<PlayerRange>&, vector<Card>&, Hand&, CurrentGameInfo&);
	static PlayerRange getCallRaiseRange(double, PlayerRange&, CurrentGameInfo&, BayesUserPreflop&, BayesUserFlop&, BayesUserTurn&, BayesUserRiver&);

	static bool committed(CurrentGameInfo&);
	static double nrOfPlayersWithMaxBetSize(CurrentGameInfo&);
	static vector<double> getRaiseSizes(CurrentGameInfo&);

	//IMPLEMENT IMPLIED ODDS

	static vector<double> getFoldEquities(double, CurrentGameInfo&, vector<PlayerRange>&, BayesUserPreflop&, BayesUserFlop&, BayesUserTurn&, BayesUserRiver&);
	static double calculateEVRaise(CurrentGameInfo&, vector<PlayerRange>&, vector<double>& , double, bool&, bool);
	static double calculateEVCall(CurrentGameInfo&, vector<PlayerRange>&);
	static bool canCallAfterRaise(CurrentGameInfo&, PlayerRange&, double, BayesUserPreflop&, BayesUserFlop&, BayesUserTurn&, BayesUserRiver&);
public:
	//range-t kap
	static Action makeDecision(CurrentGameInfo&, vector<PlayerRange>&, BayesUserPreflop&, BayesUserFlop&, BayesUserTurn&, BayesUserRiver&);
};