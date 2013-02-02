#include "eqcalculator.h"
#include "logger.h"
#include <time.h>

Hand EqCalculator::sampleRange(PlayerRange &range)
{
	double x = (double)rand()/(double)RAND_MAX;

	double sum = 0;
	set<pair<Hand, double>>::iterator it = range.range.begin();
	while (sum + it->second < x && it != range.range.end())
	{
		sum += it->second;
		it++;
	}

	if (it == range.range.end())
	{
		//Logger& logger = Logger::getLogger(BOT_LOGIC);
		//logger.logExp("ERROR END OF SAMPLING", BOT_LOGIC);

		it--;
		return it->first; 
	}

	return it->first;
}

bool EqCalculator::validSample(vector<Hand> &sample, vector<Card> &board)
{
	set<Card> dead;
	for (int i = 0; i < board.size(); ++i)
	{
		dead.insert(board[i]);
	}

	for (int i = 0; i < sample.size(); ++i)
	{
		if (dead.find(sample[i].getCard1()) == dead.end() && dead.find(sample[i].getCard2()) == dead.end())
		{
			dead.insert(sample[i].getCard1());
			dead.insert(sample[i].getCard2());
		}
		else
		{
			return false;
		}
	}

	return true;
}

void EqCalculator::makeSampleValid(vector<Hand> &sample, vector<Card> &board)
{
	set<Card> dead;
	vector<Hand> temp;

	for (int i = 0; i < board.size(); ++i)
	{
		dead.insert(board[i]);
	}

	for (int i = 0; i < sample.size(); ++i)
	{
		if (dead.find(sample[i].getCard1()) == dead.end() && dead.find(sample[i].getCard2()) == dead.end())
		{
			dead.insert(sample[i].getCard1());
			dead.insert(sample[i].getCard2());
			temp.push_back(sample[i]);
		}
	}

	sample = temp;
}

string EqCalculator::boardToString(vector<Card> &board)
{
	string res = "";
	for (int i = 0; i < board.size(); ++i)
	{
		res.push_back(board[i].getRank());
		res.push_back(board[i].getSuit());
	}

	return res;
}

string EqCalculator::sampleToString(vector<Hand> &sample)
{
	string res = "";

	for (int i = 0; i < sample.size(); ++i)
	{
		res.push_back(sample[i].getCard1().getRank());
		res.push_back(sample[i].getCard1().getSuit());
		res.push_back(sample[i].getCard2().getRank());
		res.push_back(sample[i].getCard2().getSuit());

		if (i < sample.size() - 1)
			res.push_back('|');
	}

	return res;
}

StdDeck_CardMask EqCalculator::cardToMask(Card card)
{
	char str[2];
	str[0] = card.getRank();
	str[1] = card.getSuit();

	int cardindex = -1;
	StdDeck_stringToCard(str, &cardindex);

	return StdDeck_MASK(cardindex);
}

double EqCalculator::evaluate(vector<Hand> &sample, vector<Card> &board)
{
	StdDeck_CardMask boardmask;
	StdDeck_CardMask_RESET(boardmask);
	for (int i = 0; i < board.size(); ++i)
		StdDeck_CardMask_OR(boardmask, boardmask, cardToMask(board[i]));

	HandVal best = 0;
	int nrofbests = 0;
	int nrofwinners = 0;

	for (int i = 0; i < sample.size(); ++i)
	{
		StdDeck_CardMask temp;
		StdDeck_CardMask_RESET(temp);

		StdDeck_CardMask_OR(temp, temp, boardmask);
		StdDeck_CardMask_OR(temp, temp, cardToMask(sample[i].getCard1()));
		StdDeck_CardMask_OR(temp, temp, cardToMask(sample[i].getCard2()));

		HandVal actual = StdDeck_StdRules_EVAL_N(temp, 7);

		if (actual > best)
		{
			if (best > 0)
			{
				return 0;
			}
			else
			{
				best = actual;
				nrofwinners = 1;
			}
		}
		else if (actual == best)
		{
			nrofwinners++;
		}
	}

	return 1.0/(double)nrofwinners;
}

Card EqCalculator::numberToCard(int x)
{
	// 0-12 13-25 26-38 39-51
	char suitmap[4] = {'h','d','s','c'};
	char rankmap[13] = {'2','3','4','5','6','7','8','9','T','J','Q','K','A'};
	Card res;

	res.setSuit(suitmap[x/13]);
	res.setRank(rankmap[x%13]);

	return res;
}

Card EqCalculator::getRandomCard(set<Card> &deadCards)
{
	Card c = numberToCard(rand() % 52);
	while (deadCards.find(c) != deadCards.end())
		c = numberToCard(rand() % 52);

	return c;
}

double EqCalculator::calculate(vector<PlayerRange> &range, vector<Card> &board, int nrtrials)
{
	double res = 0;
	vector<Hand> playerHands;
	vector<Card> sampleBoard;
	set<Card> deadCards;
	int nr;

	for (int i = 0; i < nrtrials; ++i)
	{
		nr = 0;
		playerHands.clear();
		deadCards.clear();

		while ((playerHands.empty() || !validSample(playerHands, board)) && nr < 100)
		{
			playerHands.clear();
			for (int j = 0; j < range.size(); ++j)
				playerHands.push_back(sampleRange(range[j]));
			nr++;
		}
		
		makeSampleValid(playerHands, board);

		for (int j = 0; j < board.size(); ++j)
			deadCards.insert(board[j]);

		for (int j = 0; j < playerHands.size(); ++j)
		{
			deadCards.insert(playerHands[j].getCard1());
			deadCards.insert(playerHands[j].getCard2());
		}

		sampleBoard = board;

		while (sampleBoard.size() < 5)
		{
			Card c = getRandomCard(deadCards);
			deadCards.insert(c);
			sampleBoard.push_back(c);
		}

		res += evaluate(playerHands, sampleBoard);
	}

	return res/(double)nrtrials;
}

EqCalculator::EqCalculator()
{
	srand(time(NULL));
}