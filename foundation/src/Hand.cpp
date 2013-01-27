#include "Hand.h"

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