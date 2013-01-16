#pragma once
#include "foundation\src\bayesUtils.h"
#include "foundation\src\database.h"
#include "foundation\src\Hand.h"

struct PreflopSituation
{
	int type;
	double stacksize;
	double betsize;
	double vpip, pfr;
	int poz;
	int line;
	int handknown;
	Hand hand;
};

class PreflopHandHistoryGenerator
{
public:
	void generateHandHistory(string filename, int n, PreflopSituation& situation)
	{
		FILE *f = fopen(filename.c_str(), "w");
		Database* database = new Database("127.0.0.1", "root", "root", "kutya");
		
		fprintf(f,"%d\n", n);
		for (; n; --n)
		{
			fprintf(f,"0\n"); //preflop
			fprintf(f,"7\n"); //buttonseat...not important
			int playernum = 1;
			fprintf(f,"%d\n", playernum); //playernum
			
			database->setVPIP(situation.vpip, 1, "00000");
			database->setPFR(situation.pfr, 1, "00000");
			fprintf(f, "000000\n");
			fprintf(f, "%.2lf\n", situation.stacksize);
			fprintf(f, "%d\n", situation.poz);

			int random = 1 + rand() % 5;
			fprintf(f, "%d\n", random);
			for (int i = 0; i < random - 1; ++i)
			{
				int r = rand() % 3;
				if (r == 0)
				{
					fprintf(f, "c %.2lf\n", 0);
				}
				if (r == 1)
				{
					fprintf(f, "r %.2lf\n", 0);
				}
				if (r == 2)
				{
					fprintf(f, "x 0\n");
				}
			}

			if (situation.line == 0)
			{
				fprintf(f, "c %.2lf\n", situation.betsize);
			}
			if (situation.line == 0)
			{
				fprintf(f, "c %.2lf\n", situation.betsize);
			}
			if (situation.line == 0)
			{
				fprintf(f, "c %.2lf\n", situation.betsize);
			}

			fprintf(f, "%d", situation.handknown);
			if (situation.handknown == 1)
			{
				fprintf(f, "%c%c%c%c", situation.hand.getCard1().getRank(), situation.hand.getCard1().getSuit(), situation.hand.getCard2().getRank(), situation.hand.getCard2().getSuit());
			}
		}

		fclose(f);
	}
};