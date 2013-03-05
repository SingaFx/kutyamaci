#pragma once

#include <string>
#include <vector>

#include "bayesFunctions.h"
#include "rangeFunctions.h"
#include "logger.h"

using namespace std;

class BayesUserPreflop : public BayesPreflop
{
	static const int GRAF[2][8][8];

	struct node
	{
		int VPIP, PFR;
	};

	struct edge
	{
		node base;
		node x;

		int weight()
		{
			int res = 0;
			res += BayesUserPreflop::GRAF[0][base.VPIP][x.VPIP];
			res += BayesUserPreflop::GRAF[1][base.PFR][x.PFR];

			return res;
		}

		bool operator <(edge& e)
		{
			return weight() < e.weight();
		}
	};

	vector<edge> grafResult;

public:
	BayesUserPreflop()
	{
		preflop_nums[0] = PREFLOP_HAND_STRENGTH_NUM;
		preflop_nums[1] = PREFLOP_PLAYER_STACK_SIZE_NUM;
		preflop_nums[2] = PREFLOP_PLAYER_BET_SIZE_NUM;
		preflop_nums[3] = PREFLOP_PLAYER_LINE_NUM;
		preflop_nums[4] = PREFLOP_PLAYER_VPIP_NUM;
		preflop_nums[5] = PREFLOP_PLAYER_PFR_NUM;
		preflop_nums[6] = PREFLOP_PLAYER_POZ_NUM;

		memset(totalPreflop, 0, sizeof(totalPreflop));
		memset(probabilityPreflop, 0, sizeof(probabilityPreflop));
		memset(totalFE, 0, sizeof(totalFE));
		memset(probabilityFE, 0, sizeof(probabilityFE));
		totalSituation = 0;
		totalLittle = 0;
	}

	void read(string filename)
	{
		FILE* f = fopen(filename.c_str(), "r");
		back(f, false, 1);
		backFE(f, false, 1);
		fclose(f);
	}

	void write(string filename)
	{
		FILE* f = fopen(filename.c_str(), "w");
		back(f, true, 1);
		backFE(f, true, 1);

		fclose(f);
	}
	void createGrafResult(int v[])
	{
		grafResult.clear();
		
		node base;
		base.VPIP = v[4];
		base.PFR = v[5];

		for (int vpip = 0; vpip < PREFLOP_PLAYER_VPIP_NUM; ++vpip)
		{
			for (int pfr = 0; pfr < PREFLOP_PLAYER_PFR_NUM; ++pfr)
			{
				node x;
				x.VPIP = vpip;
				x.PFR = pfr;

				edge akt;
				akt.base = base;
				akt.x = x;

				if (akt.weight() > 0) grafResult.push_back(akt);
			}
		}
	}
	double getProbability(int v[])
	{

		if (totalPreflop[v[1]][v[2]][v[3]][v[4]][v[5]][v[6]] == 0)
		{
			return -2.0;
		}
		return (double)probabilityPreflop[v[0]][v[1]][v[2]][v[3]][v[4]][v[5]][v[6]] / (double)totalPreflop[v[1]][v[2]][v[3]][v[4]][v[5]][v[6]];
	}
	double getProbability(int v[], int x)
	{
		int res = 0;
		int total = 0;

		total += totalPreflop[v[1]][v[2]][v[3]][v[4]][v[5]][v[6]];
		res += probabilityPreflop[v[0]][v[1]][v[2]][v[3]][v[4]][v[5]][v[6]];
		
		if (x <= total) return (double) res / (double) total;
		//poz szerint
		for (int i = 0; i < PREFLOP_PLAYER_POZ_NUM; ++i)
		{
			if (i == v[6]) continue;
			total += totalPreflop[v[1]][v[2]][v[3]][v[4]][v[5]][i];
			res += probabilityPreflop[v[0]][v[1]][v[2]][v[3]][v[4]][v[5]][i];
		}
		if (x <= total) return (double) res / (double) total;
		//stacksize szerint
		for (int i = 0; i < PREFLOP_PLAYER_STACK_SIZE_NUM; ++i)
		{
			if (i == v[1]) continue;
			total += totalPreflop[i][v[2]][v[3]][v[4]][v[1]][v[6]];
			res += probabilityPreflop[v[0]][i][v[2]][v[3]][v[4]][i][v[6]];
		}
		if (x <= total) return (double) res / (double) total;

		createGrafResult(v);
		sort(grafResult.begin(), grafResult.end());

		for (int i = 0; i < grafResult.size(); ++i)
		{
			total += totalPreflop[v[1]][v[2]][v[3]][grafResult[i].x.VPIP][grafResult[i].x.PFR][v[6]];
			res += probabilityPreflop[v[0]][v[1]][v[2]][v[3]][grafResult[i].x.VPIP][grafResult[i].x.PFR][v[6]];

			if (x <= total) return (double) res / (double) total;
		}

		return -2.0;
	}
	double getProbabilityFE(int v[], int x)
	{
		int res = 0;
		int total = 0;

		total += totalFE[v[1]][v[2]][v[3]][v[4]][v[5]][v[6]];
		res += probabilityFE[v[1]][v[2]][v[3]][v[4]][v[5]][v[6]];
		
		if (x <= total) return (double) res / (double) total;
		//poz szerint
		for (int i = 0; i < PREFLOP_PLAYER_POZ_NUM; ++i)
		{
			if (i == v[6]) continue;
			total += totalFE[v[1]][v[2]][v[3]][v[4]][v[5]][i];
			res += probabilityFE[v[1]][v[2]][v[3]][v[4]][v[5]][i];
		}
		if (x <= total) return (double) res / (double) total;
		//stacksize szerint
		for (int i = 0; i < PREFLOP_PLAYER_STACK_SIZE_NUM; ++i)
		{
			if (i == v[1]) continue;
			total += totalFE[i][v[2]][v[3]][v[4]][v[5]][v[6]];
			res += probabilityFE[i][v[2]][v[3]][v[4]][v[5]][v[6]];
		}
		if (x <= total) return (double) res / (double) total;

		return -2.0;
	}
	double getProbabilityFE2(int v[], int x)
	{
		v[2] = 4;
		v[3] = 2;

		return getProbabilityFE(v, x);
	}

	double getProbabilityFE(double VPIP, double PFR, double stacksize, int poz, int line, double betsize, double bblind, double potcommon, int x)
	{
		int v[7];
		v[1] = normalizeStackSize(stacksize, bblind);
		v[2] = normalizeBetSize(1, betsize, potcommon, bblind);
		v[3] = line;
		v[4] = normalizeVPIP(VPIP);
		v[5] = normalizePFR(PFR);
		v[6] = poz + 3;

		if (line == 5)
			return getProbabilityFE2(v, x);

		return getProbabilityFE(v, x);
	}

	int preflopHandType(Hand hand)
	{
		int rank1 = convertRankToNumbers(hand.getCard1());
		int rank2 = convertRankToNumbers(hand.getCard2());
		char suit1 = hand.getCard1().getSuit();
		char suit2 = hand.getCard2().getSuit();

		if (rank1 < rank2)
		{
			swap(rank1, rank2);
		}

		if (rank1 == rank2)
		{
			if (rank1 < 10) return 0; // alacsony PP
			if (rank1 < 12) return 13; //TT-JJ
			if (rank1 < 13) return 14; //QQ
			return 1; // KK,AA
		}

		if (rank1 == 14 && rank2 == 13)
		{
			return 14;//AK
		}

		if (rank1 == 14 && rank2 > 11) //strong broadway
		{
			return 6;
		}

		if (rank1 > 10 && rank2 > 10) //broadway
		{
			if (suit1 == suit2)
			{
				return 7;
			}

			return 8;
		}

		if (rank1 - rank2 == 1) // connector
		{
			if (suit1 == suit2) // suited
			{
				return 2; //SC
			}

			return 3; //OC
		}

		if (rank1 - rank2 < 4) //1-2 gapper
		{
			if (suit1 == suit2)
			{
				return 4;
			}

			return 5;
		}

		if (rank1 == 14)
		{
			if (suit1 == suit2) return 9;

			return 10;
		}

		if (rank1 > 6 && rank2 > 6 && suit1 == suit2)
		{
			return 11;
		}

		return 12; //gyenge!!
	}

	PlayerRange getRange(int v[], int x)
	{
		//FOLD nincs hozzaadva!!!!!!!!!!

		//hasonlo tipusu handek HS
		//PP = 0 22-99 PP = 1 TT-AA   -> kategoriara osszeget es elosztani kozottuk!
		//SC = 2
		//SC12GP = 3
		//OC = 8
		//OC12GP = 9
		//STRONGBROADWAY (AK, AQ) = 12
		//BROADWAY = 4
		//BROADWAYs = 5
		//Axs = 6
		//Axo = 7
		//7+ suited = 10
		//any2 = 11
		PlayerRange result;

		char map[4];
		map[0] = 's';
		map[1] = 'h';
		map[2] = 'd';
		map[3] = 'c';


		Hand hand;
		int types[15];
		double ptypes[15];
		memset(types, 0, sizeof(types));
		memset(ptypes, 0, sizeof(ptypes));

		int count = 0;
		for (int i = 2; i < 15; ++i)
			for (int j = i + 1; j < 15; ++j)
			{
				++count;
				v[0] = count;
				double res;
				res = getProbability(v, x);
				if (res < 0) return result;
				hand.getCard1().setRank(numberToCard(i));
				hand.getCard2().setRank(numberToCard(j));

				for (int k = 0; k < 4; ++k)
				{
					hand.getCard1().setSuit(map[k]);
					hand.getCard2().setSuit(map[k]);
					int type = preflopHandType(hand);
					types[type]++;		
					ptypes[type] += res / 4;
				}
			}
		
		//offsuited
		for (int i = 2; i < 15; ++i)
			for (int j = i; j < 15; ++j)
			{
				++count;
				v[0] = count;
				double res;
				res = getProbability(v, x);
				if (res < 0) return result;
				hand.getCard1().setRank(numberToCard(i));
				hand.getCard2().setRank(numberToCard(j));

				for (int k1 = 0; k1 < 4; ++k1)
				{
					for (int k2 = 0; k2 < 4; ++k2)
					{
						if (k1 >= k2) continue;
						hand.getCard1().setSuit(map[k1]);
						hand.getCard2().setSuit(map[k2]);
						int type = preflopHandType(hand);
						types[type]++;
						ptypes[type] += res / 6;
					}
				}						
			}

		for (int i = 2; i < 15; ++i)
			for (int j = i + 1; j < 15; ++j)
			{
				hand.getCard1().setRank(numberToCard(i));
				hand.getCard2().setRank(numberToCard(j));

				for (int k = 0; k < 4; ++k)
				{
					hand.getCard1().setSuit(map[k]);
					hand.getCard2().setSuit(map[k]);
					int type = preflopHandType(hand);
					double akt = ptypes[type]/(double)types[type];
					if (akt > 0)
						result.range.insert(make_pair(hand, akt));
				}
			}
		

		//offsuited
		for (int i = 2; i < 15; ++i)
			for (int j = i; j < 15; ++j)
			{
				hand.getCard1().setRank(numberToCard(i));
				hand.getCard2().setRank(numberToCard(j));

				for (int k1 = 0; k1 < 4; ++k1)
				{
					for (int k2 = 0; k2 < 4; ++k2)
					{
						if (k1 >= k2) continue;
						hand.getCard1().setSuit(map[k1]);
						hand.getCard2().setSuit(map[k2]);
						int type = preflopHandType(hand);
						double akt = ptypes[type]/(double)types[type];
						if (akt > 0)
							result.range.insert(make_pair(hand, akt));
					}
				}										
			}

		result = result.normalize();

		return result;
	}

	PlayerRange getRange(double VPIP, double PFR, double stackSize, int poz, int line, double betsize, double bblind, double potcommon, int x)
	{
		int v[7];
		v[1] = normalizeStackSize(stackSize, bblind);
		v[2] = normalizeBetSize(1, betsize, potcommon, bblind);
		v[3] = line;
		v[4] = normalizeVPIP(VPIP);
		v[5] = normalizePFR(PFR);
		v[6] = poz + 3;

		return getRange(v, x);
	}

	PlayerRange getCallRaiseRange(double VPIP, double PFR, double stackSize, int poz, int line, double betsize, double bblind, double potcommon, int x, bool allIn)
	{
		PlayerRange res;

		int nBetSize = normalizeBetSize(1, betsize, potcommon, bblind);

		int v[7];
		v[1] = normalizeStackSize(stackSize, bblind);
		v[2] = nBetSize;
		v[3] = line;
		v[4] = normalizeVPIP(VPIP);
		v[5] = normalizePFR(PFR);
		v[6] = poz + 3;

		PlayerRange callingRange = getRange(v, x);
		PlayerRange raiseRangeTotal;

		if (!allIn)
		{
			v[3] = 1;

			for (int i = nBetSize + 1; i < PREFLOP_PLAYER_BET_SIZE_NUM; ++i)
			{
				v[2] = i;
				PlayerRange raiseRange = getRange(v, x);
				raiseRangeTotal = RangeUtils::addRange(raiseRangeTotal, raiseRange);
			}

			raiseRangeTotal = raiseRangeTotal.normalize();

			callingRange = RangeUtils::addRange(callingRange, raiseRangeTotal, 0.7);
		}

		callingRange = callingRange.normalize();

		return callingRange;
	}

	PlayerRange getRaiseRange(double VPIP, double PFR, double stackSize, int poz, double betsize, double bblind, double potcommon, int x)
	{
		int nBetSize = normalizeBetSize(1, betsize, potcommon, bblind);

		int v[7];
		v[1] = normalizeStackSize(stackSize, bblind);
		v[2] = nBetSize;
		v[3] = 1;
		v[4] = normalizeVPIP(VPIP);
		v[5] = normalizePFR(PFR);
		v[6] = poz + 3;

		PlayerRange raiseRangeTotal;

		for (int i = nBetSize + 1; i < PREFLOP_PLAYER_BET_SIZE_NUM; ++i)
		{
			v[2] = i;
			PlayerRange raiseRange = getRange(v, x);
			raiseRangeTotal = RangeUtils::addRange(raiseRangeTotal, raiseRange);
		}
		
		raiseRangeTotal = raiseRangeTotal.normalize();

		return raiseRangeTotal;
	}

	void printRange(int v[])
	{
		//suited
		double total = 0;

		int count = 0;
		for (int i = 2; i < 15; ++i)
			for (int j = i + 1; j < 15; ++j)
			{
				++count;
				v[0] = count;
				double res;
				if ((res = getProbability(v)) > 0)
				{
					total += res;
					cout << numberToCard(i) << numberToCard(j) << "s = " << res << endl;
				}
			}
		

		//offsuited
		for (int i = 2; i < 15; ++i)
			for (int j = i; j < 15; ++j)
			{
				++count;
				v[0] = count;
				double res;
				if ((res = getProbability(v)) > 0)
				{
					total += res;
					cout << numberToCard(i) << numberToCard(j) << "o = " << res << endl;
				}
			}
		printf("Total preflop %lf = ", total);
	}

	//TODO:save range to gnuplot
	void plotRange(int v[])
	{
		//suited
		int count = 0;
		for (int i = 2; i < 15; ++i)
			for (int j = i + 1; j < 15; ++j)
			{
				++count;
				//printf("%d %d s = ", i, j);
				v[0] = count;
				double res;
				if ((res = getProbability(v)) > 0)
					cout << numberToCard(i) << numberToCard(j) << "s = " << res << endl;
			}
		

		//offsuited
		for (int i = 2; i < 15; ++i)
			for (int j = i; j < 15; ++j)
			{
				++count;
				//printf("%d %d o", i, j);
				v[0] = count;
				double res;
				if ((res = getProbability(v)) > 0)
					cout << numberToCard(i) << numberToCard(j) << "o = " << res << endl;
			}
	}

private:
	int v[preflop_node_number];
	int totalSituation;
	int totalLittle;
	void back(FILE* f, bool b, int k)
	{
		if (k == preflop_node_number)
		{
			if (b)
			{
				fprintf(f,"%d ", totalPreflop[v[0]][v[1]][v[2]][v[3]][v[4]][v[5]]);
				for (int i = 0; i < PREFLOP_HAND_STRENGTH_NUM; ++i)
					fprintf(f,"%d ", probabilityPreflop[i][v[0]][v[1]][v[2]][v[3]][v[4]][v[5]]);
			}
			else
			{
				fscanf(f,"%d ", &totalPreflop[v[1]][v[2]][v[3]][v[4]][v[5]][v[6]]);

				if (totalPreflop[v[1]][v[2]][v[3]][v[4]][v[5]][v[6]] > 100000)
				{
					printf("%d %d %d %d %d %d\n", v[1], v[2], v[3], v[4], v[5], v[6]);
				}

				for (int i = 0; i < PREFLOP_HAND_STRENGTH_NUM; ++i)
					fscanf(f,"%d ", &probabilityPreflop[i][v[1]][v[2]][v[3]][v[4]][v[5]][v[6]]);
			}

			return ;
		}

		for (int i = 0; i < preflop_nums[k]; ++i)
		{
			v[k] = i;
			back(f, b, k + 1);
		}
	}
	void backFE(FILE* f, bool b, int k)
	{
		if (k == preflop_node_number)
		{
			if (b)
			{
				fprintf(f,"%d ", totalFE[v[1]][v[2]][v[3]][v[4]][v[5]][v[6]]);
				fprintf(f,"%d ", probabilityFE[v[1]][v[2]][v[3]][v[4]][v[5]][v[6]]);
				
			}
			else
			{
				fscanf(f,"%d ", &totalFE[v[1]][v[2]][v[3]][v[4]][v[5]][v[6]]);
				fscanf(f,"%d ", &probabilityFE[v[1]][v[2]][v[3]][v[4]][v[5]][v[6]]);
			}

			return ;
		}

		for (int i = 0; i < preflop_nums[k]; ++i)
		{
			v[k] = i;
			backFE(f, b, k + 1);
		}
	}
};
class BayesUserFlop : public BayesFlop
{
	static const int GRAF[5][8][8];

	struct node
	{
		int VPIP, PFR, AF;
		int betsize;
	};

	struct edge
	{
		node base;
		node x;

		int weight()
		{
			int res = 0;
			res += BayesUserFlop::GRAF[0][base.VPIP][x.VPIP];
			res += BayesUserFlop::GRAF[1][base.PFR][x.PFR];
			res += BayesUserFlop::GRAF[2][base.AF][x.AF];
			res += BayesUserFlop::GRAF[3][base.betsize][x.betsize];

			return res;
		}

		bool operator <(edge& e)
		{
			return weight() < e.weight();
		}
	};

	vector<edge> grafResult;


public:
	BayesUserFlop()
	{
		nums[0] = HAND_STRENGTH_NUM;
		nums[1] = FOLD_EQUITY_NUM;
		nums[2] = POT_SIZE_NUM;
		nums[3] = PLAYER_STACK_SIZE_NUM;
		nums[4] = PLAYER_BET_SIZE_NUM;
		nums[5] = PLAYER_LINE_NUM;
		nums[6] = PLAYER_VPIP_NUM;
		nums[7] = PLAYER_PFR_NUM;
		nums[8] = PLAYER_AF_NUM;

		memset(totalS, 0, sizeof(totalS));
		memset(probabilityHS, 0, sizeof(probabilityHS));
		memset(probabilityFE, 0, sizeof(probabilityFE));
	}

	void read(string filename)
	{
		FILE* f = fopen(filename.c_str(), "r");
		backHS(f, false, 2);
		backFE(f, false, 2);

		int totalS = 1;
		for (int i = 2; i < node_number; ++i)
			totalS *= nums[i];
		fclose(f);
	}

	void readNormalize(string filename)
	{
		FILE* f = fopen(filename.c_str(), "r");
		//
		int n;
		for (int iter = 0; iter < 4; ++iter)
		{
			scanf("%d", &n);
			for (int i = 0; i < n; ++i)
			{
				for (int j = 0; j < n; ++j)
				{
					scanf("%d", &GRAF[iter][i][j]);
				}
			}
		}

		fclose(f);
	}

	void createGrafResult(int v[])
	{
		grafResult.clear();
		
		node base;
		base.VPIP = v[5];
		base.PFR = v[6];
		base.AF = v[7];
		base.betsize = v[3];

		for (int vpip = 0; vpip < PLAYER_VPIP_NUM; ++vpip)
		{
			for (int pfr = 0; pfr < PLAYER_PFR_NUM; ++pfr)
			{
				for (int af = 0; af < PLAYER_AF_NUM; ++af)
				{
					for (int betsize = 0; betsize < PLAYER_BET_SIZE_NUM; ++betsize)
					{
						node x;
						x.VPIP = vpip;
						x.PFR = pfr;
						x.AF = af;
						x.betsize = betsize;

						edge akt;
						akt.base = base;
						akt.x = x;

						if (akt.weight() > 0) grafResult.push_back(akt);
					}
				}
			}
		}
	}

	void write(string filename)
	{
		printf("Writing bayes network\n");
		FILE* f = fopen(filename.c_str(), "w");
		backHS(f, true, 2);
		backFE(f, true, 2);
		fclose(f);
	}

	double getProbabilityHS(int v[], int x)
	{
		int res = 0;
		int total = 0;

		total += totalS[v[1]][v[2]][v[3]][v[4]][v[5]][v[6]][v[7]];
		res += probabilityHS[v[0]][v[1]][v[2]][v[3]][v[4]][v[5]][v[6]][v[7]];
		if (x <= total) return (double) res / (double) total;

		for (int i = 0; i < PLAYER_STACK_SIZE_NUM; ++i)
		{
			if (v[2] == i) continue;
			total += totalS[v[1]][i][v[3]][v[4]][v[5]][v[6]][v[7]];
			res += probabilityHS[v[0]][v[1]][i][v[3]][v[4]][v[5]][v[6]][v[7]];
		}
		printf("Total %d\n", total);
		if (x <= total) return (double) res / (double) total;

		createGrafResult(v);
		sort(grafResult.begin(), grafResult.end());

		for (int i = 0; i < grafResult.size(); ++i)
		{
			total += totalS[v[1]][v[2]][grafResult[i].x.betsize][v[4]][grafResult[i].x.VPIP][grafResult[i].x.PFR][grafResult[i].x.AF];
			res += probabilityHS[v[0]][v[1]][v[2]][grafResult[i].x.betsize][v[4]][grafResult[i].x.VPIP][grafResult[i].x.PFR][grafResult[i].x.AF];

			printf("GENERALIZE: %d %d %d %d %d %d %d\n", v[1], v[2], grafResult[i].x.betsize, v[4], grafResult[i].x.VPIP, grafResult[i].x.PFR, grafResult[i].x.AF);
			if (x <= total) return (double) res / (double) total;
		}

		return -2.0;
	}


	double getProbabilityFE(int v[], int x)
	{
		int res = 0;
		int total = 0;

		total += totalFE[v[1]][v[2]][v[3]][v[4]][v[5]][v[6]][v[7]];
		res += probabilityFE[0][v[1]][v[2]][v[3]][v[4]][v[5]][v[6]][v[7]];

		if (x <= total) return (double) res / (double) total;
		for (int i = 0; i < PLAYER_STACK_SIZE_NUM; ++i)
		{
			if (i == v[1]) continue;
			total += totalFE[i][v[2]][v[3]][v[4]][v[5]][v[6]][v[7]];
			res += probabilityFE[0][i][v[2]][v[3]][v[4]][v[5]][v[6]][v[7]];
		}
		if (x <= total) return (double) res / (double) total;


		return -2.0;
	}
	double getProbabilityFE2(int v[], int x)
	{
		v[3] = 4;
		v[4] = 2;

		return getProbabilityFE(v, x);
	}

	double getProbabilityFE(double VPIP, double PFR, double AF, double stackSize, int line, double betsize, double bblind, double potcommon, int x)
	{
		int v[8];
		v[0] = 0;
		v[1] = normalizePotSize(2, potcommon, bblind);
		v[2] = normalizeStackSize(stackSize, bblind);
		v[3] = normalizeBetSize(2, betsize, potcommon, bblind);
		v[4] = line;
		v[5] = normalizeVPIP(VPIP);
		v[6] = normalizePFR(PFR);
		v[7] = normalizeAF(AF);

		if (line == 5)
			return getProbabilityFE2(v, x);

		return getProbabilityFE(v, x);
	}

	PlayerRange getRange(double VPIP, double PFR, double AF, double stackSize, int line, double betsize, double bblind, double potcommon, vector<Card>& cards, Hand own, int x)
	{
		int v[8];
		v[1] = normalizePotSize(2, potcommon, bblind);
		v[2] = normalizeStackSize(stackSize, bblind);
		v[3] = normalizeBetSize(2, betsize, potcommon, bblind);
		v[4] = line;
		v[5] = normalizeVPIP(VPIP);
		v[6] = normalizePFR(PFR);
		v[7] = normalizeAF(AF);

		return getRange(v, cards, own, x);
	}

	bool isRegFish(int v[])
	{
		return v[5] <= 2;
	}

	bool isRegular(int v[])
	{
		return v[5] <= 2 && !(v[5] == 2 && v[6] == 0) && !(v[5] == 2 && v[6] == 1);
	}

	PlayerRange getRange(int v[], vector<Card>& cards, Hand own, int x)
	{
		PlayerRange res;
		Logger& logger = Logger::getLogger(RANGE_LOGGER);

		double HS[10];
		for (int i = 0; i < HAND_STRENGTH_NUM; ++i)
		{
			v[0] = i;
			HS[i] = getProbabilityHS(v, x);

			if (HS[i] < 0)
			{
				memset(HS, 0, sizeof(HS));
				if (v[2] < 3)
				{
					HS[0] = 0.5;
					HS[1] = 0.5;
				}
				else
				{
					HS[0] = 1;
				}
				break;
			}
		}

		res = RangeUtils::createRange(8, HS, cards, own);

		return res;
	}
	PlayerRange getCallRaiseRange(double VPIP, double PFR, double AF, double stackSize, int line, double betsize, double bblind, double potcommon, vector<Card>& cards, Hand own, int x, bool allIn)
	{
		PlayerRange res;

		int nBetSize = normalizeBetSize(2, betsize, potcommon, bblind);

		int v[8];
		v[1] = normalizePotSize(2, potcommon, bblind);
		v[2] = normalizeStackSize(stackSize, bblind);
		v[3] = nBetSize;
		v[4] = line;
		v[5] = normalizeVPIP(VPIP);
		v[6] = normalizePFR(PFR);
		v[7] = normalizeAF(AF);

		PlayerRange callingRange = getRange(v, cards, own, x);
		
		if (!allIn)
		{
			PlayerRange raiseRangeTotal;

			v[4] = 1;
		
			int aktBetSize = nBetSize + 1;
			if (aktBetSize < 4) aktBetSize = 4;
			
			double effectiv = stackSize - potcommon / 3;
			int maxBetSize = 0;
			if (effectiv > potcommon * 1) maxBetSize = 4;
			if (effectiv > potcommon * 2) maxBetSize = 5;
			if (effectiv > potcommon * 4) maxBetSize = 6;
			if (effectiv > potcommon * 10) maxBetSize = 7;

			for (int i = aktBetSize; i < maxBetSize; ++i)
			{
				v[3] = i;
				PlayerRange raiseRange = getRange(v, cards, own, x);
				raiseRangeTotal = RangeUtils::addRange(raiseRangeTotal, raiseRange);
			}

			raiseRangeTotal = raiseRangeTotal.normalize();

			double weight = 0;

			if (AF <= 1)
			{
				weight = 0.9;
			}
			else if (AF <= 2)
			{
				weight = 0.8;
			}
			else if (AF <= 3)
			{
				weight = 0.7;
			}
			else if (AF <= 5)
			{
				weight = 0.65;
			}
			else
			{
				weight = 0.55;
			}


			callingRange = RangeUtils::addRange(callingRange, raiseRangeTotal, weight);
		}
		callingRange = callingRange.normalize();

		return callingRange;
	}

	PlayerRange getRaiseRange(double VPIP, double PFR, double AF, double stackSize, double betsize, double bblind, double potcommon, vector<Card>& cards, Hand own, int x)
	{
		Logger& logger = Logger::getLogger(BOT_LOGIC);

		int nBetSize = normalizeBetSize(2, betsize, potcommon, bblind);

		int v[8];
		v[1] = normalizePotSize(2, potcommon, bblind);
		v[2] = normalizeStackSize(stackSize, bblind);
		v[3] = nBetSize;
		v[4] = 1;
		v[5] = normalizeVPIP(VPIP);
		v[6] = normalizePFR(PFR);
		v[7] = normalizeAF(AF);
		
		PlayerRange raiseRange = getRange(v, cards, own, x);

		return raiseRange;
	}

	void printRange(int v[], int x)
	{
		cout << "Total situations = " << totalS[v[1]][v[2]][v[3]][v[4]][v[5]][v[6]][v[7]] << endl;
		double total = 0;
		for (int i = 0; i < HAND_STRENGTH_NUM; ++i)
		{
			v[0] = i;
			cout << "P(HS = " << i << ") = " << getProbabilityHS(v, 50) << endl;
			total += getProbabilityHS(v, 50);
		}
		printf("Total = %lf\n", total);
	}

private:
	int v[node_number];

	void backHS(FILE* f, bool b, int k)
	{
		if (k == node_number)
		{
			if (b)
			{
				fprintf(f,"%d ", totalS[v[2]][v[3]][v[4]][v[5]][v[6]][v[7]][v[8]]);
				for (int i = 0; i < HAND_STRENGTH_NUM; ++i)
				{
					fprintf(f,"%d ", probabilityHS[i][v[2]][v[3]][v[4]][v[5]][v[6]][v[7]][v[8]]);
				}
			}
			else
			{
				fscanf(f,"%d ", &totalS[v[2]][v[3]][v[4]][v[5]][v[6]][v[7]][v[8]]);
				if (totalS[v[2]][v[3]][v[4]][v[5]][v[6]][v[7]][v[8]] > 100000)
				{
					printf("%d %d %d %d %d %d %d\n", v[2], v[3], v[4], v[5], v[6], v[7], v[8]);
				}
				for (int i = 0; i < HAND_STRENGTH_NUM; ++i)
					fscanf(f,"%d ", &probabilityHS[i][v[2]][v[3]][v[4]][v[5]][v[6]][v[7]][v[8]]);
			}

			return ;
		}

		for (int i = 0; i < nums[k]; ++i)
		{
			v[k] = i;
			backHS(f, b, k + 1);
		}
	}

	void backFE(FILE* f, bool b, int k)
	{
		if (k == node_number)
		{
			if (b)
			{
				fprintf(f,"%d ", totalFE[v[2]][v[3]][v[4]][v[5]][v[6]][v[7]][v[8]]);
				for (int i = 0; i < FOLD_EQUITY_NUM; ++i)
				{
					fprintf(f,"%d ", probabilityFE[i][v[2]][v[3]][v[4]][v[5]][v[6]][v[7]][v[8]]);
				}
			}
			else
			{
				fscanf(f,"%d ", &totalFE[v[2]][v[3]][v[4]][v[5]][v[6]][v[7]][v[8]]);
				for (int i = 0; i < FOLD_EQUITY_NUM; ++i)
					fscanf(f,"%d ", &probabilityFE[i][v[2]][v[3]][v[4]][v[5]][v[6]][v[7]][v[8]]);
			}

			return ;
		}

		for (int i = 0; i < nums[k]; ++i)
		{
			v[k] = i;
			backFE(f, b, k + 1);
		}
	}
};
class BayesUserTurn : public BayesTurn
{
	static const int GRAF[5][8][8];

	struct node
	{
		int VPIP, PFR, AF;
		int betsize, flopPotSize;
	};

	struct edge
	{
		node base;
		node x;

		int weight()
		{
			int res = 0;
			res += BayesUserTurn::GRAF[0][base.VPIP][x.VPIP];
			res += BayesUserTurn::GRAF[1][base.PFR][x.PFR];
			res += BayesUserTurn::GRAF[2][base.AF][x.AF];
			res += BayesUserTurn::GRAF[3][base.betsize][x.betsize];
			res += BayesUserTurn::GRAF[4][base.flopPotSize][x.flopPotSize];

			return res;
		}

		bool operator <(edge& e)
		{
			return weight() < e.weight();
		}
	};

	vector<edge> grafResult;

public:
	BayesUserTurn()
	{
		nums[0] = HAND_STRENGTH_NUM;
		nums[1] = FOLD_EQUITY_NUM;
		nums[2] = POT_SIZE_NUM;
		nums[3] = PLAYER_STACK_SIZE_NUM;
		nums[4] = PLAYER_BET_SIZE_NUM;
		nums[5] = PLAYER_LINE_NUM;
		nums[6] = PLAYER_VPIP_NUM;
		nums[7] = PLAYER_PFR_NUM;
		nums[8] = PLAYER_AF_NUM;
		nums[9] = FLOP_POT_SIZE_NUM;

		memset(totalS, 0, sizeof(totalS));
		memset(probabilityHS, 0, sizeof(probabilityHS));
		memset(probabilityFE, 0, sizeof(probabilityFE));
	}

	void read(string filename)
	{
		FILE* f = fopen(filename.c_str(), "r");
		backHS(f, false, 2);
		backFE(f, false, 2);

		int totalS = 1;
		for (int i = 2; i < node_number; ++i)
			totalS *= nums[i];
		fclose(f);
	}

	void readNormalize(string filename)
	{
		FILE* f = fopen(filename.c_str(), "r");
		//
		int n;
		for (int iter = 0; iter < 5; ++iter)
		{
			scanf("%d", &n);
			for (int i = 0; i < n; ++i)
			{
				for (int j = 0; j < n; ++j)
				{
					scanf("%d", &GRAF[iter][i][j]);
				}
			}
		}

		fclose(f);
	}

	void createGrafResult(int v[])
	{
		grafResult.clear();
		
		node base;
		base.VPIP = v[5];
		base.PFR = v[6];
		base.AF = v[7];
		base.betsize = v[3];
		base.flopPotSize = v[8];

		for (int vpip = 0; vpip < PLAYER_VPIP_NUM; ++vpip)
		{
			for (int pfr = 0; pfr < PLAYER_PFR_NUM; ++pfr)
			{
				for (int af = 0; af < PLAYER_AF_NUM; ++af)
				{
					for (int betsize = 0; betsize < PLAYER_BET_SIZE_NUM; ++betsize)
					{
						for (int flopPotSize = 0; flopPotSize < FLOP_POT_SIZE_NUM; ++flopPotSize)
						{
							node x;
							x.VPIP = vpip;
							x.PFR = pfr;
							x.AF = af;
							x.betsize = betsize;
							x.flopPotSize = flopPotSize;

							edge akt;
							akt.base = base;
							akt.x = x;

							if (akt.weight() > 0) grafResult.push_back(akt);
						}
					}
				}
			}
		}
	}

	void write(string filename)
	{
		printf("Writing bayes network\n");
		FILE* f = fopen(filename.c_str(), "w");
		backHS(f, true, 2);
		backFE(f, true, 2);
		fclose(f);
	}

		/*
	ha < x minta!
	altalanositani: 
		-af szerint 8
		-stacksize szerint 3
		-pfr szerint 7
		-vpip szerint 6
	*/
	double getProbabilityHS(int v[], int x)
	{
		int res = 0;
		int total = 0;

		total += totalS[v[1]][v[2]][v[3]][v[4]][v[5]][v[6]][v[7]][v[8]];
		res += probabilityHS[v[0]][v[1]][v[2]][v[3]][v[4]][v[5]][v[6]][v[7]][v[8]];
		
		if (x <= total) return (double) res / (double) total;

		for (int i = 0; i < PLAYER_STACK_SIZE_NUM; ++i)
		{
			if (v[2] == i) continue;
			total += totalS[v[1]][i][v[3]][v[4]][v[5]][v[6]][v[7]][v[8]];
			res += probabilityHS[v[0]][v[1]][i][v[3]][v[4]][v[5]][v[6]][v[7]][v[8]];
		}

		if (x <= total) return (double) res / (double) total;

		createGrafResult(v);
		sort(grafResult.begin(), grafResult.end());

		for (int i = 0; i < grafResult.size(); ++i)
		{
			total += totalS[v[1]][v[2]][grafResult[i].x.betsize][v[4]][grafResult[i].x.VPIP][grafResult[i].x.PFR][grafResult[i].x.AF][grafResult[i].x.flopPotSize];
			res += probabilityHS[v[0]][v[1]][v[2]][grafResult[i].x.betsize][v[4]][grafResult[i].x.VPIP][grafResult[i].x.PFR][grafResult[i].x.AF][grafResult[i].x.flopPotSize];

			//printf("GENERALIZE: %d %d %d %d %d %d %d %d\n", v[1], v[2], grafResult[i].x.betsize, v[4], grafResult[i].x.VPIP, grafResult[i].x.PFR, grafResult[i].x.AF, grafResult[i].x.flopPotSize);
			if (x <= total) return (double) res / (double) total;
		}

		return -2.0;
	}


	double getProbabilityHS(int v[])
	{
		return (double)probabilityHS[v[0]][v[1]][v[2]][v[3]][v[4]][v[5]][v[6]][v[7]][v[8]] / (double)totalS[v[1]][v[2]][v[3]][v[4]][v[5]][v[6]][v[7]][v[8]];
	}

	double getProbabilityFE(int v[], int x)
	{
		int res = 0;
		int total = 0;

		total += totalFE[v[1]][v[2]][v[3]][v[4]][v[5]][v[6]][v[7]][v[8]];
		res += probabilityFE[0][v[1]][v[2]][v[3]][v[4]][v[5]][v[6]][v[7]][v[8]];

		if (x <= total) return (double) res / (double) total;
		
		for (int i = 0; i < PLAYER_STACK_SIZE_NUM; ++i)
		{
			if (i == v[1]) continue;
			total += totalFE[i][v[2]][v[3]][v[4]][v[5]][v[6]][v[7]][v[8]];
			res += probabilityFE[0][i][v[2]][v[3]][v[4]][v[5]][v[6]][v[7]][v[8]];
		}
		if (x <= total) return (double) res / (double) total;
		
		for (int i = 0; i < FLOP_POT_SIZE_NUM; ++i)
		{
			if (v[8] == i) continue;
			total += totalFE[v[1]][v[2]][v[3]][v[4]][v[5]][v[6]][v[7]][i];
			res += probabilityFE[0][v[1]][v[2]][v[3]][v[4]][v[5]][v[6]][v[7]][i];
		}
		if (x <= total) return (double) res / (double) total;

		return -2.0;
	}
	double getProbabilityFE2(int v[], int x)
	{
		v[3] = 4;
		v[4] = 2;

		return getProbabilityFE(v, x);
	}

	double getProbabilityFE(double VPIP, double PFR, double AF, double stackSize, int line, double betsize, double bblind, double potcommon, double flop_potcommon, int x)
	{
		int v[9];
		v[0] = 0;
		v[1] = normalizePotSize(3, potcommon, bblind);
		v[2] = normalizeStackSize(stackSize, bblind);
		v[3] = normalizeBetSize(3, betsize, potcommon, bblind);
		v[4] = line;
		v[5] = normalizeVPIP(VPIP);
		v[6] = normalizePFR(PFR);
		v[7] = normalizeAF(AF);
		v[8] = normalizePotSize(2, potcommon, bblind);


		if (line == 5)
			return getProbabilityFE2(v, x);

		return getProbabilityFE(v, x);
	}

	PlayerRange getRange(double VPIP, double PFR, double AF, double stackSize, int line, double betsize, double bblind, double potcommon, double flop_potcommon, vector<Card>& cards, Hand own, int x)
	{
		PlayerRange res;
		int v[9];
		v[1] = normalizePotSize(3, potcommon, bblind);
		v[2] = normalizeStackSize(stackSize, bblind);
		v[3] = normalizeBetSize(3, betsize, potcommon, bblind);
		v[4] = line;
		v[5] = normalizeVPIP(VPIP);
		v[6] = normalizePFR(PFR);
		v[7] = normalizeAF(AF);
		v[8] = normalizePotSize(2, flop_potcommon, bblind);

		return getRange(v, cards, own, x);
	}

	bool isRegFish(int v[])
	{
		return v[5] <= 2;
	}

	bool isRegular(int v[])
	{
		return v[5] <= 2 && !(v[5] == 2 && v[6] == 0);
	}

	PlayerRange getRange(int v[], vector<Card>& cards, Hand own, int x)
	{
		Logger& logger = Logger::getLogger(RANGE_LOGGER);

		PlayerRange res;

		double HS[10];
		for (int i = 0; i < HAND_STRENGTH_NUM; ++i)
		{
			v[0] = i;
			HS[i] = getProbabilityHS(v, x);
			//HACK
			if (HS[i] < 0)
			{
				memset(HS, 0, sizeof(HS));
				if (v[2] < 3)
				{
					HS[0] = 0.5;
					HS[1] = 0.5;
				}
				else
				{
					HS[0] = 1;
				}
				break;
			}
			//if (HS[i] < 0) return res;
		}

		res = RangeUtils::createRange(8, HS, cards, own);

		return res;
	}
	PlayerRange getCallRaiseRange(double VPIP, double PFR, double AF, double stackSize, int line, double betsize, double bblind, double potcommon, double flop_potcommon, vector<Card>& cards, Hand own, int x, bool allIn)
	{
		PlayerRange res;

		int nBetSize = normalizeBetSize(3, betsize, potcommon, bblind);

		int v[9];
		v[1] = normalizePotSize(3, potcommon, bblind);
		v[2] = normalizeStackSize(stackSize, bblind);
		v[3] = nBetSize;
		v[4] = line;
		v[5] = normalizeVPIP(VPIP);
		v[6] = normalizePFR(PFR);
		v[7] = normalizeAF(AF);
		v[8] = normalizePotSize(2, flop_potcommon, bblind);

		PlayerRange callingRange = getRange(v, cards, own, x);
		
		if (!allIn)
		{
			PlayerRange raiseRangeTotal;

			v[4] = 1;
		
			int aktBetSize = nBetSize + 1;
			if (aktBetSize < 4) aktBetSize = 4;
			
			double effectiv = stackSize - potcommon / 3;
			int maxBetSize = 0;
			if (effectiv > potcommon * 1) maxBetSize = 4;
			if (effectiv > potcommon * 2) maxBetSize = 5;
			if (effectiv > potcommon * 4) maxBetSize = 6;
			if (effectiv > potcommon * 10) maxBetSize = 7;

			for (int i = aktBetSize; i < maxBetSize; ++i)
			{
				v[3] = i;
				PlayerRange raiseRange = getRange(v, cards, own, x);
				raiseRangeTotal = RangeUtils::addRange(raiseRangeTotal, raiseRange);
			}

			raiseRangeTotal = raiseRangeTotal.normalize();

			double weight = 0;

			if (AF <= 1)
			{
				weight = 0.9;
			}
			else if (AF <= 2)
			{
				weight = 0.80;
			}
			else if (AF <= 3)
			{
				weight = 0.75;
			}
			else if (AF <= 5)
			{
				weight = 0.65;
			}
			else
			{
				weight = 0.6;
			}


			callingRange = RangeUtils::addRange(callingRange, raiseRangeTotal, weight);
		}
		callingRange = callingRange.normalize();

		return callingRange;
	}

	PlayerRange getRaiseRange(double VPIP, double PFR, double AF, double stackSize, double betsize, double bblind, double potcommon, double flop_potcommon, vector<Card>& cards, Hand own, int x)
	{
		int nBetSize = normalizeBetSize(3, betsize, potcommon, bblind);

		int v[9];
		v[1] = normalizePotSize(3, potcommon, bblind);
		v[2] = normalizeStackSize(stackSize, bblind);
		v[3] = nBetSize;
		v[4] = 1;
		v[5] = normalizeVPIP(VPIP);
		v[6] = normalizePFR(PFR);
		v[7] = normalizeAF(AF);
		v[8] = normalizePotSize(2, flop_potcommon, bblind);
		
		PlayerRange raiseRange = getRange(v, cards, own, x);

		return raiseRange;
	}

	void printRange(int v[])
	{
		cout << "Total situations = " << totalS[v[1]][v[2]][v[3]][v[4]][v[5]][v[6]][v[7]] << endl;
		for (int i = 0; i < HAND_STRENGTH_NUM; ++i)
		{
			v[0] = i;
			cout << "P(HS = " << i << ") = " << getProbabilityHS(v) << endl;
		}
	}

private:
	int v[node_number];

	void backHS(FILE* f, bool b, int k)
	{
		if (k == node_number)
		{
			if (b)
			{
				fprintf(f,"%d ", totalS[v[2]][v[3]][v[4]][v[5]][v[6]][v[7]][v[8]][v[9]]);
				for (int i = 0; i < HAND_STRENGTH_NUM; ++i)
				{
					fprintf(f,"%d ", probabilityHS[i][v[2]][v[3]][v[4]][v[5]][v[6]][v[7]][v[8]][v[9]]);
				}
			}
			else
			{
				fscanf(f,"%d ", &totalS[v[2]][v[3]][v[4]][v[5]][v[6]][v[7]][v[8]][v[9]]);
				if (v[2] == 3 && v[4] == 3 && totalS[v[2]][v[3]][v[4]][v[5]][v[6]][v[7]][v[8]][v[9]] > 50)
				{
					//printf("%d ", totalS[v[2]][v[3]][v[4]][v[5]][v[6]][v[7]][v[8]][v[9]]);
					//printf("%d %d %d %d %d %d %d %d\n", v[2], v[3], v[4], v[5], v[6], v[7], v[8], v[9]);
				}
				for (int i = 0; i < HAND_STRENGTH_NUM; ++i)
				{
					fscanf(f,"%d ", &probabilityHS[i][v[2]][v[3]][v[4]][v[5]][v[6]][v[7]][v[8]][v[9]]);
					if (v[2] == 3 && v[4] == 3 && totalS[v[2]][v[3]][v[4]][v[5]][v[6]][v[7]][v[8]][v[9]] > 50)
					{
						//printf("HS(%d) = %d\n", i, probabilityHS[i][v[2]][v[3]][v[4]][v[5]][v[6]][v[7]][v[8]][v[9]]);
					}
				}
			}

			return ;
		}

		for (int i = 0; i < nums[k]; ++i)
		{
			v[k] = i;
			backHS(f, b, k + 1);
		}
	}

	void backFE(FILE* f, bool b, int k)
	{
		if (k == node_number)
		{
			if (b)
			{
				fprintf(f,"%d ", totalFE[v[2]][v[3]][v[4]][v[5]][v[6]][v[7]][v[8]][v[9]]);
				for (int i = 0; i < FOLD_EQUITY_NUM; ++i)
				{
					fprintf(f,"%d ", probabilityFE[i][v[2]][v[3]][v[4]][v[5]][v[6]][v[7]][v[8]][v[9]]);
				}
			}
			else
			{
				fscanf(f,"%d ", &totalFE[v[2]][v[3]][v[4]][v[5]][v[6]][v[7]][v[8]][v[9]]);
				for (int i = 0; i < FOLD_EQUITY_NUM; ++i)
					fscanf(f,"%d ", &probabilityFE[i][v[2]][v[3]][v[4]][v[5]][v[6]][v[7]][v[8]][v[9]]);
			}

			return ;
		}

		for (int i = 0; i < nums[k]; ++i)
		{
			v[k] = i;
			backFE(f, b, k + 1);
		}
	}

};
class BayesUserRiver : public BayesRiver
{
	static const int GRAF[5][8][8];

	struct node
	{
		int VPIP, PFR, AF;
		int betsize, flopPotSize;
	};

	struct edge
	{
		node base;
		node x;

		int weight()
		{
			int res = 0;
			res += BayesUserRiver::GRAF[0][base.VPIP][x.VPIP];
			res += BayesUserRiver::GRAF[1][base.PFR][x.PFR];
			res += BayesUserRiver::GRAF[2][base.AF][x.AF];
			res += BayesUserRiver::GRAF[3][base.betsize][x.betsize];
			res += BayesUserRiver::GRAF[4][base.flopPotSize][x.flopPotSize];

			return res;
		}

		bool operator <(edge& e)
		{
			return weight() < e.weight();
		}
	};

	vector<edge> grafResult;

public:
	BayesUserRiver()
	{
		nums[0] = HAND_STRENGTH_NUM;
		nums[1] = FOLD_EQUITY_NUM;
		nums[2] = POT_SIZE_NUM;
		nums[3] = PLAYER_STACK_SIZE_NUM;
		nums[4] = PLAYER_BET_SIZE_NUM;
		nums[5] = PLAYER_LINE_NUM;
		nums[6] = PLAYER_VPIP_NUM;
		nums[7] = PLAYER_PFR_NUM;
		nums[8] = PLAYER_AF_NUM;
		nums[9] = FLOP_POT_SIZE_NUM;

		memset(totalS, 0, sizeof(totalS));
		memset(probabilityHS, 0, sizeof(probabilityHS));
		memset(probabilityFE, 0, sizeof(probabilityFE));
	}

	void read(string filename)
	{
		FILE* f = fopen(filename.c_str(), "r");
		backHS(f, false, 2);
		backFE(f, false, 2);

		int totalS = 1;
		for (int i = 2; i < node_number; ++i)
			totalS *= nums[i];
		fclose(f);
	}

	void readNormalize(string filename)
	{
		FILE* f = fopen(filename.c_str(), "r");
		//
		int n;
		for (int iter = 0; iter < 5; ++iter)
		{
			scanf("%d", &n);
			for (int i = 0; i < n; ++i)
			{
				for (int j = 0; j < n; ++j)
				{
					scanf("%d", &GRAF[iter][i][j]);
				}
			}
		}

		fclose(f);
	}

	void createGrafResult(int v[])
	{
		grafResult.clear();
		
		node base;
		base.VPIP = v[5];
		base.PFR = v[6];
		base.AF = v[7];
		base.betsize = v[3];
		base.flopPotSize = v[8];

		for (int vpip = 0; vpip < PLAYER_VPIP_NUM; ++vpip)
		{
			for (int pfr = 0; pfr < PLAYER_PFR_NUM; ++pfr)
			{
				for (int af = 0; af < PLAYER_AF_NUM; ++af)
				{
					for (int betsize = 0; betsize < PLAYER_BET_SIZE_NUM; ++betsize)
					{
						for (int flopPotSize = 0; flopPotSize < FLOP_POT_SIZE_NUM; ++flopPotSize)
						{
							node x;
							x.VPIP = vpip;
							x.PFR = pfr;
							x.AF = af;
							x.betsize = betsize;
							x.flopPotSize = flopPotSize;

							edge akt;
							akt.base = base;
							akt.x = x;

							if (akt.weight() > 0) grafResult.push_back(akt);
						}
					}
				}
			}
		}
	}

	void write(string filename)
	{
		printf("Writing bayes network\n");
		FILE* f = fopen(filename.c_str(), "w");
		backHS(f, true, 2);
		backFE(f, true, 2);
		fclose(f);
	}

	double getProbabilityHS(int v[], int x)
	{
		int res = 0;
		int total = 0;

		total += totalS[v[1]][v[2]][v[3]][v[4]][v[5]][v[6]][v[7]][v[8]];
		res += probabilityHS[v[0]][v[1]][v[2]][v[3]][v[4]][v[5]][v[6]][v[7]][v[8]];
		
		printf("X= %d, Total= %d\n", x, total);
		if (x <= total) return (double) res / (double) total;
		
		for (int i = 0; i < PLAYER_STACK_SIZE_NUM; ++i)
		{
			if (v[2] == i) continue;
			total += totalS[v[1]][i][v[3]][v[4]][v[5]][v[6]][v[7]][v[8]];
			res += probabilityHS[v[0]][v[1]][i][v[3]][v[4]][v[5]][v[6]][v[7]][v[8]];
		}
		printf("X= %d, Total= %d\n", x, total);
		if (x <= total) return (double) res / (double) total;

		createGrafResult(v);
		sort(grafResult.begin(), grafResult.end());

		for (int i = 0; i < grafResult.size(); ++i)
		{
			total += totalS[v[1]][v[2]][grafResult[i].x.betsize][v[4]][grafResult[i].x.VPIP][grafResult[i].x.PFR][grafResult[i].x.AF][grafResult[i].x.flopPotSize];
			res += probabilityHS[v[0]][v[1]][v[2]][grafResult[i].x.betsize][v[4]][grafResult[i].x.VPIP][grafResult[i].x.PFR][grafResult[i].x.AF][grafResult[i].x.flopPotSize];

			printf("GENERALIZE: %d %d %d %d %d %d %d %d\n", v[1], v[2], grafResult[i].x.betsize, v[4], grafResult[i].x.VPIP, grafResult[i].x.PFR, grafResult[i].x.AF, grafResult[i].x.flopPotSize);
			if (x <= total) return (double) res / (double) total;
		}

		return -2.0;
	}

	double getProbabilityFE(double VPIP, double PFR, double AF, double stackSize, int line, double betsize, double bblind, double potcommon, double flop_potcommon, int x)
	{
		int v[8];
		v[0] = 0;
		v[1] = normalizePotSize(4, potcommon, bblind);
		v[2] = normalizeStackSize(stackSize, bblind);
		v[3] = normalizeBetSize(4, betsize, potcommon, bblind);
		v[4] = line;
		v[5] = normalizeVPIP(VPIP);
		v[6] = normalizePFR(PFR);
		v[7] = normalizeAF(AF);
		v[8] = normalizePotSize(2, flop_potcommon, bblind);

		if (line == 5)
			return getProbabilityFE2(v, x);

		return getProbabilityFE(v, x);
	}

	PlayerRange getRange(double VPIP, double PFR, double AF, double stackSize, int line, double betsize, double bblind, double potcommon, double flop_potcommon, vector<Card>& cards, Hand own, int x)
	{
		PlayerRange res;
		int v[8];
		v[1] = normalizePotSize(4, potcommon, bblind);
		v[2] = normalizeStackSize(stackSize, bblind);
		v[3] = normalizeBetSize(4, betsize, potcommon, bblind);
		v[4] = line;
		v[5] = normalizeVPIP(VPIP);
		v[6] = normalizePFR(PFR);
		v[7] = normalizeAF(AF);
		v[8] = normalizePotSize(2, flop_potcommon, bblind);

		return getRange(v, cards, own, x);
	}

	bool isRegFish(int v[])
	{
		return v[5] <= 2;
	}

	bool isRegular(int v[])
	{
		return v[5] <= 2 && !(v[5] == 2 && v[6] == 0);
	}

	PlayerRange getRange(int v[], vector<Card>& cards, Hand own, int x)
	{
		Logger& logger = Logger::getLogger(RANGE_LOGGER);

		printf("GETTING RANGE\n");
		PlayerRange res;

		double HS[10];
		for (int i = 0; i < HAND_STRENGTH_NUM; ++i)
		{
			v[0] = i;
			HS[i] = getProbabilityHS(v, x);
			printf("HS(%d) = %lf\n", i, HS[i]);
			//HACK
			if (HS[i] < 0)
			{
				memset(HS, 0, sizeof(HS));
				if (v[2] < 3)
				{
					HS[0] = 0.5;
					HS[1] = 0.5;
				}
				else
				{
					HS[0] = 1;
				}
				break;
			}
			//if (HS[i] < 0) return res;
		}

		res = RangeUtils::createRange(8, HS, cards, own);

		if (v[8] <= 4 && isRegular(v) && v[3] > 3 && v[2] > 2)
		{
			res = RangeUtils::justNutsRange(res, cards);
		}

		return res;
	}

	PlayerRange getCallRaiseRange(double VPIP, double PFR, double AF, double stackSize, int line, double betsize, double bblind, double potcommon, double flop_potcommon, vector<Card>& cards, Hand own, int x, bool allIn = false)
	{
		PlayerRange res;

		int nBetSize = normalizeBetSize(2, betsize, potcommon, bblind);

		int v[8];
		v[1] = normalizePotSize(4, potcommon, bblind);
		v[2] = normalizeStackSize(stackSize, bblind);
		v[3] = nBetSize;
		v[4] = line;
		v[5] = normalizeVPIP(VPIP);
		v[6] = normalizePFR(PFR);
		v[7] = normalizeAF(AF);
		v[8] = normalizePotSize(2, flop_potcommon, bblind);

		PlayerRange callingRange = getRange(v, cards, own, x);
		
		if (!allIn)
		{
			PlayerRange raiseRangeTotal;

			v[4] = 1;
		
			int aktBetSize = nBetSize + 1;
			if (aktBetSize < 4) aktBetSize = 4;

			double effectiv = stackSize - potcommon / 3;
			int maxBetSize = 0;
			if (effectiv > potcommon * 1) maxBetSize = 4;
			if (effectiv > potcommon * 2) maxBetSize = 5;
			if (effectiv > potcommon * 4) maxBetSize = 6;
			if (effectiv > potcommon * 10) maxBetSize = 7;

			for (int i = aktBetSize; i < maxBetSize; ++i)
			{
				v[3] = i;
				PlayerRange raiseRange = getRange(v, cards, own, x);
				raiseRangeTotal = RangeUtils::addRange(raiseRangeTotal, raiseRange);
			}

			raiseRangeTotal = raiseRangeTotal.normalize();

			double weight = 0;

			if (AF <= 1)
			{
				weight = 0.90;
			}
			else if (AF <= 2)
			{
				weight = 0.80;
			}
			else if (AF <= 3)
			{
				weight = 0.75;
			}
			else if (AF <= 5)
			{
				weight = 0.70;
			}
			else
			{
				weight = 0.70;
			}

			callingRange = RangeUtils::addRange(callingRange, raiseRangeTotal, weight);
		}

		callingRange = callingRange.normalize();
		return callingRange;
	}

	PlayerRange getRaiseRange(double VPIP, double PFR, double AF, double stackSize, double betsize, double bblind, double potcommon, double flop_potcommon, vector<Card>& cards, Hand own, int x)
	{
		int nBetSize = normalizeBetSize(4, betsize, potcommon, bblind);

		int v[8];
		v[1] = normalizePotSize(4, potcommon, bblind);
		v[2] = normalizeStackSize(stackSize, bblind);
		v[3] = nBetSize;
		v[4] = 1;
		v[5] = normalizeVPIP(VPIP);
		v[6] = normalizePFR(PFR);
		v[7] = normalizeAF(AF);
		v[8] = normalizePotSize(2, flop_potcommon, bblind);
		
		PlayerRange raiseRange = getRange(v, cards, own, x);
		return raiseRange;
	}

	double getProbabilityHS(int v[])
	{
		return (double)probabilityHS[v[0]][v[1]][v[2]][v[3]][v[4]][v[5]][v[6]][v[7]][v[8]] / (double)totalS[v[1]][v[2]][v[3]][v[4]][v[5]][v[6]][v[7]][v[8]];
	}

	double getProbabilityFE(int v[], int x)
	{
		int res = 0;
		int total = 0;

		total += totalFE[v[1]][v[2]][v[3]][v[4]][v[5]][v[6]][v[7]][v[8]];
		res += probabilityFE[0][v[1]][v[2]][v[3]][v[4]][v[5]][v[6]][v[7]][v[8]];

		printf("Total = %d\n", total);

		if (x <= total) return (double) res / (double) total;

		for (int i = 0; i < PLAYER_STACK_SIZE_NUM; ++i)
		{
			if (i == v[1]) continue;
			total += totalFE[i][v[2]][v[3]][v[4]][v[5]][v[6]][v[7]][v[8]];
			res += probabilityFE[0][i][v[2]][v[3]][v[4]][v[5]][v[6]][v[7]][v[8]];
		}
		if (x <= total) return (double) res / (double) total;

		for (int i = 0; i < FLOP_POT_SIZE_NUM; ++i)
		{
			if (v[8] == i) continue;
			total += totalFE[v[1]][v[2]][v[3]][v[4]][v[5]][v[6]][v[7]][i];
			res += probabilityFE[0][v[1]][v[2]][v[3]][v[4]][v[5]][v[6]][v[7]][i];
		}
		if (x <= total) return (double) res / (double) total;


		return -2.0;
	}
	double getProbabilityFE2(int v[], int x)
	{
		v[3] = 4;
		v[4] = 2;

		return getProbabilityFE(v, x);
	}

	double getNormProbabilityHS(int v[])
	{
		//normalize!!
		return (double)probabilityHS[v[0]][v[1]][v[2]][v[3]][v[4]][v[5]][v[6]][v[7]][v[8]] / (double)totalS[v[1]][v[2]][v[3]][v[4]][v[5]][v[6]][v[7]][v[8]];
	}

	double getNormProbabilityFE(int v[])
	{
		//normalize!!
		return (double)probabilityFE[0][v[1]][v[2]][v[3]][v[4]][v[5]][v[6]][v[7]][v[8]] / (double)totalFE[v[1]][v[2]][v[3]][v[4]][v[5]][v[6]][v[7]][v[8]];
	}

	void printRange(int v[])
	{
		cout << "Total situations = " << totalS[v[1]][v[2]][v[3]][v[4]][v[5]][v[6]][v[7]][v[8]] << endl;
		for (int i = 0; i < HAND_STRENGTH_NUM; ++i)
		{
			v[0] = i;
			cout << "P(HS = " << i << ") = " << getProbabilityHS(v) << endl;
		}
	}

private:
	int v[node_number];

	void backHS(FILE* f, bool b, int k)
	{
		if (k == node_number)
		{
			if (b)
			{
				fprintf(f,"%d ", totalS[v[2]][v[3]][v[4]][v[5]][v[6]][v[7]][v[8]][v[9]]);
				for (int i = 0; i < HAND_STRENGTH_NUM; ++i)
				{
					fprintf(f,"%d ", probabilityHS[i][v[2]][v[3]][v[4]][v[5]][v[6]][v[7]][v[8]][v[9]]);
				}
			}
			else
			{
				fscanf(f,"%d ", &totalS[v[2]][v[3]][v[4]][v[5]][v[6]][v[7]][v[8]][v[9]]);
				//printf("%d ", totalS[v[2]][v[3]][v[4]][v[5]][v[6]][v[7]][v[8]][v[9]]);
				for (int i = 0; i < HAND_STRENGTH_NUM; ++i)
				{
					fscanf(f,"%d ", &probabilityHS[i][v[2]][v[3]][v[4]][v[5]][v[6]][v[7]][v[8]][v[9]]);
					//printf("%d ", probabilityHS[i][v[2]][v[3]][v[4]][v[5]][v[6]][v[7]][v[8]][v[9]]);
				}
			}

			return ;
		}

		for (int i = 0; i < nums[k]; ++i)
		{
			v[k] = i;
			backHS(f, b, k + 1);
		}
	}

	void backFE(FILE* f, bool b, int k)
	{
		if (k == node_number)
		{
			if (b)
			{
				fprintf(f,"%d ", totalFE[v[2]][v[3]][v[4]][v[5]][v[6]][v[7]][v[8]][v[9]]);
				for (int i = 0; i < FOLD_EQUITY_NUM; ++i)
				{
					fprintf(f,"%d ", probabilityFE[i][v[2]][v[3]][v[4]][v[5]][v[6]][v[7]][v[8]][v[9]]);
				}
			}
			else
			{
				fscanf(f,"%d ", &totalFE[v[2]][v[3]][v[4]][v[5]][v[6]][v[7]][v[8]][v[9]]);
				for (int i = 0; i < FOLD_EQUITY_NUM; ++i)
					fscanf(f,"%d ", &probabilityFE[i][v[2]][v[3]][v[4]][v[5]][v[6]][v[7]][v[8]][v[9]]);
			}

			return ;
		}

		for (int i = 0; i < nums[k]; ++i)
		{
			v[k] = i;
			backFE(f, b, k + 1);
		}
	}

};