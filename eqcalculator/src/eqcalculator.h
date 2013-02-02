#pragma once

#include "rangeFunctions.h"
#include "Hand.h"
#include "stdafx.h"
#include "HoldemCalculator.h"

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
	StdDeck_CardMask cardToMask(Card);
public:
	EqCalculator();
	double calculate(vector<PlayerRange> &range, vector<Card> &board, int nrtrials);
};