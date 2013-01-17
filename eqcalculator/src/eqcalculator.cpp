#include "eqcalculator.h"
#include <time.h>

Hand EqCalculator::sampleRange(PlayerRange &range)
{
	double x = (double)rand()/(double)RAND_MAX;

	double sum = 0;
	set<pair<Hand, double>>::iterator it = range.range.begin();
	while (sum + it->second < x)
	{
		sum += it->second;
		it++;
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

double EqCalculator::evaluate(vector<Hand> &sample, vector<Card> &board)
{
	string hands = sampleToString(sample);
	string sboard = boardToString(board);

	double results[10];
	HoldemCalculator calc;

	calc.CalculateMC(hands.c_str(), sboard.c_str(), "", 1, results);

	return results[0];
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