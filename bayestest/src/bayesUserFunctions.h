#pragma once
#include <string>
#include <vector>
#include "handHistoryParser.h"
#include "rangeFunctions.h"
#include "bayesUtils.h"
#include "bayesFunctions.h"

using namespace std;

class BayesUserPreflop : public BayesPreflop
{
public:
	FILE* logfile;

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
class BayesUserFlop : public BayesFlop
{
public:
	FILE* logfile;

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
		double total = 0;
		for (int i = 0; i < HAND_STRENGTH_NUM; ++i)
		{
			v[0] = i;
			cout << "P(HS = " << i << ") = " << getProbabilityHS(v) << endl;
			total += getProbabilityHS(v);
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
public:
	FILE* logfile;

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
class BayesUserRiver : public BayesRiver
{
public:
	FILE* logfile;

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