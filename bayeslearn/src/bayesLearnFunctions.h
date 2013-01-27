#pragma once

//DECLARE A CONST BBLIND!!

#include "evaluator.h"
#include "handHistoryParser.h"
#include "database.h"
#include "bayesUtils.h"
#include "bayesFunctions.h"
#include <string>
#include <vector>

using namespace std;

struct DatabaseMapStruct
{
	string name;
	double VPIP;
	double PFR;
	double AF;
};

class BayesLearnPreflop : public BayesPreflop
{
public:
	Database* database;

public:
	BayesLearnPreflop()
	{
		preflop_nums[0] = PREFLOP_HAND_STRENGTH_NUM;
		preflop_nums[1] = PREFLOP_PLAYER_STACK_SIZE_NUM;
		preflop_nums[2] = PREFLOP_PLAYER_BET_SIZE_NUM;
		preflop_nums[3] = PREFLOP_PLAYER_LINE_NUM;
		preflop_nums[4] = PREFLOP_PLAYER_VPIP_NUM;
		preflop_nums[5] = PREFLOP_PLAYER_PFR_NUM;
		preflop_nums[6] = PREFLOP_PLAYER_POZ_NUM;

		database = new Database("127.0.0.1", "root", "root", "kutya");
		memset(totalPreflop, 0, sizeof(totalPreflop));
		memset(probabilityPreflop, 0, sizeof(probabilityPreflop));
		memset(totalFE, 0, sizeof(totalFE));
		memset(probabilityFE, 0, sizeof(probabilityFE));
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
		printf("Writing bayes network\n");
		FILE* f = fopen(filename.c_str(), "w");
		back(f, true, 1);
		backFE(f, true, 1);
		fclose(f);
	}

	void updateProbabilities(HandHistory& handhistory, map<string, DatabaseMapStruct>& databaseMAP)
	{
		if (handhistory.getPlayerHistories().size() > 6)
		{
			printf("Hand with more then 6 players\n");
			return ;
		}

		int mSize = maxBetSize(handhistory);
		for (int i = 0; i < handhistory.getPlayerHistories().size(); ++i)
		{
			PlayerHistory player = handhistory.getPlayerHistories()[i];
			double VPIP;
			double PFR;

			string name = player.getPlayerName();
			if (databaseMAP[name].name == name)
			{
				VPIP = databaseMAP[name].VPIP;
				if (VPIP < 0) continue;
				PFR = databaseMAP[name].PFR;
			}
			else
			{
				int handnr = database->getHandnr(name);
				if (handnr < 15)
				{
					databaseMAP[name].name = name;
					databaseMAP[name].VPIP = -1;
					continue;
				}
				else
				{
					VPIP = database->getVPIP(player.getPlayerName());
					if (VPIP > 100 || VPIP < 0)
					{
						printf("ERROR VPIP: %lf\n", VPIP);
						printf("NAME: %s\n", player.getPlayerName().c_str());
					}

					PFR = database->getPFR(player.getPlayerName());
					if (PFR > 100 || PFR < 0)
					{
						printf("ERROR PFR: %lf\n", PFR);
						printf("NAME: %s\n", player.getPlayerName().c_str());
					}

					double AF = database->getAF(player.getPlayerName());
					
					databaseMAP[name].name = name;
					databaseMAP[name].VPIP = VPIP;
					databaseMAP[name].PFR = PFR;
					databaseMAP[name].AF = AF;
				}
			}

			int poz = player.getPosition() + 3;
			if (poz < 0 || poz > 5)
			{
				printf("ERROR poz\n");
			}

			int nVPIP = normalizeVPIP(VPIP);
			int nPFR = normalizePFR(PFR);
			int nStackSize = normalizeStackSize(player.getBalance(), 0.04);
			int nLine = 0;

			if (player.getPreflopAction().size() > 0)
			{
				if (player.getPreflopAction()[0].getType() == 'c' || player.getPreflopAction()[0].getType() == 'f')
				{
					++totalFE[nStackSize][4][2][nVPIP][nPFR][poz];
				}
				if (player.getPreflopAction()[0].getType() == 'r' && player.getPreflopAction()[0].getSize() > firstRaise(handhistory))
				{
					++totalFE[nStackSize][4][2][nVPIP][nPFR][poz];
				}
			}

			if (player.isHandKnown())
			{
				int hand = normalizeHand(player.getHand());

				for (int j = 0; j < player.getPreflopAction().size(); ++j)
				{
					Action action = player.getPreflopAction()[j];
					int size = normalizeBetSize(1, action.getSize(), 0, 0.04);
					if (action.getType() == 'c')
					{
						++probabilityPreflop[hand][nStackSize][size][0][nVPIP][nPFR][poz];
						++totalPreflop[nStackSize][size][0][nVPIP][nPFR][poz];
						if (j + 1 < player.getPreflopAction().size())
							++totalFE[nStackSize][size][0][nVPIP][nPFR][poz];
					}

					if (action.getType() == 'r')
					{
						if (size > mSize) size = mSize;
						++probabilityPreflop[hand][nStackSize][size][1][nVPIP][nPFR][poz];
						++totalPreflop[nStackSize][size][1][nVPIP][nPFR][poz];
						if (j + 1 < player.getPreflopAction().size())
							++totalFE[nStackSize][size][1][nVPIP][nPFR][poz];
					}
				}
			}
			else
			{
				bool folded = player.isFolded();
				bool FE = player.isFoldedInRound(player.getPreflopAction());

				if (folded)
				{
					for (int j = 0; j < player.getPreflopAction().size(); ++j)
					{
						Action action = player.getPreflopAction()[j];
						int size = normalizeBetSize(1, action.getSize(), 0, 0.04);
						if (action.getType() == 'c')
						{
							++probabilityPreflop[0][nStackSize][size][0][nVPIP][nPFR][poz];
							++totalPreflop[nStackSize][size][0][nVPIP][nPFR][poz];
							if (j + 1 < player.getPreflopAction().size())
								++totalFE[nStackSize][size][0][nVPIP][nPFR][poz];
						}

						if (action.getType() == 'r')
						{
							++probabilityPreflop[0][nStackSize][size][1][nVPIP][nPFR][poz];
							++totalPreflop[nStackSize][size][1][nVPIP][nPFR][poz];
							if (j + 1 < player.getPreflopAction().size())
								++totalFE[nStackSize][size][1][nVPIP][nPFR][poz];
						}
					}


					if (FE)
					{
						if (player.getPreflopAction().size() == 1)
						{
							++probabilityFE[nStackSize][4][2][nVPIP][nPFR][poz];
						}
						else
						{
							Action action = player.getPreflopAction()[player.getPreflopAction().size() - 2];
							int size = normalizeBetSize(1, action.getSize(), 0, 0.04);
							if (action.getType() == 'c')
							{
								++probabilityFE[nStackSize][size][0][nVPIP][nPFR][poz];
							}

							if (action.getType() == 'r')
							{
								if (size > mSize) size = mSize;
								++probabilityFE[nStackSize][size][1][nVPIP][nPFR][poz];
							}

							if (action.getType() == 'x')
							{
								++probabilityFE[nStackSize][0][2][nVPIP][nPFR][poz];
							}
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
				fprintf(f,"%d ", totalPreflop[v[1]][v[2]][v[3]][v[4]][v[5]][v[6]]);
				for (int i = 0; i < PREFLOP_HAND_STRENGTH_NUM; ++i)
				{
					fprintf(f,"%d ", probabilityPreflop[i][v[1]][v[2]][v[3]][v[4]][v[5]][v[6]]);
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

	int maxBetSize(HandHistory& handhistory)
	{
		int lastActionRound = 0;
		for (int i = 0; i < handhistory.getPlayerHistories().size(); ++i)
		{
			PlayerHistory player = handhistory.getPlayerHistories()[i];
			lastActionRound = max(lastActionRound, player.getPreflopAction().size());
		}
		double mSize = 0;
		double mSize2 = 0;
		
		for (int i = 0; i < handhistory.getPlayerHistories().size(); ++i)
		{
			PlayerHistory player = handhistory.getPlayerHistories()[i];
			if (player.getPreflopAction().size() == lastActionRound || player.getRiverAction().size() == lastActionRound - 1)
			{
				double size = player.getBalance();
				if (size > mSize)
				{
					mSize2 = mSize;
					mSize = size;
				}
				else if (size > mSize2)
				{
					mSize2 = size;
				}
			}
		}

		int size = normalizeBetSize(1, mSize2, 0, 0.04);
		return size;
	}

	double firstRaise(HandHistory& handhistory)
	{
		double min = 0x3f3f3f3f;
		for (int i = 0; i < handhistory.getPlayerHistories().size(); ++i)
		{
			PlayerHistory player = handhistory.getPlayerHistories()[i];
			if (player.getPreflopAction().size() > 0)
			{
				if (player.getPreflopAction()[0].getType() == 'r')
				{
					if (min > player.getPreflopAction()[0].getSize()) min = player.getPreflopAction()[0].getSize();
				}
			}
		}

		return min;
	}
};
class BayesLearnFlop : public BayesFlop
{
public:
	Database* database;

public:
	BayesLearnFlop()
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

	void updateProbabilities(HandHistory& handhistory, map<string, DatabaseMapStruct>& databaseMAP)
	{
		if (handhistory.getPlayerHistories().size() > 6)
		{
			printf("Hand with more then 6 players\n");
			return ;
		}

		double totalpot = 1.5 * 0.04;
		for (int i = 0; i < handhistory.getPlayerHistories().size(); ++i)
		{
			PlayerHistory player = handhistory.getPlayerHistories()[i];
			double maxcall = 0;
			for (int j = 0; j < player.getPreflopAction().size(); ++j)
				{
					Action action = player.getPreflopAction()[j];
					if (action.getType() == 'c' || action.getType() == 'r')
					{
						if (action.getSize() > maxcall) maxcall = action.getSize();
					}
				}
			totalpot += maxcall;
		}


		int mSize = maxBetSize(handhistory, totalpot);
		int nPotSize = normalizePotSize(2, totalpot, 0.04);

		for (int i = 0; i < handhistory.getPlayerHistories().size(); ++i)
		{
			PlayerHistory player = handhistory.getPlayerHistories()[i];
			double VPIP;
			double PFR;
			double AF;

			string name = player.getPlayerName();
			if (databaseMAP[name].name == name)
			{
				VPIP = databaseMAP[name].VPIP;
				if (VPIP < 0) continue;
				PFR = databaseMAP[name].PFR;
				AF = databaseMAP[name].AF;
			}
			else
			{
				int handnr = database->getHandnr(name);
				if (handnr < 15)
				{
					databaseMAP[name].name = name;
					databaseMAP[name].VPIP = -1;
					continue;
				}
				else
				{
					VPIP = database->getVPIP(player.getPlayerName());
					if (VPIP > 100 || VPIP < 0)
					{
						printf("ERROR VPIP: %lf\n", VPIP);
						printf("NAME: %s\n", player.getPlayerName().c_str());
					}

					PFR = database->getPFR(player.getPlayerName());
					if (PFR > 100 || PFR < 0)
					{
						printf("ERROR PFR: %lf\n", PFR);
						printf("NAME: %s\n", player.getPlayerName().c_str());
					}

					AF = database->getAF(player.getPlayerName());
					if (AF < 0) AF = 4;

					databaseMAP[name].name = name;
					databaseMAP[name].VPIP = VPIP;
					databaseMAP[name].PFR = PFR;
					databaseMAP[name].AF = AF;
				}
			}

			int nVPIP = normalizeVPIP(VPIP);
			int nPFR = normalizePFR(PFR);
			int nAF = normalizeAF(AF);
			int nStackSize = normalizeStackSize(player.getBalance(), 0.04);
			int nLine = 0;

			
			if (player.getFlopAction().size() > 0)
			{
				if (player.getFlopAction()[0].getType() == 'c' || player.getFlopAction()[0].getType() == 'f')
				{
					++totalFE[nPotSize][nStackSize][4][2][nVPIP][nPFR][nAF];
				}
				if (player.getFlopAction()[0].getType() == 'r' && player.getFlopAction()[0].getSize() > firstRaise(handhistory))
				{
					++totalFE[nPotSize][nStackSize][4][2][nVPIP][nPFR][nAF];
				}
			}

			if (player.isHandKnown())
			{
				int hand = Evaluator::cardStrength(player.getHand().getCard1(), player.getHand().getCard2(), handhistory.getFlopCard1(), handhistory.getFlopCard2(), handhistory.getFlopCard3());

				for (int j = 0; j < player.getFlopAction().size(); ++j)
				{
					Action action = player.getFlopAction()[j];
					int size = normalizeBetSize(2, action.getSize(), totalpot, 0.04);
					if (action.getType() == 'c')
					{
						++probabilityHS[hand][nPotSize][nStackSize][size][0][nVPIP][nPFR][nAF];
						++totalS[nPotSize][nStackSize][size][0][nVPIP][nPFR][nAF];
						if (j + 1 < player.getFlopAction().size())
							++totalFE[nPotSize][nStackSize][size][0][nVPIP][nPFR][nAF];
					}

					if (action.getType() == 'r')
					{
						if (size > mSize) size = mSize;
						++probabilityHS[hand][nPotSize][nStackSize][size][1][nVPIP][nPFR][nAF];
						++totalS[nPotSize][nStackSize][size][1][nVPIP][nPFR][nAF];
						if (j + 1 < player.getFlopAction().size())
							++totalFE[nPotSize][nStackSize][size][1][nVPIP][nPFR][nAF];
					}

					if (action.getType() == 'x')
					{
						++probabilityHS[hand][nPotSize][nStackSize][0][2][nVPIP][nPFR][nAF];
						++totalS[nPotSize][nStackSize][0][2][nVPIP][nPFR][nAF];
						if (j + 1 < player.getFlopAction().size())
							++totalFE[nPotSize][nStackSize][0][2][nVPIP][nPFR][nAF];
					}
				}
			}
			else
			{
				bool folded = player.isFolded();
				bool FE = player.isFoldedInRound(player.getFlopAction());

				if (folded)
				{
					for (int j = 0; j < player.getFlopAction().size(); ++j)
					{
						Action action = player.getFlopAction()[j];
						int size = normalizeBetSize(2, action.getSize(), totalpot, 0.04);
						if (action.getType() == 'c')
						{
							++probabilityHS[HAND_STRENGTH_NUM - 1][nPotSize][nStackSize][size][0][nVPIP][nPFR][nAF];
							++totalS[nPotSize][nStackSize][size][0][nVPIP][nPFR][nAF];
							if (j + 1 < player.getFlopAction().size())
								++totalFE[nPotSize][nStackSize][size][0][nVPIP][nPFR][nAF];
						}

						if (action.getType() == 'r')
						{
							++probabilityHS[HAND_STRENGTH_NUM - 1][nPotSize][nStackSize][size][1][nVPIP][nPFR][nAF];
							++totalS[nPotSize][nStackSize][size][1][nVPIP][nPFR][nAF];
							if (j + 1 < player.getFlopAction().size())
								++totalFE[nPotSize][nStackSize][size][1][nVPIP][nPFR][nAF];
						}

						if (action.getType() == 'x')
						{
							++probabilityHS[HAND_STRENGTH_NUM - 1][nPotSize][nStackSize][0][2][nVPIP][nPFR][nAF];
							++totalS[nPotSize][nStackSize][0][2][nVPIP][nPFR][nAF];
							if (j + 1 < player.getFlopAction().size())
								++totalFE[nPotSize][nStackSize][0][2][nVPIP][nPFR][nAF];
						}
					}

					if (FE)
					{
						if (player.getFlopAction().size() == 1)
						{
							++probabilityFE[0][nPotSize][nStackSize][4][2][nVPIP][nPFR][nAF];
						}
						else
						{
							Action action = player.getFlopAction()[player.getFlopAction().size() - 2];
							int size = normalizeBetSize(2, action.getSize(), totalpot, 0.04);
							if (action.getType() == 'c')
							{
								++probabilityFE[0][nPotSize][nStackSize][size][0][nVPIP][nPFR][nAF];
							}

							if (action.getType() == 'r')
							{
								//if (size > mSize) size = mSize;
								++probabilityFE[0][nPotSize][nStackSize][size][1][nVPIP][nPFR][nAF];
							}

							if (action.getType() == 'x')
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

	int maxBetSize(HandHistory& handhistory, double potcommon)
	{
		int lastActionRound = 0;
		for (int i = 0; i < handhistory.getPlayerHistories().size(); ++i)
		{
			PlayerHistory player = handhistory.getPlayerHistories()[i];
			lastActionRound = max(lastActionRound, player.getFlopAction().size());
		}
		double mSize = 0;
		double mSize2 = 0;
		
		for (int i = 0; i < handhistory.getPlayerHistories().size(); ++i)
		{
			PlayerHistory player = handhistory.getPlayerHistories()[i];
			if (player.getFlopAction().size() == lastActionRound || player.getRiverAction().size() == lastActionRound - 1)
			{
				double size = player.getBalanceInRound(2);
				if (size > mSize)
				{
					mSize2 = mSize;
					mSize = size;
				}
				else if (size > mSize2)
				{
					mSize2 = size;
				}
			}
		}

		int size = normalizeBetSize(2, mSize2, potcommon, 0.04);
		return size;
	}

	double firstRaise(HandHistory& handhistory)
	{
		double min = 0x3f3f3f3f;
		for (int i = 0; i < handhistory.getPlayerHistories().size(); ++i)
		{
			PlayerHistory player = handhistory.getPlayerHistories()[i];
			if (player.getFlopAction().size() > 0)
			{
				if (player.getFlopAction()[0].getType() == 'r')
				{
					if (min > player.getFlopAction()[0].getSize()) min = player.getFlopAction()[0].getSize();
				}
			}
		}

		return min;
	}

};
class BayesLearnTurn : public BayesTurn
{
public:
	Database* database;

public:
	BayesLearnTurn()
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

		database = new Database("127.0.0.1", "root", "root", "kutya");
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

	void updateProbabilities(HandHistory& handhistory, map<string, DatabaseMapStruct>& databaseMAP)
	{
		if (handhistory.getPlayerHistories().size() > 6)
		{
			printf("Hand with more then 6 players\n");
			return ;
		}

		double totalpot = 1.5 * 0.04;
		for (int i = 0; i < handhistory.getPlayerHistories().size(); ++i)
		{
			PlayerHistory player = handhistory.getPlayerHistories()[i];
			double maxcall = 0;
			for (int j = 0; j < player.getPreflopAction().size(); ++j)
				{
					Action action = player.getPreflopAction()[j];
					if (action.getType() == 'c' || action.getType() == 'r')
					{
						if (action.getSize() > maxcall) maxcall = action.getSize();
					}
				}
			totalpot += maxcall;
		}

		int flopPotCommon = normalizePotSize(2, totalpot, 0.04);

		for (int i = 0; i < handhistory.getPlayerHistories().size(); ++i)
		{
			PlayerHistory player = handhistory.getPlayerHistories()[i];
			double maxcall = 0;
			for (int j = 0; j < player.getFlopAction().size(); ++j)
				{
					Action action = player.getFlopAction()[j];
					if (action.getType() == 'c' || action.getType() == 'r')
					{
						if (action.getSize() > maxcall) maxcall = action.getSize();
					}
				}
			totalpot += maxcall;
		}

		int mSize = maxBetSize(handhistory, totalpot);
		int nPotSize = normalizePotSize(3, totalpot, 0.04);

		for (int i = 0; i < handhistory.getPlayerHistories().size(); ++i)
		{
			PlayerHistory player = handhistory.getPlayerHistories()[i];
			double VPIP;
			double PFR;
			double AF;

			string name = player.getPlayerName();
			if (databaseMAP[name].name == name)
			{
				VPIP = databaseMAP[name].VPIP;
				if (VPIP < 0) continue;
				PFR = databaseMAP[name].PFR;
				AF = databaseMAP[name].AF;
			}
			else
			{
				int handnr = database->getHandnr(name);
				if (handnr < 15)
				{
					databaseMAP[name].name = name;
					databaseMAP[name].VPIP = -1;
					continue;
				}
				else
				{
					VPIP = database->getVPIP(player.getPlayerName());
					if (VPIP > 100 || VPIP < 0)
					{
						printf("ERROR VPIP: %lf\n", VPIP);
						printf("NAME: %s\n", player.getPlayerName().c_str());
					}

					PFR = database->getPFR(player.getPlayerName());
					if (PFR > 100 || PFR < 0)
					{
						printf("ERROR PFR: %lf\n", PFR);
						printf("NAME: %s\n", player.getPlayerName().c_str());
					}

					AF = database->getAF(player.getPlayerName());
					if (AF < 0) AF = 4;

					databaseMAP[name].name = name;
					databaseMAP[name].VPIP = VPIP;
					databaseMAP[name].PFR = PFR;
					databaseMAP[name].AF = AF;
				}
			}

			int nVPIP = normalizeVPIP(VPIP);
			int nPFR = normalizePFR(PFR);
			int nAF = normalizeAF(AF);
			int nStackSize = normalizeStackSize(player.getBalance(), 0.04);
			int nLine = 0;

			if (player.getTurnAction().size() > 0)
			{
				if (player.getTurnAction()[0].getType() == 'c' || player.getTurnAction()[0].getType() == 'f')
				{
					++totalFE[nPotSize][nStackSize][4][2][nVPIP][nPFR][nAF][flopPotCommon];
				}
				if (player.getTurnAction()[0].getType() == 'r' && player.getTurnAction()[0].getSize() > firstRaise(handhistory))
				{
					++totalFE[nPotSize][nStackSize][4][2][nVPIP][nPFR][nAF][flopPotCommon];
				}
			}

			if (player.isHandKnown())
			{
				int hand = Evaluator::cardStrength(player.getHand().getCard1(), player.getHand().getCard2(), handhistory.getFlopCard1(), handhistory.getFlopCard2(), handhistory.getFlopCard3(), handhistory.getTurnCard());

				for (int j = 0; j < player.getTurnAction().size(); ++j)
				{
					Action action = player.getTurnAction()[j];
					int size = normalizeBetSize(2, action.getSize(), totalpot, 0.04);
					if (action.getType() == 'c')
					{
						++probabilityHS[hand][nPotSize][nStackSize][size][0][nVPIP][nPFR][nAF][flopPotCommon];
						++totalS[nPotSize][nStackSize][size][0][nVPIP][nPFR][nAF][flopPotCommon];

						if (j + 1 < player.getTurnAction().size())
							++totalFE[nPotSize][nStackSize][size][0][nVPIP][nPFR][nAF][flopPotCommon];
					}

					if (action.getType() == 'r')
					{
						if (size > mSize) size = mSize;
						++probabilityHS[hand][nPotSize][nStackSize][size][1][nVPIP][nPFR][nAF][flopPotCommon];
						++totalS[nPotSize][nStackSize][size][1][nVPIP][nPFR][nAF][flopPotCommon];

						if (j + 1 < player.getTurnAction().size())
							++totalFE[nPotSize][nStackSize][size][1][nVPIP][nPFR][nAF][flopPotCommon];
					}

					if (action.getType() == 'x')
					{
						++probabilityHS[hand][nPotSize][nStackSize][0][2][nVPIP][nPFR][nAF][flopPotCommon];
						++totalS[nPotSize][nStackSize][0][2][nVPIP][nPFR][nAF][flopPotCommon];
						if (j + 1 < player.getTurnAction().size())
							++totalFE[nPotSize][nStackSize][0][2][nVPIP][nPFR][nAF][flopPotCommon];
					}
				}
			}
			else
			{
				bool folded = player.isFolded();
				bool FE = player.isFoldedInRound(player.getTurnAction());

				if (folded)
				{
					for (int j = 0; j < player.getTurnAction().size(); ++j)
					{
						Action action = player.getTurnAction()[j];
						int size = normalizeBetSize(2, action.getSize(), totalpot, 0.04);
						if (action.getType() == 'c')
						{
							++probabilityHS[HAND_STRENGTH_NUM - 1][nPotSize][nStackSize][size][0][nVPIP][nPFR][nAF][flopPotCommon];
							++totalS[nPotSize][nStackSize][size][0][nVPIP][nPFR][nAF][flopPotCommon];
							if (j + 1 < player.getTurnAction().size())
								++totalFE[nPotSize][nStackSize][size][0][nVPIP][nPFR][nAF][flopPotCommon];
						}

						if (action.getType() == 'r')
						{
							++probabilityHS[HAND_STRENGTH_NUM - 1][nPotSize][nStackSize][size][1][nVPIP][nPFR][nAF][flopPotCommon];
							++totalS[nPotSize][nStackSize][size][1][nVPIP][nPFR][nAF][flopPotCommon];
							if (j + 1 < player.getTurnAction().size())
								++totalFE[nPotSize][nStackSize][size][1][nVPIP][nPFR][nAF][flopPotCommon];
						}

						if (action.getType() == 'x')
						{
							++probabilityHS[HAND_STRENGTH_NUM - 1][nPotSize][nStackSize][0][2][nVPIP][nPFR][nAF][flopPotCommon];
							++totalS[nPotSize][nStackSize][0][2][nVPIP][nPFR][nAF][flopPotCommon];
							if (j + 1 < player.getTurnAction().size())
								++totalFE[nPotSize][nStackSize][0][2][nVPIP][nPFR][nAF][flopPotCommon];
						}
					}

					if (FE)
					{
						if (player.getTurnAction().size() == 1)
						{
							++probabilityFE[0][nPotSize][nStackSize][4][2][nVPIP][nPFR][nAF][flopPotCommon];
						}
						else
						{
							Action action = player.getTurnAction()[player.getTurnAction().size() - 2];
							int size = normalizeBetSize(2, action.getSize(), totalpot, 0.04);
							if (action.getType() == 'c')
							{
								++probabilityFE[0][nPotSize][nStackSize][size][0][nVPIP][nPFR][nAF][flopPotCommon];
							}

							if (action.getType() == 'r')
							{
								//if (size > mSize) size = mSize;
								++probabilityFE[0][nPotSize][nStackSize][size][1][nVPIP][nPFR][nAF][flopPotCommon];
							}

							if (action.getType() == 'x')
							{
								++probabilityFE[0][nPotSize][nStackSize][0][2][nVPIP][nPFR][nAF][flopPotCommon];
							}
						}
					}
				}
			}
		}
	}

	double getProbabilityHS(int v[])
	{
		return (double)probabilityHS[v[0]][v[1]][v[2]][v[3]][v[4]][v[5]][v[6]][v[7]][v[8]] / (double)totalS[v[1]][v[2]][v[3]][v[4]][v[5]][v[6]][v[7]][v[8]];
	}

	double getProbabilityFE(int v[])
	{
		return (double)probabilityFE[v[0]][v[1]][v[2]][v[3]][v[4]][v[5]][v[6]][v[7]][v[8]] / (double)totalS[v[1]][v[2]][v[3]][v[4]][v[5]][v[6]][v[7]][v[8]];
	}

	double getNormProbabilityHS(int v[])
	{
		//normalize!!
		return (double)probabilityHS[v[0]][v[1]][v[2]][v[3]][v[4]][v[5]][v[6]][v[7]][v[8]] / (double)totalS[v[1]][v[2]][v[3]][v[4]][v[5]][v[6]][v[7]][v[8]];
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
				for (int i = 0; i < HAND_STRENGTH_NUM; ++i)
					fscanf(f,"%d ", &probabilityHS[i][v[2]][v[3]][v[4]][v[5]][v[6]][v[7]][v[8]][v[9]]);
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


	int maxBetSize(HandHistory& handhistory, double potcommon)
	{
		int lastActionRound = 0;
		for (int i = 0; i < handhistory.getPlayerHistories().size(); ++i)
		{
			PlayerHistory player = handhistory.getPlayerHistories()[i];
			lastActionRound = max(lastActionRound, player.getTurnAction().size());
		}
		double mSize = 0;
		double mSize2 = 0;
		
		for (int i = 0; i < handhistory.getPlayerHistories().size(); ++i)
		{
			PlayerHistory player = handhistory.getPlayerHistories()[i];
			if (player.getTurnAction().size() == lastActionRound || player.getRiverAction().size() == lastActionRound - 1)
			{
				double size = player.getBalanceInRound(3);
				if (size > mSize)
				{
					mSize2 = mSize;
					mSize = size;
				}
				else if (size > mSize2)
				{
					mSize2 = size;
				}
			}
		}

		int size = normalizeBetSize(2, mSize2, potcommon, 0.04);
		return size;
	}

	double firstRaise(HandHistory& handhistory)
	{
		double min = 0x3f3f3f3f;
		for (int i = 0; i < handhistory.getPlayerHistories().size(); ++i)
		{
			PlayerHistory player = handhistory.getPlayerHistories()[i];
			if (player.getTurnAction().size() > 0)
			{
				if (player.getTurnAction()[0].getType() == 'r')
				{
					if (min > player.getTurnAction()[0].getSize()) min = player.getTurnAction()[0].getSize();
				}
			}
		}

		return min;
	}

};
class BayesLearnRiver : public BayesRiver
{
public:
	Database* database;

public:
	BayesLearnRiver()
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

		database = new Database("127.0.0.1", "root", "root", "kutya");
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

	void updateProbabilities(HandHistory& handhistory, map<string, DatabaseMapStruct>& databaseMAP)
	{
		if (handhistory.getPlayerHistories().size() > 6)
		{
			printf("Hand with more then 6 players\n");
			return ;
		}

		double totalpot = 1.5 * 0.04;
		for (int i = 0; i < handhistory.getPlayerHistories().size(); ++i)
		{
			PlayerHistory player = handhistory.getPlayerHistories()[i];
			double maxcall = 0;
			for (int j = 0; j < player.getPreflopAction().size(); ++j)
				{
					Action action = player.getPreflopAction()[j];
					if (action.getType() == 'c' || action.getType() == 'r')
					{
						if (action.getSize() > maxcall) maxcall = action.getSize();
					}
				}
			totalpot += maxcall;
		}

		int flopPotCommon = normalizePotSize(2, totalpot, 0.04);

		for (int i = 0; i < handhistory.getPlayerHistories().size(); ++i)
		{
			PlayerHistory player = handhistory.getPlayerHistories()[i];
			double maxcall = 0;
			for (int j = 0; j < player.getFlopAction().size(); ++j)
				{
					Action action = player.getFlopAction()[j];
					if (action.getType() == 'c' || action.getType() == 'r')
					{
						if (action.getSize() > maxcall) maxcall = action.getSize();
					}
				}
			totalpot += maxcall;
		}
		for (int i = 0; i < handhistory.getPlayerHistories().size(); ++i)
		{
			PlayerHistory player = handhistory.getPlayerHistories()[i];
			double maxcall = 0;
			for (int j = 0; j < player.getTurnAction().size(); ++j)
				{
					Action action = player.getTurnAction()[j];
					if (action.getType() == 'c' || action.getType() == 'r')
					{
						if (action.getSize() > maxcall) maxcall = action.getSize();
					}
				}
			totalpot += maxcall;
		}

		int mSize = maxBetSize(handhistory, totalpot);
		int nPotSize = normalizePotSize(4, totalpot, 0.04);

		for (int i = 0; i < handhistory.getPlayerHistories().size(); ++i)
		{
			PlayerHistory player = handhistory.getPlayerHistories()[i];
			double VPIP;
			double PFR;
			double AF;

			string name = player.getPlayerName();
			if (databaseMAP[name].name == name)
			{
				VPIP = databaseMAP[name].VPIP;
				if (VPIP < 0) continue;
				PFR = databaseMAP[name].PFR;
				AF = databaseMAP[name].AF;
			}
			else
			{
				int handnr = database->getHandnr(name);
				if (handnr < 15)
				{
					databaseMAP[name].name = name;
					databaseMAP[name].VPIP = -1;
					continue;
				}
				else
				{
					VPIP = database->getVPIP(player.getPlayerName());
					if (VPIP > 100 || VPIP < 0)
					{
						printf("ERROR VPIP: %lf\n", VPIP);
						printf("NAME: %s\n", player.getPlayerName().c_str());
					}

					PFR = database->getPFR(player.getPlayerName());
					if (PFR > 100 || PFR < 0)
					{
						printf("ERROR PFR: %lf\n", PFR);
						printf("NAME: %s\n", player.getPlayerName().c_str());
					}

					AF = database->getAF(player.getPlayerName());
					if (AF < 0) AF = 4;

					databaseMAP[name].name = name;
					databaseMAP[name].VPIP = VPIP;
					databaseMAP[name].PFR = PFR;
					databaseMAP[name].AF = AF;
				}
			}

			int nVPIP = normalizeVPIP(VPIP);
			int nPFR = normalizePFR(PFR);
			int nAF = normalizeAF(AF);
			int nStackSize = normalizeStackSize(player.getBalance(), 0.04);
			int nLine = 0;

			if (player.getRiverAction().size() > 0)
			{
				if (player.getRiverAction()[0].getType() == 'c' || player.getRiverAction()[0].getType() == 'f')
				{
					++totalFE[nPotSize][nStackSize][4][2][nVPIP][nPFR][nAF][flopPotCommon];
				}
				if (player.getRiverAction()[0].getType() == 'r' && player.getRiverAction()[0].getSize() > firstRaise(handhistory))
				{
					++totalFE[nPotSize][nStackSize][4][2][nVPIP][nPFR][nAF][flopPotCommon];
				}
			}

			if (player.isHandKnown())
			{
				int hand = Evaluator::cardStrength(player.getHand().getCard1(), player.getHand().getCard2(), handhistory.getFlopCard1(), handhistory.getFlopCard2(), handhistory.getFlopCard3(), handhistory.getTurnCard(), handhistory.getRiverCard());

				for (int j = 0; j < player.getRiverAction().size(); ++j)
				{
					Action action = player.getRiverAction()[j];
					int size = normalizeBetSize(4, action.getSize(), totalpot, 0.04);
					if (action.getType() == 'c')
					{
						++probabilityHS[hand][nPotSize][nStackSize][size][0][nVPIP][nPFR][nAF][flopPotCommon];
						++totalS[nPotSize][nStackSize][size][0][nVPIP][nPFR][nAF][flopPotCommon];
						if (j + 1 < player.getRiverAction().size())
							++totalFE[nPotSize][nStackSize][size][0][nVPIP][nPFR][nAF][flopPotCommon];
					}

					if (action.getType() == 'r')
					{
						if (size > mSize) size = mSize;
						++probabilityHS[hand][nPotSize][nStackSize][size][1][nVPIP][nPFR][nAF][flopPotCommon];
						++totalS[nPotSize][nStackSize][size][1][nVPIP][nPFR][nAF][flopPotCommon];
						if (j + 1 < player.getRiverAction().size())
						{
							++totalFE[nPotSize][nStackSize][size][1][nVPIP][nPFR][nAF][flopPotCommon];
							
							/*
							if (action.getSize() > 60 * 0.04 && nPotSize > 5)
							{
								printf("Situation= %d %d %d %d %d %d %d %d\n", nPotSize, nStackSize, size, 1, nVPIP, nPFR, nAF, flopPotCommon);
								printf("Total= %d\n", totalFE[nPotSize][nStackSize][size][1][nVPIP][nPFR][nAF][flopPotCommon]);
								printf("Potcommon= %lf Balance= %lf\n", totalpot, player.getBalance());
								printf("%s\n", player.getPlayerName().c_str());
								for (int k = 0; k < player.getRiverAction().size(); ++k)
								{
									printf("%s\n", player.getRiverAction()[k].toString().c_str());
								}
								printf("Hand: %s\n", player.getHand().toString().c_str());
								printf("Board: %s%s%s%s%s\n", handhistory.getFlopCard1().toString().c_str(), 
									handhistory.getFlopCard2().toString().c_str(), handhistory.getFlopCard3().toString().c_str(), handhistory.getTurnCard().toString().c_str(), handhistory.getRiverCard().toString().c_str());
							}
							*/
						}
					}

					if (action.getType() == 'x')
					{
						++probabilityHS[hand][nPotSize][nStackSize][0][2][nVPIP][nPFR][nAF][flopPotCommon];
						++totalS[nPotSize][nStackSize][0][2][nVPIP][nPFR][nAF][flopPotCommon];
						if (j + 1 < player.getRiverAction().size())
						{
							++totalFE[nPotSize][nStackSize][0][2][nVPIP][nPFR][nAF][flopPotCommon];
							
							/*
							if (nVPIP == 3 && nPotSize == 6 && flopPotCommon < 3)
							{
								printf("Situation= %d %d %d %d %d %d %d %d\n", nPotSize, nStackSize, size, 1, nVPIP, nPFR, nAF, flopPotCommon);
								printf("%s\n", player.getPlayerName().c_str());
								printf("Total= %d\n", totalFE[nPotSize][nStackSize][size][1][nVPIP][nPFR][nAF][flopPotCommon]);
								printf("Potcommon= %lf Balance= %lf\n", totalpot, player.getBalance());
								for (int k = 0; k < player.getRiverAction().size(); ++k)
								{
									printf("%s\n", player.getRiverAction()[k].toString().c_str());
								}
								printf("Hand: %s\n", player.getHand().toString().c_str());
								printf("Board: %s%s%s%s%s\n", handhistory.getFlopCard1().toString().c_str(), 
									handhistory.getFlopCard2().toString().c_str(), handhistory.getFlopCard3().toString().c_str(), handhistory.getTurnCard().toString().c_str(), handhistory.getRiverCard().toString().c_str());
							}
							*/
						}
					}
				}
			}
			else
			{
				bool folded = false;
				bool FE = false;
				for (int j = 0; j < player.getRiverAction().size(); ++j)
				{
					Action action = player.getRiverAction()[j];
					if (action.getType() == 'f')
					{
						folded = true;
						FE = true;
						break;
					}
				}

				if (folded)
				{
					for (int j = 0; j < player.getRiverAction().size(); ++j)
					{
						Action action = player.getRiverAction()[j];
						int size = normalizeBetSize(2, action.getSize(), totalpot, 0.04);
						if (action.getType() == 'c')
						{
							++probabilityHS[HAND_STRENGTH_NUM - 1][nPotSize][nStackSize][size][0][nVPIP][nPFR][nAF][flopPotCommon];
							++totalS[nPotSize][nStackSize][size][0][nVPIP][nPFR][nAF][flopPotCommon];
							if (j + 1 < player.getRiverAction().size())
								++totalFE[nPotSize][nStackSize][size][0][nVPIP][nPFR][nAF][flopPotCommon];
						}

						if (action.getType() == 'r')
						{
							++probabilityHS[HAND_STRENGTH_NUM - 1][nPotSize][nStackSize][size][1][nVPIP][nPFR][nAF][flopPotCommon];
							++totalS[nPotSize][nStackSize][size][1][nVPIP][nPFR][nAF][flopPotCommon];
							if (j + 1 < player.getRiverAction().size())
							{
								++totalFE[nPotSize][nStackSize][size][1][nVPIP][nPFR][nAF][flopPotCommon];
								
								/*
								if (action.getSize() > 60 * 0.04 && nPotSize > 5)
								{
									printf("Size = %d\n", player.getRiverAction().size());
									printf("%s\n", player.getPlayerName().c_str());
									for (int k = 0; k < player.getRiverAction().size(); ++k)
									{
										printf("%s\n", player.getRiverAction()[k].toString().c_str());
									}
								}
								*/
							}
						}

						if (action.getType() == 'x')
						{
							++probabilityHS[HAND_STRENGTH_NUM - 1][nPotSize][nStackSize][0][2][nVPIP][nPFR][nAF][flopPotCommon];
							++totalS[nPotSize][nStackSize][0][2][nVPIP][nPFR][nAF][flopPotCommon];
							if (j + 1 < player.getRiverAction().size())
								++totalFE[nPotSize][nStackSize][0][2][nVPIP][nPFR][nAF][flopPotCommon];
						}
					}

					if (FE)
					{
						if (player.getRiverAction().size() == 1)
						{
							++probabilityFE[0][nPotSize][nStackSize][4][2][nVPIP][nPFR][nAF][flopPotCommon];
						}
						else
						{
							Action action = player.getRiverAction()[player.getRiverAction().size() - 2];
							int size = normalizeBetSize(2, action.getSize(), totalpot, 0.04);
							if (action.getType() == 'c')
							{
								++probabilityFE[0][nPotSize][nStackSize][size][0][nVPIP][nPFR][nAF][flopPotCommon];
							}

							if (action.getType() == 'r')
							{
								++probabilityFE[0][nPotSize][nStackSize][size][1][nVPIP][nPFR][nAF][flopPotCommon];
								
								/*
								if (action.getSize() > 60 * 0.04 && nPotSize > 5)
								{printf("Situation= %d %d %d %d %d %d %d %d\n", nPotSize, nStackSize, size, 1, nVPIP, nPFR, nAF, flopPotCommon);
								printf("Total= %d\n", probabilityFE[0][nPotSize][nStackSize][size][1][nVPIP][nPFR][nAF][flopPotCommon]);
									printf("============FOLDED=========================================\n");
									printf("%s\n", player.getPlayerName().c_str());
									for (int k = 0; k < player.getRiverAction().size(); ++k)
									{
										printf("%s\n", player.getRiverAction()[k].toString().c_str());
									}
								}
								*/
							}
							/*
							if (action.getType() == 'x')
							{
								if (nVPIP == 3 && nPotSize == 6 && flopPotCommon < 3)
								{
									printf("============FOLDED=========================================\n");
									printf("Situation= %d %d %d %d %d %d %d %d\n", nPotSize, nStackSize, size, 1, nVPIP, nPFR, nAF, flopPotCommon);
									printf("%s\n", player.getPlayerName().c_str());
									printf("Total= %d\n", totalFE[nPotSize][nStackSize][size][1][nVPIP][nPFR][nAF][flopPotCommon]);
									printf("Potcommon= %lf Balance= %lf\n", totalpot, player.getBalance());
									for (int k = 0; k < player.getRiverAction().size(); ++k)
									{
										printf("%s\n", player.getRiverAction()[k].toString().c_str());
									}
									printf("Board: %s%s%s%s%s\n", handhistory.getFlopCard1().toString().c_str(), 
										handhistory.getFlopCard2().toString().c_str(), handhistory.getFlopCard3().toString().c_str(), handhistory.getTurnCard().toString().c_str(), handhistory.getRiverCard().toString().c_str());
								}
								++probabilityFE[0][nPotSize][nStackSize][0][2][nVPIP][nPFR][nAF][flopPotCommon];
							}
							*/
						}
					}
				}
			}
		}
	}

	double getProbabilityHS(int v[])
	{
		return (double)probabilityHS[v[0]][v[1]][v[2]][v[3]][v[4]][v[5]][v[6]][v[7]][v[8]] / (double)totalS[v[1]][v[2]][v[3]][v[4]][v[5]][v[6]][v[7]][v[8]];
	}

	double getProbabilityFE(int v[])
	{
		return (double)probabilityFE[v[0]][v[1]][v[2]][v[3]][v[4]][v[5]][v[6]][v[7]][v[8]] / (double)totalS[v[1]][v[2]][v[3]][v[4]][v[5]][v[6]][v[7]][v[8]];
	}

	double getNormProbabilityHS(int v[])
	{
		//normalize!!
		return (double)probabilityHS[v[0]][v[1]][v[2]][v[3]][v[4]][v[5]][v[6]][v[7]][v[8]] / (double)totalS[v[1]][v[2]][v[3]][v[4]][v[5]][v[6]][v[7]][v[8]];
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
				for (int i = 0; i < HAND_STRENGTH_NUM; ++i)
					fscanf(f,"%d ", &probabilityHS[i][v[2]][v[3]][v[4]][v[5]][v[6]][v[7]][v[8]][v[9]]);
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

	int maxBetSize(HandHistory& handhistory, double potcommon)
	{
		int lastActionRound = 0;
		for (int i = 0; i < handhistory.getPlayerHistories().size(); ++i)
		{
			PlayerHistory player = handhistory.getPlayerHistories()[i];
			lastActionRound = max(lastActionRound, player.getRiverAction().size());
		}
		double mSize = 0;
		double mSize2 = 0;
		
		for (int i = 0; i < handhistory.getPlayerHistories().size(); ++i)
		{
			PlayerHistory player = handhistory.getPlayerHistories()[i];
			if (player.getRiverAction().size() == lastActionRound || player.getRiverAction().size() == lastActionRound - 1)
			{
				double size = player.getBalanceInRound(4);
				if (size > mSize)
				{
					mSize2 = mSize;
					mSize = size;
				}
				else if (size > mSize2)
				{
					mSize2 = size;
				}
			}
		}

		int size = normalizeBetSize(2, mSize2, potcommon, 0.04);
		return size;
	}

	double firstRaise(HandHistory& handhistory)
	{
		double min = 0x3f3f3f3f;
		for (int i = 0; i < handhistory.getPlayerHistories().size(); ++i)
		{
			PlayerHistory player = handhistory.getPlayerHistories()[i];
			if (player.getRiverAction().size() > 0)
			{
				if (player.getRiverAction()[0].getType() == 'r')
				{
					if (min > player.getRiverAction()[0].getSize()) min = player.getRiverAction()[0].getSize();
				}
			}
		}

		return min;
	}
};