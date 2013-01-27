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
	static double calculateEQ(PlayerRange&, vector<Card>&, Hand&);
	static PlayerRange getCallRaiseRange(double, PlayerRange&, CurrentGameInfo&, BayesUserPreflop&, BayesUserFlop&, BayesUserTurn&, BayesUserRiver&);

	static char calculateDecision(CurrentGameInfo&, vector<PlayerRange>&, BayesUserPreflop&, BayesUserFlop&, BayesUserTurn&, BayesUserRiver&);
	static double calculateBetSize(CurrentGameInfo&, vector<PlayerRange>&, BayesUserPreflop&, BayesUserFlop&, BayesUserTurn&, BayesUserRiver&);

	static double modifyValue(double, double);

	static double modifyFEbyBetSize(int, CurrentPlayerInfo&, double, double, double, double);
	static double modifyEQbyBetSize(CurrentGameInfo&, double, double, double, double);
	static double modifyEQbyRelativePosition(CurrentGameInfo&, vector<CurrentPlayerInfo>&, double EQ);
	static double modifyFEbyPlayersInPlay(int, double);
	static double modifyFEbyRelativePosition(CurrentGameInfo&, double);
	static double modifyFEbyBoardType(CurrentGameInfo&, vector<Card>& board, double);
	

	static bool committed(CurrentGameInfo&);
	static double nrOfPlayersWithMaxBetSize(CurrentGameInfo&);
	static vector<double> getRaiseSizes(CurrentGameInfo&);

	static vector<double> getFoldEquities(double, CurrentGameInfo&, vector<PlayerRange>&, BayesUserPreflop&, BayesUserFlop&, BayesUserTurn&, BayesUserRiver&);
	static double calculateEVRaise(CurrentGameInfo&, vector<PlayerRange>&, vector<double>& , double, bool);
	static bool canCallAfterRaise(CurrentGameInfo&, PlayerRange&, double, BayesUserPreflop&, BayesUserFlop&, BayesUserTurn&, BayesUserRiver&);
public:
	//range-t kap
	static Action makeDecision(CurrentGameInfo&, vector<PlayerRange>&, BayesUserPreflop&, BayesUserFlop&, BayesUserTurn&, BayesUserRiver&);
};