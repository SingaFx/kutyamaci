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
			if (cards[i].rank == hand.card1.rank && cards[i].suit == hand.card1.suit) return false;
			if (cards[i].rank == hand.card2.rank && cards[i].suit == hand.card2.suit) return false;
		}

		if (hand.card1 == own.card1 || hand.card2 == own.card2) return false;

		return true;
	}

	static bool hardValidate(vector<Card>& cards, Hand hand)
	{
		cards.push_back(hand.card1);
		cards.push_back(hand.card2);

		return true;
	}
};