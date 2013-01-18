#include "bayesDecision.h"

static char calculateDecision(CurrentGameInfo& game, vector<PlayerRange>& ranges, BayesUserPreflop& preflop, BayesUserFlop& flop, BayesUserTurn& turn, BayesUserRiver& river)
{
	char res = ' ';

	double EQ, FE;



	if (game.getStreet() == 0)
	{
	}
	
	if (game.getStreet() == 1)
	{
	}

	if (game.getStreet() == 2)
	{
	}

	if (game.getStreet() == 3)
	{
	}


	return res;
}

Action BayesDecision::makeDecision(CurrentGameInfo& game, vector<PlayerRange>& ranges, BayesUserPreflop& preflop, BayesUserFlop& flop, BayesUserTurn& turn, BayesUserRiver& river)
{
	Action res;

	char decision = calculateDecision(game, ranges, preflop, flop, turn, river);


	return res;
};