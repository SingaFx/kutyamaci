#pragma once
#include "handHistoryParser.h"
#include "evaluator.h"
#include "validator.h"
#include "bayesUtils.h"
#include <set>
#include <vector>
#include <sstream>

using namespace std;

class PlayerRange
{
public:
	std::set<pair<Hand, double> > range;
	bool valid;
	int id;

	PlayerRange()
	{
	}

	void printRange()
	{
		std::set<pair<Hand, double> >::iterator it;
		double total = 0;
		for (it = range.begin(); it != range.end(); ++it)
		{
			printf("%c%c%c%c: %lf\n", it->first.getCard1().getRank(), it->first.getCard1().getSuit(), it->first.getCard2().getRank(), it->first.getCard2().getSuit(), it->second);
			total += it->second;
		}
		printf("Total = %lf\n", total);
	}
	string toString()
	{
		stringstream os;

		std::set<pair<Hand, double> >::iterator it;
		double total = 0;
		for (it = range.begin(); it != range.end(); ++it)
		{
			os << it->first.getCard1().getRank() << it->first.getCard1().getSuit() << it->first.getCard2().getRank() << it->first.getCard2().getSuit() << " " <<  it->second << endl;
			total += it->second;
		}
		os << total << endl;

		return os.str();
	}
	PlayerRange create100()
	{
		PlayerRange res;
		char map[4];
		map[0] = 's';
		map[1] = 'h';
		map[2] = 'd';
		map[3] = 'c';

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
						hand.getCard1().setRank(numberToCard(rank1));
						hand.getCard2().setRank(numberToCard(rank2));
						hand.getCard1().setSuit(map[i1]);
						hand.getCard2().setSuit(map[i2]);

						res.range.insert(make_pair(hand, 1.0/1326));
						
					}

					if (i2 > i1)
					{
						int rank1 = j1 + 2;
						int rank2 = j1 + 2;
						Hand hand;
						hand.getCard1().setRank(numberToCard(rank1));
						hand.getCard2().setRank(numberToCard(rank2));
						hand.getCard1().setSuit(map[i1]);
						hand.getCard2().setSuit(map[i2]);

						res.range.insert(make_pair(hand, 1.0/1326));
					}
				}
			}
		}

		res = res.normalize();

		return res;
	}

	void setValid(bool valid)
	{
		this->valid = valid;
	}
	bool getValid()
	{
		return valid;
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

	void setId(int id)
	{
		this->id = id;
	}
	int getId()
	{
		return id;
	}

	void printHS(vector<Card> board)
	{
		double HS[10];
		memset(HS, 0, sizeof(HS));

		std::set<pair<Hand, double> >::iterator it;
		double total = 0;
		for (it = range.begin(); it != range.end(); ++it)
		{
			int str = Evaluator::cardStrength(it->first.getCard1(),  it->first.getCard2(), board);
			HS[str] += it->second;
			total += it->second;
		}

		for (int i = 0; i < 9; ++i)
		{
			printf("HS(%d) = %lf\n", i, HS[i]);
		}

		printf("Total = %lf\n", total);
	}

	
	string toStringHS(vector<Card> board)
	{
		stringstream os;

		double HS[10];
		memset(HS, 0, sizeof(HS));

		std::set<pair<Hand, double> >::iterator it;
		double total = 0;
		for (it = range.begin(); it != range.end(); ++it)
		{
			int str = Evaluator::cardStrength(it->first.getCard1(),  it->first.getCard2(), board);
			HS[str] += it->second;
			total += it->second;
		}

		for (int i = 0; i < 9; ++i)
		{
			os << "HS(" << i << ") = " << HS[i] << endl;
		}

		return os.str();
	}

	double getHS(int k, vector<Card> board)
	{
		double HS[10];
		memset(HS, 0, sizeof(HS));

		std::set<pair<Hand, double> >::iterator it;
		double total = 0;
		for (it = range.begin(); it != range.end(); ++it)
		{
			int str = Evaluator::cardStrength(it->first.getCard1(),  it->first.getCard2(), board);
			HS[str] += it->second;
			total += it->second;
		}

		return HS[k];
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
						hand.getCard1().setRank(numberToCard(rank1));
						hand.getCard2().setRank(numberToCard(rank2));
						hand.getCard1().setSuit(map[i1]);
						hand.getCard2().setSuit(map[i2]);

						if (Validator::softValidate(v, hand, own))
						{
							int str = Evaluator::cardStrength(hand.getCard1(), hand.getCard2(), v);
							hsn[str]++;
							total++;
						}
					}

					if (i2 > i1)
					{
						int rank1 = j1 + 2;
						int rank2 = j1 + 2;
						Hand hand;
						hand.getCard1().setRank(numberToCard(rank1));
						hand.getCard2().setRank(numberToCard(rank2));
						hand.getCard1().setSuit(map[i1]);
						hand.getCard2().setSuit(map[i2]);

						if (Validator::softValidate(v, hand, own))
						{
							int str = Evaluator::cardStrength(hand.getCard1(), hand.getCard2(), v);
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
						hand.getCard1().setRank(numberToCard(rank1));
						hand.getCard2().setRank(numberToCard(rank2));
						hand.getCard1().setSuit(map[i1]);
						hand.getCard2().setSuit(map[i2]);

						if (Validator::softValidate(v, hand, own))
						{
							int str = Evaluator::cardStrength(hand.getCard1(), hand.getCard2(), v);
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
						hand.getCard1().setRank(numberToCard(rank1));
						hand.getCard2().setRank(numberToCard(rank2));
						hand.getCard1().setSuit(map[i1]);
						hand.getCard2().setSuit(map[i2]);

						if (Validator::softValidate(v, hand, own))
						{
							int str = Evaluator::cardStrength(hand.getCard1(), hand.getCard2(), v);
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

	static PlayerRange reCreate(PlayerRange& range, vector<Card>& cards, Hand& own)
	{
		PlayerRange res;
		set<pair<Hand, double> >::iterator it;
		for (it = range.range.begin(); it != range.range.end(); ++it)
		{
			if (Validator::softValidate(cards, it->first, own))
			{
				res.range.insert(*it);
			}
		}

		res = res.normalize();
		return res;
	}

public:

	//create range from HS
	static PlayerRange createRange(int n, double HS[], std::vector<Card>& v, Hand own)
	{

		//BOARD TYPE?

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

	static PlayerRange justNutsRange(PlayerRange range, std::vector<Card>& v)
	{
		PlayerRange res;

		std::set<pair<Hand, double> >::iterator it;
		for (it = range.range.begin(); it != range.range.end(); ++it)
		{
			if (Evaluator::isNuts(it->first.getCard1(), it->first.getCard2(), v))
			{
				res.range.insert(*it);
			}
		}

		res.normalize();
		return res;
	}

	static PlayerRange mergeRange(PlayerRange r1, PlayerRange r2, vector<Card> v, Hand own)
	{
		double hsr1[20], hsr2[20];
		memset(hsr1, 0, sizeof(hsr1));
		memset(hsr2, 0, sizeof(hsr2));

		r1 = reCreate(r1, v, own);
		r2 = reCreate(r2, v, own);

		set<pair<Hand, double> >::iterator it1, it2, iter;
		for (it1 = r1.range.begin(); it1 != r1.range.end(); ++it1)
		{
			if (Validator::softValidate(v, it1->first, own))
			{
				int str = Evaluator::cardStrength(it1->first.getCard1(), it1->first.getCard2(), v);
				hsr1[str] += it1->second;
			}
		}
		for (it2 = r2.range.begin(); it2 != r2.range.end(); ++it2)
		{
			if (Validator::softValidate(v, it2->first, own))
			{
				int str = Evaluator::cardStrength(it2->first.getCard1(), it2->first.getCard2(), v);
				hsr2[str] += it2->second;
			}
		}

		PlayerRange res;

		for (it1 = r1.range.begin(); it1 != r1.range.end(); it1++)
		{
			iter = r2.range.lower_bound(make_pair(it1->first, 0.0));
			if (iter->first == it1->first)
			{
				int str = Evaluator::cardStrength(it1->first.getCard1(), it1->first.getCard2(), v);
				double akt = 0;
				if (hsr1[str] > 0)
					akt = it1->second * (hsr2[str] / hsr1[str]);
				if (akt > 0)
					res.range.insert(make_pair(it1->first, akt));
			}
		}

		res = res.normalize();

		return res;
	}
	static PlayerRange addRange(PlayerRange& r1, PlayerRange& r2)
	{
		PlayerRange res;

		set<pair<Hand, double> >::iterator it1, iter;
		for (it1 = r2.range.begin(); it1 != r2.range.end(); it1++)
		{

			iter = r1.range.lower_bound(make_pair(it1->first, 0.0));
			if (iter->first == it1->first)
			{
				double prob = it1->second + iter->second;
				res.range.insert(make_pair(it1->first, prob));
			}
			else
			{
				res.range.insert(make_pair(it1->first, it1->second));
			}
		}

		for (it1 = r1.range.begin(); it1 != r1.range.end(); it1++)
		{
			iter = res.range.lower_bound(make_pair(it1->first, 0.0));
			if (iter->first == it1->first)
			{

			}
			else
			{
				res.range.insert(make_pair(it1->first, it1->second));
			}
		}

		//res = res.normalize();
		return res;
	}
	static PlayerRange addRange(PlayerRange& r1, PlayerRange& r2, double weight)
	{
		PlayerRange res;

		set<pair<Hand, double> >::iterator it1, iter;
		for (it1 = r2.range.begin(); it1 != r2.range.end(); it1++)
		{

			iter = r1.range.lower_bound(make_pair(it1->first, 0.0));
			if (iter->first == it1->first)
			{
				double prob = it1->second * (1 - weight) + iter->second * weight;
				res.range.insert(make_pair(it1->first, prob));
			}
			else
			{
				res.range.insert(make_pair(it1->first, it1->second));
			}
		}

		for (it1 = r1.range.begin(); it1 != r1.range.end(); it1++)
		{
			iter = res.range.lower_bound(make_pair(it1->first, 0.0));
			if (iter->first == it1->first)
			{

			}
			else
			{
				res.range.insert(make_pair(it1->first, it1->second));
			}
		}

		//res = res.normalize();
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
