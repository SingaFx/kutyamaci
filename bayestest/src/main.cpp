#include <iostream>
#include <vector>
#include "bayesUserFunctions.h"

using namespace std;

BayesUserPreflop preflop;
BayesUserFlop flop;
BayesUserTurn turn;


void basic()
{
	//nev
	//poz
	//balance
	//potcommon
	//c, r, x
	//betsize(currentbet)

	//Rangek
	//mennyit kell call-olni ha van bet
	//potcommon
	//pot

	double VPIP = 21.5;
	double PFR = 10;
	double betsize = 0.2;
	int poz = 0;
	double stacksize = 2;
		
	//preflop.read("preflopBayes");
	flop.read("flopBayes");
	//turn.read("turnBayes");




	//preflop.read("..\\..\\tests\\resource\\bayes\\preflopBayes");
	//flop.read("..\\..\\tests\\resource\\bayes\\flopBayes");
	//turn.read("..\\..\\tests\\resource\\bayes\\turnBayes");


	//======PREFLOP===================================================
	/*
	int nStackSize = 2; //150
	int nBetSize = 0;  //betsize 0 - limp, 1 - emeles, 2 - 3bet
	int nLine = 0;
	int nVPIP = 2;
	int nPFR = 1;
	poz = 4;
	int nAF;
	int nPotSize;
		
	int v[8];
	v[1] = nStackSize;
	v[2] = nBetSize;
	v[3] = nLine;
	v[4] = nVPIP;
	v[5] = nPFR;
	v[6] = poz;
		
	PlayerRange rangePre = preflop.getRange(v, 0);
	rangePre = rangePre.normalize();
	preflop.printRange(v);
	v[0] = 0;
	cout << "FOLD = " << preflop.getProbability(v, 0) << endl;
		
	printf("PREFLOP RANGE\n");
	rangePre.printRange();

	//======FLOP===================================================

	nStackSize = 3;  //110 - 150bblin
	nBetSize = 1;  //3/4pot
	nLine = 1; //raise
	nVPIP = 2;  //20 - 30
	nPFR = 1; //10 - 20
	nAF = 1;
	nPotSize = 2; //35*bblind

	v[1] = nPotSize;	
	v[2] = nStackSize;
	v[3] = nBetSize;
	v[4] = nLine;
	v[5] = nVPIP;
	v[6] = nPFR;
	v[7] = nAF;
		

	printf("FLOP RANGE\n");

	cout << "FE = " << flop.getProbabilityFE(v, 0) << endl;
	flop.printRange(v); 

	double HS[10];
	for (int i = 0; i < flop.HAND_STRENGTH_NUM; ++i)
	{
		v[0] = i;
		HS[i] = flop.getProbabilityHS(v, 0);
	}
		
	Card card1('A','s'), card2('A','d'), card3('A','c');
	vector<Card> cards;
	cards.push_back(card1); cards.push_back(card2); cards.push_back(card3);


	Hand own;
	PlayerRange range1 = RangeUtils::createRange(8, HS, cards, own);
		
	range1.printRange();
	range1 = RangeUtils::mergeRange(rangePre, range1, cards);
	printf("MERGED PREFLOP-FLOP RANGE\n");
	range1.printRange();

	//======TURN===================================================

	nStackSize = 2;  //110 - 150bblin
	nBetSize = 1;  //3/4pot
	nLine = 1; //raise
	nVPIP = 2;  //20 - 30
	nPFR = 1; //10 - 20
	nAF = 1;
	nPotSize = 2; //30 - 70 *bblind

	v[1] = nPotSize;	
	v[2] = nStackSize;
	v[3] = nBetSize;
	v[4] = nLine;
	v[5] = nVPIP;
	v[6] = nPFR;
	v[7] = nAF;

	printf("TURN RANGE\n");

	cout << "FE = " << turn.getProbabilityFE(v, 0) << endl;
	turn.printRange(v);

	for (int i = 0; i < turn.HAND_STRENGTH_NUM; ++i)
	{
		v[0] = i;
		HS[i] = turn.getProbabilityHS(v, 0);
	}

	Card card4('A','h');
	cards.push_back(card4);

	PlayerRange rangeTurn = RangeUtils::createRange(8, HS, cards, own);
		
	rangeTurn.printRange();

	range1 = RangeUtils::mergeRange(range1, rangeTurn, cards);
	printf("PREFLOP - FLOP - TURN RANGE\n");
	range1.printRange();
	*/
	//TODO: normalizalasok!//turn/river
	//TODO: test full line!
}

void test1()
{
}

int main()
{
    try
    {
		flop.read("flopBayes");
		
		int nPotSize = 0;
		int nStackSize = 1;
		int nBetSize = 5;
		int nLine = 1;
		int nVPIP = 3;
		int nPFR = 1;
		int nAF = 2;
		

		int v[8];

		v[1] = nPotSize;	
		v[2] = nStackSize;
		v[3] = nBetSize;
		v[4] = nLine;
		v[5] = nVPIP;
		v[6] = nPFR;
		v[7] = nAF;

		cout << "FE = " << flop.getProbabilityFE(v, 0) << endl;
		flop.printRange(v);
    }
    catch (std::exception& e)
    {
        cout << "exception thrown: " << endl;
        cout << e.what() << endl;
        cout << "hit enter to terminate" << endl;
        cin.get();
    }
}




