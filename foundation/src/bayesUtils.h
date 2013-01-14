#pragma once
#include "handHistoryParser.h"

char numberToCard(int num)
{
	char card;

	if (num < 10)
	{
		card = '0' + num;
	}
	else
	{
		switch(num)
		{
		case 10: card = 'T'; break;
		case 11: card = 'J'; break;
		case 12: card = 'Q'; break;
		case 13: card = 'K'; break;
		case 14: card = 'A'; break;
		}
	}
	return card;
}
int convertRankToNumbers(Card c)
{
	int map[100];
	map['A'] = 14;
	map['K'] = 13;
	map['Q'] = 12;
	map['J'] = 11;
	map['T'] = 10;

	if ((c.rank >= '2') && (c.rank <= '9'))
	{
		c.rank = c.rank - '0';
	}
	else
	{
		c.rank = map[c.rank];
	}
	return c.rank;
}
int normalizeHand(Hand hand) //-> 1..169
{
	if (convertRankToNumbers(hand.card1) > convertRankToNumbers(hand.card2))
	{
		int temp = hand.card1.rank;
		hand.card1.rank = hand.card2.rank;
		hand.card2.rank = temp;
	}

	if (hand.card1.suit == hand.card2.suit) //suited
	{
		int count = 0;
		for (int i = 2; i < 15; ++i)
			for (int j = i + 1; j < 15; ++j)
			{
				++count;
				if (numberToCard(i) == hand.card1.rank && numberToCard(j) == hand.card2.rank)
					return count;
			}
			
	}
	else //offsuited
	{	
		int count = 0;
		for (int i = 2; i < 15; ++i)
			for (int j = i; j < 15; ++j)
			{
				++count;
				if (numberToCard(i) == hand.card1.rank && numberToCard(j) == hand.card2.rank)
					return (count + 78);
			}
	}

	return -1;
}
int normalizeVPIP(double VPIP)
{
	if (VPIP < 14) //ultra nit
	{
		return 0;
	}

	if (VPIP < 20) //reg
	{
		return 1;
	}

	if (VPIP < 30) //reg fish
	{
		return 2;
	}

	if (VPIP < 50) //fish
	{
		return 3;
	}

	return 4; //huge fish
}
int normalizePFR(double PFR)
{
	if (PFR < 10) //nit
	{
		return 0;
	}

	if (PFR < 20) //normal
	{
		return 1;
	}

	return 2; //huge
}
int normalizeAF(double AF)
{
	if (AF <= 1) //passive
	{
		return 0;
	}

	if (AF <= 3.5) //normal
	{
		return 1;
	}

	return 2;
}
int normalizeBetSize(int street, double betsize, double potcommon, double bblind)
{
	if (street == 1)
	{
		if (betsize == bblind)
		{
			return 0;
		}
		else if (betsize <= 4 * bblind)
		{
			return 1;
		}
		else if (betsize <= 7 * bblind)
		{
			return 2;
		}
		else if (betsize <= 15 * bblind)
		{
			return 3;
		}
		else if (betsize <= 24 * bblind)
		{
			return 4;
		}
		else if (betsize <= 35 * bblind)
		{
			return 5;
		}
		else if (betsize <= 50 * bblind)
		{
			return 6;
		}
		else if (betsize <= 110 * bblind)
		{
			return 7;
		}
		else if (betsize <= 200 * bblind)
		{
			return 8;
		}
		
		return 9;
	}
	else
	{
		if ((betsize/potcommon) <= 0.3)
		{
			return 0;
		}

		if ((betsize/potcommon) <= 0.7)
		{
			return 1;
		}

		if ((betsize/potcommon) <= 1)
		{
			return 2;
		}

		if ((betsize/potcommon) <= 2)
		{
			return 3;
		}

		if ((betsize/potcommon) <= 3)
		{
			return 4;
		}

		return 5;
	}

	return 5;
}
int normalizePotSize(int street, double potcommon, double bblind)
{
	if (street == 2)//flop
	{
		if (potcommon <= 5 * bblind)  //limped pot
		{
			return 0;
		}

		if (potcommon <= 15 * bblind) //emelt pot
		{
			return 1;
		}

		if (potcommon <= 35 * bblind)
		{
			return 2;
		}

		if (potcommon <= 50 * bblind)
		{
			return 3;
		}

		return 4;
	}

	if (street == 3)
	{
		if (potcommon <= 10 * bblind) 
		{
			return 0;
		}

		if (potcommon <= 30 * bblind) 
		{
			return 1;
		}

		if (potcommon <= 70 * bblind)
		{
			return 2;
		}

		if (potcommon <= 100 * bblind)
		{
			return 3;
		}

		return 4;
	}

	if (street == 4)
	{
		if (potcommon <= 20 * bblind) 
		{
			return 0;
		}

		if (potcommon <= 40 * bblind) 
		{
			return 1;
		}

		if (potcommon <= 80 * bblind)
		{
			return 2;
		}

		if (potcommon <= 120 * bblind)
		{
			return 3;
		}

		return 4;
	}

	return 0;
}
int normalizeStackSize(double stacksize, double bblind)
{
	if (stacksize < 30 * bblind)
	{
		return 0;
	}

	if (stacksize < 70 * bblind)
	{
		return 1;
	}

	if (stacksize < 110 * bblind)
	{
		return 2;
	}

	if (stacksize < 150 * bblind)
	{
		return 3;
	}

	return 4;
}