#pragma once

#include "rangeFunctions.h"
#include "Hand.h"
#include "deck.h"
#include "poker_defs.h"
#include "rules_std.h"
#include "eval.h"

class EqCalculator
{
	Hand sampleRange(PlayerRange &range);
	bool validSample(vector<Hand> &sample, vector<Card> &board);
	void makeSampleValid(vector<Hand> &sample, vector<Card> &board);
	double evaluate(vector<Hand> &sample, vector<Card> &board);
	Card getRandomCard(set<Card> &deadCards);
	Card numberToCard(int x);
	string sampleToString(vector<Hand> &sample);
	string boardToString(vector<Card> &board);
	StdDeck_CardMask cardToMask(Card card);
public:
	EqCalculator();
	double calculate(vector<PlayerRange> &range, vector<Card> &board, int nrtrials);
};