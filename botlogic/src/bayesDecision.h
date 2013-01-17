#pragma once
#include "Action.h"
#include "rangeFunctions.h"
#include "currentGameInfo.h"

class BayesDecision
{
private:
	static int calculateDecision();
	static double calculateBetSize();

public:
	//range-t kap
	static Action MakeDecision(CurrentGameInfo&, vector<PlayerRange>&);
};