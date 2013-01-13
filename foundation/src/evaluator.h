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

		if ((c.rank >= '2') && (c.rank <= '9'))
		{
			c.rank = c.rank - '0';
		}
		else
		{
			c.rank = map[c.rank];
		}

		return;
	}

	static bool compareCards(Card &c1, Card &c2)
	{
		return c1.rank < c2.rank;
	}

	static bool flopStraightFlush(vector<Card> &cards)
	{
		assert(cards.size() == 5);
		sort(cards.begin(), cards.end(), compareCards);

		return ((((cards[0].rank == cards[1].rank-1) && (cards[1].rank == cards[2].rank-1) && (cards[2].rank == cards[3].rank-1) && (cards[3].rank == cards[4].rank-1)) || 
				((cards[0].rank == 2) && (cards[1].rank == 3) && (cards[2].rank == 4) && (cards[3].rank == 5) && (cards[4].rank == 14))) &&
				(cards[0].suit == cards[1].suit) && (cards[1].suit == cards[2].suit) && (cards[2].suit == cards[3].suit) && (cards[3].suit == cards[4].suit));
	}

	static bool flopPoker(vector<Card> &cards)
	{
		assert(cards.size() == 5);
		sort(cards.begin(), cards.end(), compareCards);

		return (((cards[0].rank == cards[1].rank) && (cards[1].rank == cards[2].rank) && (cards[2].rank == cards[3].rank)) ||
				((cards[1].rank == cards[2].rank) && (cards[2].rank == cards[3].rank) && (cards[3].rank == cards[4].rank)));
	}

	static bool flopFullHouse(vector<Card> &cards)
	{
		assert(cards.size() == 5);
		sort(cards.begin(), cards.end(), compareCards);

		return (((cards[0].rank == cards[1].rank) && (cards[2].rank == cards[3].rank) && (cards[3].rank == cards[4].rank)) ||
				((cards[0].rank == cards[1].rank) && (cards[1].rank == cards[2].rank) && (cards[3].rank == cards[4].rank)));
	}

	static bool flopFlush(vector<Card> &cards)
	{
		assert(cards.size() == 5);
		sort(cards.begin(), cards.end(), compareCards);

		if (flopStraightFlush(cards)) return false;

		return ((cards[0].suit == cards[1].suit) && (cards[1].suit == cards[2].suit) && (cards[2].suit == cards[3].suit) && (cards[3].suit == cards[4].suit));
	}

	static bool flopStraight(vector<Card> &cards)
	{
		assert(cards.size() == 5);
		sort(cards.begin(), cards.end(), compareCards);

		if (flopStraightFlush(cards)) return false;

		return (((cards[0].rank == cards[1].rank-1) && (cards[1].rank == cards[2].rank-1) && (cards[2].rank == cards[3].rank-1) && (cards[3].rank == cards[4].rank-1)) || 
				((cards[0].rank == 2) && (cards[1].rank == 3) && (cards[2].rank == 4) && (cards[3].rank == 5) && (cards[4].rank == 14)));
	}

	static bool flopThreeofaKind(vector<Card> &cards)
	{
		assert(cards.size() == 5);
		sort(cards.begin(), cards.end(), compareCards);

		if (flopPoker(cards)) return false;
		if (flopFullHouse(cards)) return false;

		return (((cards[0].rank == cards[1].rank) && (cards[1].rank == cards[2].rank)) || ((cards[1].rank == cards[2].rank) && (cards[2].rank == cards[3].rank))
				|| ((cards[2].rank == cards[3].rank) && (cards[3].rank == cards[4].rank)));
	}

	static bool flopTwoPair(vector<Card> &cards)
	{
		assert(cards.size() == 5);
		sort(cards.begin(), cards.end(), compareCards);

		if (flopFullHouse(cards)) return false;
		if (flopPoker(cards)) return false;
		if (flopThreeofaKind(cards)) return false;

		return (((cards[0].rank == cards[1].rank) && (cards[2].rank == cards[3].rank)) || ((cards[1].rank == cards[2].rank) && (cards[3].rank == cards[4].rank))
				|| ((cards[0].rank == cards[1].rank) && (cards[3].rank == cards[4].rank)));
	}

	static bool flopOnePair(vector<Card> &cards)
	{
		assert(cards.size() == 5);
		sort(cards.begin(), cards.end(), compareCards);

		if (flopTwoPair(cards)) return false;
		if (flopThreeofaKind(cards)) return false;
		if (flopFullHouse(cards)) return false;
		if (flopPoker(cards)) return false;

		return ((cards[0].rank == cards[1].rank) || (cards[1].rank == cards[2].rank) || (cards[2].rank == cards[3].rank) || (cards[3].rank == cards[4].rank));
	}

	static Card flopHighestCard(Card &b1, Card &b2, Card &b3)
	{
		if ((b1.rank >= b2.rank) && (b1.rank >= b3.rank))
			return b1;

		if ((b2.rank >= b1.rank) && (b2.rank >= b3.rank))
			return b2;

		if ((b3.rank >= b2.rank) && (b3.rank >= b1.rank))
			return b3;

		cout << "Error in flopHighestCard : Script is supposed to terminate earlier." << endl;
	}

	static bool flopFlushDraw(Card &h1, Card &h2, Card &b1, Card &b2, Card &b3)
	{
		Card cards[] = {h1,h2,b1,b2,b3};
		vector<Card> vcards(cards, cards + sizeof(cards) / sizeof(Card));

		if (flopFlush(vcards))
			return false;

		return ((h1.suit == h2.suit) && (h2.suit == b1.suit) && (b1.suit == b2.suit)) ||
			((h1.suit == h2.suit) && (h2.suit == b1.suit) && (b1.suit == b3.suit)) ||
			((h1.suit == h2.suit) && (h2.suit == b2.suit) && (b2.suit == b3.suit)) ||
			((h1.suit == b1.suit) && (b1.suit == b2.suit) && (b2.suit == b3.suit)) ||
			((h2.suit == b1.suit) && (b1.suit == b2.suit) && (b2.suit == b3.suit));
	}

	static bool flopTripleBoard(Card &b1, Card &b2, Card &b3)
	{
		return (b1.rank == b2.rank) && (b2.rank == b3.rank);
	}

	static bool flopDoubleBoard(Card &b1, Card &b2, Card &b3)
	{
		if (flopTripleBoard(b1,b2,b3))
			return false;

		return (b1.rank == b2.rank) || (b1.rank == b3.rank) || (b2.rank == b3.rank);
	}

	static bool flopSingleBoard(Card &b1, Card &b2, Card &b3)
	{
		return !(flopDoubleBoard(b1,b2,b3) || flopTripleBoard(b1,b2,b3));
	}

	static bool flopMonotoneBoard(Card &b1, Card &b2, Card &b3)
	{
		return (b1.suit == b2.suit) && (b2.suit == b3.suit);
	}

	static bool flopTwoToneBoard(Card &b1, Card &b2, Card &b3)
	{
		if (flopMonotoneBoard(b1,b2,b3))
			return false;

		return ((b1.suit == b2.suit) || (b1.suit == b3.suit) || (b2.suit == b3.suit));
	}

	static int flopHiLoDistance(Card &b1, Card &b2, Card &b3)
	{
		int dist1 = abs(b1.rank - b2.rank);
		int dist2 = abs(b1.rank - b3.rank);
		int dist3 = abs(b2.rank - b3.rank);

		return (dist1 > dist2) ? ((dist1 > dist3) ? dist1 : dist3) : ((dist2 > dist3) ? dist2 : dist3);
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

	static bool flopTwoUnderCards(Card &h1, Card &h2, Card &b1, Card &b2, Card &b3)
	{
		return (h1.rank < b1.rank) && (h1.rank < b2.rank) && (h1.rank < b3.rank) &&
				(h2.rank < b1.rank) && (h2.rank < b2.rank) && (h2.rank < b3.rank);
	}

	static bool flopTwoOverCards(Card &h1, Card &h2, Card &b1, Card &b2, Card &b3)
	{
		return (h1.rank > b1.rank) && (h1.rank > b2.rank) && (h1.rank > b3.rank) &&
				(h2.rank > b1.rank) && (h2.rank > b2.rank) && (h2.rank > b3.rank);
	}


	static bool flopNutFlushDraw(Card &h1, Card &h2, Card &b1, Card &b2, Card &b3)
	{
		if (!flopFlushDraw(h1,h2,b1,b2,b3))
			return false;

		if (flopMonotoneBoard(b1,b2,b3))
		{
			if (h1.suit == b1.suit)
			{
				if (flopHighestCard(b1,b2,b3).rank < 14)
					return h1.rank == 14;
				if (flopHighestCard(b1,b2,b3).rank < 13)
					return h1.rank == 13;
				if (flopHighestCard(b1,b2,b3).rank < 12)
					return h1.rank == 12;
				return h1.rank == 11;
			}
			if (h2.suit == b1.suit)
			{
				if (flopHighestCard(b1,b2,b3).rank < 14)
					return h2.rank == 14;
				if (flopHighestCard(b1,b2,b3).rank < 13)
					return h2.rank == 13;
				if (flopHighestCard(b1,b2,b3).rank < 12)
					return h2.rank == 12;
				return h2.rank == 11;
			}
		}
		
		Card highestSuitedCard, secondHighestSuitedCard;
		if (flopHighestCard(b1,b2,b3).suit == h1.suit)
		{
			highestSuitedCard = flopHighestCard(b1,b2,b3);
		}
		else
		{
			if (b1.suit != h1.suit)
			{
				highestSuitedCard = (b2.rank > b3.rank) ? b2 : b3;
				secondHighestSuitedCard = (b2.rank > b3.rank) ? b3 : b2;
			}
			if (b2.suit != h1.suit)
			{
				highestSuitedCard = (b1.rank > b3.rank) ? b1 : b3;
				secondHighestSuitedCard = (b1.rank > b3.rank) ? b3 : b1;
			}
			if (b3.suit != h1.suit)
			{
				highestSuitedCard = (b1.rank > b2.rank) ? b1 : b2;
				secondHighestSuitedCard = (b1.rank > b2.rank) ? b2 : b1;
			}
		}

		if (highestSuitedCard.rank < 14)
			return (h1.rank == 14) || (h2.rank == 14);

		if (secondHighestSuitedCard.rank < 13)
			return (h1.rank == 13) || (h2.rank == 13);

		return (h1.rank == 12) || (h2.rank == 12);
	}

	static bool flopDangerousBoard(Card &b1, Card &b2, Card b3)
	{
		 return flopMonotoneBoard(b1,b2,b3) || flopVeryConnectedBoard(b1,b2,b3) || (flopConnectedBoard(b1,b2,b3) && flopTwoToneBoard(b1,b2,b3));
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
			if (cards[i].rank != cards[i-1].rank)
				uniqueCards.push_back(cards[i]);

		if (uniqueCards.size() < 4)
			return false;

		if ((uniqueCards[3].rank-uniqueCards[0].rank == 3 && uniqueCards[3].rank != 14) || (uniqueCards.size() == 5 && uniqueCards[4].rank-uniqueCards[1].rank == 3 && uniqueCards[4].rank != 14))
			return true;

		return (((cards[0].rank == cards[1].rank-1) && (cards[1].rank == cards[2].rank-1) && (cards[2].rank == cards[3].rank-1) && (cards[3].rank < 14)) || 
				((cards[1].rank == cards[2].rank-1) && (cards[2].rank == cards[3].rank-1) && (cards[3].rank == cards[4].rank-1) && (cards[4].rank < 14)) ||
				((cards[0].rank == cards[1].rank-2) && (cards[1].rank == cards[2].rank-1) && (cards[2].rank == cards[3].rank-1) && (cards[3].rank == cards[4].rank-2)) ||
				((cards[0].rank == 3) && (cards[1].rank == 4) && (cards[2].rank == 5) && (cards[3].rank == 7) && (cards[4].rank == 14)));
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
			if (cards[i].rank != cards[i-1].rank)
				uniqueCards.push_back(cards[i]);

		return (uniqueCards.size() >= 4 && uniqueCards[3].rank - uniqueCards[0].rank <= 4) || (uniqueCards.size() >= 5 && uniqueCards[4].rank - uniqueCards[1].rank <= 4) ||
			(uniqueCards.size() >= 4 && uniqueCards[uniqueCards.size()-1].rank == 14 && uniqueCards[2].rank <= 5);
	}

	static bool flopNutOESD(Card &h1, Card &h2, Card &b1, Card &b2, Card &b3)
	{
		Card placeHolderCard;
		placeHolderCard.rank = 20;
		placeHolderCard.suit = -1;
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
			return ((h2.rank > b1.rank) && (h2.rank > b2.rank) && (h2.rank > b3.rank));
		}
		if (flopOESD(vcards3))
		{
			return ((h1.rank > b1.rank) && (h1.rank > b2.rank) && (h1.rank > b3.rank));
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

		return (board[0].rank == board[1].rank) && (board[1].rank != board[2].rank) && (board[2].rank == board[3].rank);
	}

	static bool turnQuadBoard(vector<Card> &board)
	{
		assert(board.size() == 4);

		return (board[0].rank == board[1].rank) && (board[1].rank == board[2].rank) && (board[2].rank == board[3].rank);
	}

	static bool turnTripleBoard(vector<Card> &board)
	{
		assert(board.size() == 4);

		if (turnQuadBoard(board))
			return false;

		return ((board[0].rank == board[1].rank) && (board[1].rank == board[2].rank)) || ((board[1].rank == board[2].rank) && (board[2].rank == board[3].rank));
	}

	static bool turnDoubleBoard(vector<Card> &board)
	{
		if (turnQuadBoard(board) || turnTripleBoard(board))
			return false;

		return ((board[0].rank == board[1].rank) || (board[1].rank == board[2].rank) || (board[2].rank == board[3].rank));
	}

	static bool turnSingleBoard(vector<Card> &board)
	{
		return !turnQuadBoard(board) && !turnTripleBoard(board) && !turnDoubleBoard(board);
	}

	static bool turnMonotoneBoard(vector<Card> &board)
	{
		return (board[0].suit == board[1].suit) && (board[1].suit == board[2].suit) && (board[2].suit == board[3].suit);
	}

	static bool turnOneCardStrBoard(vector<Card> &board)
	{
		if (!turnSingleBoard(board))
			return false;

		return (board[3].rank - board[0].rank <= 4) || (board[3].rank == 14 && board[2].rank <= 5);
	}

	static bool turnOneCardNutFlush(Card &h1, Card &h2, vector<Card> &board)
	{
		if (!turnMonotoneBoard(board))
			return false;

		Card suitedCard;
		if (h1.suit == board[0].suit)
		{
			suitedCard = h1;
		}
		else if (h2.suit == board[0].suit)
		{
			suitedCard = h2;
		}
		else
		{
			return false;
		}

		return suitedCard.rank == 14 || (board[3].rank == 14 && suitedCard.rank == 13) || (board[3].rank == 14 && board[2].rank == 13 && suitedCard.rank == 12) ||
			(board[3].rank == 14 && board[2].rank == 13 && board[1].rank == 12 && suitedCard.rank == 11);
	}

	static bool turnOneCardStr(Card &h1, Card &h2, vector<Card> &board)
	{
		vector<Card> temp2;
		temp2.push_back(board[1]);temp2.push_back(board[2]);temp2.push_back(board[3]);temp2.push_back(h1);temp2.push_back(h2);

		vector<Card> temp = board;
		temp.push_back(h1);
		if (flopStraight(temp))
		{
			if (flopStraight(temp2) && (h2.rank > board[3].rank))
				return false;
			return true;
		}

		temp = board;
		temp.push_back(h2);
		if (flopStraight(temp))
		{
			if (flopStraight(temp2) && (h1.rank > board[3].rank))
				return false;
			return true;
		}

		return false;
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
			return h1.rank < board[0].rank && h1.rank < board[1].rank && h1.rank < board[2].rank && h1.rank < board[3].rank && 
				h2.rank < board[0].rank && h2.rank < board[1].rank && h2.rank < board[2].rank && h2.rank < board[3].rank;
		if (flopStraight(temp))
			return h1.rank < board[0].rank && h1.rank < board[1].rank && h1.rank < board[2].rank && h1.rank < board[3].rank;

		return h2.rank < board[0].rank && h2.rank < board[1].rank && h2.rank < board[2].rank && h2.rank < board[3].rank; 
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
			if ((h1.rank == 14) && (h2.rank == 6))
			{
				straight = temp2;
			}
			else if ((h1.rank == 6) && (h2.rank == 14))
			{
				straight = temp;
			}
			else
			{
				straight = h1.rank > h2.rank ? temp : temp2;
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

		return straight[4].rank == 14 && straight[3].rank == 13;
	}

	static bool turnOneCardNutFlushDraw(Card &h1, Card &h2, vector<Card> &board)
	{
		if (!turnThreeToneBoard(board))
			return false;

		if (!turnFlushDraw(h1,h2,board[0],board[1],board[2],board[3]))
			return false;

		char suit = board[0].suit == board[1].suit ? board[0].suit : board[2].suit;
		Card suitedCard = h1.suit == suit ? h1 : h2;

		vector<Card> temp;
		for (int i = 0; i < board.size(); ++i)
			if (board[i].suit == suit)
				temp.push_back(board[i]);

		return (suitedCard.rank == 14) || (temp[2].rank == 14 && suitedCard.rank == 13) || (temp[1].rank == 13 && suitedCard.rank == 12) || (temp[1].rank == 12 && suitedCard.rank == 11);
	}

	static bool turnLowTwoCardStraight(Card &h1, Card &h2, vector<Card> &board)
	{
		bool result = false;

		vector<Card> temp1;
		temp1.clear();
		temp1.push_back(h1);temp1.push_back(h2);temp1.push_back(board[0]);temp1.push_back(board[1]);temp1.push_back(board[2]);
		if (flopStraight(temp1))
			if (flopTwoUnderCards(h1,h2,board[0],board[1],board[2]) && (h1.rank < 10 || h2.rank < 10))
				result = true;

		temp1.clear();
		temp1.push_back(h1);temp1.push_back(h2);temp1.push_back(board[0]);temp1.push_back(board[2]);temp1.push_back(board[3]);
		if (flopStraight(temp1))
			if (flopTwoUnderCards(h1,h2,board[0],board[2],board[3]) && (h1.rank < 10 || h2.rank < 10))
				result = true;

		temp1.clear();
		temp1.push_back(h1);temp1.push_back(h2);temp1.push_back(board[1]);temp1.push_back(board[2]);temp1.push_back(board[3]);
		if (flopStraight(temp1))
			if (flopTwoUnderCards(h1,h2,board[1],board[2],board[3]) && (h1.rank < 10 || h2.rank < 10))
				result = true;

		return result;

		cout << "Error in turnLowTwoCardStraight : script is supposed to terminate earlier." << endl;
	}

	static bool turnThreeToneBoard(vector<Card> &board)
	{
		return (board[0].suit == board[1].suit && board[1].suit == board[2].suit) || (board[0].suit == board[2].suit && board[2].suit == board[3].suit) ||
			(board[1].suit == board[2].suit && board[2].suit == board[3].suit) || (board[0].suit == board[1].suit && board[1].suit == board[3].suit);
	}

	static bool turnDangerousBoard(Card &b1, Card &b2, Card &b3, Card &b4)
	{
		return flopDangerousBoard(b1,b2,b3) || flopDangerousBoard(b1,b2,b4) || flopDangerousBoard(b1,b3,b4) || flopDangerousBoard(b2,b3,b4);
	}

	static bool existsOnBoard(int rank, vector<Card> &board)
	{
		for (int i = 0; i < board.size(); ++i)
		{
			if (board[i].rank == rank)
				return true;
		}

		return false;
	}

	static bool turnVeryDangerousBoard(vector<Card> &board)
	{
		return ((board[0].suit == board[1].suit) && (board[1].suit == board[2].suit) && (board[2].suit == board[3].suit)) || (turnOneCardStrBoard(board));
	}

	static bool turnFlushDraw(Card &h1, Card &h2, Card &b1, Card &b2, Card &b3, Card &b4)
	{
		return flopFlushDraw(h1,h2,b1,b2,b3) || flopFlushDraw(h1,h2,b1,b3,b4) || flopFlushDraw(h1,h2,b1,b2,b4) || flopFlushDraw(h1,h2,b2,b3,b4);
	}

	static bool turnTwoCardFlushDraw(Card &h1, Card &h2, Card &b1, Card &b2, Card &b3, Card &b4)
	{
		return turnFlushDraw(h1,h2,b1,b2,b3,b4) && (h1.suit == h2.suit); 
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

		temp[b1.suit]++;temp[b2.suit]++;temp[b3.suit]++;temp[b4.suit]++;

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
			 if ((h1.rank == 14) && (h2.rank == 14))
				 return 0;

			 // JJ-KK 
			 if ((h1.rank >= 11) && (h2.rank >= 11))
				 return 1;

			 // 88-TT
			 if ((h1.rank > 8) && (h2.rank > 8))
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
				 Card doubledCard = (b1.rank == b2.rank) ? b1 : b3;
				 Card pocketNotDoubledCard = (h1.rank == doubledCard.rank) ? h2 : h1;
				 if (doubledCard.rank < 14)
				 {
					 if (pocketNotDoubledCard.rank == 14)
						 return 0;
					 if (pocketNotDoubledCard.rank >= 10)
						 return 1;
					 return 2;
				 }
				 
				 if (pocketNotDoubledCard.rank == 13)
					 return 0;
				 if (pocketNotDoubledCard.rank >= 10)
					 return 1;
				 return 2;
			 }

			 if ((h1.rank == 14) || (h2.rank == 14))
				 return 3;

			 return 4;
		 }
		 if (flopTwoPair(cards))
		 {
			 // note that dangerous boards on flop never have two same ranked cards
			 if (flopDangerousBoard(b1,b2,b3))
			 {
				 // top2 pair
				 if ((pocket[0].rank == board[1].rank) && (pocket[1].rank == board[2].rank))
					 return 1;
				 return 2;
			 }

			 if (flopSingleBoard(b1,b2,b3))
			 {
				 // top2 pair
				 if ((pocket[0].rank == board[1].rank) && (pocket[1].rank == board[2].rank))
					 return 0;
				 return 1;
			 }

			 if (h1.rank == h2.rank)
			 {
				 if (flopTwoOverCards(h1,h2,b1,b2,b3))
				 {
					 if (h1.rank >= 12)
						 return 1;
					 if (h1.rank >= 8)
						 return 2;
					 return 3;
				 }
				 return 3;
			 }

			 Card pair = ((h1.rank == board[0].rank) || (h1.rank == board[2].rank)) ? h1 : h2;
			 Card kicker = ((h1.rank == board[0].rank) || (h1.rank == board[2].rank)) ? h2 : h1;
			 if (pair.rank >= 10)
			 {
				 if ((kicker.rank == 14) || (pair.rank == 14 && kicker.rank == 13))
					 return 1;
				 if (kicker.rank >= 11)
					 return 2;
				 return 3;
			 }

			 if (kicker.rank >= 11)
				 return 2;
			 return 3;
		 }
		 if (flopOnePair(cards))
		 {
			 if (flopNutFlushDraw(h1,h2,b1,b2,b3) || (flopNutOESD(h1,h2,b1,b2,b3) && !flopMonotoneBoard(b1,b2,b3)))
			 {
				 if (h1.rank == h2.rank)
				 {
					 if (flopTwoOverCards(h1,h2,b1,b2,b3))
						return 1;
					 return 2;
				 }

				 if ((h1.rank == board[2].rank))
				 {
					 if (h2.rank >= 11)
						 return 1;
					 return 2;
				 }

				 if ((h2.rank == board[2].rank))
				 {
					 if (h1.rank >= 11)
						 return 1;
					 return 2;
				 }

				 if ((h1.rank == board[1].rank) || (h2.rank == board[1].rank))
					 return 2;

				 return 6;
			 }

			 if (flopDangerousBoard(b1,b2,b3))
			 {
				 if (h1.rank == h2.rank)
				 {
					 if (flopTwoOverCards(h1,h2,b1,b2,b3))
						return 2;
					 return 3;
				 }

				 if ((h1.rank == board[2].rank))
				 {
					 if (h2.rank >= 11)
						 return 2;
					 return 3;
				 }

				 if ((h2.rank == board[2].rank))
				 {
					 if (h1.rank >= 11)
						 return 2;
					 return 3;
				 }

				 if ((h1.rank == board[1].rank) || (h2.rank == board[1].rank))
					 return 3;

				 if (flopGutshot(cards))
					 return 5;
				 return 4;
			 }

			 if (h1.rank == h2.rank)
			 {
				 if (flopTwoOverCards(h1,h2,b1,b2,b3))
				 {
					 if (h1.rank >= 11)
						return 1;
					 return 2;
				 }
				 return 3;
			 }
			  
			 if ((h1.rank == board[2].rank))
			 {
				 if ((h2.rank == 14) || ((h1.rank == 14) && (h2.rank == 13)))
					 return 1;
				 if (h2.rank >= 11)
					 return 2;
				 return 3;
			 }
			  
			 if ((h2.rank == board[2].rank))
			 {
				 if ((h1.rank == 14) || ((h2.rank == 14) && (h1.rank == 13)))
					 return 1;
				 if (h1.rank >= 11)
					 return 2;
				 return 3;
			 }

			 if ((h1.rank == board[1].rank) || (h2.rank == board[1].rank))
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
				if (h1.rank > b1.rank || h1.rank > b2.rank || h1.rank > b3.rank || h1.rank > b4.rank || (h1.rank == 10 && board[0].rank > 10 ) || (h1.rank == 2 && board[3].rank == 14))
					return 0;

				if (flopStraightFlush(h2,b1,b2,b3,b4))
					return 0;

				return 1;
			}

			if (flopStraightFlush(h2,b1,b2,b3,b4))
			{
				return (h2.rank > b1.rank || h2.rank > b2.rank || h2.rank > b3.rank || h2.rank > b4.rank || (h2.rank == 10 && board[0].rank > 10 ) || (h2.rank == 2 && board[3].rank == 14)) ? 0 : 1;
			}

			return 0;
		}
		else if (turnPoker(cards))
		{
			if ((b1.rank == b2.rank) && (b2.rank == b3.rank) && (b3.rank == b4.rank))
			{
				if ((h1.rank == 14) || (h2.rank == 14) || ((b1.rank == 14) && ((h1.rank == 13) || (h2.rank == 13))))
					return 0;
				if ((h1.rank == 13) || (h2.rank == 13) || ((b1.rank == 14) && ((h1.rank == 12) || (h2.rank == 12))))
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
				if ((board[3].rank == h1.rank) || (board[3].rank == h2.rank))
					return 0;
				return 2;
			}

			Card standAloneCard = (board[0].rank == board[1].rank) ? board[3] : board[0];

			if (h1.rank == h2.rank)
			{
				if (h1.rank > standAloneCard.rank)
				{
					if (h1.rank >= 11)
						return 1;
					if (h1.rank >= 8)
						return 2;
					return 3;
				}
				return 3;
			}

			if (standAloneCard.rank >= 12)
				return 1;
			if (standAloneCard.rank >= 8)
				return 2;
			return 3;
		}
		else if (turnFlush(cards))
		{
			if (!turnMonotoneBoard(board) && !turnDoubleBoard(board))
				return 0;

			if (!turnMonotoneBoard(board) && turnDoubleBoard(board))
				return 1;

			Card suitedCard = (h1.suit == b1.suit) ? h1 : h2;

			if (turnOneCardNutFlush(h1,h2,board))
			{
				if (turnOneCardStrBoard(board))
					return 1;
				return 0;
			}

			if (suitedCard.rank >= 11)
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
				if (existsOnBoard(h2.rank,board))
					kicker = h1;

				if (kicker.rank == 14)
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
				if (kicker.rank >= 10)
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
				// TODO : If we have a pocker pair bigger than on the board.
				if ((h1.rank == 14) || (h2.rank == 14))
					return 3;
				return 4;
			}

			if (turnSingleBoard(board))
			{
				if ((h1.rank == board[3].rank) || (h2.rank == board[3].rank))
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
				if (h1.rank == h2.rank)
				{
					if (h1.rank > board[3].rank)
					{
						if (h1.rank >= 12)
						{
							if (turnOneCardNutFlushDraw(h1,h2,board))
								return 2;
							if (turnDangerousBoard(b1,b2,b3,b4))
								return 2;
							return 1;
						}
						if (h1.rank >= 8)
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
				if (existsOnBoard(h1.rank,board))
				{
					kicker = h2;
					pair = h1;
				}

				if ((pair.rank == board[3].rank) || ((board[3].rank == board[2].rank) && (pair.rank == board[1].rank)))
				{
					if (pair.rank >= 10)
					{
						if ((kicker.rank == 14) || ((pair.rank == 14) && (kicker.rank == 13)))
						{
							if (turnStrongFlushDraw(h1,h2,board))
								return 1;
							if (turnDangerousBoard(b1,b2,b3,b4))
								return 2;
							return 1;
						}
						if (kicker.rank >= 10)
							return 2;
						return 3;
					}
					if (kicker.rank >= 10)
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

			if (h1.rank == h2.rank)
			{
				if (h1.rank > board[3].rank)
				{
					if (h1.rank >= 11)
					{
						if (turnOneCardNutFlushDraw(h1,h2,board))
							return 1;
						if (turnDangerousBoard(b1,b2,b3,b4))
							return 2;
						return 1;
					}
					return 2;
				}
				if (h1.rank > board[2].rank)
					return 3;
				return 4;
			}
			
			Card pair = h2;
			Card kicker = h1;
			if (existsOnBoard(h1.rank,board))
			{
				kicker = h2;
				pair = h1;
			}

			if (pair.rank == board[3].rank)
			{
				if ((kicker.rank == 14) || (pair.rank == 14 && kicker.rank == 13))
				{
					if (turnStrongFlushDraw(h1,h2,board) || (turnStrongOESD(h1,h2,b1,b2,b3,b4) && turnSuitedNumber(b1,b2,b3,b4) < 3))
						return 1;
					if (turnDangerousBoard(b1,b2,b3,b4))
						return 2;
					return 1;
				}
				if (kicker.rank >= 10)
				{
					if (turnStrongFlushDraw(h1,h2,board) || (turnStrongOESD(h1,h2,b1,b2,b3,b4) && turnSuitedNumber(b1,b2,b3,b4) < 3))
						return 1;
					return 2;
				}
				if (turnStrongFlushDraw(h1,h2,board) || (turnStrongOESD(h1,h2,b1,b2,b3,b4) && turnSuitedNumber(b1,b2,b3,b4) < 3))
					return 2;
				return 3;
			}

			if (pair.rank == board[2].rank)
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
			cout << "Straight flush" << endl;
		}
		else if (riverPoker(cards))
		{
			cout << "Poker" << endl;
		}
		else if (riverFullHouse(cards))
		{
			cout << "Full House" << endl;
		}
		else if (riverFlush(cards))
		{
			cout << "Flush" << endl;
		}
		else if (riverStraight(cards))
		{
			cout << "Straight" << endl;
		}
		else if (riverThreeofaKind(cards))
		{
			cout << "Three of a kind" << endl;
		}
		else if (riverTwoPair(cards))
		{
			cout << "Two pair" << endl;
		}
		else if (riverOnePair(cards))
		{
			cout << "One pair" << endl;
		}
		else
		{
			cout << "High card" << endl;
		}

		return 4;
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

};