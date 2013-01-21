#include "testBotLogic.h"

PlayerRange& TestBotLogic::calculateRange(string name, CurrentGameInfo& game, PlayerRange& player)
{
	return player;
}

Action TestBotLogic::makeDecision(CurrentGameInfo& game, vector<PlayerRange>& ranges)
{
	return Action('n', 0);
}