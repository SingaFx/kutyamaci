#include "simpleDecision.h"

set<Hand> SimpleDecision::getRange(string& s)
{
	set<Hand> result;
	Hand h;
	char suitmap[4] = {'s','d','h','c'};
	h.getCard1().setRank(' ');
	h.getCard2().setRank(' ');

	for (int i = 0; i < s.size(); ++i)
	{
		if (s[i] == ',')
		{
			if (s[i-1] == 's')
			{
				for (int j = 0; j < 4; ++j)
				{
					h.getCard1().setSuit(suitmap[j]);
					h.getCard2().setSuit(suitmap[j]);
					result.insert(h);

					h.setHand(h.getCard2(), h.getCard1());
					result.insert(h);
				}
			}
			else if (h.getCard1().getRank() == h.getCard2().getRank())
			{
				for (int j = 0; j < 4; ++j)
				{
					for (int k = 0; k < 4; ++k)
					{
						if (j != k)
						{
							h.getCard1().setSuit(suitmap[j]);
							h.getCard2().setSuit(suitmap[k]);
							result.insert(h);
						}
					}
				}
			}
			else
			{
				for (int j = 0; j < 4; ++j)
				{
					for (int k = 0; k < 4; ++k)
					{
						if (j != k)
						{
							h.getCard1().setSuit(suitmap[j]);
							h.getCard2().setSuit(suitmap[k]);
							result.insert(h);

							h.setHand(h.getCard2(), h.getCard1());
							result.insert(h);
						}
					}
				}
			}
		}
		else
		{
			if (h.getCard1().getRank() == ' ')
			{
				h.getCard1().setRank(s[i]);
			}
			else if (h.getCard2().getRank() == ' ')
			{
				h.getCard2().setRank(s[i]);
			}
		}
	}

	return result;
}

char SimpleDecision::calculateDecision(CurrentGameInfo& game)
{
	if (game.getStreet() == 0)
	{
		if (game.getAmountToCall() == game.getBblind())
		{
			// openraise
			if (game.getTotalPot() == 1.5*game.getBblind())
			{
				
			}
			// 1 or 2 limpers
			else if (game.getTotalPot() <= 3.5*game.getBblind())
			{

			}
			// 2+ limpers
			else
			{

			}
		}
	}

	return ' ';
}

double SimpleDecision::calculateBetSize(CurrentGameInfo& game)
{
	return 0;
}

Action SimpleDecision::makeDecision(CurrentGameInfo& game)
{
	Action res;

	char type = calculateDecision(game);
	res.setAction(type, type == 'r' ? calculateBetSize(game)*game.getBblind() : 0);

	return res;
}