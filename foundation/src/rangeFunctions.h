#pragma once
#include "handHistoryParser.h"
#include "evaluator.h"
#include "validator.h"
#include "bayesUtils.h"
#include <set>
#include <vector>

class PlayerRange
{
public:
	std::set<pair<Hand, double> > range;

	void printRange()
	{
		std::set<pair<Hand, double> >::iterator it;
		double total = 0;
		for (it = range.begin(); it != range.end(); ++it)
		{
			printf("%c%c%c%c: %lf\n", it->first.card1.rank, it->first.card1.suit, it->first.card2.rank, it->first.card2.suit, it->second);
			total += it->second;
		}
		printf("Total = %lf\n", total);
	}

	double totalPercentage()
	{
		std::set<pair<Hand, double> >::iterator it;
		double total = 0;
		for (it = range.begin(); it != range.end(); ++it)
		{
			total += it->second;
		}
		
		return total;
	}

	PlayerRange normalize()
	{
		PlayerRange res;
		double total = totalPercentage();
		std::set<pair<Hand, double> >::iterator it;
		for (it = range.begin(); it != range.end(); ++it)
		{
			double x = it->second;
			x *= 1 / total;
			res.range.insert(make_pair(it->first, x));
		}

		return res;
	}
};
class RangeUtils
{
private:

	static void calculateStrengths(int n, int hsn[], vector<Card>& v, Hand& own, char map[])
	{
		int total = 0;
		int count1 = 0;
		for (int i1 = 0; i1 < 4; ++i1)
		{
			for (int j1 = 0; j1 < 13; ++j1)
			{
				count1 = 13 * i1 + j1;

				int count2 = 0;

				for (int i2 = 0; i2 < 4; ++i2)
				{
					for (int j2 = j1 + 1; j2 < 13; ++j2)
					{
						count2 = 13 * i2 + j2;
						if (count1 == count2) continue;

						int rank1 = j1 + 2;
						int rank2 = j2 + 2;
						Hand hand;
						hand.card1.rank = numberToCard(rank1);
						hand.card2.rank = numberToCard(rank2);
						hand.card1.suit = map[i1];
						hand.card2.suit = map[i2];

						if (Validator::softValidate(v, hand, own))
						{
							int str = Evaluator::cardStrength(hand.card1, hand.card2, v);
							hsn[str]++;
							total++;
						}
					}

					if (i2 > i1)
					{
						int rank1 = j1 + 2;
						int rank2 = j1 + 2;
						Hand hand;
						hand.card1.rank = numberToCard(rank1);
						hand.card2.rank = numberToCard(rank2);
						hand.card1.suit = map[i1];
						hand.card2.suit = map[i2];

						if (Validator::softValidate(v, hand, own))
						{
							int str = Evaluator::cardStrength(hand.card1, hand.card2, v);
							hsn[str]++;
							total++;
						}
					}
				}
			}
		}
	}

	static void calculateRanges(int n, double HS[], int hsn[], vector<Card>& v, Hand& own, char map[], PlayerRange& res)
	{
		int count1 = 0;
		for (int i1 = 0; i1 < 4; ++i1)
		{
			for (int j1 = 0; j1 < 13; ++j1)
			{
				count1 = 13 * i1 + j1;

				int count2 = 0;

				for (int i2 = 0; i2 < 4; ++i2)
				{
					for (int j2 = j1 + 1; j2 < 13; ++j2)
					{
						count2 = 13 * i2 + j2;
						if (count1 == count2) continue;

						int rank1 = j1 + 2;
						int rank2 = j2 + 2;
						Hand hand;
						hand.card1.rank = numberToCard(rank1);
						hand.card2.rank = numberToCard(rank2);
						hand.card1.suit = map[i1];
						hand.card2.suit = map[i2];

						if (Validator::softValidate(v, hand, own))
						{
							int str = Evaluator::cardStrength(hand.card1, hand.card2, v);
							double akt = HS[str] / hsn[str];
							if (akt > 0)
								res.range.insert(make_pair(hand, akt));
						}
					}

					if (i2 > i1)
					{
						int rank1 = j1 + 2;
						int rank2 = j1 + 2;
						Hand hand;
						hand.card1.rank = numberToCard(rank1);
						hand.card2.rank = numberToCard(rank2);
						hand.card1.suit = map[i1];
						hand.card2.suit = map[i2];

						if (Validator::softValidate(v, hand, own))
						{
							int str = Evaluator::cardStrength(hand.card1, hand.card2, v);
							double akt = HS[str] / hsn[str];
							if (akt > 0)
								res.range.insert(make_pair(hand, akt));
						}
					}
				}
			}
		}
	}

	static void normalize(int n, double HS[])
	{
		double totalProb = 0;
		for (int i = 0; i < n; ++i)
		{
			totalProb += HS[i];
		}
		for (int i = 0; i < n; ++i)
		{
			HS[i] *= 1 / totalProb;
		}
	}

	static int totalTypes(int n, int hsn[])
	{
		int cnt = 0;

		for (int i = 0; i < n; ++i)
		{
			if (hsn[i] != 0)
			{
				cnt++;
			}
		}

		return cnt;
	}

	static void distributeRanges(int n, double HS[], int hsn[])
	{
		for (int i = 0; i < 5; ++i)
		{
			int mini = -1;
			int min = 1000;
			if (hsn[i] == 0)
			{
				for (int j = 0; j < 5; ++j)
				{
					if (hsn[j] != 0)
					{
						if (min > abs(i - j))
						{
							min = abs(i - j);
							mini = j;
						}
					}
				}
				if (mini != -1)
				{
					HS[mini] += HS[i];
					HS[i] = 0;
				}
			}
		}

		for (int i = 5; i < 8; ++i)
		{
			int mini = -1;
			int min = 1000;
			if (hsn[i] == 0)
			{
				for (int j = 5; j < 8; ++j)
				{
					if (hsn[j] != 0)
					{
						if (min > abs(i - j))
						{
							min = abs(i - j);
							mini = j;
						}
					}
				}
				if (mini != -1)
				{
					HS[mini] += HS[i];
					HS[i] = 0;
				}
			}
		}

		//add to FOLD otherwise
		for (int i = 0; i < 8; ++i)
		{
			if (hsn[i] == 0)
			{
				HS[8] += HS[i];
				HS[i] = 0;
			}
		}
	}

public:

	//create range from HS
	static PlayerRange createRange(int n, double HS[], std::vector<Card>& v, Hand own)
	{
		char map[4];
		map[0] = 's';
		map[1] = 'h';
		map[2] = 'd';
		map[3] = 'c';

		PlayerRange res;

		int hsn[20];
		memset(hsn, 0, sizeof(hsn));
		
		calculateStrengths(n, hsn, v, own, map);
		int cnt = totalTypes(n, hsn);
		distributeRanges(n, HS, hsn);

		//add FOLD
		double FOLD = HS[n];
		int div = ((cnt + 1) * cnt) / 2;
		double egyseg = FOLD / (double) div;

		cnt = 1;
		if (hsn[0] != 0) HS[0] += cnt++ * egyseg; //nagyon eros
		if (hsn[1] != 0) HS[1] += cnt++ * egyseg; //eros
		if (hsn[7] != 0) HS[7] += cnt++ * egyseg; //monster draw
		if (hsn[2] != 0) HS[2] += cnt++ * egyseg; //kozepesen eros
		if (hsn[6] != 0) HS[6] += cnt++ * egyseg; //jo draw
		if (hsn[3] != 0) HS[3] += cnt++ * egyseg; //gyenge
		if (hsn[5] != 0) HS[5] += cnt++ * egyseg; //GS
		if (hsn[4] != 0) HS[4] += cnt++ * egyseg; //AIR	
		HS[8] = 0;
	
		normalize(n, HS);
		calculateRanges(n, HS, hsn, v, own, map, res);

		return res;
	}

	static PlayerRange mergeRange(PlayerRange& r1, PlayerRange& r2, vector<Card>& v)
	{
		double hsr1[20], hsr2[20];
		memset(hsr1, 0, sizeof(hsr1));
		memset(hsr2, 0, sizeof(hsr2));

		set<pair<Hand, double> >::iterator it1, it2, iter;
		for (it1 = r1.range.begin(); it1 != r1.range.end(); ++it1)
		{
			int str = Evaluator::cardStrength(it1->first.card1, it1->first.card2, v);
			hsr1[str] += it1->second;
		}
		for (it2 = r2.range.begin(); it2 != r2.range.end(); ++it2)
		{
			int str = Evaluator::cardStrength(it2->first.card1, it2->first.card2, v);
			hsr2[str] += it2->second;
		}

		PlayerRange res;

		for (it1 = r1.range.begin(); it1 != r1.range.end(); it1++)
		{
			iter = r2.range.lower_bound(make_pair(it1->first, 0.0));
			if (iter->first == it1->first) 
			{
				int str = Evaluator::cardStrength(it1->first.card1, it1->first.card2, v);
				double akt = 0;
				if (hsr1[str] > 0)
					akt = it1->second * (hsr2[str] / hsr1[str]);
				if (akt > 0)
					res.range.insert(make_pair(it1->first, akt));
			}
		}
		return res;
	}

	//FE randomizalas a FOLD szerint
	static double randomizeFE(double FE, double FOLD)
	{
		return FE + unifRand(-0.2, 0.1);
	}
private:
	static double unifRand()
	{
		return rand() / double(RAND_MAX);
	}

	static double unifRand(double a, double b)
	{
		return (b-a)*unifRand() + a;
	}
};
