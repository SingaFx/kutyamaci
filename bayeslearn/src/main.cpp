#include <iostream>
#include <fstream>
#include <vector>
#include <cstdio>
#include <map>

#include "handHistoryParser.h"
#include "bayesLearnFunctions.h"
#include "database.h"
#include "HandHistoryUtils.h"

//using namespace dlib;
;
using namespace std;
//using namespace bayes_node_utils;

FILE* logfile;
Database* database;
//directed_graph<bayes_node>::kernel_1a_c bn[4]; //Preflop, Flop, Turn, River
//assignment parent_state;

/*
hand strength -> 5
	0. FOLD
	1. WEAK DRAW
	2. DRAW
	3. WEAK MADE HAND
	4. NORMAL MADE HAND
	5. STRONG MADE HAND
pot size -> 3
	0. small pot (limped)
	1. normal pot
	2. big pot
stack size -> 5 (effective stack size?)
	0. 0 - 30bb
	1. 30 - 70bb
	2. 70 - 100bb
	3. 100 - 150bb
	4. 150+
bet size -> 3
	0. small bet
	1. normal bet
	2. overbet
player line -> 4
	1. WEAK (c, c, c)
	2. NORMAL (r/f, r)
	3. STRONG (r/c, r/r, x/r)
(player type -> 3
	0. passive FISH
	1. aggressive FISH
	2. REG)
(VPIP(4), PFR(3), AF(3))
------------------
540
*/

BayesLearnPreflop* preflop;
BayesLearnFlop* flop;
BayesLearnTurn* turn;
BayesLearnRiver* river;

int total;


map<string, DatabaseMapStruct> DatabaseMAP;

void parseHands(string filename)
{
	int total = 0;
	printf("Begin parsing hands\n");

	FILE *f = fopen(filename.c_str(),"r");
	char s[100];

	int historyNumber;
	while (fscanf(f,"%d\n", &historyNumber) == 1 && historyNumber != 0)
	{
		std::vector<HandHistory> history = HandHistoryUtils::importFromFile(f, historyNumber);
		total += history.size();
		printf("Total number of parsed hands = %d\n", total);
		//if (total < 110000) continue;
		for (int i = 0; i < history.size(); ++i)
		{
			preflop->updateProbabilities(history[i], DatabaseMAP);
			flop->updateProbabilities(history[i], DatabaseMAP);
			turn->updateProbabilities(history[i], DatabaseMAP);
			river->updateProbabilities(history[i], DatabaseMAP);
		}

		//if (total > 1000000) return ;
	}
}
int main(int argc, char* argv[])
{
	try
	{
		preflop = new BayesLearnPreflop();
		flop = new BayesLearnFlop();
		turn = new BayesLearnTurn();
		river = new BayesLearnRiver();
		parseHands("d:\\pokerbot\\hh.txt");
		//parseHands("..\\tests\\resource\\bayes\\testhh.txt");

		preflop->write("preflopBayes");
		flop->write("flopBayes");
		turn->write("turnBayes");
		river->write("riverBayes");
	}
	catch (exception& e)
    {
        cout << "exception thrown: " << e.what() << endl;
        return 1;
    }
	return 0;
}