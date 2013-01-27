#pragma once
#include <string>
#include "handHistoryParser.h"
#include <assert.h>
#include <algorithm>
#include <vector>
#include <iostream>

using namespace std;

class Evaluator
{
	static int maxim(int a, int b)
	{
		return a < b ? b : a;
	}

	static void convertRankToNumbers(Card &c)
	{
		int map[100];
		map['A'] = 14;
		map['K'] = 13;
		map['Q'] = 12;
		map['J'] = 11;
		map['T'] = 10;

		if ((c.getRank() >= '2') && (c.getRank() <= '9'))
		{
			c.setRank(c.getRank() - '0');
		}
		else
		{
			c.setRank(map[c.getRank()]);
		}

		return;
	}

	static bool compareCards(Card &c1, Card &c2)
	{
		return c1.getRank() < c2.getRank();
	}

	static bool flopStraightFlush(vector<Card> &cards)
	{
		assert(cards.size() == 5);
		sort(cards.begin(), cards.end(), compareCards);

		return ((((cards[0].getRank() == cards[1].getRank()-1) && (cards[1].getRank() == cards[2].getRank()-1) && (cards[2].getRank() == cards[3].getRank()-1) && (cards[3].getRank() == cards[4].getRank()-1)) ||
				((cards[0].getRank() == 2) && (cards[1].getRank() == 3) && (cards[2].getRank() == 4) && (cards[3].getRank() == 5) && (cards[4].getRank() == 14))) &&
				(cards[0].getSuit() == cards[1].getSuit()) && (cards[1].getSuit() == cards[2].getSuit()) && (cards[2].getSuit() == cards[3].getSuit()) && (cards[3].getSuit() == cards[4].getSuit()));
	}

	static bool flopPoker(vector<Card> &cards)
	{
		assert(cards.size() == 5);
		sort(cards.begin(), cards.end(), compareCards);

		return (((cards[0].getRank() == cards[1].getRank()) && (cards[1].getRank() == cards[2].getRank()) && (cards[2].getRank() == cards[3].getRank())) ||
				((cards[1].getRank() == cards[2].getRank()) && (cards[2].getRank() == cards[3].getRank()) && (cards[3].getRank() == cards[4].getRank())));
	}

	static bool flopFullHouse(vector<Card> &cards)
	{
		assert(cards.size() == 5);
		sort(cards.begin(), cards.end(), compareCards);

		return (((cards[0].getRank() == cards[1].getRank()) && (cards[2].getRank() == cards[3].getRank()) && (cards[3].getRank() == cards[4].getRank())) ||
				((cards[0].getRank() == cards[1].getRank()) && (cards[1].getRank() == cards[2].getRank()) && (cards[3].getRank() == cards[4].getRank())));
	}

	static bool flopFlush(vector<Card> &cards)
	{
		assert(cards.size() == 5);
		sort(cards.begin(), cards.end(), compareCards);

		if (flopStraightFlush(cards)) return false;

		return ((cards[0].getSuit() == cards[1].getSuit()) && (cards[1].getSuit() == cards[2].getSuit()) && (cards[2].getSuit() == cards[3].getSuit()) && (cards[3].getSuit() == cards[4].getSuit()));
	}

	static bool flopStraight(vector<Card> &cards)
	{
		assert(cards.size() == 5);
		sort(cards.begin(), cards.end(), compareCards);

		if (flopStraightFlush(cards)) return false;

		return (((cards[0].getRank() == cards[1].getRank()-1) && (cards[1].getRank() == cards[2].getRank()-1) && (cards[2].getRank() == cards[3].getRank()-1) && (cards[3].getRank() == cards[4].getRank()-1)) ||
				((cards[0].getRank() == 2) && (cards[1].getRank() == 3) && (cards[2].getRank() == 4) && (cards[3].getRank() == 5) && (cards[4].getRank() == 14)));
	}

	static bool flopThreeofaKind(vector<Card> &cards)
	{
		assert(cards.size() == 5);
		sort(cards.begin(), cards.end(), compareCards);

		if (flopPoker(cards)) return false;
		if (flopFullHouse(cards)) return false;

		return (((cards[0].getRank() == cards[1].getRank()) && (cards[1].getRank() == cards[2].getRank())) || ((cards[1].getRank() == cards[2].getRank()) && (cards[2].getRank() == cards[3].getRank()))
				|| ((cards[2].getRank() == cards[3].getRank()) && (cards[3].getRank() == cards[4].getRank())));
	}

	static bool flopTwoPair(vector<Card> &cards)
	{
		assert(cards.size() == 5);
		sort(cards.begin(), cards.end(), compareCards);

		if (flopFullHouse(cards)) return false;
		if (flopPoker(cards)) return false;
		if (flopThreeofaKind(cards)) return false;

		return (((cards[0].getRank() == cards[1].getRank()) && (cards[2].getRank() == cards[3].getRank())) || ((cards[1].getRank() == cards[2].getRank()) && (cards[3].getRank() == cards[4].getRank()))
				|| ((cards[0].getRank() == cards[1].getRank()) && (cards[3].getRank() == cards[4].getRank())));
	}

	static bool flopOnePair(vector<Card> &cards)
	{
		assert(cards.size() == 5);
		sort(cards.begin(), cards.end(), compareCards);

		if (flopTwoPair(cards)) return false;
		if (flopThreeofaKind(cards)) return false;
		if (flopFullHouse(cards)) return false;
		if (flopPoker(cards)) return false;

		return ((cards[0].getRank() == cards[1].getRank()) || (cards[1].getRank() == cards[2].getRank()) || (cards[2].getRank() == cards[3].getRank()) || (cards[3].getRank() == cards[4].getRank()));
	}

	static Card flopHighestCard(Card &b1, Card &b2, Card &b3)
	{
		if ((b1.getRank() >= b2.getRank()) && (b1.getRank() >= b3.getRank()))
			return b1;

		if ((b2.getRank() >= b1.getRank()) && (b2.getRank() >= b3.getRank()))
			return b2;

		if ((b3.getRank() >= b2.getRank()) && (b3.getRank() >= b1.getRank()))
			return b3;

		cout << "Error in flopHighestCard : Script is supposed to terminate earlier." << endl;
	}

	static bool flopFlushDraw(Card &h1, Card &h2, Card &b1, Card &b2, Card &b3)
	{
		Card cards[] = {h1,h2,b1,b2,b3};
		vector<Card> vcards(cards, cards + sizeof(cards) / sizeof(Card));

		if (flopFlush(vcards))
			return false;

		return ((h1.getSuit() == h2.getSuit()) && (h2.getSuit() == b1.getSuit()) && (b1.getSuit() == b2.getSuit())) ||
			((h1.getSuit() == h2.getSuit()) && (h2.getSuit() == b1.getSuit()) && (b1.getSuit() == b3.getSuit())) ||
			((h1.getSuit() == h2.getSuit()) && (h2.getSuit() == b2.getSuit()) && (b2.getSuit() == b3.getSuit())) ||
			((h1.getSuit() == b1.getSuit()) && (b1.getSuit() == b2.getSuit()) && (b2.getSuit() == b3.getSuit())) ||
			((h2.getSuit() == b1.getSuit()) && (b1.getSuit() == b2.getSuit()) && (b2.getSuit() == b3.getSuit()));
	}

	static bool flopTripleBoard(Card &b1, Card &b2, Card &b3)
	{
		return (b1.getRank() == b2.getRank()) && (b2.getRank() == b3.getRank());
	}

	static bool flopDoubleBoard(Card &b1, Card &b2, Card &b3)
	{
		if (flopTripleBoard(b1,b2,b3))
			return false;

		return (b1.getRank() == b2.getRank()) || (b1.getRank() == b3.getRank()) || (b2.getRank() == b3.getRank());
	}

	static bool flopSingleBoard(Card &b1, Card &b2, Card &b3)
	{
		return !(flopDoubleBoard(b1,b2,b3) || flopTripleBoard(b1,b2,b3));
	}

	static bool flopMonotoneBoard(Card &b1, Card &b2, Card &b3)
	{
		return (b1.getSuit() == b2.getSuit()) && (b2.getSuit() == b3.getSuit());
	}

	static bool flopTwoToneBoard(Card &b1, Card &b2, Card &b3)
	{
		if (flopMonotoneBoard(b1,b2,b3))
			return false;

		return ((b1.getSuit() == b2.getSuit()) || (b1.getSuit() == b3.getSuit()) || (b2.getSuit() == b3.getSuit()));
	}

	static int flopHiLoDistance(Card &b1, Card &b2, Card &b3)
	{
		int dist1 = abs(b1.getRank() - b2.getRank());
		int dist2 = abs(b1.getRank() - b3.getRank());
		int dist3 = abs(b2.getRank() - b3.getRank());

		return (dist1 > dist2) ? ((dist1 > dist3) ? dist1 : dist3) : ((dist2 > dist3) ? dist2 : dist3);
	}

	static int flopMinDistance(Card &b1, Card &b2, Card &b3)
	{
		int dist1 = abs(b1.getRank() - b2.getRank());
		int dist2 = abs(b1.getRank() - b3.getRank());
		int dist3 = abs(b2.getRank() - b3.getRank());

		return (dist1 < dist2) ? ((dist1 < dist3) ? dist1 : dist3) : ((dist2 < dist3) ? dist2 : dist3);
	}

	static bool flopVeryConnectedBoard(Card &b1, Card &b2, Card &b3)
	{
		if (!flopSingleBoard(b1,b2,b3))
			return false;

		return flopHiLoDistance(b1,b2,b3) == 2;
	}

	static bool flopConnectedBoard(Card &b1, Card &b2, Card &b3)
	{
		if (!flopSingleBoard(b1,b2,b3))
			return false;

		return flopHiLoDistance(b1,b2,b3) <= 4;
	}

	static bool flopLessConnectedBoard(Card &b1, Card &b2, Card &b3)
	{
		if (!flopSingleBoard(b1,b2,b3))
			return false;

		return flopMinDistance(b1,b2,b3) <= 2;
	}

	static bool flopTwoUnderCards(Card &h1, Card &h2, Card &b1, Card &b2, Card &b3)
	{
		return (h1.getRank() < b1.getRank()) && (h1.getRank() < b2.getRank()) && (h1.getRank() < b3.getRank()) &&
				(h2.getRank() < b1.getRank()) && (h2.getRank() < b2.getRank()) && (h2.getRank() < b3.getRank());
	}

	static bool flopTwoOverCards(Card &h1, Card &h2, Card &b1, Card &b2, Card &b3)
	{
		return (h1.getRank() > b1.getRank()) && (h1.getRank() > b2.getRank()) && (h1.getRank() > b3.getRank()) &&
				(h2.getRank() > b1.getRank()) && (h2.getRank() > b2.getRank()) && (h2.getRank() > b3.getRank());
	}

	static bool flopStraightTwoUnderCards(Card &h1, Card &h2, Card &b1, Card &b2, Card &b3)
	{
		if (h1.getRank() < 14 && h2.getRank() < 14)
			return flopTwoUnderCards(h1,h2,b1,b2,b3) && (h1.getRank() < 10 || h2.getRank() < 10);

		if (h1.getRank() == 14)
			return h2.getRank() == 2 && b1.getRank() <= 5 && b2.getRank() <= 5 && b3.getRank() <=5;
		return h1.getRank() == 2 && b1.getRank() <= 5 && b2.getRank() <= 5 && b3.getRank() <=5;
	}

	static bool flopNutFlushDraw(Card &h1, Card &h2, Card &b1, Card &b2, Card &b3)
	{
		if (!flopFlushDraw(h1,h2,b1,b2,b3))
			return false;

		if (flopMonotoneBoard(b1,b2,b3))
		{
			if (h1.getSuit() == b1.getSuit())
			{
				if (flopHighestCard(b1,b2,b3).getRank() < 14)
					return h1.getRank() == 14;
				if (flopHighestCard(b1,b2,b3).getRank() < 13)
					return h1.getRank() == 13;
				if (flopHighestCard(b1,b2,b3).getRank() < 12)
					return h1.getRank() == 12;
				return h1.getRank() == 11;
			}
			if (h2.getSuit() == b1.getSuit())
			{
				if (flopHighestCard(b1,b2,b3).getRank() < 14)
					return h2.getRank() == 14;
				if (flopHighestCard(b1,b2,b3).getRank() < 13)
					return h2.getRank() == 13;
				if (flopHighestCard(b1,b2,b3).getRank() < 12)
					return h2.getRank() == 12;
				return h2.getRank() == 11;
			}
		}

		Card highestSuitedCard, secondHighestSuitedCard;
		if (flopHighestCard(b1,b2,b3).getSuit() == h1.getSuit())
		{
			highestSuitedCard = flopHighestCard(b1,b2,b3);
		}
		else
		{
			if (b1.getSuit() != h1.getSuit())
			{
				highestSuitedCard = (b2.getRank() > b3.getRank()) ? b2 : b3;
				secondHighestSuitedCard = (b2.getRank() > b3.getRank()) ? b3 : b2;
			}
			if (b2.getSuit() != h1.getSuit())
			{
				highestSuitedCard = (b1.getRank() > b3.getRank()) ? b1 : b3;
				secondHighestSuitedCard = (b1.getRank() > b3.getRank()) ? b3 : b1;
			}
			if (b3.getSuit() != h1.getSuit())
			{
				highestSuitedCard = (b1.getRank() > b2.getRank()) ? b1 : b2;
				secondHighestSuitedCard = (b1.getRank() > b2.getRank()) ? b2 : b1;
			}
		}

		if (highestSuitedCard.getRank() < 14)
			return (h1.getRank() == 14) || (h2.getRank() == 14);

		if (secondHighestSuitedCard.getRank() < 13)
			return (h1.getRank() == 13) || (h2.getRank() == 13);

		return (h1.getRank() == 12) || (h2.getRank() == 12);
	}

	static bool flopDangerousBoard(Card &b1, Card &b2, Card b3)
	{
		 return flopMonotoneBoard(b1,b2,b3) || flopVeryConnectedBoard(b1,b2,b3) || (flopConnectedBoard(b1,b2,b3) && flopTwoToneBoard(b1,b2,b3));
	}

	static bool flopSemiDangerousBoard(Card &b1, Card &b2, Card b3)
	{
		if (flopDangerousBoard(b1,b2,b3))
			return false;

		return flopLessConnectedBoard(b1,b2,b3) && flopTwoToneBoard(b1,b2,b3);
	}

	static bool flopOESD(vector<Card> &cards)
	{
		assert(cards.size() == 5);
		sort(cards.begin(), cards.end(), compareCards);

		if (flopStraight(cards) || flopFlush(cards))
			return false;

		vector<Card> uniqueCards;
		uniqueCards.push_back(cards[0]);
		for (int i = 1; i < cards.size(); ++i)
			if (cards[i].getRank() != cards[i-1].getRank())
				uniqueCards.push_back(cards[i]);

		if (uniqueCards.size() < 4)
			return false;

		if ((uniqueCards[3].getRank()-uniqueCards[0].getRank() == 3 && uniqueCards[3].getRank() != 14) || (uniqueCards.size() == 5 && uniqueCards[4].getRank()-uniqueCards[1].getRank() == 3 && uniqueCards[4].getRank() != 14))
			return true;

		return (((cards[0].getRank() == cards[1].getRank()-1) && (cards[1].getRank() == cards[2].getRank()-1) && (cards[2].getRank() == cards[3].getRank()-1) && (cards[3].getRank() < 14)) ||
				((cards[1].getRank() == cards[2].getRank()-1) && (cards[2].getRank() == cards[3].getRank()-1) && (cards[3].getRank() == cards[4].getRank()-1) && (cards[4].getRank() < 14)) ||
				((cards[0].getRank() == cards[1].getRank()-2) && (cards[1].getRank() == cards[2].getRank()-1) && (cards[2].getRank() == cards[3].getRank()-1) && (cards[3].getRank() == cards[4].getRank()-2)) ||
				((cards[0].getRank() == 3) && (cards[1].getRank() == 4) && (cards[2].getRank() == 5) && (cards[3].getRank() == 7) && (cards[4].getRank() == 14)));
	}

	static bool flopGutshot(vector<Card> &cards)
	{
		assert(cards.size() == 5);
		sort(cards.begin(), cards.end(), compareCards);

		if (flopStraight(cards) || flopFlush(cards) || flopOESD(cards))
			return false;

		vector<Card> uniqueCards;
		uniqueCards.push_back(cards[0]);
		for (int i = 1; i < cards.size(); ++i)
			if (cards[i].getRank() != cards[i-1].getRank())
				uniqueCards.push_back(cards[i]);

		return (uniqueCards.size() >= 4 && uniqueCards[3].getRank() - uniqueCards[0].getRank() <= 4) || (uniqueCards.size() >= 5 && uniqueCards[4].getRank() - uniqueCards[1].getRank() <= 4) ||
			(uniqueCards.size() >= 4 && uniqueCards[uniqueCards.size()-1].getRank() == 14 && uniqueCards[2].getRank() <= 5);
	}

	static bool flopNutOESD(Card &h1, Card &h2, Card &b1, Card &b2, Card &b3)
	{
		Card placeHolderCard;
		placeHolderCard.setRank(20);
		placeHolderCard.setSuit(-1);
		Card cards[] = {h1,h2,b1,b2,b3};
		vector<Card> vcards(cards, cards + sizeof(cards) / sizeof(Card));
		Card cards2[] = {h2,b1,b2,b3,placeHolderCard};
		vector<Card> vcards2(cards2, cards2 + sizeof(cards2) / sizeof(Card));
		Card cards3[] = {h1,b1,b2,b3,placeHolderCard};
		vector<Card> vcards3(cards3, cards3 + sizeof(cards3) / sizeof(Card));

		if (!flopOESD(vcards))
			return false;

		if (flopOESD(vcards2))
		{
			return ((h2.getRank() > b1.getRank()) && (h2.getRank() > b2.getRank()) && (h2.getRank() > b3.getRank()));
		}
		if (flopOESD(vcards3))
		{
			return ((h1.getRank() > b1.getRank()) && (h1.getRank() > b2.getRank()) && (h1.getRank() > b3.getRank()));
		}

		Card cards4[] = {h1,h2,b1,b2,placeHolderCard};
		vector<Card> vcards4(cards4, cards4+sizeof(cards4) / sizeof(Card));
		if (flopOESD(vcards4))
		{
			return !flopTwoUnderCards(h1,h2,b1,b2,placeHolderCard);
		}
		Card cards5[] = {h1,h2,b1,b3,placeHolderCard};
		vector<Card> vcards5(cards5, cards5+sizeof(cards5) / sizeof(Card));
		if (flopOESD(vcards5))
		{
			return !flopTwoUnderCards(h1,h2,b1,b3,placeHolderCard);
		}
		Card cards6[] = {h1,h2,b2,b3,placeHolderCard};
		vector<Card> vcards6(cards6, cards6+sizeof(cards6) / sizeof(Card));
		if (flopOESD(vcards6))
		{
			return !flopTwoUnderCards(h1,h2,b2,b3,placeHolderCard);
		}

		return true;
	}

	static bool turnHandStrength(vector<Card> &cards, bool fn(vector<Card>&))
	{
		assert(cards.size() == 6);

		for (int i = 0; i < cards.size(); ++i)
		{
			vector<Card> temp = cards;
			temp.erase(temp.begin() + i);
			if (fn(temp))
				return true;
		}

		return false;
	}

	static bool turnStraightFlush(vector<Card> &cards)
	{
		return turnHandStrength(cards,flopStraightFlush);
	}

	static bool turnPoker(vector<Card> &cards)
	{
		return turnHandStrength(cards, flopPoker);
	}

	static bool turnFullHouse(vector<Card> &cards)
	{
		return turnHandStrength(cards, flopFullHouse);
	}

	static bool turnFlush(vector<Card> &cards)
	{
		return turnHandStrength(cards, flopFlush);
	}

	static bool turnStraight(vector<Card> &cards)
	{
		return turnHandStrength(cards, flopStraight);
	}

	static bool turnThreeofaKind(vector<Card> &cards)
	{
		return turnHandStrength(cards, flopThreeofaKind);
	}

	static bool turnTwoPair(vector<Card> &cards)
	{
		return turnHandStrength(cards, flopTwoPair);
	}

	static bool turnOnePair(vector<Card> &cards)
	{
		return turnHandStrength(cards, flopOnePair);
	}

	static bool flopHandStrength(Card &c1, Card &c2, Card &c3, Card &c4, Card &c5, bool fn(vector<Card>&))
	{
		vector<Card> temp;
		temp.push_back(c1);temp.push_back(c2);temp.push_back(c3);temp.push_back(c4);temp.push_back(c5);

		return fn(temp);
	}

	static bool flopStraightFlush(Card &c1, Card &c2, Card &c3, Card &c4, Card &c5)
	{
		return flopHandStrength(c1, c2, c3, c4, c5, flopStraightFlush);
	}

	static bool flopPoker(Card &c1, Card &c2, Card &c3, Card &c4, Card &c5)
	{
		return flopHandStrength(c1, c2, c3, c4, c5, flopPoker);
	}

	static bool flopFullHouse(Card &c1, Card &c2, Card &c3, Card &c4, Card &c5)
	{
		return flopHandStrength(c1, c2, c3, c4, c5, flopFullHouse);
	}

	static bool flopFlush(Card &c1, Card &c2, Card &c3, Card &c4, Card &c5)
	{
		return flopHandStrength(c1, c2, c3, c4, c5, flopFlush);
	}

	static bool flopStraight(Card &c1, Card &c2, Card &c3, Card &c4, Card &c5)
	{
		return flopHandStrength(c1, c2, c3, c4, c5, flopStraight);
	}

	static bool flopThreeofaKind(Card &c1, Card &c2, Card &c3, Card &c4, Card &c5)
	{
		return flopHandStrength(c1, c2, c3, c4, c5, flopThreeofaKind);
	}

	static bool flopTwoPair(Card &c1, Card &c2, Card &c3, Card &c4, Card &c5)
	{
		return flopHandStrength(c1, c2, c3, c4, c5, flopTwoPair);
	}

	static bool flopOnePair(Card &c1, Card &c2, Card &c3, Card &c4, Card &c5)
	{
		return flopHandStrength(c1, c2, c3, c4, c5, flopOnePair);
	}

	static bool turnDoublePairedBoard(vector<Card> &board)
	{
		assert(board.size() == 4);

		return (board[0].getRank() == board[1].getRank()) && (board[1].getRank() != board[2].getRank()) && (board[2].getRank() == board[3].getRank());
	}

	static bool turnQuadBoard(vector<Card> &board)
	{
		assert(board.size() == 4);

		return (board[0].getRank() == board[1].getRank()) && (board[1].getRank() == board[2].getRank()) && (board[2].getRank() == board[3].getRank());
	}

	static bool turnTripleBoard(vector<Card> &board)
	{
		assert(board.size() == 4);

		if (turnQuadBoard(board))
			return false;

		return ((board[0].getRank() == board[1].getRank()) && (board[1].getRank() == board[2].getRank())) || ((board[1].getRank() == board[2].getRank()) && (board[2].getRank() == board[3].getRank()));
	}

	static bool turnDoubleBoard(vector<Card> &board)
	{
		if (turnQuadBoard(board) || turnTripleBoard(board))
			return false;

		return ((board[0].getRank() == board[1].getRank()) || (board[1].getRank() == board[2].getRank()) || (board[2].getRank() == board[3].getRank()));
	}

	static bool turnSingleBoard(vector<Card> &board)
	{
		return !turnQuadBoard(board) && !turnTripleBoard(board) && !turnDoubleBoard(board);
	}

	static bool turnMonotoneBoard(vector<Card> &board)
	{
		return (board[0].getSuit() == board[1].getSuit()) && (board[1].getSuit() == board[2].getSuit()) && (board[2].getSuit() == board[3].getSuit());
	}

	static bool turnOneCardStrBoard(vector<Card> &board)
	{
		if (!turnSingleBoard(board))
			return false;

		return (board[3].getRank() - board[0].getRank() <= 4) || (board[3].getRank() == 14 && board[2].getRank() <= 5);
	}

	static bool turnOneCardNutFlush(Card &h1, Card &h2, vector<Card> &board)
	{
		if (!turnMonotoneBoard(board))
			return false;

		Card suitedCard;
		if (h1.getSuit() == board[0].getSuit())
		{
			suitedCard = h1;
		}
		else if (h2.getSuit() == board[0].getSuit())
		{
			suitedCard = h2;
		}
		else
		{
			return false;
		}

		return suitedCard.getRank() == 14 || (board[3].getRank() == 14 && suitedCard.getRank() == 13) || (board[3].getRank() == 14 && board[2].getRank() == 13 && suitedCard.getRank() == 12) ||
			(board[3].getRank() == 14 && board[2].getRank() == 13 && board[1].getRank() == 12 && suitedCard.getRank() == 11);
	}

	static bool turnOneCardStr(Card &h1, Card &h2, vector<Card> &board)
	{
		vector<Card> temp2;
		temp2.push_back(board[1]);temp2.push_back(board[2]);temp2.push_back(board[3]);temp2.push_back(h1);temp2.push_back(h2);

		vector<Card> temp = board;
		temp.push_back(h1);
		if (flopStraight(temp))
		{
			if (flopStraight(temp2) && (h2.getRank() > board[3].getRank()))
				return false;
			return true;
		}

		temp = board;
		temp.push_back(h2);
		if (flopStraight(temp))
		{
			if (flopStraight(temp2) && (h1.getRank() > board[3].getRank()))
				return false;
			return true;
		}

		return false;
	}

	static bool turnLowCardinStraight(Card &h, vector<Card> &board)
	{
		if (h.getRank() < 14)
			return h.getRank() < board[0].getRank() && h.getRank() < board[1].getRank() && h.getRank() < board[2].getRank() && h.getRank() < board[3].getRank() && (h.getRank() > 2 || board[3].getRank() < 14);

		return board[0].getRank() == 2 && board[1].getRank() == 3 && board[2].getRank() == 4 && board[3].getRank() == 5;
	}

	static bool turnOneCardLowStr(Card &h1, Card &h2, vector<Card> &board)
	{
		if (!turnOneCardStr(h1,h2,board))
			return false;

		vector<Card> temp = board;
		vector<Card> temp2 = board;
		temp.push_back(h1);
		temp2.push_back(h2);
		if (flopStraight(temp) && flopStraight(temp2))
			return turnLowCardinStraight(h1,board) && turnLowCardinStraight(h2,board);
		if (flopStraight(temp))
			return turnLowCardinStraight(h1,board);

		return turnLowCardinStraight(h2,board);
	}

	static bool turnOneCardNutStraight(Card &h1, Card &h2, vector<Card> &board)
	{
		if (!turnOneCardStr(h1,h2,board))
			return false;

		vector<Card> straight;
		vector<Card> temp = board;
		vector<Card> temp2 = board;
		temp.push_back(h1);
		temp2.push_back(h2);
		if (flopStraight(temp) && flopStraight(temp2))
		{
			if ((h1.getRank() == 14) && (h2.getRank() == 6))
			{
				straight = temp2;
			}
			else if ((h1.getRank() == 6) && (h2.getRank() == 14))
			{
				straight = temp;
			}
			else
			{
				straight = h1.getRank() > h2.getRank() ? temp : temp2;
			}
		}
		else if (flopStraight(temp))
		{
			straight = temp;
		}
		else
		{
			straight = temp2;
		}

		sort(straight.begin(), straight.end(), compareCards);

		return straight[4].getRank() == 14 && straight[3].getRank() == 13;
	}

	static bool turnOneCardNutFlushDraw(Card &h1, Card &h2, vector<Card> &board)
	{
		if (!turnThreeToneBoard(board))
			return false;

		if (!turnFlushDraw(h1,h2,board[0],board[1],board[2],board[3]))
			return false;

		char suit = board[0].getSuit() == board[1].getSuit() ? board[0].getSuit() : board[2].getSuit();
		Card suitedCard = h1.getSuit() == suit ? h1 : h2;

		vector<Card> temp;
		for (int i = 0; i < board.size(); ++i)
			if (board[i].getSuit() == suit)
				temp.push_back(board[i]);

		return (suitedCard.getRank() == 14) || (temp[2].getRank() == 14 && suitedCard.getRank() == 13) || (temp[1].getRank() == 13 && suitedCard.getRank() == 12) || (temp[1].getRank() == 12 && suitedCard.getRank() == 11);
	}

	static bool turnLowTwoCardStraight(Card &h1, Card &h2, vector<Card> &board)
	{
		bool result = false;

		vector<Card> temp1;
		temp1.clear();
		temp1.push_back(h1);temp1.push_back(h2);temp1.push_back(board[0]);temp1.push_back(board[1]);temp1.push_back(board[2]);
		if (flopStraight(temp1))
			if (flopStraightTwoUnderCards(h1,h2,board[0],board[1],board[2]) && (h1.getRank() < 10 || h2.getRank() < 10))
				result = true;

		temp1.clear();
		temp1.push_back(h1);temp1.push_back(h2);temp1.push_back(board[0]);temp1.push_back(board[2]);temp1.push_back(board[3]);
		if (flopStraight(temp1))
			if (flopStraightTwoUnderCards(h1,h2,board[0],board[2],board[3]) && (h1.getRank() < 10 || h2.getRank() < 10))
				result = true;

		temp1.clear();
		temp1.push_back(h1);temp1.push_back(h2);temp1.push_back(board[1]);temp1.push_back(board[2]);temp1.push_back(board[3]);
		if (flopStraight(temp1))
			if (flopStraightTwoUnderCards(h1,h2,board[1],board[2],board[3]) && (h1.getRank() < 10 || h2.getRank() < 10))
				result = true;

		return result;

		cout << "Error in turnLowTwoCardStraight : script is supposed to terminate earlier." << endl;
	}

	static bool turnThreeToneBoard(vector<Card> &board)
	{
		return (board[0].getSuit() == board[1].getSuit() && board[1].getSuit() == board[2].getSuit()) || (board[0].getSuit() == board[2].getSuit() && board[2].getSuit() == board[3].getSuit()) ||
			(board[1].getSuit() == board[2].getSuit() && board[2].getSuit() == board[3].getSuit()) || (board[0].getSuit() == board[1].getSuit() && board[1].getSuit() == board[3].getSuit());
	}

	static bool turnDangerousBoard(Card &b1, Card &b2, Card &b3, Card &b4)
	{
		return flopDangerousBoard(b1,b2,b3) || flopDangerousBoard(b1,b2,b4) || flopDangerousBoard(b1,b3,b4) || flopDangerousBoard(b2,b3,b4);
	}

	static bool existsOnBoard(int rank, vector<Card> &board)
	{
		for (int i = 0; i < board.size(); ++i)
		{
			if (board[i].getRank() == rank)
				return true;
		}

		return false;
	}

	static bool turnVeryDangerousBoard(vector<Card> &board)
	{
		return ((board[0].getSuit() == board[1].getSuit()) && (board[1].getSuit() == board[2].getSuit()) && (board[2].getSuit() == board[3].getSuit())) || (turnOneCardStrBoard(board));
	}

	static bool turnFlushDraw(Card &h1, Card &h2, Card &b1, Card &b2, Card &b3, Card &b4)
	{
		return flopFlushDraw(h1,h2,b1,b2,b3) || flopFlushDraw(h1,h2,b1,b3,b4) || flopFlushDraw(h1,h2,b1,b2,b4) || flopFlushDraw(h1,h2,b2,b3,b4);
	}

	static bool turnTwoCardFlushDraw(Card &h1, Card &h2, Card &b1, Card &b2, Card &b3, Card &b4)
	{
		return turnFlushDraw(h1,h2,b1,b2,b3,b4) && (h1.getSuit() == h2.getSuit());
	}

	static bool turnStrongFlushDraw(Card &h1, Card &h2, vector<Card> &board)
	{
		return turnTwoCardFlushDraw(h1,h2,board[0],board[1],board[2],board[3]) || turnOneCardNutFlushDraw(h1,h2,board);
	}

	static bool turnStrongOESD(Card &h1, Card &h2, Card &b1, Card &b2, Card &b3, Card &b4)
	{
		return flopNutOESD(h1,h2,b1,b2,b3) || flopNutOESD(h1,h2,b1,b2,b4) || flopNutOESD(h1,h2,b1,b3,b4) || flopNutOESD(h1,h2,b2,b3,b4);
	}

	static vector<Card> createVector(Card &h1, Card &h2, Card &b1, Card &b2, Card &b3)
	{
		vector<Card> temp; temp.push_back(h1); temp.push_back(h2); temp.push_back(b1); temp.push_back(b2); temp.push_back(b3);
		return temp;
	}

	static vector<Card> createVector(Card &h1, Card &h2, Card &b1, Card &b2, Card &b3, Card &b4)
	{
		vector<Card> temp; temp.push_back(h1); temp.push_back(h2); temp.push_back(b1); temp.push_back(b2); temp.push_back(b3); temp.push_back(b4);
		return temp;
	}

	static bool turnOESD(Card &h1, Card &h2, Card &b1, Card &b2, Card &b3, Card &b4)
	{
		return flopOESD(createVector(h1,h2,b1,b2,b3)) || flopOESD(createVector(h1,h2,b1,b2,b4)) || flopOESD(createVector(h1,h2,b1,b3,b4)) ||
			flopOESD(createVector(h1,h2,b2,b3,b4));
	}

	static bool turnGutShot(Card &h1, Card &h2, Card &b1, Card &b2, Card &b3, Card &b4)
	{
		return flopGutshot(createVector(h1,h2,b1,b2,b3)) || flopGutshot(createVector(h1,h2,b1,b2,b4)) || flopGutshot(createVector(h1,h2,b1,b3,b4)) ||
			flopGutshot(createVector(h1,h2,b2,b3,b4));
	}

	static int turnSuitedNumber(Card &b1, Card &b2, Card &b3, Card &b4)
	{
		int temp[255];
		temp['c'] = 0; temp['d'] = 0; temp['h'] = 0; temp['s'] = 0;

		temp[b1.getSuit()]++;temp[b2.getSuit()]++;temp[b3.getSuit()]++;temp[b4.getSuit()]++;

		return maxim(temp['c'],maxim(temp['d'],maxim(temp['h'],temp['s'])));
	}

	static bool riverHandStrength(vector<Card> &cards, bool fn(vector<Card>&))
	{
		assert(cards.size() == 7);

		for (int i = 0; i < cards.size() - 1; ++i)
		{
			for (int j = i+1; j < cards.size(); ++j)
			{
				vector<Card> temp = cards;
				temp.erase(temp.begin() + i);
				temp.erase(temp.begin() + j - 1);
				if (fn(temp))
					return true;
			}
		}

		return false;
	}

	static bool riverStraightFlush(vector<Card> &cards)
	{
		return riverHandStrength(cards, flopStraightFlush);
	}

	static bool riverPoker(vector<Card> &cards)
	{
		return riverHandStrength(cards, flopPoker);
	}

	static bool riverFullHouse(vector<Card> &cards)
	{
		return riverHandStrength(cards, flopFullHouse);
	}

	static bool riverFlush(vector<Card> &cards)
	{
		return riverHandStrength(cards, flopFlush);
	}

	static bool riverStraight(vector<Card> &cards)
	{
		return riverHandStrength(cards, flopStraight);
	}

	static bool riverThreeofaKind(vector<Card> &cards)
	{
		return riverHandStrength(cards, flopThreeofaKind);
	}

	static bool riverTwoPair(vector<Card> &cards)
	{
		return riverHandStrength(cards, flopTwoPair);
	}

	static bool riverOnePair(vector<Card> &cards)
	{
		return riverHandStrength(cards, flopOnePair);
	}

	static bool riverOneCardLowStrFlush(Card &h1, Card &h2, Card &b1, Card &b2, Card &b3, Card &b4, Card &b5)
	{
		Card strcard = h1;
		if (turnStraightFlush(createVector(h2,b1,b2,b3,b4,b5)))
			strcard = h2;

		if (flopStraightFlush(strcard, b1, b2, b3, b4))
			return (strcard.getRank() < b1.getRank() && strcard.getRank() < b2.getRank() && strcard.getRank() < b3.getRank() && strcard.getRank() < b4.getRank()) || (strcard.getRank() == 14 && b1.getRank() <= 5);
		if (flopStraightFlush(strcard, b1, b2, b3, b5))
			return (strcard.getRank() < b1.getRank() && strcard.getRank() < b2.getRank() && strcard.getRank() < b3.getRank() && strcard.getRank() < b5.getRank()) || (strcard.getRank() == 14 && b1.getRank() <= 5);
		if (flopStraightFlush(strcard, b1, b2, b4, b5))
			return (strcard.getRank() < b1.getRank() && strcard.getRank() < b2.getRank() && strcard.getRank() < b4.getRank() && strcard.getRank() < b5.getRank()) || (strcard.getRank() == 14 && b1.getRank() <= 5);
		if (flopStraightFlush(strcard, b1, b3, b4, b5))
			return (strcard.getRank() < b1.getRank() && strcard.getRank() < b3.getRank() && strcard.getRank() < b4.getRank() && strcard.getRank() < b5.getRank()) || (strcard.getRank() == 14 && b1.getRank() <= 5);
		if (flopStraightFlush(strcard, b2, b3, b4, b5))
			return (strcard.getRank() < b2.getRank() && strcard.getRank() < b3.getRank() && strcard.getRank() < b4.getRank() && strcard.getRank() < b5.getRank()) || (strcard.getRank() == 14 && b2.getRank() <= 5);

		cout << "Error in riverOneCardLowStrFlush : script is supposed to terminate earlier." << endl;
	}

	static bool riverDoublePairedBoard(vector<Card> &board)
	{
		assert(board.size() == 5);

		return flopTwoPair(board);
	}

	static bool riverTripleBoard(vector<Card> &board)
	{
		assert(board.size() == 5);

		return flopThreeofaKind(board);
	}

	static bool riverDoubleBoard(vector<Card> &board)
	{
		assert(board.size() == 5);

		return flopOnePair(board[0], board[1], board[2], board[3], board[4]);
	}

	static int riverSuitedNumber(Card &b1, Card &b2, Card &b3, Card &b4, Card &b5)
	{
		int temp[255];
		temp['c'] = 0; temp['d'] = 0; temp['h'] = 0; temp['s'] = 0;

		temp[b1.getSuit()]++;temp[b2.getSuit()]++;temp[b3.getSuit()]++;temp[b4.getSuit()]++;temp[b5.getSuit()]++;

		return maxim(temp['c'],maxim(temp['d'],maxim(temp['h'],temp['s'])));
	}

	static bool riverOneCardNutFlush(Card &h1, Card &h2, vector<Card> &board)
	{
		if (riverSuitedNumber(board[0], board[1], board[2], board[3], board[4]) < 4)
			return false;

		char suit = board[0].getSuit();
		if ((board[0].getSuit() != board[1].getSuit()) && (board[1].getSuit() == board[2].getSuit()))
			suit = board[1].getSuit();

		if ((h1.getSuit() != suit) && (h2.getSuit() != suit))
			return false;
		Card suitedCard = (h1.getSuit() == suit && (h2.getSuit() != suit || h2.getRank() < h1.getRank())) ? h1 : h2;

		vector<Card> suiteds = board;
		for (int i = 0; i < suiteds.size(); ++i)
			if (suiteds[i].getSuit() != suit)
				suiteds.erase(suiteds.begin() + i);

		return suitedCard.getRank() == 14 || (suitedCard.getRank() == 13 && suiteds[3].getRank() == 14) || (suitedCard.getRank() == 12 && suiteds[2].getRank() == 13 && suiteds[3].getRank() == 14)
			|| (suitedCard.getRank() == 11 && suiteds[1].getRank() == 12 && suiteds[2].getRank() == 13 && suiteds[3].getRank() == 14);
	}

	static bool riverOneCardStrBoard(vector<Card> &board)
	{
		for (int i = 0; i < board.size(); ++i)
		{
			vector<Card> temp = board;
			temp.erase(temp.begin() + i);
			if (turnOneCardStrBoard(temp))
				return true;
		}

		return false;
	}

	static bool riverPossibleOneCardStrFlush(vector<Card> &board)
	{
		if (riverSuitedNumber(board[0], board[1], board[2], board[3], board[4]) < 4)
			return false;

		char suit = board[0].getSuit();
		if ((board[0].getSuit() != board[1].getSuit()) && (board[1].getSuit() == board[2].getSuit()))
			suit = board[1].getSuit();

		vector<Card> temp = board;
		for (int i = 0; i < temp.size(); ++i)
			if (temp[i].getSuit() != suit)
				temp.erase(temp.begin() + i);

		return (temp.size() == 4 && turnOneCardStrBoard(temp)) || (temp.size() == 5 && riverOneCardStrBoard(temp));
	}

	static int highestStrCard(Card &c1, Card &c2, Card &c3, Card &c4, Card &c5)
	{
		vector<Card> temp = createVector(c1,c2,c3,c4,c5);
		sort(temp.begin(), temp.end(), compareCards);
		return (temp[0].getRank() > 2 || temp[4].getRank() < 14) ? temp[4].getRank() : 5;
	}

	static bool riverOneCardStr(Card &h1, Card &h2, vector<Card> &board)
	{
		assert(board.size() == 5);

		if (flopStraight(board))
			return false;

		int maxstr = 0;
		for (int i = 0; i < board.size(); ++i)
		{
			for (int j = i+1; j < board.size(); ++j)
			{
				vector<Card> temp = board;
				temp.erase(temp.begin() + i);
				temp.erase(temp.begin() + j - 1);
				if (flopStraight(h1,h2,temp[0],temp[1],temp[2]) && (highestStrCard(h1,h2,temp[0],temp[1],temp[2]) > maxstr))
					maxstr = highestStrCard(h1,h2,temp[0],temp[1],temp[2]);
			}
		}

		if (maxstr == 0)
			return true;

		for (int i = 0; i < board.size(); ++i)
		{
			vector<Card> temp = board;
			temp.erase(temp.begin() + i);
			if (flopStraight(h1,temp[0],temp[1],temp[2],temp[3]) && highestStrCard(h1,temp[0],temp[1],temp[2],temp[3]) >= maxstr)
				return true;
			if (flopStraight(h2,temp[0],temp[1],temp[2],temp[3]) && highestStrCard(h2,temp[0],temp[1],temp[2],temp[3]) >= maxstr)
				return true;
		}

		return false;
	}

	static bool riverOneCardLowStr(Card &h1, Card &h2, vector<Card> &board)
	{
		if (!riverOneCardStr(h1,h2,board))
			return false;

		int maxstr = 0;
		bool result;

		for (int i = 0; i < board.size(); ++i)
		{
			vector<Card> temp = board;
			temp.erase(temp.begin() + i);
			if (flopStraight(h1,temp[0],temp[1],temp[2],temp[3]) && highestStrCard(h1,temp[0],temp[1],temp[2],temp[3]) >= maxstr)
			{
				maxstr = highestStrCard(h1,temp[0],temp[1],temp[2],temp[3]);
				result = (h1.getRank() < temp[0].getRank() && (h1.getRank() > 2 || temp[3].getRank() < 14)) || (h1.getRank() == 14 && temp[0].getRank() <= 5);
			}
			if (flopStraight(h2,temp[0],temp[1],temp[2],temp[3]) && highestStrCard(h2,temp[0],temp[1],temp[2],temp[3]) >= maxstr)
			{
				maxstr = highestStrCard(h2,temp[0],temp[1],temp[2],temp[3]);
				result = (h2.getRank() < temp[0].getRank() && (h2.getRank() > 2 || temp[3].getRank() < 14)) || (h1.getRank() == 14 && temp[0].getRank() <= 5);
			}
		}

		return result;
	}

	static bool riverTwoCardLowStr(Card &h1, Card &h2, vector<Card> &board)
	{
		bool result = false;

		for (int i = 0; i < board.size(); ++i)
		{
			for (int j = i+1; j < board.size(); ++j)
			{
				vector<Card> temp = board;
				temp.erase(temp.begin() + i);
				temp.erase(temp.begin() + j - 1);
				if (flopStraight(h1,h2,temp[0],temp[1],temp[2]))
					if (flopStraightTwoUnderCards(h1,h2,temp[0],temp[1],temp[2]))
						result = true;
			}
		}

		return result;
	}

	static bool riverOneCardNutStraight(Card &h1, Card &h2, vector<Card> &board)
	{
		if (!riverOneCardStr(h1,h1,board))
			return false;

		int maxstr = 0;
		bool result;

		for (int i = 0; i < board.size(); ++i)
		{
			vector<Card> temp = board;
			temp.erase(temp.begin() + i);
			if (flopStraight(h1,temp[0],temp[1],temp[2],temp[3]) && highestStrCard(h1,temp[0],temp[1],temp[2],temp[3]) > maxstr)
			{
				maxstr = highestStrCard(h1,temp[0],temp[1],temp[2],temp[3]);
				result = turnOneCardNutStraight(h1,h2,temp);
			}
			if (flopStraight(h2,temp[0],temp[1],temp[2],temp[3]) && highestStrCard(h2,temp[0],temp[1],temp[2],temp[3]) > maxstr)
			{
				maxstr = highestStrCard(h2,temp[0],temp[1],temp[2],temp[3]);
				result = turnOneCardNutStraight(h1,h2,temp);
			}
		}

		return result;
	}

	static int riverPossibleStraights(vector<Card> &board)
	{
		Card c1, c2;
		c1.setSuit('a');
		c2.setSuit('a');
		int result = 0;
		for (int i = 2; i <= 14; ++i)
			for (int j = i+1; j <= 14; ++j)
			{
				c1.setRank(i);
				c2.setRank(j);
				vector<Card> temp = board;
				temp.push_back(c1); temp.push_back(c2);
				if (riverStraight(temp))
					result++;
			}

		return result;
	}

	static bool riverFuckedUpBoard(vector<Card> &board)
	{
		return riverOneCardStrBoard(board) || riverSuitedNumber(board[0], board[1], board[2], board[3], board[4]) == 4;
	}

	static bool riverVeryDangerousBoard(vector<Card> &board)
	{
		return riverSuitedNumber(board[0], board[1], board[2], board[3], board[4]) == 3 && riverPossibleStraights(board) >= 2;
	}

	static bool riverDangerousBoard(vector<Card> &board)
	{
		return riverSuitedNumber(board[0], board[1], board[2], board[3], board[4]) == 3 || riverPossibleStraights(board) >= 2;
	}

public:
	// Flop
	static int cardStrength(Card h1, Card h2, Card b1, Card b2, Card b3)
	{
		 convertRankToNumbers(h1);
		 convertRankToNumbers(h2);
		 convertRankToNumbers(b1);
		 convertRankToNumbers(b2);
		 convertRankToNumbers(b3);
		 vector<Card> cards;
		 cards.push_back(h1);
		 cards.push_back(h2);
		 cards.push_back(b1);
		 cards.push_back(b2);
		 cards.push_back(b3);

		 vector<Card> board;
		 vector<Card> pocket;
		 board.push_back(b1);board.push_back(b2);board.push_back(b3);
		 pocket.push_back(h1);pocket.push_back(h2);
		 sort(board.begin(),board.end(),compareCards);
		 sort(pocket.begin(),pocket.end(),compareCards);

		 if (flopStraightFlush(cards))
			 return 0;
		 if (flopPoker(cards))
			 return 0;
		 if (flopFullHouse(cards))
		 {
			 // real full house is always very strong on flop
			 if (!flopTripleBoard(b1,b2,b3))
				 return 0;

			 // AA and trips on board is also very strong
			 if ((h1.getRank() == 14) && (h2.getRank() == 14))
				 return 0;

			 // JJ-KK
			 if ((h1.getRank() >= 11) && (h2.getRank() >= 11))
				 return 1;

			 // 88-TT
			 if ((h1.getRank() > 8) && (h2.getRank() > 8))
				 return 2;

			 return 3;
		 }
		 if (flopFlush(cards))
			return 0;
		 if (flopStraight(cards))
		 {
			 // wow, this is a monster hand, straight with nut flush draw
			 if (flopNutFlushDraw(h1,h2,b1,b2,b3))
				return 0;

			 if (!flopMonotoneBoard(b1,b2,b3))
			 {
				 if (flopTwoUnderCards(h1,h2,b1,b2,b3))
					return 1;

				 return 0;
			 }

			 return 1;
		 }
		 if (flopThreeofaKind(cards))
		 {
			 if (flopSingleBoard(b1,b2,b3))
				 return 0;

			 if (flopDoubleBoard(b1,b2,b3))
			 {
				 Card doubledCard = (b1.getRank() == b2.getRank()) ? b1 : b3;
				 Card pocketNotDoubledCard = (h1.getRank() == doubledCard.getRank()) ? h2 : h1;
				 if (doubledCard.getRank() < 14)
				 {
					 if (pocketNotDoubledCard.getRank() == 14)
						 return 0;
					 if (pocketNotDoubledCard.getRank() >= 10)
						 return 1;
					 return 2;
				 }

				 if (pocketNotDoubledCard.getRank() == 13)
					 return 0;
				 if (pocketNotDoubledCard.getRank() >= 10)
					 return 1;
				 return 2;
			 }

			 if ((h1.getRank() == 14) || (h2.getRank() == 14))
				 return 3;

			 return 4;
		 }
		 if (flopTwoPair(cards))
		 {
			 // note that dangerous boards on flop never have two same ranked cards
			 if (flopDangerousBoard(b1,b2,b3))
			 {
				 // top2 pair
				 if ((pocket[0].getRank() == board[1].getRank()) && (pocket[1].getRank() == board[2].getRank()))
					 return 1;
				 return 2;
			 }

			 if (flopSingleBoard(b1,b2,b3))
			 {
				 // top2 pair
				 if ((pocket[0].getRank() == board[1].getRank()) && (pocket[1].getRank() == board[2].getRank()))
					 return 0;
				 return 1;
			 }

			 if (h1.getRank() == h2.getRank())
			 {
				 if (flopTwoOverCards(h1,h2,b1,b2,b3))
				 {
					 if (h1.getRank() >= 12)
						 return 1;
					 if (h1.getRank() >= 8)
						 return 2;
					 return 3;
				 }
				 return 3;
			 }

			 Card pair = ((h1.getRank() == board[0].getRank()) || (h1.getRank() == board[2].getRank())) ? h1 : h2;
			 Card kicker = ((h1.getRank() == board[0].getRank()) || (h1.getRank() == board[2].getRank())) ? h2 : h1;
			 if (pair.getRank() >= 10)
			 {
				 if ((kicker.getRank() == 14) || (pair.getRank() == 14 && kicker.getRank() == 13))
					 return 1;
				 if (kicker.getRank() >= 11)
					 return 2;
				 return 3;
			 }

			 if (kicker.getRank() >= 11)
				 return 2;
			 return 3;
		 }
		 if (flopOnePair(cards))
		 {
			 if (flopNutFlushDraw(h1,h2,b1,b2,b3) || (flopNutOESD(h1,h2,b1,b2,b3) && !flopMonotoneBoard(b1,b2,b3)))
			 {
				 if (h1.getRank() == h2.getRank())
				 {
					 if (flopTwoOverCards(h1,h2,b1,b2,b3))
						return 1;
					 return 2;
				 }

				 if ((h1.getRank() == board[2].getRank()))
				 {
					 if (h2.getRank() >= 11)
						 return 1;
					 return 2;
				 }

				 if ((h2.getRank() == board[2].getRank()))
				 {
					 if (h1.getRank() >= 11)
						 return 1;
					 return 2;
				 }

				 if ((h1.getRank() == board[1].getRank()) || (h2.getRank() == board[1].getRank()))
					 return 2;

				 return 6;
			 }

			 if (flopDangerousBoard(b1,b2,b3))
			 {
				 if (h1.getRank() == h2.getRank())
				 {
					 if (flopTwoOverCards(h1,h2,b1,b2,b3))
						return 2;
					 return 3;
				 }

				 if ((h1.getRank() == board[2].getRank()))
				 {
					 if (h2.getRank() >= 11)
						 return 2;
					 return 3;
				 }

				 if ((h2.getRank() == board[2].getRank()))
				 {
					 if (h1.getRank() >= 11)
						 return 2;
					 return 3;
				 }

				 if ((h1.getRank() == board[1].getRank()) || (h2.getRank() == board[1].getRank()))
					 return 3;

				 if (flopGutshot(cards))
					 return 5;
				 return 4;
			 }

			 if (h1.getRank() == h2.getRank())
			 {
				 if (flopTwoOverCards(h1,h2,b1,b2,b3))
				 {
					 if (h1.getRank() >= 11)
						return 1;
					 return 2;
				 }
				 return 3;
			 }

			 if ((h1.getRank() == board[2].getRank()))
			 {
				 if ((h2.getRank() == 14) || ((h1.getRank() == 14) && (h2.getRank() == 13)))
					 return 1;
				 if (h2.getRank() >= 11)
					 return 2;
				 return 3;
			 }

			 if ((h2.getRank() == board[2].getRank()))
			 {
				 if ((h1.getRank() == 14) || ((h2.getRank() == 14) && (h1.getRank() == 13)))
					 return 1;
				 if (h1.getRank() >= 11)
					 return 2;
				 return 3;
			 }

			 if ((h1.getRank() == board[1].getRank()) || (h2.getRank() == board[1].getRank()))
				 return 3;

			 if (flopGutshot(cards))
				 return 5;
			 return 4;
		 }

		 if (((flopFlushDraw(h1,h2,b1,b2,b3) && !flopMonotoneBoard(b1,b2,b3)) || (flopNutFlushDraw(h1,h2,b1,b2,b3))) && (flopOESD(cards) || flopGutshot(cards)))
			 return 7;
		 if ((flopFlushDraw(h1,h2,b1,b2,b3) && !flopMonotoneBoard(b1,b2,b3)) || flopNutFlushDraw(h1,h2,b1,b2,b3) || (!flopMonotoneBoard(b1,b2,b3) && flopNutOESD(h1,h2,b1,b2,b3)))
			 return 6;
		 if ((!flopMonotoneBoard(b1,b2,b3) && (flopGutshot(cards) || flopOESD(cards))))
			 return 5;

		 return 4;
	}

	// Turn
	static int cardStrength(Card h1, Card h2, Card b1, Card b2, Card b3, Card b4)
	{
		convertRankToNumbers(h1);
		convertRankToNumbers(h2);
		convertRankToNumbers(b1);
		convertRankToNumbers(b2);
		convertRankToNumbers(b3);
		convertRankToNumbers(b4);
		vector<Card> cards;
		cards.push_back(h1);
		cards.push_back(h2);
		cards.push_back(b1);
		cards.push_back(b2);
		cards.push_back(b3);
		cards.push_back(b4);

		vector<Card> board;
		vector<Card> pocket;
		board.push_back(b1);board.push_back(b2);board.push_back(b3);board.push_back(b4);
		pocket.push_back(h1);pocket.push_back(h2);
		sort(board.begin(),board.end(),compareCards);
		sort(pocket.begin(),pocket.end(),compareCards);

		if (turnStraightFlush(cards))
		{
			if (flopStraightFlush(h1,b1,b2,b3,b4))
			{
				if (h1.getRank() > b1.getRank() || h1.getRank() > b2.getRank() || h1.getRank() > b3.getRank() || h1.getRank() > b4.getRank() || (h1.getRank() == 10 && board[0].getRank() > 10 ) || (h1.getRank() == 2 && board[3].getRank() == 14))
					return 0;

				if (flopStraightFlush(h2,b1,b2,b3,b4))
					return 0;

				return 1;
			}

			if (flopStraightFlush(h2,b1,b2,b3,b4))
			{
				return (h2.getRank() > b1.getRank() || h2.getRank() > b2.getRank() || h2.getRank() > b3.getRank() || h2.getRank() > b4.getRank() || (h2.getRank() == 10 && board[0].getRank() > 10 ) || (h2.getRank() == 2 && board[3].getRank() == 14)) ? 0 : 1;
			}

			return 0;
		}
		else if (turnPoker(cards))
		{
			if ((b1.getRank() == b2.getRank()) && (b2.getRank() == b3.getRank()) && (b3.getRank() == b4.getRank()))
			{
				if ((h1.getRank() == 14) || (h2.getRank() == 14) || ((b1.getRank() == 14) && ((h1.getRank() == 13) || (h2.getRank() == 13))))
					return 0;
				if ((h1.getRank() == 13) || (h2.getRank() == 13) || ((b1.getRank() == 14) && ((h1.getRank() == 12) || (h2.getRank() == 12))))
					return 3;
				return 4;
			}

			return 0;
		}
		else if (turnFullHouse(cards))
		{
			if (!turnDoublePairedBoard(board) && !turnTripleBoard(board))
				return 0;

			if (turnDoublePairedBoard(board))
			{
				if ((board[3].getRank() == h1.getRank()) || (board[3].getRank() == h2.getRank()))
					return 0;
				return 2;
			}

			Card standAloneCard = (board[0].getRank() == board[1].getRank()) ? board[3] : board[0];
			Card trips = (board[0].getRank() == board[1].getRank()) ? board[0] : board[3];

			if (h1.getRank() == h2.getRank())
			{
				if (h1.getRank() == standAloneCard.getRank() && h1.getRank() > trips.getRank())
					return 0;
				if (h1.getRank() > standAloneCard.getRank())
				{
					if (h1.getRank() >= 11)
						return 1;
					if (h1.getRank() >= 8)
						return 2;
					return 3;
				}
				return 3;
			}

			if (standAloneCard.getRank() >= 12)
				return 1;
			if (standAloneCard.getRank() >= 8)
				return 2;
			return 3;
		}
		else if (turnFlush(cards))
		{
			if (!turnMonotoneBoard(board) && !turnDoubleBoard(board))
				return 0;

			if (!turnMonotoneBoard(board) && turnDoubleBoard(board))
				return 1;

			Card suitedCard = (h1.getSuit() == b1.getSuit() && (h2.getSuit() != b1.getSuit() || h2.getRank() < h1.getRank())) ? h1 : h2;

			if (turnOneCardNutFlush(h1,h2,board))
			{
				if (turnOneCardStrBoard(board))
					return 1;
				return 0;
			}

			if (suitedCard.getRank() >= 11)
				return 3;

			return 4;
		}
		else if (turnStraight(cards))
		{
			if (turnMonotoneBoard(board))
				return 4;

			if (turnOneCardStr(h1,h2,board))
			{
				if (turnOneCardLowStr(h1, h2, board))
					return 3;
				if (!turnDoubleBoard(board))
				{
					if (!turnThreeToneBoard(board))
					{
						if (turnOneCardNutStraight(h1,h2,board))
							return 0;
						return 1;
					}

					if (turnOneCardNutFlushDraw(h1,h2,board))
						return 1;

					return 2;
				}

				return 2;
			}

			if (turnLowTwoCardStraight(h1,h2,board))
			{
				if (turnDoubleBoard(board))
					return 2;

				if (!turnThreeToneBoard(board))
					return 1;

				if (turnOneCardNutFlushDraw(h1,h2,board))
					return 1;

				return 2;
			}

			if (turnDoubleBoard(board))
			{
				if (turnThreeToneBoard(board))
					return 2;
				return 1;
			}

			if (!turnThreeToneBoard(board))
				return 0;

			if (turnOneCardNutFlushDraw(h1,h2,board))
				return 0;

			return 1;
		}
		else if (turnThreeofaKind(cards))
		{
			if (turnVeryDangerousBoard(board))
				return 3;
			// szet
			if (turnSingleBoard(board))
			{
				if (turnOneCardNutFlushDraw(h1,h2,board))
				{
					return 0;
				}
				if (turnDangerousBoard(b1,b2,b3,b4))
				{
					return 1;
				}
				return 0;
			}
			if (turnDoubleBoard(board))
			{
				Card kicker = h2;
				if (existsOnBoard(h2.getRank(),board))
					kicker = h1;

				if (kicker.getRank() == 14)
				{
					if (turnOneCardNutFlushDraw(h1,h2,board))
					{
						return 0;
					}
					if (turnDangerousBoard(b1,b2,b3,b4))
					{
						return 1;
					}
					return 0;
				}
				if (kicker.getRank() >= 10)
				{
					return 1;
				}
				if (turnDangerousBoard(b1,b2,b3,b4))
				{
					return 2;
				}
				return 1;
			}
			return 4;
		}
		else if (turnTwoPair(cards))
		{
			if (turnDoublePairedBoard(board))
			{
				if (h1.getRank() == h2.getRank() && h1.getRank() > board[0].getRank())
					return 3;
				if ((h1.getRank() == 14) || (h2.getRank() == 14))
					return 3;
				return 4;
			}

			if (turnSingleBoard(board))
			{
				if ((h1.getRank() == board[3].getRank()) || (h2.getRank() == board[3].getRank()))
				{
					if (turnVeryDangerousBoard(board))
						return 3;

					if (turnStrongFlushDraw(h1,h2,board))
						return 0;

					if (turnDangerousBoard(b1,b2,b3,b4))
						return 1;

					return 0;
				}

				if (turnVeryDangerousBoard(board))
					return 3;

				if (turnStrongFlushDraw(h1,h2,board))
					return 0;

				if (turnDangerousBoard(b1,b2,b3,b4))
					return 2;

				return 1;
			}

			if (turnDoubleBoard(board))
			{
				if (h1.getRank() == h2.getRank())
				{
					if (h1.getRank() > board[3].getRank())
					{
						if (h1.getRank() >= 12)
						{
							if (turnOneCardNutFlushDraw(h1,h2,board))
								return 2;
							if (turnDangerousBoard(b1,b2,b3,b4))
								return 2;
							return 1;
						}
						if (h1.getRank() >= 8)
						{
							if (turnOneCardNutFlushDraw(h1,h2,board))
								return 2;
							if (turnDangerousBoard(b1,b2,b3,b4))
								return 3;
							return 2;
						}
						return 3;
					}
					return 3;
				}

				Card kicker = h1;
				Card pair = h2;
				if (existsOnBoard(h1.getRank(),board) && (!(existsOnBoard(h2.getRank(), board) || (h1.getRank() > h2.getRank()))))
				{
					kicker = h2;
					pair = h1;
				}

				if ((pair.getRank() == board[3].getRank()) || ((board[3].getRank() == board[2].getRank()) && (pair.getRank() == board[1].getRank())))
				{
					if (pair.getRank() >= 10)
					{
						if ((kicker.getRank() == 14) || ((pair.getRank() == 14) && (kicker.getRank() == 13)))
						{
							if (turnStrongFlushDraw(h1,h2,board))
								return 1;
							if (turnDangerousBoard(b1,b2,b3,b4))
								return 2;
							return 1;
						}
						if (kicker.getRank() >= 10)
							return 2;
						return 3;
					}
					if (kicker.getRank() >= 10)
					{
						if (turnStrongFlushDraw(h1,h2,board))
							return 2;
						if (turnDangerousBoard(b1,b2,b3,b4))
							return 3;
						return 2;
					}
				}

				return 3;
			}

			cout << "Error in cardStrength (turn)  : script is supposed to terminate earlier" << endl;
		}
		else if (turnOnePair(cards))
		{
			if (turnDoubleBoard(board))
			{
				if (turnStrongFlushDraw(h1,h2,board) && (turnGutShot(h1,h2,b1,b2,b3,b4) || turnOESD(h1,h2,b1,b2,b3,b4)))
					return 7;
				if (turnStrongFlushDraw(h1,h2,board) || (turnStrongOESD(h1,h2,b1,b2,b3,b4) && turnSuitedNumber(b1,b2,b3,b4) < 3))
					return 6;
				if (turnGutShot(h1,h2,b1,b2,b3,b4) || turnOESD(h1,h2,b1,b2,b3,b4))
					return 5;

				return 4;
			}

			if (turnVeryDangerousBoard(board))
				return 4;

			if (h1.getRank() == h2.getRank())
			{
				if (h1.getRank() > board[3].getRank())
				{
					if (h1.getRank() >= 11)
					{
						if (turnOneCardNutFlushDraw(h1,h2,board))
							return 1;
						if (turnDangerousBoard(b1,b2,b3,b4))
							return 2;
						return 1;
					}
					return 2;
				}
				if (h1.getRank() > board[2].getRank())
					return 3;
				return 4;
			}

			Card pair = h2;
			Card kicker = h1;
			if (existsOnBoard(h1.getRank(),board))
			{
				kicker = h2;
				pair = h1;
			}

			if (pair.getRank() == board[3].getRank())
			{
				if ((kicker.getRank() == 14) || (pair.getRank() == 14 && kicker.getRank() == 13))
				{
					if (turnStrongFlushDraw(h1,h2,board) || (turnStrongOESD(h1,h2,b1,b2,b3,b4) && turnSuitedNumber(b1,b2,b3,b4) < 3))
						return 1;
					if (turnDangerousBoard(b1,b2,b3,b4))
						return 2;
					return 1;
				}
				if (kicker.getRank() >= 10)
				{
					if (turnStrongFlushDraw(h1,h2,board) || (turnStrongOESD(h1,h2,b1,b2,b3,b4) && turnSuitedNumber(b1,b2,b3,b4) < 3))
						return 1;
					return 2;
				}
				if (turnStrongFlushDraw(h1,h2,board) || (turnStrongOESD(h1,h2,b1,b2,b3,b4) && turnSuitedNumber(b1,b2,b3,b4) < 3))
					return 2;
				return 3;
			}

			if (pair.getRank() == board[2].getRank())
			{
				if (turnStrongFlushDraw(h1,h2,board) || (turnStrongOESD(h1,h2,b1,b2,b3,b4) && turnSuitedNumber(b1,b2,b3,b4) < 3))
					return 2;
				return 3;
			}

			if (turnStrongFlushDraw(h1,h2,board) && (turnGutShot(h1,h2,b1,b2,b3,b4) || turnOESD(h1,h2,b1,b2,b3,b4)))
				return 7;
			if (turnStrongFlushDraw(h1,h2,board) || (turnStrongOESD(h1,h2,b1,b2,b3,b4) && turnSuitedNumber(b1,b2,b3,b4) < 3))
				return 6;
			if (turnGutShot(h1,h2,b1,b2,b3,b4) || turnOESD(h1,h2,b1,b2,b3,b4))
				return 5;

			return 4;
		}
		else
		{
			if (turnStrongFlushDraw(h1,h2,board) && (turnGutShot(h1,h2,b1,b2,b3,b4) || turnOESD(h1,h2,b1,b2,b3,b4)))
				return 7;
			if (turnStrongFlushDraw(h1,h2,board) || (turnStrongOESD(h1,h2,b1,b2,b3,b4) && turnSuitedNumber(b1,b2,b3,b4) < 3))
				return 6;
			if (turnGutShot(h1,h2,b1,b2,b3,b4) || turnOESD(h1,h2,b1,b2,b3,b4))
				return 5;

			return 4;
		}

		return 4;
	}

	// River
	static int cardStrength(Card h1, Card h2, Card b1, Card b2, Card b3, Card b4, Card b5)
	{
		convertRankToNumbers(h1);
		convertRankToNumbers(h2);
		convertRankToNumbers(b1);
		convertRankToNumbers(b2);
		convertRankToNumbers(b3);
		convertRankToNumbers(b4);
		convertRankToNumbers(b5);
		vector<Card> cards;
		cards.push_back(h1);
		cards.push_back(h2);
		cards.push_back(b1);
		cards.push_back(b2);
		cards.push_back(b3);
		cards.push_back(b4);
		cards.push_back(b5);

		vector<Card> board;
		vector<Card> pocket;
		board.push_back(b1);board.push_back(b2);board.push_back(b3);board.push_back(b4);board.push_back(b5);
		pocket.push_back(h1);pocket.push_back(h2);
		sort(board.begin(),board.end(),compareCards);
		sort(pocket.begin(),pocket.end(),compareCards);

		if (riverStraightFlush(cards))
		{
			if (flopStraightFlush(b1,b2,b3,b4,b5))
			{
				if (flopStraightFlush(h1,h2,board[2],board[3],board[4]))
					return 0;
				if (flopStraightFlush(h1,board[1],board[2],board[3],board[4]) || flopStraightFlush(h2,board[1],board[2],board[3],board[4]))
					return 0;
				if ((board[4].getRank() == 14) && (board[3].getRank() == 13))
					return 8;
				return 4;
			}
			if (turnStraightFlush(createVector(h1,b1,b2,b3,b4,b5)) || turnStraightFlush(createVector(h2,b1,b2,b3,b4,b5)))
			{
				if (riverOneCardLowStrFlush(h1,h2,b1,b2,b3,b4,b5))
					return 1;

				return 0;
			}
			return 0;
		}
		else if (riverPoker(cards))
		{
			if (flopPoker(b1,b2,b3,b4,b5))
			{
				if ((board[4].getRank() == 14 && board[3].getRank() < 14) || (board[4].getRank() == 14 && board[3].getRank() == 14 && board[0].getRank() == 13))
					return 8;
				if (h1.getRank() == 14 || h2.getRank() == 14 || (board[4].getRank() == 14 && board[3].getRank() == 14 && (h1.getRank() == 13 || h2.getRank() == 13)))
					return 0;
				if (((h1.getRank() == 13 || h2.getRank() == 13) && (board[4].getRank() < 13)) || (board[4].getRank() == 14 && board[3].getRank() == 14 && (h1.getRank() == 12 || h2.getRank() == 12) && (board[0].getRank() < 12)))
					return 3;
				return 4;
			}
			return 0;
		}
		else if (riverFullHouse(cards))
		{
			if (flopFullHouse(b1,b2,b3,b4,b5))
			{
				Card pair = board[0];
				Card trips = board[4];
				if (board[1].getRank() == board[2].getRank())
				{
					pair = board[4];
					trips = board[0];
				}

				if ((pair.getRank() > trips.getRank()) && (h1.getRank() == pair.getRank() || h2.getRank() == pair.getRank()))
					return 0;
				if ((h1.getRank() == h2.getRank()) && (h1.getRank() > pair.getRank()))
				{
					if (h1.getRank() >= 12)
						return 1;
					if (h1.getRank() >= 88)
						return 2;
					return 3;
				}

				return 4;
			}

			if (!riverDoublePairedBoard(board) && !riverTripleBoard(board))
				return 0;

			if (riverDoublePairedBoard(board))
			{
				Card bigpair = board[4];
				Card lowpair = board[2];
				Card standAloneCard = board[0];
				if (board[4].getRank() != board[3].getRank())
				{
					bigpair = board[3];
					lowpair = board[1];
					standAloneCard = board[4];
				}
				if ((board[0].getRank() == board[1].getRank()) && (board[4].getRank() == board[3].getRank()))
				{
					bigpair = board[4];
					lowpair = board[0];
					standAloneCard = board[2];
				}

				if ((bigpair.getRank() == h1.getRank()) || (bigpair.getRank() == h2.getRank()))
					return 0;
				if ((lowpair.getRank() == h1.getRank()) || (lowpair.getRank() == h2.getRank()))
					return 2;

				if ((h1.getRank() == h2.getRank()) && (h1.getRank() == standAloneCard.getRank()))
				{
					if (h1.getRank() > bigpair.getRank())
						return 0;
					if (h1.getRank() > lowpair.getRank())
						return 2;
					return 3;
				}

				cout << "Error in cardStrength(river, full house) : script is supposed to terminate earlier" << endl;
			}

			Card highStandAloneCard = board[0];
			Card lowStandAloneCard = board[1];
			Card trips = board[2];
			if (board[0].getRank() == board[1].getRank())
			{
				highStandAloneCard = board[3];
				lowStandAloneCard = board[4];
			}
			if ((board[1].getRank() == board[2].getRank()) && (board[2].getRank() == board[3].getRank()))
			{
				highStandAloneCard = board[0];
				lowStandAloneCard = board[4];
			}

			if (highStandAloneCard.getRank() > lowStandAloneCard.getRank())
			{
				Card temp = highStandAloneCard;
				highStandAloneCard = lowStandAloneCard;
				lowStandAloneCard = temp;
			}

			if (h1.getRank() == h2.getRank())
			{
				if ((h1.getRank() == highStandAloneCard.getRank() || h1.getRank() == lowStandAloneCard.getRank()) && (h1.getRank() > trips.getRank()))
					return 0;
				if (h1.getRank() > highStandAloneCard.getRank())
				{
					if (h1.getRank() >= 11)
						return 1;
					if (h1.getRank() >= 8)
						return 2;
					return 3;
				}
				return 3;
			}

			if (h1.getRank() == highStandAloneCard.getRank() || h2.getRank() == highStandAloneCard.getRank())
			{
				if (highStandAloneCard.getRank() >= 12)
					return 1;
				if (highStandAloneCard.getRank() >= 8)
					return 2;
				return 3;
			}
			return 3;
		}
		else if (riverFlush(cards))
		{
			if (flopFlush(b1,b2,b3,b4,b5))
			{
				if (riverOneCardNutFlush(h1,h2,board))
				{
					if (riverPossibleOneCardStrFlush(board))
						return 1;
					return 0;
				}
				if ((h1.getSuit() == b1.getSuit()) || (h2.getSuit() == b2.getSuit()))
				{
					Card suitedCard = (h1.getSuit() == b1.getSuit() && (h2.getSuit() != b1.getSuit() || h2.getRank() < h1.getRank())) ? h1 : h2;
					if (suitedCard.getRank() >= 11)
						return 3;
					return 4;
				}

				if ((board[4].getRank() == 14) && (!riverPossibleOneCardStrFlush(board)))
					return 8;
				return 4;
			}

			if (riverTripleBoard(board))
				return 4;

			if (riverDoublePairedBoard(board))
				return 3;

			if (riverSuitedNumber(b1,b2,b3,b4,b5) == 3 && !riverDoubleBoard(board))
				return 0;

			if (riverSuitedNumber(b1,b2,b3,b4,b5) == 3 && riverDoubleBoard(board))
				return 1;

			char suit = board[1].getSuit();
			if ((board[1].getSuit() != board[2].getSuit()) && (board[2].getSuit() == board[3].getSuit()))
				suit = board[2].getSuit();
			Card suitedCard = (h1.getSuit() == suit && (h2.getSuit() != suit || h2.getRank() < h1.getRank())) ? h1 : h2;

			if (riverOneCardNutFlush(h1,h2,board))
			{
				if (riverDoubleBoard(board))
					return 1;
				if (riverPossibleOneCardStrFlush(board))
					return 1;
				return 0;
			}

			if (suitedCard.getRank() >= 11)
				return 3;
			return 4;
		}
		else if (riverStraight(cards))
		{
			if (riverSuitedNumber(b1,b2,b3,b4,b5) == 4)
				return 4;

			if (flopStraight(b1,b2,b3,b4,b5))
			{
				if ((h1.getRank() == board[4].getRank() + 1) || (h2.getRank() == board[4].getRank() + 1))
				{
					Card str = (h1.getRank() == board[4].getRank() + 1) ? h1 : h2;
					Card other = (h1.getRank() == board[4].getRank() + 1) ? h2 : h1;

					if (other.getRank() == str.getRank() + 1)
					{
						if (riverSuitedNumber(b1,b2,b3,b4,b5) == 3)
							return 1;
						return 0;
					}

					if (riverSuitedNumber(b1,b2,b3,b4,b5) == 3)
						return 2;
					if (str.getRank() == 14)
						return 0;
					return 1;
				}

				if ((board[0].getRank() == 10) && (riverSuitedNumber(b1,b2,b3,b4,b5) < 3))
					return 8;
				return 4;
			}

			if (riverTripleBoard(board))
				return 4;
			if (riverDoublePairedBoard(board))
				return 3;

			if (riverOneCardStr(h1,h2,board))
			{
				if (riverOneCardLowStr(h1, h2, board))
					return 3;
				if (!riverDoubleBoard(board))
				{
					if (riverSuitedNumber(board[0], board[1], board[2], board[3], board[4]) < 3)
					{
						if (riverOneCardNutStraight(h1,h2,board))
							return 0;
						return 1;
					}

					if (riverOneCardNutStraight(h1,h2,board))
							return 1;
					return 2;
				}

				if (riverSuitedNumber(board[0], board[1], board[2], board[3], board[4]) == 3)
					return 3;
				return 2;
			}

			if (riverTwoCardLowStr(h1,h2,board))
			{
				if (riverDoubleBoard(board))
					return 2;

				if (riverSuitedNumber(board[0], board[1], board[2], board[3], board[4]) < 3)
					return 1;

				return 2;
			}

			if (riverDoubleBoard(board))
			{
				if (riverSuitedNumber(board[0], board[1], board[2], board[3], board[4]) == 3)
					return 2;
				return 1;
			}

			if (riverSuitedNumber(board[0], board[1], board[2], board[3], board[4]) < 3)
				return 0;

			return 1;
		}
		else if (riverThreeofaKind(cards))
		{
			if (riverTripleBoard(board))
				return 4;

			if (h1.getRank() == h2.getRank())
			{
				if (riverFuckedUpBoard(board))
					return 3;
				if (riverVeryDangerousBoard(board))
					return 2;
				if (riverDangerousBoard(board))
					return 1;
				return 0;
			}

			Card kicker = h1;
			Card drill = h2;
			if (existsOnBoard(h1.getRank(),board))
			{
				kicker = h2;
				drill = h1;
			}

			if (kicker.getRank() == 14 || (kicker.getRank() == 13 && drill.getRank() == 14))
			{
				if (riverFuckedUpBoard(board))
					return 3;
				if (riverVeryDangerousBoard(board))
					return 2;
				if (riverDangerousBoard(board))
					return 1;
				return 0;
			}

			if (riverFuckedUpBoard(board))
				return 4;
			if (riverVeryDangerousBoard(board))
				return 3;
			if (riverDangerousBoard(board))
				return 2;
			return 1;
		}
		else if (riverTwoPair(cards))
		{
			if (riverDoublePairedBoard(board))
			{
				Card lowpair = board[0];
				if (board[0].getRank() != board[1].getRank())
					lowpair = board[1];

				if ((h1.getRank() == h2.getRank() && h1.getRank() > lowpair.getRank()) || (existsOnBoard(h1.getRank(),board) && h1.getRank() > lowpair.getRank()) || (existsOnBoard(h2.getRank(),board) && h2.getRank() > lowpair.getRank()))
					return 3;
				return 4;
			}

			if (riverFuckedUpBoard(board))
				return 4;

			if (!riverDoubleBoard(board))
			{
				if ((h1.getRank() == board[4].getRank()) || (h2.getRank() == board[4].getRank()))
				{
					if (riverVeryDangerousBoard(board))
						return 2;
					if (riverDangerousBoard(board))
						return 1;
					return 0;
				}

				if (riverVeryDangerousBoard(board))
					return 2;
				if (riverDangerousBoard(board))
					return 2;
				return 1;
			}

			if (h1.getRank() == h2.getRank())
			{
				if (h1.getRank() > board[4].getRank())
				{
					if (h1.getRank() >= 12)
					{
						if (riverVeryDangerousBoard(board))
							return 2;
						if (riverDangerousBoard(board))
							return 2;
						return 1;
					}
					if (h1.getRank() >= 8)
					{
						if (riverVeryDangerousBoard(board))
							return 3;
						if (riverDangerousBoard(board))
							return 3;
						return 2;
					}
					return 3;
				}
				return 3;
			}

			Card kicker = h1;
			Card pair = h2;
			if (existsOnBoard(h1.getRank(),board))
			{
				kicker = h2;
				pair = h1;
			}

			if ((pair.getRank() == board[4].getRank()) || ((board[4].getRank() == board[3].getRank()) && (pair.getRank() == board[2].getRank())))
			{
				if (pair.getRank() >= 10)
				{
					if ((kicker.getRank() == 14) || ((pair.getRank() == 14) && (kicker.getRank() == 13)))
					{
						if (riverVeryDangerousBoard(board))
							return 2;
						if (riverDangerousBoard(board))
							return 2;
						return 1;
					}
					if (kicker.getRank() >= 10)
						return 2;
					return 3;
				}
				if (kicker.getRank() >= 10)
				{
					if (riverVeryDangerousBoard(board))
						return 3;
					if (riverDangerousBoard(board))
						return 3;
					return 2;
				}
			}

			return 3;
		}
		else if (riverOnePair(cards))
		{
			if (riverDoubleBoard(board))
				return 4;

			if (riverFuckedUpBoard(board))
				return 4;

			if (h1.getRank() == h2.getRank())
			{
				if (h1.getRank() > board[4].getRank())
				{
					if (h1.getRank() >= 11)
					{
						if (riverVeryDangerousBoard(board))
							return 2;
						if (riverDangerousBoard(board))
							return 2;
						return 1;
					}
					if (riverVeryDangerousBoard(board))
						return 3;
					return 2;
				}
				if (h1.getRank() > board[3].getRank())
					return 3;
				return 4;
			}

			Card pair = h2;
			Card kicker = h1;
			if (existsOnBoard(h1.getRank(),board) && (!(existsOnBoard(h2.getRank(), board) || (h1.getRank() > h2.getRank()))))
			{
				kicker = h2;
				pair = h1;
			}

			if (pair.getRank() == board[4].getRank())
			{
				if ((kicker.getRank() == 14) || (pair.getRank() == 14 && kicker.getRank() == 13))
				{
					if (riverVeryDangerousBoard(board))
						return 2;
					if (riverDangerousBoard(board))
						return 2;
					return 1;
				}
				if (kicker.getRank() >= 10)
				{
					return 2;
				}

				if (riverVeryDangerousBoard(board))
					return 3;
				if (riverDangerousBoard(board))
					return 3;
				return 2;
			}

			if (pair.getRank() == board[3].getRank())
				return 3;
			return 4;
		}
		else
		{
			return 4;
		}
	}

	static int cardStrength(Card h1, Card h2, vector<Card>& v)
	{
		switch(v.size())
		{
			case 3: return cardStrength(h1, h2, v[0], v[1], v[2]);
			case 4: return cardStrength(h1, h2, v[0], v[1], v[2], v[3]);
			case 5: return cardStrength(h1, h2, v[0], v[1], v[2], v[3], v[4]);
		}

		return -1;
	}

	static int boardType(vector<Card>& board)
	{
		vector<Card> v = board;
		
		for (int i = 0; i < v.size(); ++i)
		{
			convertRankToNumbers(v[i]);
		}

		if (v.size() == 3)
		{
			if (flopDangerousBoard(v[0],v[1],v[2]))
			{
				return 2;
			}
			else if (flopSemiDangerousBoard(v[0],v[1],v[2]))
			{
				return 1;
			}
			else
			{
				return 0;
			}
		}
		else if (v.size() == 4)
		{
			if (turnVeryDangerousBoard(v))
			{
				return 2;
			}
			else if (turnDangerousBoard(v[0],v[1],v[2],v[3]))
			{
				return 1;
			}
			else
			{
				return 0;
			}
		}
		else if (v.size() == 5)
		{
			if (riverFuckedUpBoard(v))
			{
				return 3;
			}
			else if (riverVeryDangerousBoard(v))
			{
				return 2;
			}
			else if (riverDangerousBoard(v))
			{
				return 1;
			}
			else
			{
				return 0;
			}
		}

		return -1;
	}
};