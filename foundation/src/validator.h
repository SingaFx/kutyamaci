#pragma once

#include "handHistoryParser.h"
#include <vector>

using namespace std;

class Validator
{
public:
	static bool softValidate(vector<Card>& cards, Hand hand, Hand own)
	{
		for (int i = 0; i < cards.size(); ++i)
		{
			if (cards[i].getRank() == hand.getCard1().getRank() && cards[i].getSuit() == hand.getCard1().getSuit()) return false;
			if (cards[i].getRank() == hand.getCard2().getRank() && cards[i].getSuit() == hand.getCard2().getSuit()) return false;
		}

		if (hand.getCard1() == own.getCard1() || hand.getCard1() == own.getCard2() || hand.getCard2() == own.getCard1() || hand.getCard2() == own.getCard2()) return false;

		return true;
	}

	static bool hardValidate(vector<Card>& cards, Hand hand)
	{
		cards.push_back(hand.getCard1());
		cards.push_back(hand.getCard2());

		return true;
	}
};