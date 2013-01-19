#include "bayesDecision.h"
#include "eqcalculator.h"

double BayesDecision::calculateEQ(PlayerRange& range, vector<Card>& board, Hand& hand)
{
	EqCalculator calc;

	vector<PlayerRange> ranges;
	ranges.push_back(range);
	PlayerRange myRange;
	myRange.range.insert(make_pair(hand, 1));
	ranges.push_back(myRange);

	return calc.calculate(ranges, board, 25000);
}

char BayesDecision::calculateDecision(CurrentGameInfo& game, vector<PlayerRange>& ranges, BayesUserPreflop& preflop, BayesUserFlop& flop, BayesUserTurn& turn, BayesUserRiver& river)
{
	char res = 'n';

	vector<double> FE, EQ;
	vector<double> EV;

	//EV = FE * totalpot + (1 - FE) * (EQ * (totalpot + 2 * betsize) - (1 - EQ) * (betsize))
	//--> EQ vs calling range? EQ vs calling + raiserange
	//--> FE: novelni, ha pozicioban van...
	//--> minEQ ahhoz h beteljen
	//??--> RR = random raise

	if (game.getStreet() == 0)
	{
		for (int i = 0; i < ranges.size(); ++i)
		{
			double akt = calculateEQ(ranges[i], game.getBoard(), game.getHand());
		}
	}
	
	if (game.getStreet() > 0)
	{
	}

	return res;
}

double BayesDecision::calculateBetSize(CurrentGameInfo& game, vector<PlayerRange>& ranges, BayesUserPreflop& preflop, BayesUserFlop& flop, BayesUserTurn& turn, BayesUserRiver& river)
{
	double betsize = 0;

	// preflop
	if (game.getStreet() == 0)
	{
		
	}

	return betsize*game.getBblind();
}

Action BayesDecision::makeDecision(CurrentGameInfo& game, vector<PlayerRange>& ranges, BayesUserPreflop& preflop, BayesUserFlop& flop, BayesUserTurn& turn, BayesUserRiver& river)
{
	Action res;

	char decision = calculateDecision(game, ranges, preflop, flop, turn, river);


	return res;
};