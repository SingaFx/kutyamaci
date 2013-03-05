#include "Hand.h"
#include "bayesUtils.h"

Hand::Hand()
 : card1()
 , card2()
{
}

Hand::Hand(Card c1, Card c2)
{
    setHand(c1, c2);
};

Hand::~Hand()
{
}

void Hand::setHand(Card c1, Card c2)
{
    card1 = c1;
    card2 = c2;
};

const Card& Hand::getCard1() const
{
    return card1;
};

Card& Hand::getCard1()
{
    return card1;
};

const Card& Hand::getCard2() const
{
    return card2;
};

Card& Hand::getCard2()
{
    return card2;
};

bool Hand::operator == (const Hand& h) const
{
    return ((h.card1 == card1) && (h.card2 == card2))
        || ((h.card1 == card2) && (h.card2 == card1));
}

bool Hand::operator!=(const Hand& h) const
{
    return !(*this == h);
}

bool Hand::operator < (const Hand& h) const
{
	bool less = false;
    if (h.card1 < card1)
    {
        less = true;
    } else if (h.card1 == card1 && h.card2 < card2)
    {
        less = true;
    }

    return less;
}

string Hand::toString()
{
	string res = "";

	res += card1.toString();
	res += card2.toString();

	return res;
}

string Hand::toString() const
{
	string res = "";

	res += card1.toString();
	res += card2.toString();

	return res;
}

bool Hand::valid()
{
	return card1.valid() && card2.valid() && !(card1 == card2);
}

bool Hand::isPocket()
{
	return card1.getRank() == card2.getRank();
}

bool Hand::isAK()
{
	char cardRank1 = card1.getRank();
	char cardRank2 = card2.getRank();

	if ((cardRank1 == 'A' && cardRank2 == 'K') || (cardRank1 == 'K' && cardRank2 == 'A'))
	{
		return true;
	}

	return false;
}

bool Hand::isAQ()
{
	char cardRank1 = card1.getRank();
	char cardRank2 = card2.getRank();

	if ((cardRank1 == 'A' && cardRank2 == 'Q') || (cardRank1 == 'Q' && cardRank2 == 'A'))
	{
		return true;
	}

	return false;
}

bool Hand::isAJ()
{
	char cardRank1 = card1.getRank();
	char cardRank2 = card2.getRank();

	if ((cardRank1 == 'A' && cardRank2 == 'J') || (cardRank1 == 'J' && cardRank2 == 'A'))
	{
		return true;
	}

	return false;
}

bool Hand::isKJ()
{
	char cardRank1 = card1.getRank();
	char cardRank2 = card2.getRank();

	if ((cardRank1 == 'K' && cardRank2 == 'J') || (cardRank1 == 'J' && cardRank2 == 'K'))
	{
		return true;
	}

	return false;
}

bool Hand::isKQ()
{
	char cardRank1 = card1.getRank();
	char cardRank2 = card2.getRank();

	if ((cardRank1 == 'K' && cardRank2 == 'Q') || (cardRank1 == 'Q' && cardRank2 == 'K'))
	{
		return true;
	}

	return false;
}

bool Hand::isAT()
{
	char cardRank1 = card1.getRank();
	char cardRank2 = card2.getRank();

	if ((cardRank1 == 'A' && cardRank2 == 'T') || (cardRank1 == 'T' && cardRank2 == 'A'))
	{
		return true;
	}

	return false;
}

bool Hand::isKT()
{
	char cardRank1 = card1.getRank();
	char cardRank2 = card2.getRank();

	if ((cardRank1 == 'K' && cardRank2 == 'T') || (cardRank1 == 'T' && cardRank2 == 'K'))
	{
		return true;
	}

	return false;
}

bool Hand::isQJ()
{
	char cardRank1 = card1.getRank();
	char cardRank2 = card2.getRank();

	if ((cardRank1 == 'Q' && cardRank2 == 'J') || (cardRank1 == 'J' && cardRank2 == 'Q'))
	{
		return true;
	}

	return false;
}


bool Hand::isOOP3Bet()
{
	if (isAK() || isAQ() || (isPocket() && card1.isBroadway() && card1.getRank() != 'T')) return true;

	return false;
}

bool Hand::is100BBStackOff()
{
	if (isAK() || (isPocket() && (card1.getRank() == 'Q' || card1.getRank() == 'K' || card1.getRank() == 'A'))) return true;

	return false;
}

bool Hand::isKingsPlus()
{
	if ((isPocket() && (card1.getRank() == 'K' || card1.getRank() == 'A'))) return true;

	return false;
}

bool Hand::isBigSC()
{
	if (card1.getSuit() != card2.getSuit()) return false;

	if ((card1.getRank() == '8' && card2.getRank() == '9') || (card1.getRank() == '9' || card2.getRank() == '8')) return true;
	if ((card1.getRank() == '9' && card2.getRank() == 'T') || (card1.getRank() == 'T' || card2.getRank() == '9')) return true;

	return false;
}

bool Hand::isAxs()
{
	if (card1.getSuit() != card2.getSuit()) return false;
	
	int rank1 = convertRankToNumbers(card1);
	int rank2 = convertRankToNumbers(card2);
	if ((rank1 == 14 && rank2 > 10) || (rank2 == 14 && rank1 > 10)) return true; 

	return false;
}

bool Hand::isStrongBroadway()
{
	if (is100BBStackOff()) return true;
	if (isAQ() || isAJ() || isKQ() || isKJ()) return true;

	return false;
}

bool Hand::isSuitedBroadway()
{
	if (card1.getSuit() != card2.getSuit()) return false;

	if (isAK() || isAQ() || isAJ() || isAT() || isKQ() || isKJ() || isKT() || isQJ()) return true;

	return false;
}