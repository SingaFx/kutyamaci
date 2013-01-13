//TODO:merge with bayes test, bayes_functions.h
#pragma once
#include "evaluator.h"
#include "handHistoryParser.h"
#include "database.h"
#include <string>
#include <vector>

using namespace std;
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

class BayesPreflop// : public Bayes
{
public:
	FILE* logfile;
	Database* database;

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
	//BayesPreflop(Database* database, FILE* logfile)
	BayesPreflop()
	{
		preflop_nums[0] = PREFLOP_HAND_STRENGTH_NUM;//, PREFLOP_PLAYER_STACK_SIZE_NUM, PREFLOP_PLAYER_BET_SIZE_NUM, PREFLOP_PLAYER_LINE_NUM, PREFLOP_PLAYER_VPIP_NUM, PREFLOP_PLAYER_PFR_NUM, PREFLOP_PLAYER_POZ_NUM};
		preflop_nums[1] = PREFLOP_PLAYER_STACK_SIZE_NUM;
		preflop_nums[2] = PREFLOP_PLAYER_BET_SIZE_NUM;
		preflop_nums[3] = PREFLOP_PLAYER_LINE_NUM;
		preflop_nums[4] = PREFLOP_PLAYER_VPIP_NUM;
		preflop_nums[5] = PREFLOP_PLAYER_PFR_NUM;
		preflop_nums[6] = PREFLOP_PLAYER_POZ_NUM;

		database = new Database("127.0.0.1", "root", "root", "kutya");
		logfile = fopen("preflopbayeslog", "w");
		memset(totalPreflop, 0, sizeof(totalPreflop));
		memset(probabilityPreflop, 0, sizeof(probabilityPreflop));
	}

	void read(string filename)
	{
		FILE* f = fopen(filename.c_str(), "r");
		back(f, false, 1);
	}

	void write(string filename)
	{
		printf("Writing bayes network\n");
		FILE* f = fopen(filename.c_str(), "w");
		back(f, true, 1);
		fclose(f);
	}

	void updateProbabilities(HandHistory& handhistory)
	{
		if (handhistory.players.size() > 6)
		{
			printf("Hand with more then 6 players\n");
			return ;
		}

		int mSize = 0;
		for (int i = 0; i < handhistory.players.size(); ++i)
		{
			PlayerHistory player = handhistory.players[i];
			for (int j = 0; j < player.preflopAction.size(); ++j)
				{
					Action action = player.preflopAction[j];
					int size = normalizeBetSize(1, action.size, 0, 0.04);
					if (action.type == 'c')
					{
						if (size > mSize) mSize = size;
					}
				}
		}

		for (int i = 0; i < handhistory.players.size(); ++i)
		{
			PlayerHistory player = handhistory.players[i];
			double VPIP = database->getVPIP(player.player.name);
			if (VPIP > 100 || VPIP < 0)
			{
				printf("ERROR VPIP: %lf\n", VPIP);
				printf("NAME: %s\n", player.player.name.c_str());
				fprintf(logfile,"ERROR VPIP: %lf\n", VPIP);
				fprintf(logfile,"NAME: %s\n", player.player.name.c_str());
				//exit(1);
			}
		
			double PFR = database->getPFR(player.player.name);
			if (PFR > 100 || PFR < 0)
			{
				printf("ERROR PFR: %lf\n", PFR);
				printf("NAME: %s\n", player.player.name.c_str());
				fprintf(logfile,"ERROR PFR: %lf\n", PFR);
				fprintf(logfile,"NAME: %s\n", player.player.name.c_str());
				//exit(1);
			}

			int poz = player.position + 3;
			if (poz < 0 || poz > 5)
			{
				printf("ERROR poz\n");
				fprintf(logfile,"ERROR POZ: %d\n", poz);
				fprintf(logfile,"NAME: %s\n", player.player.name.c_str());
				//exit(1);
			}

			int nVPIP = normalizeVPIP(VPIP);
			int nPFR = normalizePFR(PFR);
			int nStackSize = normalizeStackSize(player.balance, 0.04);
			int nLine = 0; // <- create functions for this


			if (player.handKnown)
			{
				int hand = normalizeHand(player.hand);
				//fprintf(logfile, "Hand %d: %c%c%c%c\n", hand, player.hand.card1.rank, player.hand.card1.suit, player.hand.card2.rank, player.hand.card2.suit);

				for (int j = 0; j < player.preflopAction.size(); ++j)
				{
					Action action = player.preflopAction[j];
					int size = normalizeBetSize(1, action.size, 0, 0.04);
					if (action.type == 'c')
					{
						++probabilityPreflop[hand][nStackSize][size][0][nVPIP][nPFR][poz];
						++totalPreflop[nStackSize][size][0][nVPIP][nPFR][poz];
						//fprintf(logfile,"Situation: %d %d %d %d %d %d %d\n", hand, nStackSize, size, 0, nVPIP, nPFR, poz);
					}
				
					if (action.type == 'r')
					{
						if (size > mSize) size = mSize;
						++probabilityPreflop[hand][nStackSize][size][1][nVPIP][nPFR][poz];
						++totalPreflop[nStackSize][size][1][nVPIP][nPFR][poz];
						//fprintf(logfile,"Situation: %d %d %d %d %d %d %d\n", hand, nStackSize, size, 1, nVPIP, nPFR, poz);
					}
				}
			}
			else
			{
				bool folded = false;
				for (int j = 0; j < player.preflopAction.size(); ++j)
				{
					Action action = player.preflopAction[j];
					if (action.type == 'f')
					{
						folded = true;
						break;
					}
				}
				for (int j = 0; j < player.flopAction.size(); ++j)
				{
					Action action = player.flopAction[j];
					if (action.type == 'f')
					{
						folded = true;
						break;
					}
				}
				for (int j = 0; j < player.turnAction.size(); ++j)
				{
					Action action = player.turnAction[j];
					if (action.type == 'f')
					{
						folded = true;
						break;
					}
				}
				for (int j = 0; j < player.riverAction.size(); ++j)
				{
					Action action = player.riverAction[j];
					if (action.type == 'f')
					{
						folded = true;
						break;
					}
				}

				if (folded)
				{
					for (int j = 0; j < player.preflopAction.size(); ++j)
					{
						Action action = player.preflopAction[j];
						if (action.type == 'c')
						{
							++probabilityPreflop[0][nStackSize][normalizeBetSize(1, action.size, 0, 0.04)][0][nVPIP][nPFR][poz];
							++totalPreflop[nStackSize][normalizeBetSize(1, action.size, 0, 0.04)][0][nVPIP][nPFR][poz];
						}
				
						if (action.type == 'r')
						{
							++probabilityPreflop[0][nStackSize][normalizeBetSize(1, action.size, 0, 0.04)][1][nVPIP][nPFR][poz];
							++totalPreflop[nStackSize][normalizeBetSize(1, action.size, 0, 0.04)][1][nVPIP][nPFR][poz];
						}
					}
				}
			}
		}
	}

	double getProbability(int v[])
	{
		return (double)probabilityPreflop[v[0]][v[1]][v[2]][v[3]][v[4]][v[5]][v[6]] / (double)totalPreflop[v[1]][v[2]][v[3]][v[4]][v[5]][v[6]];
	}

	double getNormProbability(int v[])
	{
		//normalize!!
		return (double)probabilityPreflop[v[0]][v[1]][v[2]][v[3]][v[4]][v[5]][v[6]] / (double)totalPreflop[v[1]][v[2]][v[3]][v[4]][v[5]][v[6]];
	}
private:
	int v[preflop_node_number];
	void back(FILE* f, bool b, int k)
	{
		if (k == preflop_node_number)
		{
			if (b)
			{
				//fprintf(f, "Situation %d %d %d %d %d %d\n", v[1], v[2], v[3], v[4], v[5], v[6]);
				fprintf(f,"%d ", totalPreflop[v[1]][v[2]][v[3]][v[4]][v[5]][v[6]]);
				for (int i = 0; i < PREFLOP_HAND_STRENGTH_NUM; ++i)
				{
					fprintf(f,"%d ", probabilityPreflop[i][v[1]][v[2]][v[3]][v[4]][v[5]][v[6]]);
					//printf("Value: %d\n", probabilityPreflop[i][v[1]][v[2]][v[3]][v[4]][v[5]][v[6]]);
				}
			}
			else
			{
				fscanf(f,"%d ", &totalPreflop[v[1]][v[2]][v[3]][v[4]][v[5]][v[6]]);
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
	Database* database;

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
		BOARD_TYPE = 9,
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

		database = new Database("127.0.0.1", "root", "root", "kutya");
		logfile = fopen("flopbayeslog", "w");
		memset(totalS, 0, sizeof(totalS));
		memset(totalFE, 0, sizeof(totalFE));
		memset(probabilityHS, 0, sizeof(probabilityHS));
		memset(probabilityFE, 0, sizeof(probabilityFE));
	}

	void read(string filename)
	{
		FILE* f = fopen(filename.c_str(), "r");
		backHS(f, false, 2);
		backFE(f, false, 2);
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

	void updateProbabilities(HandHistory& handhistory)
	{
		if (handhistory.players.size() > 6)
		{
			printf("Hand with more then 6 players\n");
			return ;
		}

		int mSize = 0;
		for (int i = 0; i < handhistory.players.size(); ++i)
		{
			PlayerHistory player = handhistory.players[i];
			for (int j = 0; j < player.flopAction.size(); ++j)
				{
					Action action = player.flopAction[j];
					int size = normalizeBetSize(1, action.size, 0, 0.04);
					if (action.type == 'c')
					{
						if (size > mSize) mSize = size;
					}
				}
		}

		double totalpot = 1.5 * 0.04;
		for (int i = 0; i < handhistory.players.size(); ++i)
		{
			PlayerHistory player = handhistory.players[i];
			double maxcall = 0;
			for (int j = 0; j < player.preflopAction.size(); ++j)
				{
					Action action = player.preflopAction[j];
					if (action.type == 'c')
					{
						if (action.size > maxcall) maxcall = action.size;
					}
				}
			totalpot += maxcall;
		}

		int nPotSize = normalizePotSize(2, totalpot, 0.04);

		for (int i = 0; i < handhistory.players.size(); ++i)
		{
			PlayerHistory player = handhistory.players[i];
			double VPIP = database->getVPIP(player.player.name);
			if (VPIP > 100 || VPIP < 0)
			{
				printf("ERROR VPIP: %lf\n", VPIP);
				printf("NAME: %s\n", player.player.name.c_str());
				fprintf(logfile,"ERROR VPIP: %lf\n", VPIP);
				fprintf(logfile,"NAME: %s\n", player.player.name.c_str());
			}
		
			double PFR = database->getPFR(player.player.name);
			if (PFR > 100 || PFR < 0)
			{
				printf("ERROR PFR: %lf\n", PFR);
				printf("NAME: %s\n", player.player.name.c_str());
				fprintf(logfile,"ERROR PFR: %lf\n", PFR);
				fprintf(logfile,"NAME: %s\n", player.player.name.c_str());
			}
			
			double AF = database->getAF(player.player.name);

			int nVPIP = normalizeVPIP(VPIP);
			int nPFR = normalizePFR(PFR);
			int nAF = normalizeAF(AF);
			int nStackSize = normalizeStackSize(player.balance, 0.04);
			int nLine = 0;

			if (player.flopAction.size() > 0) //flopra jutott de meg nem csinalt semmit, de volt floppon emeles!
			{
				if (player.flopAction[0].type == 'c')
				{
					++totalFE[nPotSize][nStackSize][4][2][nVPIP][nPFR][nAF];
				}
				if (player.flopAction[0].type == 'r' && player.flopAction[0].size > totalpot)
				{
					++totalFE[nPotSize][nStackSize][4][2][nVPIP][nPFR][nAF];
				}
			}

			if (player.handKnown)
			{
				int hand = Evaluator::cardStrength(player.hand.card1, player.hand.card2, handhistory.flopHand1.card1, handhistory.flopHand2.card1, handhistory.flopHand3.card1);
				
				/*
				fprintf(logfile,"HS:%d = ", hand);
				fprintf(logfile,"%c%c %c%c %c%c %c%c %c%c\n", player.hand.card1.rank, player.hand.card1.suit, player.hand.card2.rank, player.hand.card2.suit,
					handhistory.flopHand1.card1.rank, handhistory.flopHand1.card1.suit, 
					handhistory.flopHand2.card1.rank, handhistory.flopHand2.card1.suit, 
					handhistory.flopHand3.card1.rank, handhistory.flopHand3.card1.suit);
				*/

				for (int j = 0; j < player.flopAction.size(); ++j)
				{
					Action action = player.flopAction[j];
					int size = normalizeBetSize(1, action.size, 0, 0.04);
					if (action.type == 'c')
					{
						++probabilityHS[hand][nPotSize][nStackSize][size][0][nVPIP][nPFR][nAF];
						++totalS[nPotSize][nStackSize][size][0][nVPIP][nPFR][nAF];
						if (j + 1 < player.flopAction.size())
							++totalFE[nPotSize][nStackSize][size][0][nVPIP][nPFR][nAF];
						//fprintf(logfile,"Situation: %d %d %d %d %d %d %d\n", hand, nStackSize, size, 0, nVPIP, nPFR, poz);
					}
				
					if (action.type == 'r')
					{
						if (size > mSize) size = mSize;
						++probabilityHS[hand][nPotSize][nStackSize][size][1][nVPIP][nPFR][nAF];
						++totalS[nPotSize][nStackSize][size][1][nVPIP][nPFR][nAF];
						if (j + 1 < player.flopAction.size())
							++totalFE[nPotSize][nStackSize][size][1][nVPIP][nPFR][nAF];
						//fprintf(logfile,"Situation: %d %d %d %d %d %d %d\n", hand, nStackSize, size, 1, nVPIP, nPFR, poz);
					}

					if (action.type == 'x')
					{
						++probabilityHS[hand][nPotSize][nStackSize][0][2][nVPIP][nPFR][nAF];
						++totalS[nPotSize][nStackSize][0][2][nVPIP][nPFR][nAF];
						if (j + 1 < player.flopAction.size())
							++totalFE[nPotSize][nStackSize][0][2][nVPIP][nPFR][nAF];
					}
				}
			}
			else
			{
				bool folded = false;
				bool FE = false;

				for (int j = 0; j < player.flopAction.size(); ++j)
				{
					Action action = player.flopAction[j];
					if (action.type == 'f')
					{
						folded = true;
						FE = true;
						break;
					}
				}
				for (int j = 0; j < player.turnAction.size(); ++j)
				{
					Action action = player.turnAction[j];
					if (action.type == 'f')
					{
						folded = true;
						break;
					}
				}
				for (int j = 0; j < player.riverAction.size(); ++j)
				{
					Action action = player.riverAction[j];
					if (action.type == 'f')
					{
						folded = true;
						break;
					}
				}

				if (folded)
				{
					for (int j = 0; j < player.flopAction.size(); ++j)
					{
						Action action = player.flopAction[j];
						if (action.type == 'c')
						{
							++probabilityHS[HAND_STRENGTH_NUM - 1][nPotSize][nStackSize][normalizeBetSize(1, action.size, 0, 0.04)][0][nVPIP][nPFR][nAF];
							++totalS[nPotSize][nStackSize][normalizeBetSize(1, action.size, 0, 0.04)][0][nVPIP][nPFR][nAF];
							if (j + 1 < player.flopAction.size())
								++totalFE[nPotSize][nStackSize][normalizeBetSize(1, action.size, 0, 0.04)][0][nVPIP][nPFR][nAF];
						}
				
						if (action.type == 'r')
						{
							++probabilityHS[HAND_STRENGTH_NUM - 1][nPotSize][nStackSize][normalizeBetSize(1, action.size, 0, 0.04)][1][nVPIP][nPFR][nAF];
							++totalS[nPotSize][nStackSize][normalizeBetSize(1, action.size, 0, 0.04)][1][nVPIP][nPFR][nAF];
							if (j + 1 < player.flopAction.size())
								++totalFE[nPotSize][nStackSize][normalizeBetSize(1, action.size, 0, 0.04)][1][nVPIP][nPFR][nAF];
						}

						if (action.type == 'x')
						{
							++probabilityHS[HAND_STRENGTH_NUM - 1][nPotSize][nStackSize][0][2][nVPIP][nPFR][nAF];
							++totalS[nPotSize][nStackSize][0][2][nVPIP][nPFR][nAF];
							if (j + 1 < player.flopAction.size())
								++totalFE[nPotSize][nStackSize][0][2][nVPIP][nPFR][nAF];
						}

					}

					if (FE)
					{
						if (player.flopAction.size() == 1)
						{
							++probabilityFE[0][nPotSize][nStackSize][4][2][nVPIP][nPFR][nAF];
						}
						else
						{
							Action action = player.flopAction[player.flopAction.size() - 2];
							if (action.type == 'c')
							{
								++probabilityFE[0][nPotSize][nStackSize][normalizeBetSize(1, action.size, 0, 0.04)][0][nVPIP][nPFR][nAF];
							}
				
							if (action.type == 'r')
							{
								++probabilityFE[0][nPotSize][nStackSize][normalizeBetSize(1, action.size, 0, 0.04)][1][nVPIP][nPFR][nAF];
							}

							if (action.type == 'x')
							{
								++probabilityFE[0][nPotSize][nStackSize][0][2][nVPIP][nPFR][nAF];
							}
						}
					}
				}
			}
		}
	}

	double getProbabilityHS(int v[])
	{
		return (double)probabilityHS[v[0]][v[1]][v[2]][v[3]][v[4]][v[5]][v[6]][v[7]] / (double)totalS[v[1]][v[2]][v[3]][v[4]][v[5]][v[6]][v[7]];
	}

	double getProbabilityFE(int v[])
	{
		return (double)probabilityFE[v[0]][v[1]][v[2]][v[3]][v[4]][v[5]][v[6]][v[7]] / (double)totalS[v[1]][v[2]][v[3]][v[4]][v[5]][v[6]][v[7]];
	}

	double getNormProbabilityHS(int v[])
	{
		//normalize!!
		return (double)probabilityHS[v[0]][v[1]][v[2]][v[3]][v[4]][v[5]][v[6]][v[7]] / (double)totalS[v[1]][v[2]][v[3]][v[4]][v[5]][v[6]][v[7]];
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
	Database* database;

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
	static const int POT_SIZE_NUM = 5; //!!
	static const int PLAYER_STACK_SIZE_NUM = 5;
	static const int PLAYER_BET_SIZE_NUM = 6; 
	static const int PLAYER_LINE_NUM = 3; //->
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

		database = new Database("127.0.0.1", "root", "root", "kutya");
		logfile = fopen("turnbayeslog", "w");
		memset(totalS, 0, sizeof(totalS));
		memset(totalFE, 0, sizeof(totalFE));
		memset(probabilityHS, 0, sizeof(probabilityHS));
		memset(probabilityFE, 0, sizeof(probabilityFE));
	}

	void read(string filename)
	{
		FILE* f = fopen(filename.c_str(), "r");
		backHS(f, false, 2);
		backFE(f, false, 2);
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

	void updateProbabilities(HandHistory& handhistory)
	{
		if (handhistory.players.size() > 6)
		{
			printf("Hand with more then 6 players\n");
			return ;
		}

		int mSize = 0;
		for (int i = 0; i < handhistory.players.size(); ++i)
		{
			PlayerHistory player = handhistory.players[i];
			for (int j = 0; j < player.turnAction.size(); ++j)
				{
					Action action = player.turnAction[j];
					int size = normalizeBetSize(1, action.size, 0, 0.04);
					if (action.type == 'c')
					{
						if (size > mSize) mSize = size;
					}
				}
		}

		double totalpot = 1.5 * 0.04;
		for (int i = 0; i < handhistory.players.size(); ++i)
		{
			PlayerHistory player = handhistory.players[i];
			double maxcall = 0;
			for (int j = 0; j < player.preflopAction.size(); ++j)
				{
					Action action = player.preflopAction[j];
					if (action.type == 'c')
					{
						if (action.size > maxcall) maxcall = action.size;
					}
				}
			totalpot += maxcall;
		}
		for (int i = 0; i < handhistory.players.size(); ++i)
		{
			PlayerHistory player = handhistory.players[i];
			double maxcall = 0;
			for (int j = 0; j < player.flopAction.size(); ++j)
				{
					Action action = player.flopAction[j];
					if (action.type == 'c')
					{
						if (action.size > maxcall) maxcall = action.size;
					}
				}
			totalpot += maxcall;
		}

		int nPotSize = normalizePotSize(3, totalpot, 0.04);

		for (int i = 0; i < handhistory.players.size(); ++i)
		{
			PlayerHistory player = handhistory.players[i];
			double VPIP = database->getVPIP(player.player.name);
			if (VPIP > 100 || VPIP < 0)
			{
				printf("ERROR VPIP: %lf\n", VPIP);
				printf("NAME: %s\n", player.player.name.c_str());
				fprintf(logfile,"ERROR VPIP: %lf\n", VPIP);
				fprintf(logfile,"NAME: %s\n", player.player.name.c_str());
			}
		
			double PFR = database->getPFR(player.player.name);
			if (PFR > 100 || PFR < 0)
			{
				printf("ERROR PFR: %lf\n", PFR);
				printf("NAME: %s\n", player.player.name.c_str());
				fprintf(logfile,"ERROR PFR: %lf\n", PFR);
				fprintf(logfile,"NAME: %s\n", player.player.name.c_str());
			}
			
			double AF = database->getAF(player.player.name);

			int nVPIP = normalizeVPIP(VPIP);
			int nPFR = normalizePFR(PFR);
			int nAF = normalizeAF(AF);
			int nStackSize = normalizeStackSize(player.balance, 0.04);
			int nLine = 0;

			if (player.turnAction.size() > 0) //flopra jutott de meg nem csinalt semmit, de volt floppon emeles!
			{
				if (player.turnAction[0].type == 'c')
				{
					++totalFE[nPotSize][nStackSize][4][2][nVPIP][nPFR][nAF];
				}
				if (player.turnAction[0].type == 'r' && player.turnAction[0].size > totalpot)
				{
					++totalFE[nPotSize][nStackSize][4][2][nVPIP][nPFR][nAF];
				}
			}

			if (player.handKnown)
			{
				int hand = Evaluator::cardStrength(player.hand.card1, player.hand.card2, handhistory.flopHand1.card1, handhistory.flopHand2.card1, handhistory.flopHand3.card1, handhistory.turnHand.card1);
				
				/*
				fprintf(logfile,"HS:%d = ", hand);
				fprintf(logfile,"%c%c %c%c %c%c %c%c %c%c\n", player.hand.card1.rank, player.hand.card1.suit, player.hand.card2.rank, player.hand.card2.suit,
					handhistory.flopHand1.card1.rank, handhistory.flopHand1.card1.suit, 
					handhistory.flopHand2.card1.rank, handhistory.flopHand2.card1.suit, 
					handhistory.flopHand3.card1.rank, handhistory.flopHand3.card1.suit);
				*/

				for (int j = 0; j < player.turnAction.size(); ++j)
				{
					Action action = player.turnAction[j];
					int size = normalizeBetSize(1, action.size, 0, 0.04);
					if (action.type == 'c')
					{
						++probabilityHS[hand][nPotSize][nStackSize][size][0][nVPIP][nPFR][nAF];
						++totalS[nPotSize][nStackSize][size][0][nVPIP][nPFR][nAF];
						if (j + 1 < player.flopAction.size())
							++totalFE[nPotSize][nStackSize][size][0][nVPIP][nPFR][nAF];
						//fprintf(logfile,"Situation: %d %d %d %d %d %d %d\n", hand, nStackSize, size, 0, nVPIP, nPFR, poz);
					}
				
					if (action.type == 'r')
					{
						if (size > mSize) size = mSize;
						++probabilityHS[hand][nPotSize][nStackSize][size][1][nVPIP][nPFR][nAF];
						++totalS[nPotSize][nStackSize][size][1][nVPIP][nPFR][nAF];
						if (j + 1 < player.flopAction.size())
							++totalFE[nPotSize][nStackSize][size][1][nVPIP][nPFR][nAF];
						//fprintf(logfile,"Situation: %d %d %d %d %d %d %d\n", hand, nStackSize, size, 1, nVPIP, nPFR, poz);
					}

					if (action.type == 'x')
					{
						++probabilityHS[hand][nPotSize][nStackSize][0][2][nVPIP][nPFR][nAF];
						++totalS[nPotSize][nStackSize][0][2][nVPIP][nPFR][nAF];
						if (j + 1 < player.flopAction.size())
							++totalFE[nPotSize][nStackSize][0][2][nVPIP][nPFR][nAF];
					}
				}
			}
			else
			{
				bool folded = false;
				bool FE = false;

				for (int j = 0; j < player.turnAction.size(); ++j)
				{
					Action action = player.turnAction[j];
					if (action.type == 'f')
					{
						folded = true;
						FE = true;
						break;
					}
				}
				for (int j = 0; j < player.riverAction.size(); ++j)
				{
					Action action = player.riverAction[j];
					if (action.type == 'f')
					{
						folded = true;
						break;
					}
				}

				if (folded)
				{
					for (int j = 0; j < player.turnAction.size(); ++j)
					{
						Action action = player.turnAction[j];
						if (action.type == 'c')
						{
							++probabilityHS[HAND_STRENGTH_NUM - 1][nPotSize][nStackSize][normalizeBetSize(1, action.size, 0, 0.04)][0][nVPIP][nPFR][nAF];
							++totalS[nPotSize][nStackSize][normalizeBetSize(1, action.size, 0, 0.04)][0][nVPIP][nPFR][nAF];
							if (j + 1 < player.flopAction.size())
								++totalFE[nPotSize][nStackSize][normalizeBetSize(1, action.size, 0, 0.04)][0][nVPIP][nPFR][nAF];
						}
				
						if (action.type == 'r')
						{
							++probabilityHS[HAND_STRENGTH_NUM - 1][nPotSize][nStackSize][normalizeBetSize(1, action.size, 0, 0.04)][1][nVPIP][nPFR][nAF];
							++totalS[nPotSize][nStackSize][normalizeBetSize(1, action.size, 0, 0.04)][1][nVPIP][nPFR][nAF];
							if (j + 1 < player.flopAction.size())
								++totalFE[nPotSize][nStackSize][normalizeBetSize(1, action.size, 0, 0.04)][1][nVPIP][nPFR][nAF];
						}

						if (action.type == 'x')
						{
							++probabilityHS[HAND_STRENGTH_NUM - 1][nPotSize][nStackSize][0][2][nVPIP][nPFR][nAF];
							++totalS[nPotSize][nStackSize][0][2][nVPIP][nPFR][nAF];
							if (j + 1 < player.flopAction.size())
								++totalFE[nPotSize][nStackSize][0][2][nVPIP][nPFR][nAF];
						}

					}

					if (FE)
					{
						if (player.turnAction.size() == 1)
						{
							++probabilityFE[0][nPotSize][nStackSize][4][2][nVPIP][nPFR][nAF];
						}
						else
						{
							Action action = player.turnAction[player.flopAction.size() - 2];
							if (action.type == 'c')
							{
								++probabilityFE[0][nPotSize][nStackSize][normalizeBetSize(1, action.size, 0, 0.04)][0][nVPIP][nPFR][nAF];
							}
				
							if (action.type == 'r')
							{
								++probabilityFE[0][nPotSize][nStackSize][normalizeBetSize(1, action.size, 0, 0.04)][1][nVPIP][nPFR][nAF];
							}

							if (action.type == 'x')
							{
								++probabilityFE[0][nPotSize][nStackSize][0][2][nVPIP][nPFR][nAF];
							}
						}
					}
				}
			}
		}
	}

	double getProbabilityHS(int v[])
	{
		return (double)probabilityHS[v[0]][v[1]][v[2]][v[3]][v[4]][v[5]][v[6]][v[7]] / (double)totalS[v[1]][v[2]][v[3]][v[4]][v[5]][v[6]][v[7]];
	}

	double getProbabilityFE(int v[])
	{
		return (double)probabilityFE[v[0]][v[1]][v[2]][v[3]][v[4]][v[5]][v[6]][v[7]] / (double)totalS[v[1]][v[2]][v[3]][v[4]][v[5]][v[6]][v[7]];
	}

	double getNormProbabilityHS(int v[])
	{
		//normalize!!
		return (double)probabilityHS[v[0]][v[1]][v[2]][v[3]][v[4]][v[5]][v[6]][v[7]] / (double)totalS[v[1]][v[2]][v[3]][v[4]][v[5]][v[6]][v[7]];
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
	Database* database;

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
	static const int POT_SIZE_NUM = 5; //!!
	static const int PLAYER_STACK_SIZE_NUM = 5;
	static const int PLAYER_BET_SIZE_NUM = 5; //!!
	static const int PLAYER_LINE_NUM = 3; //->
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

		database = new Database("127.0.0.1", "root", "root", "kutya");
		logfile = fopen("riverbayeslog", "w");
		memset(totalS, 0, sizeof(totalS));
		memset(totalFE, 0, sizeof(totalFE));
		memset(probabilityHS, 0, sizeof(probabilityHS));
		memset(probabilityFE, 0, sizeof(probabilityFE));
	}

	void read(string filename)
	{
		FILE* f = fopen(filename.c_str(), "r");
		backHS(f, false, 2);
		backFE(f, false, 2);
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

	void updateProbabilities(HandHistory& handhistory)
	{
		if (handhistory.players.size() > 6)
		{
			printf("Hand with more then 6 players\n");
			return ;
		}

		int mSize = 0;
		for (int i = 0; i < handhistory.players.size(); ++i)
		{
			PlayerHistory player = handhistory.players[i];
			for (int j = 0; j < player.riverAction.size(); ++j)
				{
					Action action = player.riverAction[j];
					int size = normalizeBetSize(1, action.size, 0, 0.04);
					if (action.type == 'c')
					{
						if (size > mSize) mSize = size;
					}
				}
		}

		double totalpot = 1.5 * 0.04;
		for (int i = 0; i < handhistory.players.size(); ++i)
		{
			PlayerHistory player = handhistory.players[i];
			double maxcall = 0;
			for (int j = 0; j < player.preflopAction.size(); ++j)
				{
					Action action = player.preflopAction[j];
					if (action.type == 'c')
					{
						if (action.size > maxcall) maxcall = action.size;
					}
				}
			totalpot += maxcall;
		}
		for (int i = 0; i < handhistory.players.size(); ++i)
		{
			PlayerHistory player = handhistory.players[i];
			double maxcall = 0;
			for (int j = 0; j < player.flopAction.size(); ++j)
				{
					Action action = player.flopAction[j];
					if (action.type == 'c')
					{
						if (action.size > maxcall) maxcall = action.size;
					}
				}
			totalpot += maxcall;
		}
		for (int i = 0; i < handhistory.players.size(); ++i)
		{
			PlayerHistory player = handhistory.players[i];
			double maxcall = 0;
			for (int j = 0; j < player.riverAction.size(); ++j)
				{
					Action action = player.riverAction[j];
					if (action.type == 'c')
					{
						if (action.size > maxcall) maxcall = action.size;
					}
				}
			totalpot += maxcall;
		}

		int nPotSize = normalizePotSize(3, totalpot, 0.04);

		for (int i = 0; i < handhistory.players.size(); ++i)
		{
			PlayerHistory player = handhistory.players[i];
			double VPIP = database->getVPIP(player.player.name);
			if (VPIP > 100 || VPIP < 0)
			{
				printf("ERROR VPIP: %lf\n", VPIP);
				printf("NAME: %s\n", player.player.name.c_str());
				fprintf(logfile,"ERROR VPIP: %lf\n", VPIP);
				fprintf(logfile,"NAME: %s\n", player.player.name.c_str());
			}
		
			double PFR = database->getPFR(player.player.name);
			if (PFR > 100 || PFR < 0)
			{
				printf("ERROR PFR: %lf\n", PFR);
				printf("NAME: %s\n", player.player.name.c_str());
				fprintf(logfile,"ERROR PFR: %lf\n", PFR);
				fprintf(logfile,"NAME: %s\n", player.player.name.c_str());
			}
			
			double AF = database->getAF(player.player.name);

			int nVPIP = normalizeVPIP(VPIP);
			int nPFR = normalizePFR(PFR);
			int nAF = normalizeAF(AF);
			int nStackSize = normalizeStackSize(player.balance, 0.04);
			int nLine = 0;

			if (player.riverAction.size() > 0) //flopra jutott de meg nem csinalt semmit, de volt floppon emeles!
			{
				if (player.riverAction[0].type == 'c')
				{
					++totalFE[nPotSize][nStackSize][4][2][nVPIP][nPFR][nAF];
				}
				if (player.riverAction[0].type == 'r' && player.riverAction[0].size > totalpot)
				{
					++totalFE[nPotSize][nStackSize][4][2][nVPIP][nPFR][nAF];
				}
			}

			if (player.handKnown)
			{
				int hand = Evaluator::cardStrength(player.hand.card1, player.hand.card2, handhistory.flopHand1.card1, handhistory.flopHand2.card1, handhistory.flopHand3.card1, handhistory.turnHand.card1, handhistory.riverHand.card1);
				
				/*
				fprintf(logfile,"HS:%d = ", hand);
				fprintf(logfile,"%c%c %c%c %c%c %c%c %c%c\n", player.hand.card1.rank, player.hand.card1.suit, player.hand.card2.rank, player.hand.card2.suit,
					handhistory.flopHand1.card1.rank, handhistory.flopHand1.card1.suit, 
					handhistory.flopHand2.card1.rank, handhistory.flopHand2.card1.suit, 
					handhistory.flopHand3.card1.rank, handhistory.flopHand3.card1.suit);
				*/

				for (int j = 0; j < player.riverAction.size(); ++j)
				{
					Action action = player.riverAction[j];
					int size = normalizeBetSize(1, action.size, 0, 0.04);
					if (action.type == 'c')
					{
						++probabilityHS[hand][nPotSize][nStackSize][size][0][nVPIP][nPFR][nAF];
						++totalS[nPotSize][nStackSize][size][0][nVPIP][nPFR][nAF];
						if (j + 1 < player.flopAction.size())
							++totalFE[nPotSize][nStackSize][size][0][nVPIP][nPFR][nAF];
						//fprintf(logfile,"Situation: %d %d %d %d %d %d %d\n", hand, nStackSize, size, 0, nVPIP, nPFR, poz);
					}
				
					if (action.type == 'r')
					{
						if (size > mSize) size = mSize;
						++probabilityHS[hand][nPotSize][nStackSize][size][1][nVPIP][nPFR][nAF];
						++totalS[nPotSize][nStackSize][size][1][nVPIP][nPFR][nAF];
						if (j + 1 < player.flopAction.size())
							++totalFE[nPotSize][nStackSize][size][1][nVPIP][nPFR][nAF];
						//fprintf(logfile,"Situation: %d %d %d %d %d %d %d\n", hand, nStackSize, size, 1, nVPIP, nPFR, poz);
					}

					if (action.type == 'x')
					{
						++probabilityHS[hand][nPotSize][nStackSize][0][2][nVPIP][nPFR][nAF];
						++totalS[nPotSize][nStackSize][0][2][nVPIP][nPFR][nAF];
						if (j + 1 < player.flopAction.size())
							++totalFE[nPotSize][nStackSize][0][2][nVPIP][nPFR][nAF];
					}
				}
			}
			else
			{
				bool folded = false;
				bool FE = false;
				for (int j = 0; j < player.riverAction.size(); ++j)
				{
					Action action = player.riverAction[j];
					if (action.type == 'f')
					{
						folded = true;
						FE = true;
						break;
					}
				}

				if (folded)
				{
					for (int j = 0; j < player.riverAction.size(); ++j)
					{
						Action action = player.riverAction[j];
						if (action.type == 'c')
						{
							++probabilityHS[HAND_STRENGTH_NUM - 1][nPotSize][nStackSize][normalizeBetSize(1, action.size, 0, 0.04)][0][nVPIP][nPFR][nAF];
							++totalS[nPotSize][nStackSize][normalizeBetSize(1, action.size, 0, 0.04)][0][nVPIP][nPFR][nAF];
							if (j + 1 < player.flopAction.size())
								++totalFE[nPotSize][nStackSize][normalizeBetSize(1, action.size, 0, 0.04)][0][nVPIP][nPFR][nAF];
						}
				
						if (action.type == 'r')
						{
							++probabilityHS[HAND_STRENGTH_NUM - 1][nPotSize][nStackSize][normalizeBetSize(1, action.size, 0, 0.04)][1][nVPIP][nPFR][nAF];
							++totalS[nPotSize][nStackSize][normalizeBetSize(1, action.size, 0, 0.04)][1][nVPIP][nPFR][nAF];
							if (j + 1 < player.flopAction.size())
								++totalFE[nPotSize][nStackSize][normalizeBetSize(1, action.size, 0, 0.04)][1][nVPIP][nPFR][nAF];
						}

						if (action.type == 'x')
						{
							++probabilityHS[HAND_STRENGTH_NUM - 1][nPotSize][nStackSize][0][2][nVPIP][nPFR][nAF];
							++totalS[nPotSize][nStackSize][0][2][nVPIP][nPFR][nAF];
							if (j + 1 < player.flopAction.size())
								++totalFE[nPotSize][nStackSize][0][2][nVPIP][nPFR][nAF];
						}

					}

					if (FE)
					{
						if (player.riverAction.size() == 1)
						{
							++probabilityFE[0][nPotSize][nStackSize][4][2][nVPIP][nPFR][nAF];
						}
						else
						{
							Action action = player.riverAction[player.flopAction.size() - 2];
							if (action.type == 'c')
							{
								++probabilityFE[0][nPotSize][nStackSize][normalizeBetSize(1, action.size, 0, 0.04)][0][nVPIP][nPFR][nAF];
							}
				
							if (action.type == 'r')
							{
								++probabilityFE[0][nPotSize][nStackSize][normalizeBetSize(1, action.size, 0, 0.04)][1][nVPIP][nPFR][nAF];
							}

							if (action.type == 'x')
							{
								++probabilityFE[0][nPotSize][nStackSize][0][2][nVPIP][nPFR][nAF];
							}
						}
					}
				}
			}
		}
	}

	double getProbabilityHS(int v[])
	{
		return (double)probabilityHS[v[0]][v[1]][v[2]][v[3]][v[4]][v[5]][v[6]][v[7]] / (double)totalS[v[1]][v[2]][v[3]][v[4]][v[5]][v[6]][v[7]];
	}

	double getProbabilityFE(int v[])
	{
		return (double)probabilityFE[v[0]][v[1]][v[2]][v[3]][v[4]][v[5]][v[6]][v[7]] / (double)totalS[v[1]][v[2]][v[3]][v[4]][v[5]][v[6]][v[7]];
	}

	double getNormProbabilityHS(int v[])
	{
		//normalize!!
		return (double)probabilityHS[v[0]][v[1]][v[2]][v[3]][v[4]][v[5]][v[6]][v[7]] / (double)totalS[v[1]][v[2]][v[3]][v[4]][v[5]][v[6]][v[7]];
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