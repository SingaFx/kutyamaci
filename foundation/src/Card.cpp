#include "Card.h"

Card::Card()
 : rank(1)
 , suit('x')
{
}

Card::Card(char r, char s)
 : rank(r)
 , suit(s)
{
}

Card::~Card()
{
}

void Card::setRank(char r)
{
    rank = r;
}

char Card::getRank()
{
    return rank;
}

char Card::getRank() const
{
    return rank;
}

void Card::setSuit(char s)
{
    suit = s;
}

char Card::getSuit()
{
    return suit;
}

char Card::getSuit() const
{
    return suit;
}

void Card::setCard(char r, char s)
{
    rank = r;
    suit = s;
}

bool Card::operator == (const Card& c) const
{
	return (c.rank == rank) && (c.suit == suit);
}

bool Card::operator < (const Card& c) const
{
    bool less = false;
    if (c.suit < suit)
    {
        less = true;
    } else if (c.suit == suit) {
        if (c.rank < rank)
        {
            less = true;
        }
    }

    return less;
}

string Card::toString()
{
	string res;

	res.push_back(rank);
	res.push_back(suit);

	return res;
}

bool Card::valid()
{
	return (suit == 's' || suit == 'h' || suit == 'd' || suit == 'c') && ((rank >= '2' && rank <= '9') || (rank == 'T' || rank == 'J' || rank == 'Q' || rank == 'K' || rank == 'A'));
}

bool Card::isBroadway()
{
	if (rank == 'A' || rank == 'K' || rank == 'Q' || rank == 'J' || rank == 'T')
	{
		return true;
	}
	return false;
}