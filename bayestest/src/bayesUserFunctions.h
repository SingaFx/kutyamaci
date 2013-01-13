#pragma once
#include <string>
#include <vector>
#include "handHistoryParser.h"
#include "rangeFunctions.h"

using namespace std;

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

class BayesPreflop// : public Bayes
{
public:
	FILE* logfile;

	enum preflop
	{
		PREFLOP_HAND_STRENGTH = 0,
		PREFLOP_PLAYER_STACK_SIZE = 1,
		PREFLOP_PLAYER_BET_SIZE = 2,
		PREFLOP_PLAYER_LINE = 3,
		PREFLOP_PLAYER_VPIP = 4,
		PREFLOP_PLAYER_PFR = 5,
		PREFLOP_PLAYER_POZ = 6
	};

	static const int PREFLOP_HAND_STRENGTH_NUM = 170;
	static const int PREFLOP_PLAYER_STACK_SIZE_NUM = 5;
	static const int PREFLOP_PLAYER_BET_SIZE_NUM = 10;
	static const int PREFLOP_PLAYER_LINE_NUM = 2;
	static const int PREFLOP_PLAYER_VPIP_NUM = 5;
	static const int PREFLOP_PLAYER_PFR_NUM = 3;
	static const int PREFLOP_PLAYER_POZ_NUM = 6;

	static const int preflop_node_number = 7;
	int preflop_nums[preflop_node_number];

	int probabilityPreflop[PREFLOP_HAND_STRENGTH_NUM][PREFLOP_PLAYER_STACK_SIZE_NUM][PREFLOP_PLAYER_BET_SIZE_NUM][PREFLOP_PLAYER_LINE_NUM][PREFLOP_PLAYER_VPIP_NUM][PREFLOP_PLAYER_PFR_NUM][PREFLOP_PLAYER_POZ_NUM];
	int totalPreflop[PREFLOP_PLAYER_STACK_SIZE_NUM][PREFLOP_PLAYER_BET_SIZE_NUM][PREFLOP_PLAYER_LINE_NUM][PREFLOP_PLAYER_VPIP_NUM][PREFLOP_PLAYER_PFR_NUM][PREFLOP_PLAYER_POZ_NUM];

public:
	BayesPreflop()
	{
		preflop_nums[0] = PREFLOP_HAND_STRENGTH_NUM;//, PREFLOP_PLAYER_STACK_SIZE_NUM, PREFLOP_PLAYER_BET_SIZE_NUM, PREFLOP_PLAYER_LINE_NUM, PREFLOP_PLAYER_VPIP_NUM, PREFLOP_PLAYER_PFR_NUM, PREFLOP_PLAYER_POZ_NUM};
		preflop_nums[1] = PREFLOP_PLAYER_STACK_SIZE_NUM;
		preflop_nums[2] = PREFLOP_PLAYER_BET_SIZE_NUM;
		preflop_nums[3] = PREFLOP_PLAYER_LINE_NUM;
		preflop_nums[4] = PREFLOP_PLAYER_VPIP_NUM;
		preflop_nums[5] = PREFLOP_PLAYER_PFR_NUM;
		preflop_nums[6] = PREFLOP_PLAYER_POZ_NUM;

		logfile = fopen("logfile", "w");
		memset(totalPreflop, 0, sizeof(totalPreflop));
		memset(probabilityPreflop, 0, sizeof(probabilityPreflop));
		totalSituation = 0;
		totalLittle = 0;
	}

	void read(string filename)
	{
		FILE* f = fopen(filename.c_str(), "r");
		back(f, false, 1);
		fprintf(logfile,"Total = %d\n", totalSituation);
		fprintf(logfile,"Total few = %d\n", totalLittle);
		int totalS = 1;
		for (int i = 1; i < preflop_node_number; ++i)
			totalS *= preflop_nums[i];
		fprintf(logfile,"Total situation = %d\n", totalS);
		fclose(f);
	}

	void write(string filename)
	{
		FILE* f = fopen(filename.c_str(), "w");
		back(f, true, 1);
		fclose(f);
	}
	double getProbability(int v[])
	{

		if (totalPreflop[v[1]][v[2]][v[3]][v[4]][v[5]][v[6]] == 0)
		{
			return -2.0;
		}
		return (double)probabilityPreflop[v[0]][v[1]][v[2]][v[3]][v[4]][v[5]][v[6]] / (double)totalPreflop[v[1]][v[2]][v[3]][v[4]][v[5]][v[6]];
	}

	/*
	ha < x minta!
	altalanositani: 
		-poz szerint 6
		-stacksize szerint 1
		-pfr szerint 5
		-vpip szerint 4
	*/
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
		
		//pfr szerint
		for (int i = 0; i < PREFLOP_PLAYER_PFR_NUM; ++i)
		{
			if (i == v[5]) continue;
			total += totalPreflop[v[1]][v[2]][v[3]][v[4]][i][v[6]];
			res += probabilityPreflop[v[0]][v[1]][v[2]][v[3]][v[4]][i][v[6]];
		}
		if (x <= total) return (double) res / (double) total;

		//vpip szerint
		for (int i = 0; i < PREFLOP_PLAYER_VPIP_NUM; ++i)
		{
			if (i == v[4]) continue;
			total += totalPreflop[v[1]][v[2]][v[3]][i][v[5]][v[6]];
			res += probabilityPreflop[v[0]][v[1]][v[2]][v[3]][i][v[5]][v[6]];
		}
		if (x <= total) return (double) res / (double) total;

		return -2.0;
	}

	int preflopHandType(Hand hand)
	{
		int rank1 = convertRankToNumbers(hand.card1);
		int rank2 = convertRankToNumbers(hand.card2);
		char suit1 = hand.card1.suit;
		char suit2 = hand.card2.suit;

		if (rank1 < rank2)
		{
			swap(rank1, rank2);
		}

		if (hand.card1.rank == hand.card2.rank)
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
		//FOLD nincs hozzaadva!
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
				hand.card1.rank = numberToCard(i);
				hand.card2.rank = numberToCard(j);

				for (int k = 0; k < 4; ++k)
				{
					hand.card1.suit = map[k];
					hand.card2.suit = map[k];
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
				hand.card1.rank = numberToCard(i);
				hand.card2.rank = numberToCard(j);

				for (int k1 = 0; k1 < 4; ++k1)
				{
					for (int k2 = 0; k2 < 4; ++k2)
					{
						if (k1 >= k2) continue;
						hand.card1.suit = map[k1];
						hand.card2.suit = map[k2];
						int type = preflopHandType(hand);
						types[type]++;
						ptypes[type] += res / 6;
					}
				}						
			}

		for (int i = 2; i < 15; ++i)
			for (int j = i + 1; j < 15; ++j)
			{
				hand.card1.rank = numberToCard(i);
				hand.card2.rank = numberToCard(j);

				for (int k = 0; k < 4; ++k)
				{
					hand.card1.suit = map[k];
					hand.card2.suit = map[k];
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
				hand.card1.rank = numberToCard(i);
				hand.card2.rank = numberToCard(j);

				for (int k1 = 0; k1 < 4; ++k1)
				{
					for (int k2 = 0; k2 < 4; ++k2)
					{
						if (k1 >= k2) continue;
						hand.card1.suit = map[k1];
						hand.card2.suit = map[k2];
						int type = preflopHandType(hand);
						double akt = ptypes[type]/(double)types[type];
						if (akt > 0)
							result.range.insert(make_pair(hand, akt));
					}
				}										
			}

		return result;
	}

	void printRange(int v[])
	{
		//suited
		int count = 0;
		for (int i = 2; i < 15; ++i)
			for (int j = i + 1; j < 15; ++j)
			{
				++count;
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
				v[0] = count;
				double res;
				if ((res = getProbability(v)) > 0)
					cout << numberToCard(i) << numberToCard(j) << "o = " << res << endl;
			}
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

	double getNormProbability(int v[])
	{
		//normalize!!
		return (double)probabilityPreflop[v[0]][v[1]][v[2]][v[3]][v[4]][v[5]][v[6]] / (double)totalPreflop[v[1]][v[2]][v[3]][v[4]][v[5]][v[6]];
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
				totalSituation += totalPreflop[v[1]][v[2]][v[3]][v[4]][v[5]][v[6]];
				if (totalPreflop[v[1]][v[2]][v[3]][v[4]][v[5]][v[6]] < 10)
				{
					if (v[4] >= v[5]) 
					{
						totalLittle++;
						fprintf(logfile,"Situation %d %d %d %d %d %d\n", v[1], v[2], v[3], v[4], v[5], v[6]);
						fprintf(logfile, "%d\n", totalPreflop[v[1]][v[2]][v[3]][v[4]][v[5]][v[6]]);
					}
					else
					{
						if (totalPreflop[v[1]][v[2]][v[3]][v[4]][v[5]][v[6]] > 0)
						{
							fprintf(logfile,"ERROR Situation %d %d %d %d %d %d\n", v[1], v[2], v[3], v[4], v[5], v[6]);
							fprintf(logfile, "%d\n", totalPreflop[v[1]][v[2]][v[3]][v[4]][v[5]][v[6]]);
						}
					}
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
};
class BayesFlop
{
public:
	FILE* logfile;
	//Database* database;

	enum node
	{
		HAND_STRENGTH = 0,
		FOLD_EQUITY = 1,
		POT_SIZE = 2,
		PLAYER_STACK_SIZE = 3,
		PLAYER_BET_SIZE = 4,
		PLAYER_LINE = 5,
		PLAYER_VPIP = 6,
		PLAYER_PFR = 7,
		PLAYER_AF = 8,
	};

	static const int HAND_STRENGTH_NUM = 9;
	static const int FOLD_EQUITY_NUM = 1;
	static const int POT_SIZE_NUM = 5; 
	static const int PLAYER_STACK_SIZE_NUM = 5;
	static const int PLAYER_BET_SIZE_NUM = 6; 
	static const int PLAYER_LINE_NUM = 3;
	static const int PLAYER_VPIP_NUM = 5;
	static const int PLAYER_PFR_NUM = 3;
	static const int PLAYER_AF_NUM = 3;

	static const int node_number = 9;
	int nums[node_number];

	int probabilityHS[HAND_STRENGTH_NUM][POT_SIZE_NUM][PLAYER_STACK_SIZE_NUM][PLAYER_BET_SIZE_NUM][PLAYER_LINE_NUM][PLAYER_VPIP_NUM][PLAYER_PFR_NUM][PLAYER_AF_NUM];
	int probabilityFE[FOLD_EQUITY_NUM][POT_SIZE_NUM][PLAYER_STACK_SIZE_NUM][PLAYER_BET_SIZE_NUM][PLAYER_LINE_NUM][PLAYER_VPIP_NUM][PLAYER_PFR_NUM][PLAYER_AF_NUM];
	int totalS[POT_SIZE_NUM][PLAYER_STACK_SIZE_NUM][PLAYER_BET_SIZE_NUM][PLAYER_LINE_NUM][PLAYER_VPIP_NUM][PLAYER_PFR_NUM][PLAYER_AF_NUM];
	int totalFE[POT_SIZE_NUM][PLAYER_STACK_SIZE_NUM][PLAYER_BET_SIZE_NUM][PLAYER_LINE_NUM][PLAYER_VPIP_NUM][PLAYER_PFR_NUM][PLAYER_AF_NUM];

public:
	BayesFlop()
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

		//database = new Database("127.0.0.1", "root", "root", "kutya");
		logfile = fopen("flopbayeslog", "w");
		memset(totalS, 0, sizeof(totalS));
		memset(probabilityHS, 0, sizeof(probabilityHS));
		memset(probabilityFE, 0, sizeof(probabilityFE));
	}

	void read(string filename)
	{
		FILE* f = fopen(filename.c_str(), "r");
		backHS(f, false, 2);
		backFE(f, false, 2);

		//fprintf(logfile,"Total = %d\n", totalSituation);
		//fprintf(logfile,"Total few = %d\n", totalLittle);
		int totalS = 1;
		for (int i = 2; i < node_number; ++i)
			totalS *= nums[i];
		fprintf(logfile,"Total situation = %d\n", totalS);
		fclose(f);
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
		//(double)probabilityHS[v[0]][v[1]][v[2]][v[3]][v[4]][v[5]][v[6]][v[7]] / (double)totalS[v[1]][v[2]][v[3]][v[4]][v[5]][v[6]][v[7]];

		total += totalS[v[1]][v[2]][v[3]][v[4]][v[5]][v[6]][v[7]];
		res += probabilityHS[v[0]][v[1]][v[2]][v[3]][v[4]][v[5]][v[6]][v[7]];
		
		if (x <= total) return (double) res / (double) total;
		//af szerint
		for (int i = 0; i < PLAYER_AF_NUM; ++i)
		{
			if (i == v[7]) continue;
			total += totalS[v[1]][v[2]][v[3]][v[4]][v[5]][v[6]][i];
			res += probabilityHS[v[0]][v[1]][v[2]][v[3]][v[4]][v[5]][v[6]][i];
		}
		if (x <= total) return (double) res / (double) total;
		//stacksize szerint
		for (int i = 0; i < PLAYER_STACK_SIZE_NUM; ++i)
		{
			if (i == v[1]) continue;
			total += totalS[i][v[2]][v[3]][v[4]][v[5]][v[6]][v[7]];
			res += probabilityHS[v[0]][i][v[2]][v[3]][v[4]][v[5]][v[6]][v[7]];
		}
		if (x <= total) return (double) res / (double) total;
		
		//pfr szerint
		for (int i = 0; i < PLAYER_PFR_NUM; ++i)
		{
			if (i == v[6]) continue;
			total += totalS[v[1]][v[2]][v[3]][v[4]][v[6]][i][v[7]];
			res += probabilityHS[v[0]][v[1]][v[2]][v[3]][v[4]][v[6]][i][v[7]];
		}
		if (x <= total) return (double) res / (double) total;

		//vpip szerint
		for (int i = 0; i < PLAYER_VPIP_NUM; ++i)
		{
			if (i == v[5]) continue;
			total += totalS[v[1]][v[2]][v[3]][v[4]][i][v[6]][v[7]];
			res += probabilityHS[v[0]][v[1]][v[2]][v[3]][v[4]][i][v[6]][v[7]];
		}
		if (x <= total) return (double) res / (double) total;

		return -2.0;
	}


	double getProbabilityHS(int v[])
	{
		return (double)probabilityHS[v[0]][v[1]][v[2]][v[3]][v[4]][v[5]][v[6]][v[7]] / (double)totalS[v[1]][v[2]][v[3]][v[4]][v[5]][v[6]][v[7]];
	}

	double getProbabilityFE(int v[], int x)
	{
		int res = 0;
		int total = 0;

		total += totalFE[v[1]][v[2]][v[3]][v[4]][v[5]][v[6]][v[7]];
		res += probabilityFE[0][v[1]][v[2]][v[3]][v[4]][v[5]][v[6]][v[7]];

		if (x <= total) return (double) res / (double) total;

		return -2.0;
	}

	double getNormProbabilityHS(int v[])
	{
		//normalize!!
		//getProbability...
		return (double)probabilityHS[v[0]][v[1]][v[2]][v[3]][v[4]][v[5]][v[6]][v[7]] / (double)totalS[v[1]][v[2]][v[3]][v[4]][v[5]][v[6]][v[7]];
	}

	double getNormProbabilityFE(int v[])
	{
		//normalize!!
		//getProbability
		return (double)probabilityFE[0][v[1]][v[2]][v[3]][v[4]][v[5]][v[6]][v[7]] / (double)totalFE[v[1]][v[2]][v[3]][v[4]][v[5]][v[6]][v[7]];
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
				fprintf(f,"%d ", totalS[v[2]][v[3]][v[4]][v[5]][v[6]][v[7]][v[8]]);
				for (int i = 0; i < HAND_STRENGTH_NUM; ++i)
				{
					fprintf(f,"%d ", probabilityHS[i][v[2]][v[3]][v[4]][v[5]][v[6]][v[7]][v[8]]);
				}
			}
			else
			{
				fscanf(f,"%d ", &totalS[v[2]][v[3]][v[4]][v[5]][v[6]][v[7]][v[8]]);
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

class BayesTurn
{
public:
	FILE* logfile;
	//Database* database;

	enum node
	{
		HAND_STRENGTH = 0,
		FOLD_EQUITY = 1,
		POT_SIZE = 2,
		PLAYER_STACK_SIZE = 3,
		PLAYER_BET_SIZE = 4,
		PLAYER_LINE = 5,
		PLAYER_VPIP = 6,
		PLAYER_PFR = 7,
		PLAYER_AF = 8,
	};

	static const int HAND_STRENGTH_NUM = 9;
	static const int FOLD_EQUITY_NUM = 1;
	static const int POT_SIZE_NUM = 5; 
	static const int PLAYER_STACK_SIZE_NUM = 5;
	static const int PLAYER_BET_SIZE_NUM = 5; 
	static const int PLAYER_LINE_NUM = 3;
	static const int PLAYER_VPIP_NUM = 5;
	static const int PLAYER_PFR_NUM = 3;
	static const int PLAYER_AF_NUM = 3;

	static const int node_number = 9;
	int nums[node_number];

	int probabilityHS[HAND_STRENGTH_NUM][POT_SIZE_NUM][PLAYER_STACK_SIZE_NUM][PLAYER_BET_SIZE_NUM][PLAYER_LINE_NUM][PLAYER_VPIP_NUM][PLAYER_PFR_NUM][PLAYER_AF_NUM];
	int probabilityFE[FOLD_EQUITY_NUM][POT_SIZE_NUM][PLAYER_STACK_SIZE_NUM][PLAYER_BET_SIZE_NUM][PLAYER_LINE_NUM][PLAYER_VPIP_NUM][PLAYER_PFR_NUM][PLAYER_AF_NUM];
	int totalS[POT_SIZE_NUM][PLAYER_STACK_SIZE_NUM][PLAYER_BET_SIZE_NUM][PLAYER_LINE_NUM][PLAYER_VPIP_NUM][PLAYER_PFR_NUM][PLAYER_AF_NUM];
	int totalFE[POT_SIZE_NUM][PLAYER_STACK_SIZE_NUM][PLAYER_BET_SIZE_NUM][PLAYER_LINE_NUM][PLAYER_VPIP_NUM][PLAYER_PFR_NUM][PLAYER_AF_NUM];

public:
	BayesTurn()
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

		//database = new Database("127.0.0.1", "root", "root", "kutya");
		logfile = fopen("flopbayeslog", "w");
		memset(totalS, 0, sizeof(totalS));
		memset(probabilityHS, 0, sizeof(probabilityHS));
		memset(probabilityFE, 0, sizeof(probabilityFE));
	}

	void read(string filename)
	{
		FILE* f = fopen(filename.c_str(), "r");
		backHS(f, false, 2);
		backFE(f, false, 2);

		//fprintf(logfile,"Total = %d\n", totalSituation);
		//fprintf(logfile,"Total few = %d\n", totalLittle);
		int totalS = 1;
		for (int i = 2; i < node_number; ++i)
			totalS *= nums[i];
		fprintf(logfile,"Total situation = %d\n", totalS);
		fclose(f);
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
		//(double)probabilityHS[v[0]][v[1]][v[2]][v[3]][v[4]][v[5]][v[6]][v[7]] / (double)totalS[v[1]][v[2]][v[3]][v[4]][v[5]][v[6]][v[7]];

		total += totalS[v[1]][v[2]][v[3]][v[4]][v[5]][v[6]][v[7]];
		res += probabilityHS[v[0]][v[1]][v[2]][v[3]][v[4]][v[5]][v[6]][v[7]];
		
		if (x <= total) return (double) res / (double) total;
		//af szerint
		for (int i = 0; i < PLAYER_AF_NUM; ++i)
		{
			if (i == v[7]) continue;
			total += totalS[v[1]][v[2]][v[3]][v[4]][v[5]][v[6]][i];
			res += probabilityHS[v[0]][v[1]][v[2]][v[3]][v[4]][v[5]][v[6]][i];
		}
		if (x <= total) return (double) res / (double) total;
		//stacksize szerint
		for (int i = 0; i < PLAYER_STACK_SIZE_NUM; ++i)
		{
			if (i == v[1]) continue;
			total += totalS[i][v[2]][v[3]][v[4]][v[5]][v[6]][v[7]];
			res += probabilityHS[v[0]][i][v[2]][v[3]][v[4]][v[5]][v[6]][v[7]];
		}
		if (x <= total) return (double) res / (double) total;
		
		//pfr szerint
		for (int i = 0; i < PLAYER_PFR_NUM; ++i)
		{
			if (i == v[6]) continue;
			total += totalS[v[1]][v[2]][v[3]][v[4]][v[6]][i][v[7]];
			res += probabilityHS[v[0]][v[1]][v[2]][v[3]][v[4]][v[6]][i][v[7]];
		}
		if (x <= total) return (double) res / (double) total;

		//vpip szerint
		for (int i = 0; i < PLAYER_VPIP_NUM; ++i)
		{
			if (i == v[5]) continue;
			total += totalS[v[1]][v[2]][v[3]][v[4]][i][v[6]][v[7]];
			res += probabilityHS[v[0]][v[1]][v[2]][v[3]][v[4]][i][v[6]][v[7]];
		}
		if (x <= total) return (double) res / (double) total;

		return -2.0;
	}


	double getProbabilityHS(int v[])
	{
		return (double)probabilityHS[v[0]][v[1]][v[2]][v[3]][v[4]][v[5]][v[6]][v[7]] / (double)totalS[v[1]][v[2]][v[3]][v[4]][v[5]][v[6]][v[7]];
	}

	double getProbabilityFE(int v[], int x)
	{
		int res = 0;
		int total = 0;

		total += totalFE[v[1]][v[2]][v[3]][v[4]][v[5]][v[6]][v[7]];
		res += probabilityFE[0][v[1]][v[2]][v[3]][v[4]][v[5]][v[6]][v[7]];

		if (x <= total) return (double) res / (double) total;

		return -2.0;
	}

	double getNormProbabilityHS(int v[])
	{
		//normalize!!
		return (double)probabilityHS[v[0]][v[1]][v[2]][v[3]][v[4]][v[5]][v[6]][v[7]] / (double)totalS[v[1]][v[2]][v[3]][v[4]][v[5]][v[6]][v[7]];
	}

	double getNormProbabilityFE(int v[])
	{
		//normalize!!
		return (double)probabilityFE[0][v[1]][v[2]][v[3]][v[4]][v[5]][v[6]][v[7]] / (double)totalFE[v[1]][v[2]][v[3]][v[4]][v[5]][v[6]][v[7]];
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
				fprintf(f,"%d ", totalS[v[2]][v[3]][v[4]][v[5]][v[6]][v[7]][v[8]]);
				for (int i = 0; i < HAND_STRENGTH_NUM; ++i)
				{
					fprintf(f,"%d ", probabilityHS[i][v[2]][v[3]][v[4]][v[5]][v[6]][v[7]][v[8]]);
				}
			}
			else
			{
				fscanf(f,"%d ", &totalS[v[2]][v[3]][v[4]][v[5]][v[6]][v[7]][v[8]]);
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
class BayesRiver
{
public:
	FILE* logfile;
	//Database* database;

	enum node
	{
		HAND_STRENGTH = 0,
		FOLD_EQUITY = 1,
		POT_SIZE = 2,
		PLAYER_STACK_SIZE = 3,
		PLAYER_BET_SIZE = 4,
		PLAYER_LINE = 5,
		PLAYER_VPIP = 6,
		PLAYER_PFR = 7,
		PLAYER_AF = 8,
	};

	static const int HAND_STRENGTH_NUM = 9;
	static const int FOLD_EQUITY_NUM = 1;
	static const int POT_SIZE_NUM = 5; 
	static const int PLAYER_STACK_SIZE_NUM = 5;
	static const int PLAYER_BET_SIZE_NUM = 5; 
	static const int PLAYER_LINE_NUM = 3;
	static const int PLAYER_VPIP_NUM = 5;
	static const int PLAYER_PFR_NUM = 3;
	static const int PLAYER_AF_NUM = 3;

	static const int node_number = 9;
	int nums[node_number];

	int probabilityHS[HAND_STRENGTH_NUM][POT_SIZE_NUM][PLAYER_STACK_SIZE_NUM][PLAYER_BET_SIZE_NUM][PLAYER_LINE_NUM][PLAYER_VPIP_NUM][PLAYER_PFR_NUM][PLAYER_AF_NUM];
	int probabilityFE[FOLD_EQUITY_NUM][POT_SIZE_NUM][PLAYER_STACK_SIZE_NUM][PLAYER_BET_SIZE_NUM][PLAYER_LINE_NUM][PLAYER_VPIP_NUM][PLAYER_PFR_NUM][PLAYER_AF_NUM];
	int totalS[POT_SIZE_NUM][PLAYER_STACK_SIZE_NUM][PLAYER_BET_SIZE_NUM][PLAYER_LINE_NUM][PLAYER_VPIP_NUM][PLAYER_PFR_NUM][PLAYER_AF_NUM];
	int totalFE[POT_SIZE_NUM][PLAYER_STACK_SIZE_NUM][PLAYER_BET_SIZE_NUM][PLAYER_LINE_NUM][PLAYER_VPIP_NUM][PLAYER_PFR_NUM][PLAYER_AF_NUM];

public:
	BayesRiver()
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

		//database = new Database("127.0.0.1", "root", "root", "kutya");
		logfile = fopen("flopbayeslog", "w");
		memset(totalS, 0, sizeof(totalS));
		memset(probabilityHS, 0, sizeof(probabilityHS));
		memset(probabilityFE, 0, sizeof(probabilityFE));
	}

	void read(string filename)
	{
		FILE* f = fopen(filename.c_str(), "r");
		backHS(f, false, 2);
		backFE(f, false, 2);

		//fprintf(logfile,"Total = %d\n", totalSituation);
		//fprintf(logfile,"Total few = %d\n", totalLittle);
		int totalS = 1;
		for (int i = 2; i < node_number; ++i)
			totalS *= nums[i];
		fprintf(logfile,"Total situation = %d\n", totalS);
		fclose(f);
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
		//(double)probabilityHS[v[0]][v[1]][v[2]][v[3]][v[4]][v[5]][v[6]][v[7]] / (double)totalS[v[1]][v[2]][v[3]][v[4]][v[5]][v[6]][v[7]];

		total += totalS[v[1]][v[2]][v[3]][v[4]][v[5]][v[6]][v[7]];
		res += probabilityHS[v[0]][v[1]][v[2]][v[3]][v[4]][v[5]][v[6]][v[7]];
		
		if (x <= total) return (double) res / (double) total;
		//af szerint
		for (int i = 0; i < PLAYER_AF_NUM; ++i)
		{
			if (i == v[7]) continue;
			total += totalS[v[1]][v[2]][v[3]][v[4]][v[5]][v[6]][i];
			res += probabilityHS[v[0]][v[1]][v[2]][v[3]][v[4]][v[5]][v[6]][i];
		}
		if (x <= total) return (double) res / (double) total;
		//stacksize szerint
		for (int i = 0; i < PLAYER_STACK_SIZE_NUM; ++i)
		{
			if (i == v[1]) continue;
			total += totalS[i][v[2]][v[3]][v[4]][v[5]][v[6]][v[7]];
			res += probabilityHS[v[0]][i][v[2]][v[3]][v[4]][v[5]][v[6]][v[7]];
		}
		if (x <= total) return (double) res / (double) total;
		
		//pfr szerint
		for (int i = 0; i < PLAYER_PFR_NUM; ++i)
		{
			if (i == v[6]) continue;
			total += totalS[v[1]][v[2]][v[3]][v[4]][v[6]][i][v[7]];
			res += probabilityHS[v[0]][v[1]][v[2]][v[3]][v[4]][v[6]][i][v[7]];
		}
		if (x <= total) return (double) res / (double) total;

		//vpip szerint
		for (int i = 0; i < PLAYER_VPIP_NUM; ++i)
		{
			if (i == v[5]) continue;
			total += totalS[v[1]][v[2]][v[3]][v[4]][i][v[6]][v[7]];
			res += probabilityHS[v[0]][v[1]][v[2]][v[3]][v[4]][i][v[6]][v[7]];
		}
		if (x <= total) return (double) res / (double) total;

		return -2.0;
	}


	double getProbabilityHS(int v[])
	{
		return (double)probabilityHS[v[0]][v[1]][v[2]][v[3]][v[4]][v[5]][v[6]][v[7]] / (double)totalS[v[1]][v[2]][v[3]][v[4]][v[5]][v[6]][v[7]];
	}

	double getProbabilityFE(int v[])
	{
		printf("%d\n", totalFE[v[1]][v[2]][v[3]][v[4]][v[5]][v[6]][v[7]]);
		return (double)probabilityFE[0][v[1]][v[2]][v[3]][v[4]][v[5]][v[6]][v[7]] / (double)totalFE[v[1]][v[2]][v[3]][v[4]][v[5]][v[6]][v[7]];
	}

	double getNormProbabilityHS(int v[])
	{
		//normalize!!
		return (double)probabilityHS[v[0]][v[1]][v[2]][v[3]][v[4]][v[5]][v[6]][v[7]] / (double)totalS[v[1]][v[2]][v[3]][v[4]][v[5]][v[6]][v[7]];
	}

	double getNormProbabilityFE(int v[])
	{
		//normalize!!
		return (double)probabilityFE[0][v[1]][v[2]][v[3]][v[4]][v[5]][v[6]][v[7]] / (double)totalFE[v[1]][v[2]][v[3]][v[4]][v[5]][v[6]][v[7]];
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
				fprintf(f,"%d ", totalS[v[2]][v[3]][v[4]][v[5]][v[6]][v[7]][v[8]]);
				for (int i = 0; i < HAND_STRENGTH_NUM; ++i)
				{
					fprintf(f,"%d ", probabilityHS[i][v[2]][v[3]][v[4]][v[5]][v[6]][v[7]][v[8]]);
				}
			}
			else
			{
				fscanf(f,"%d ", &totalS[v[2]][v[3]][v[4]][v[5]][v[6]][v[7]][v[8]]);
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