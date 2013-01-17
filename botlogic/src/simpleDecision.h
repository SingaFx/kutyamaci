#pragma once
#include "Action.h"
#include "rangeFunctions.h"
#include "currentGameInfo.h"

class SimpleDecision
{
private:
	static int calculateDecision();
	static double calculateBetSize();

public:
	//sajat handje erdekli csak
	static Action makeDecision(CurrentGameInfo&);
};