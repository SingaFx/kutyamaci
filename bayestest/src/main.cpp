//#include "plusEVBotLogic.h"
#include <iostream>
#include <vector>
#include "bayesUserFunctions.h"
#include "eqcalculator.h"

using namespace std;

BayesUserPreflop preflop;
BayesUserFlop flop;
BayesUserTurn turn;
BayesUserRiver river;

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
		EqCalculator calc;
		//PlusEVBotLogic botlogic;
		//BayesDecision decision;

		preflop.read("preflopBayes");
		flop.read("flopBayes");

		int nPotSize;
		int nStackSize;
		int nBetSize;
		int nLine;
		int nVPIP;
		int nPFR;
		int nAF;
		int poz;

		int v[8];

		nStackSize = normalizeStackSize(182.75 * 0.04, 0.04); //150
		nBetSize = normalizeBetSize(1, 0.12, 0, 0.04);  //betsize 0 - limp, 1 - emeles, 2 - 3bet
		nLine = 1;
		nVPIP = normalizeVPIP(20);
		nPFR = normalizePFR(15);
		poz = 0;

		v[1] = nStackSize;
		v[2] = nBetSize;
		v[3] = nLine;
		v[4] = nVPIP;
		v[5] = nPFR;
		v[6] = poz;

		preflop.printRange(v);
		PlayerRange preflopRange = preflop.getRange(20, 15, 182.75*0.04, -3, 1, 0.12, 0.04, 0, 10);
		//PlayerRange preflopRange = preflop.getRange(v, 10);

		printf("PREFLOP\n");
		preflopRange.printRange();

		//FLOP
		nPotSize = normalizePotSize(2, 0.08, 0.04);
		nStackSize = normalizeStackSize(185 * 0.04, 0.04);
		nBetSize = normalizeBetSize(2, 0.08, 0.08, 0.04);
		nLine = 0;
		nVPIP = normalizeVPIP(20);
		nPFR = normalizePFR(15);
		nAF = normalizeAF(3.5);

		v[1] = nPotSize;
		v[2] = nStackSize;
		v[3] = nBetSize;
		v[4] = nLine;
		v[5] = nVPIP;
		v[6] = nPFR;
		v[7] = nAF;

		cout << "FE = " << flop.getProbabilityFE(v, 100) << endl;
		flop.printRange(v, 100);

		vector<Card> cards;
		cards.push_back(Card('5','h'));
		cards.push_back(Card('6','s'));
		cards.push_back(Card('K','d'));
		Hand own(Card('5','s'), Card('6','c'));
		PlayerRange range = flop.getRange(v, cards, own, 50);
		printf("FLOP\n");
		range.printRange();

		printf("CALL RAISE RANGE\n");

		PlayerRange callRange = flop.getCallRaiseRange(20, 15, 3.5, 185 * 0.04, 0, 0.08, 0.04, 0.08, cards, own, 10);
		callRange.printRange();


		range = RangeUtils::mergeRange(preflopRange, range, cards, own);

		vector<PlayerRange> ranges;
		PlayerRange myrange;
		myrange.range.insert(make_pair(own, 1));
		ranges.push_back(myrange);
		ranges.push_back(range);

		printf("FLOP MERGED\n");
		range.printRange();

		printf("ADDED range\n");
		range = RangeUtils::addRange(range, myrange);
		range.printRange();

		//double EQ = calc.calculate(ranges, cards, 25000);
		//printf("%.2lf\n", EQ);

		
		printf("total = %lf\n", range.totalPercentage());

		printf("BOTLOGIC TESTS\n");

		/*
		CurrentPlayerInfo info;
		CurrentGameInfo gameInfo;

		info.setVPIP(40);
		info.setPFR(30);
		info.setStacksize(100);

		gameInfo.setPotcommon(100);
		range = decision.getCallRaiseRange(10, range, gameInfo, preflop, flop, turn, river);
		*/
    }
    catch (std::exception& e)
    {
        cout << "exception thrown: " << endl;
        cout << e.what() << endl;
        cout << "hit enter to terminate" << endl;
        cin.get();
    }
}