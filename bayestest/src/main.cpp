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
		turn.read("turnBayes");
		river.read("riverBayes");

		int nPotSize;
		int nStackSize;
		int nBetSize;
		int nLine;
		int nVPIP;
		int nPFR;
		int nAF;
		int poz;

		/*
		{
			PlayerRange oldRange;
			oldRange.range.insert(make_pair(Hand(Card('K','d'), Card('A','s')), 0.5));
			oldRange.range.insert(make_pair(Hand(Card('J','s'), Card('J','c')), 0.3));
			oldRange.range.insert(make_pair(Hand(Card('5','c'), Card('9','d')), 0.2));

			vector<Card> board;
			board.push_back(Card('A','c'));
			board.push_back(Card('2','s'));
			board.push_back(Card('3','d'));
			board.push_back(Card('4','d'));

			Hand own(Card('2','c'), Card('T','s'));

			double HS[9];
			memset(HS, 0, sizeof(HS));
			HS[0] = 0.3;
			//HS[1] = 0.1;
			//HS[2] = 0.1;
			//HS[3] = 0.1;
			HS[4] = 0.7;
			//HS[5] = 0.1; 
			//HS[6] = 0.1;
			//HS[7] = 0.1;
			//HS[8] = 0.1;
			
			PlayerRange turnRange = RangeUtils::createRange(8, HS, board, own);
			//turnRange.printRange();

			PlayerRange newRange = RangeUtils::mergeRange(oldRange, turnRange, board, own);
			newRange.printRange();
		}*/


		{
			double FE = river.getProbabilityFE(20, 15, 2.5, 182.5*0.04, 1, 7 * 0.04, 0.04, 24.75*0.04, 11*0.04, 0);
			printf("FE = %lf\n", FE);
		}

		{
			double FE = preflop.getProbabilityFE(20, 15, 213.25*0.04, -1, 1, 0.16, 0.04, 0, 0);
			printf("FE = %lf\n", FE);
		}

		{
			//1, 150*0.04, 1, 0.48, 0.04, 6.75 * 0.04, 50
			double FE = flop.getProbabilityFE(20, 18, 2.5, 150 * 0.04, 1, 6*0.04, 0.04, 10 * 0.04, 25);
			printf("river FE = %lf\n", FE);
		}

		{
			
			int v[9];
			v[0] = 0;
			v[1] = normalizePotSize(3, 21 * 0.04, 0.04);
			v[2] = normalizeStackSize(150 * 0.04, 0.04);
			v[3] = 0; //normalizeBetSize(3, 0, 21 * 0.04, 0.04);
			v[4] = 2;
			v[5] = normalizeVPIP(50);
			v[6] = normalizePFR(5);
			v[7] = normalizeAF(0.5);
			v[8] = normalizePotSize(2, 9 * 0.04, 0.04);
		}


		/*
		int v[8];

		nStackSize = normalizeStackSize(110.75*0.04, 0.04); //150
		nBetSize = normalizeBetSize(1, 0.12, 0, 0.04);  //betsize 0 - limp, 1 - emeles, 2 - 3bet
		nLine = 1;
		nVPIP = normalizeVPIP(20);
		nPFR = normalizePFR(15);
		poz = 3;

		v[1] = nStackSize;
		v[2] = nBetSize;
		v[3] = nLine;
		v[4] = nVPIP;
		v[5] = nPFR;
		v[6] = poz;

		preflop.printRange(v);
		//PlayerRange preflopRange = preflop.getRange(15, 10, 110.75*0.04, 0, 1, 0.56, 0.04, 0, 0);
		PlayerRange preflopRange = preflop.getRange(v, 10);

		printf("PREFLOP\n");
		preflopRange.printRange();


		Hand own(Card('A','d'), Card('Q','s'));

		vector<PlayerRange> ranges;
		PlayerRange myrange;
		myrange.range.insert(make_pair(own, 1));
		ranges.push_back(myrange);
		ranges.push_back(preflopRange);

		double EQ = calc.calculate(ranges, cards, 25000);
		printf("\n\nAQ's equity against preflopRange --------------------------------------------------------------------------------- \n%.2lf\n\n\n", EQ);

		double FE = preflop.getProbabilityFE(21, 15, 5.61, -1, 1, 0.12, 0.04, 0, 0);
		printf("FE = %lf\n", FE);

		//FLOP
		nPotSize = normalizePotSize(2, 0.37, 0.04);
		nStackSize = normalizeStackSize(185 * 0.04, 0.04);
		nBetSize = normalizeBetSize(2, 0.37, 0.37, 0.04);
		nLine = 0;
		nVPIP = normalizeVPIP(84);
		nPFR = normalizePFR(45);
		nAF = normalizeAF(3);

		v[1] = nPotSize;
		v[2] = nStackSize;
		v[3] = nBetSize;
		v[4] = nLine;
		v[5] = nVPIP;
		v[6] = nPFR;
		v[7] = nAF;

		cout << "FE = " << flop.getProbabilityFE(v, 10) << endl;
		flop.printRange(v, 100);

		cout << "FE2 = " << flop.getProbabilityFE2(v, 100) << endl;

		PlayerRange range = flop.getRange(v, cards, own, 50);
		printf("FLOP\n");
		range.printRange();

		printf("CALL RAISE RANGE\n");

		PlayerRange callRange = flop.getCallRaiseRange(20, 15, 3.5, 185 * 0.04, 0, 0.08, 0.04, 0.08, cards, own, 10);
		callRange.printRange();


		range = RangeUtils::mergeRange(preflopRange, range, cards, own);

		printf("FLOP MERGED\n");
		range.printRange();

		printf("ADDED range\n");
		range = RangeUtils::addRange(range, myrange);
		range.printRange();

		printf("100 Range\n");
		range = range.create100();
		range.printRange();

		printf("100 Range2\n");
		cout << range.toString();
		
		printf("total = %lf\n", range.totalPercentage());

		printf("BOTLOGIC TESTS\n");
		*/

		/*
		{
			vector<Card> board;
			board.push_back(Card('Q', 'c'));
			board.push_back(Card('T', 'h'));
			board.push_back(Card('T', 'c'));
			board.push_back(Card('J', 'c'));

			Hand own(Card('J', 'h'), Card('4','c'));

			printf("TURN RANGE\n");
			PlayerRange turnRange = turn.getRange(20, 15, 3.5, 174*0.04, 2, 0, 0.04, 15.75*0.04, 6.75*0.04, board, own, 50);
			turnRange.printRange();
			double FE = turn.getProbabilityFE(20, 15, 3.5, 174*0.04, 2, 0, 0.04, 15.75*0.04, 6.75*0.04, 50);
			printf("FE = %lf\n", FE);
		}


		{
			double FE = river.getProbabilityFE(40, 10, 1, 74*0.04, 2, 0, 0.04, 50*0.04, 5*0.04, 10);
			printf("FE = %lf\n", FE);
		}

		{
			double FE = preflop.getProbabilityFE(20, 15, 213.25*0.04, -1, 1, 0.16, 0.04, 0, 0);
			printf("FE = %lf\n", FE);
		}

		{
			double FE = flop.getProbabilityFE(40, 10, 1, 76.75*0.04, 1, 0.48, 0.04, 6.75 * 0.04, 50);
			printf("FLOP FE = %lf\n", FE);
		}


		{
			vector<Card> board;
			board.push_back(Card('A', 's'));
			board.push_back(Card('2', 'h'));
			board.push_back(Card('Q', 'h'));
			board.push_back(Card('2', 'd'));
			board.push_back(Card('Q', 'c'));

			Hand own(Card('6', 'c'), Card('7','d'));

			printf("RIVER RANGE\N");
			//PlayerRange riverRange = river.getRange(40, 10, 1, 49 * 0.04, 2, 0, 0.04, 6.75 * 0.04, 4 * 0.04, board, own, 0);
			//riverRange.printRange();
		}
		*/

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