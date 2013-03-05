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

void testRiverFE()
{
	printf("\n--------------------------------river tests----------------------------------\n\n");

	//                                  VPIP,     PFR,       AF,        stacksize,          line,         betsize,         bblind,         potcommon,          flop potcommon
	double FE = river.getProbabilityFE(  20,       18,       3,         125*0.04,            5,              0,             0.04,             2.75,                 0.75,            35);
	printf("river 3bet pot reg ellen triple barrel pozin kivul              = %lf\n", FE);

	//                                  VPIP,     PFR,       AF,        stacksize,          line,         betsize,         bblind,         potcommon,          flop potcommon
		   FE = river.getProbabilityFE(  20,       18,       3,         125*0.04,            2,              0,             0.04,             2.75,                 0.75,            35);
	printf("river 3bet pot reg ellen triple barrel pozibol                  = %lf\n", FE);

	//                                  VPIP,     PFR,       AF,        stacksize,          line,         betsize,         bblind,         potcommon,          flop potcommon
		   FE = river.getProbabilityFE(  40,       10,       1,         125*0.04,            5,              0,             0.04,             2.75,                 0.75,            35);
	printf("river 3bet pot fish ellen triple barrel pozin kivul             = %lf\n", FE);

	//                                  VPIP,     PFR,       AF,        stacksize,          line,         betsize,         bblind,         potcommon,          flop potcommon
		   FE = river.getProbabilityFE(  40,       10,       1,         125*0.04,            2,              0,             0.04,             2.75,                 0.75,            35);
	printf("river 3bet pot fish ellen triple barrel pozibol                 = %lf\n", FE);

	printf("--------------------------------end of river tests------------------------------\n");
}

void testTurnFE()
{
	printf("\n--------------------------------turn tests----------------------------------\n\n");

	//                                  VPIP,     PFR,       AF,        stacksize,          line,         betsize,         bblind,         potcommon,          flop potcommon
	double FE = turn.getProbabilityFE(   40,       5,         1,           4,                 2,             0,             0.04,               1,                   0.4,              35);
	printf("turn emelt pot fish ellen pozibol barrel                        = %lf\n", FE);

	//                                  VPIP,     PFR,       AF,        stacksize,          line,         betsize,         bblind,         potcommon,          flop potcommon
		   FE = turn.getProbabilityFE(   40,       5,         1,           4,                 5,             0,             0.04,               1,                   0.4,              35);
	printf("turn emelt pot fish ellen pozin kivul barrel                    = %lf\n", FE);

	//                                  VPIP,     PFR,       AF,        stacksize,          line,         betsize,         bblind,         potcommon,          flop potcommon
		   FE = turn.getProbabilityFE(   20,       18,        3,           4,                 2,             0,             0.04,               1,                   0.4,              35);
	printf("turn emelt pot regular ellen pozibol barrel                     = %lf\n", FE);

	//                                  VPIP,     PFR,       AF,        stacksize,          line,         betsize,         bblind,         potcommon,          flop potcommon
		   FE = turn.getProbabilityFE(   20,       18,        3,           4,                 5,             0,             0.04,               1,                   0.4,              35);
	printf("turn emelt pot regular ellen pozin kivul barrel                 = %lf\n", FE);

	//                                  VPIP,     PFR,       AF,        stacksize,          line,         betsize,         bblind,         potcommon,          flop potcommon
		   FE = turn.getProbabilityFE(   50,       40,        5,           4,                 2,             0,             0.04,               1,                   0.4,              35);
	printf("turn emelt pot maniac ellen pozibol barrel                      = %lf\n", FE);

	//                                  VPIP,     PFR,       AF,        stacksize,          line,         betsize,         bblind,         potcommon,          flop potcommon
		   FE = turn.getProbabilityFE(   50,       40,        5,           4,                 5,             0,             0.04,               1,                   0.4,              35);
	printf("turn emelt pot maniac ellen pozin kivul barrel                  = %lf\n", FE);

	//                                  VPIP,     PFR,       AF,        stacksize,          line,         betsize,         bblind,         potcommon,          flop potcommon
		   FE = turn.getProbabilityFE(   20,       18,        3,           4,                 1,             0.66,          0.04,               1,                   0.4,              35);
	printf("turn emelt pot regular ellen raise barrel                       = %lf\n", FE);

	//                                  VPIP,     PFR,       AF,        stacksize,          line,         betsize,         bblind,         potcommon,          flop potcommon
		   FE = turn.getProbabilityFE(   20,       18,        3,           4,                 1,             1.6,           0.04,               1,                   0.4,              10);
	printf("turn emelt pot regular ellen 3bet								= %lf\n", FE);
	//                                  VPIP,     PFR,       AF,        stacksize,          line,         betsize,         bblind,         potcommon,          flop potcommon
		   FE = turn.getProbabilityFE(   26,       19,        3,        61.25 * 0.04,         2,             0,            0.04,               42 * 0.04,         21.5 * 0.04,              15);
	printf("turn emelt pot regular ellen 3bet								= %lf\n", FE);

	printf("-----------------------------end of turn tests------------------------------\n");
}

void testFlopFE()
{
	printf("\n--------------------------------flop tests----------------------------------\n\n");

	//                                  VPIP,     PFR,       AF,        stacksize,          line,         betsize,         bblind,         potcommon,         
	double FE = flop.getProbabilityFE(  40,        5,         1,           4,                 2,             0,             0.04,              0.4,                  35);
	printf("flop emelt pot fish ellen pozibol cbet                          = %lf\n", FE);

	//                                  VPIP,     PFR,       AF,        stacksize,          line,         betsize,         bblind,         potcommon,         
		   FE = flop.getProbabilityFE(  40,        5,         1,           4,                 5,             0,             0.04,              0.4,                  35);
	printf("flop emelt pot fish ellen pozin kivul cbet                      = %lf\n", FE);

	//                                  VPIP,     PFR,       AF,        stacksize,          line,         betsize,         bblind,         potcommon,         
		   FE = flop.getProbabilityFE(  20,        18,        3,           4,                 2,             0,             0.04,              0.4,                  35);
	printf("flop emelt pot regular ellen pozibol cbet                       = %lf\n", FE);

	//                                  VPIP,     PFR,       AF,        stacksize,          line,         betsize,         bblind,         potcommon,         
		   FE = flop.getProbabilityFE(  20,        18,        3,           4,                 5,             0,             0.04,              0.4,                  35);
	printf("flop emelt pot regular ellen pozin kivul cbet                   = %lf\n", FE);

	//                                  VPIP,     PFR,       AF,        stacksize,          line,         betsize,         bblind,         potcommon,         
		   FE = flop.getProbabilityFE(  50,        40,        5,           4,                 2,             0,             0.04,              0.4,                  35);
	printf("flop emelt pot maniac ellen pozibol cbet                        = %lf\n", FE);

	//                                  VPIP,     PFR,       AF,        stacksize,          line,         betsize,         bblind,         potcommon,         
		   FE = flop.getProbabilityFE(  50,        40,        5,           4,                 5,             0,             0.04,              0.4,                  35);
	printf("flop emelt pot maniac ellen pozin kivul cbet                    = %lf\n", FE);

	//                                  VPIP,     PFR,       AF,        stacksize,          line,         betsize,         bblind,         potcommon,         
		   FE = flop.getProbabilityFE(  40,        10,        1,           2,                 2,             0,             0.04,              0.4,                  35);
	printf("flop emelt pot midstack fish ellen pozibol cbet                 = %lf\n", FE);

	//                                  VPIP,     PFR,       AF,        stacksize,          line,         betsize,         bblind,         potcommon,         
		   FE = flop.getProbabilityFE(  40,        10,        1,           2,                 5,             0,             0.04,              0.4,                  35);
	printf("flop emelt pot midstack fish pozin kivul cbet                   = %lf\n", FE);

	//                                  VPIP,     PFR,       AF,        stacksize,          line,         betsize,         bblind,         potcommon,         
		   FE = flop.getProbabilityFE(  40,        10,        1,           1,                 2,             0,             0.04,              0.4,                  35);
	printf("flop emelt pot shortstack fish ellen pozibol cbet               = %lf\n", FE);

	//                                  VPIP,     PFR,       AF,        stacksize,          line,         betsize,         bblind,         potcommon,         
		   FE = flop.getProbabilityFE(  40,        10,        1,           1,                 5,             0,             0.04,              0.4,                  35);
	printf("flop emelt pot shortstack fish pozin kivul cbet                 = %lf\n", FE);

	//                                  VPIP,     PFR,       AF,        stacksize,          line,         betsize,         bblind,         potcommon,         
           FE = flop.getProbabilityFE(  20,        18,        3,           4,                  1,             0.2,           0.04,             0.3,                 35);
	printf("flop emelt pot regular raise cbet                               = %lf\n", FE);

	//                                  VPIP,     PFR,       AF,        stacksize,          line,         betsize,         bblind,         potcommon,         
           FE = flop.getProbabilityFE(  20,        18,        3,           4,                  1,             0.3,           0.04,             0.3,                 35);
	printf("flop emelt pot regular raise cbet  (pot meretu cbet)            = %lf\n", FE);

	//                                  VPIP,     PFR,       AF,        stacksize,          line,         betsize,         bblind,         potcommon,         
           FE = flop.getProbabilityFE(  40,        10,        1,           4,                  1,             0.2,           0.04,             0.3,                 35);
	printf("flop emelt pot fish    raise cbet                               = %lf\n", FE);

	//                                  VPIP,     PFR,       AF,        stacksize,          line,         betsize,         bblind,         potcommon,         
           FE = flop.getProbabilityFE(  40,        10,        1,           4,                  1,             0.3,           0.04,             0.3,                 35);
	printf("flop emelt pot fish    raise cbet  (pot meretu cbet)            = %lf\n", FE);


	// --------------------------------------------------------------------------- 3bet pot ----------------------------------------------------------------------------


	//                                  VPIP,     PFR,       AF,        stacksize,          line,         betsize,         bblind,         potcommon,         
           FE = flop.getProbabilityFE(  20,        18,        3,           4,                  1,             0.4,          0.04,              0.75,                 35);
	printf("flop 3bet pot regular raise cbet                                = %lf\n", FE);

	//                                  VPIP,     PFR,       AF,        stacksize,          line,         betsize,         bblind,         potcommon,         
           FE = flop.getProbabilityFE(  20,        18,        3,           4,                  1,             0.75,         0.04,              0.75,                 35);
	printf("flop 3bet pot regular raise cbet   (pot meretu cbet)            = %lf\n", FE);

	//                                  VPIP,     PFR,       AF,        stacksize,          line,         betsize,         bblind,         potcommon,         
           FE = flop.getProbabilityFE(  20,        18,        3,           4,                  2,             0,            0.04,              0.75,                 35);
	printf("flop 3bet pot regular ellen pozibol cbet                        = %lf\n", FE);

	//                                  VPIP,     PFR,       AF,        stacksize,          line,         betsize,         bblind,         potcommon,         
           FE = flop.getProbabilityFE(  20,        18,        3,           4,                  5,             0,            0.04,              0.75,                 35);
	printf("flop 3bet pot regular ellen pozin  kivul                        = %lf\n", FE);

	//                                  VPIP,     PFR,       AF,        stacksize,          line,         betsize,         bblind,         potcommon,         
           FE = flop.getProbabilityFE(  40,        5,         3,           4,                  2,             0,            0.04,              0.75,                 35);
	printf("flop 3bet pot fish ellen pozibol cbet                           = %lf\n", FE);

	//                                  VPIP,     PFR,       AF,        stacksize,          line,         betsize,         bblind,         potcommon,         
           FE = flop.getProbabilityFE(  40,        5,         1,           4,                  5,             0,            0.04,              0.75,                 35);
	printf("flop 3bet pot fish ellen pozin  kivul                           = %lf\n", FE);

	//                                  VPIP,     PFR,       AF,        stacksize,          line,         betsize,         bblind,         potcommon,         
           FE = flop.getProbabilityFE(  40,        5,         1,           4,                  1,             1.2,            0.04,            0.75,                 35);
	printf("flop 3bet pot fish ellen 3bet                                   = %lf\n", FE);

	//                                  VPIP,     PFR,       AF,        stacksize,          line,         betsize,         bblind,         potcommon,         
           FE = flop.getProbabilityFE(  20,        18,         3,          4,                  1,             1.2,            0.04,            0.75,                 35);
	printf("flop 3bet pot regular ellen 3bet                                = %lf\n", FE);

	printf("-----------------------------end of flop tests------------------------------\n");

}

void testPreflopFE()
{
	printf("\n-----------------------------preflop tests----------------------------------\n\n");

	//                                   VPIP      PFR     stacksize      poz          line          betsize          bblind         
	double FE = preflop.getProbabilityFE(20,       18,        4,          -1,            1,           0.12,           0.04,            0,             35);
	printf("preflop 3bet CO regular ellen                                   = %lf\n", FE);

	//                                   VPIP      PFR     stacksize      poz          line          betsize          bblind         
		   FE = preflop.getProbabilityFE(40,       15,        4,          -1,            1,           0.12,           0.04,            0,             35);
	printf("preflop 3bet CO fish    ellen                                   = %lf\n", FE);

	printf("--------------------------end of preflop tests------------------------------\n");
}

// 8/5 pre shove 100bb
void testPreflopRange1()
{
	printf("\n--------------------------------------8/5 pre shove range--------------------------------------------\n");

	PlayerRange range = preflop.getRange(8, 5, 4, -3, 1, 4, 0.04, 0, 0);

	range.printRange();

	printf("--------------------------------------end of 8/5 pre shove range---------------------------------------\n");
}

// 8/5 pre call 100bb
void testPreflopRange2()
{
	printf("\n--------------------------------------8/5 pre call range--------------------------------------------\n");

	PlayerRange range = preflop.getRange(8, 5, 4, -3, 0, 4, 0.04, 0, 0);

	range.printRange();

	printf("--------------------------------------end of 8/5 pre call range---------------------------------------\n");
}

void testBoardType()
{
	printf("\n--------------------------------------board type testing--------------------------------------------\n");

	vector<Card> board;
	board.push_back(Card('8','c'));
	board.push_back(Card('A','s'));
	board.push_back(Card('T','c'));
	board.push_back(Card('8','s'));
	board.push_back(Card('5','d'));

	int boardType = Evaluator::boardType(board);

	printf("8c As Tc 8s 5d board type                        = %d\n", boardType);

	printf("--------------------------------------end of board type testing---------------------------------------\n");
}

void testBoardType2()
{
	printf("\n--------------------------------------board type testing--------------------------------------------\n");

	vector<Card> board;
	board.push_back(Card('2','c'));
	board.push_back(Card('T','c'));
	board.push_back(Card('3','d'));
	board.push_back(Card('2','d'));
	board.push_back(Card('Q','d'));

	int boardType = Evaluator::boardType(board);

	printf("2c TC 3d 2d Qd board type                        = %d\n", boardType);

	printf("--------------------------------------end of board type testing---------------------------------------\n");
}

void testBoardType3()
{
	printf("\n--------------------------------------board type testing--------------------------------------------\n");

	vector<Card> board;
	board.push_back(Card('6','c'));
	board.push_back(Card('Q','d'));
	board.push_back(Card('A','h'));
	board.push_back(Card('9','h'));
	board.push_back(Card('T','h'));

	int boardType = Evaluator::boardType(board);

	printf("6c Qd Ah 9h Th board type                        = %d\n", boardType);

	printf("--------------------------------------end of board type testing---------------------------------------\n");
}

void testBoardType4()
{
	printf("\n--------------------------------------board type testing--------------------------------------------\n");

	vector<Card> board;
	board.push_back(Card('T','c'));
	board.push_back(Card('J','d'));
	board.push_back(Card('A','c'));

	int boardType = Evaluator::boardType(board);

	printf("Tc Jd Ac board type                        = %d\n", boardType);

	printf("--------------------------------------end of board type testing---------------------------------------\n");
}

void testRiverRange1()
{
	printf("\n--------------------------------------River regular AI call range 3bet pot on 8s3c8c4s3h--------------------------------------------\n");

	vector<Card> board;
	board.push_back(Card('8','s'));
	board.push_back(Card('3','c'));
	board.push_back(Card('8','c'));
	board.push_back(Card('4','s'));
	board.push_back(Card('3','h'));

	Hand hand;
	hand.setHand(Card('Q','c'),Card('Q','s'));

	//PlayerRange range = river.getRange(20, 15, 2.5, 111.5*0.04, 5, 0, 0.04, 68.5*0.04, 18.25*0.04, board, hand, 35); 
	PlayerRange range = river.getCallRaiseRange(20, 15, 2.5, 111.5*0.04, 0, 1.82, 0.04, 2.74, 0.73, board, hand, 35); 

	range.printRange();

	printf("--------------------------------------end of River regular AI call range 3bet pot on 8s3c8c4s3h---------------------------------------\n");
}

void testRiverRange2()
{
	int v[9];

	double potSize = 1.78;
	double stackSize = 2.5;
	double betSize = 1.41;
	int line = 0;
	double VPIP = 40;
	double PFR = 10;
	double AF = 1;
	double flopPotSize = 0.34;

	v[1] = normalizePotSize(4, potSize, 0.04);
	v[2] = normalizeStackSize(stackSize, 0.04);
	v[3] = normalizeBetSize(2, betSize, potSize, 0.04);
	v[4] = line;
	v[5] = normalizeVPIP(VPIP);
	v[6] = normalizePFR(PFR);
	v[7] = normalizeAF(AF);
	v[8] = normalizePotSize(2, flopPotSize, 0.04);

	printf("\n--------------------------------------River regular call range 3bet pot x, x, x--------------------------------------------\n");

	vector<Card> board;
	board.push_back(Card('5','s'));
	board.push_back(Card('6','h'));
	board.push_back(Card('3','s'));
	board.push_back(Card('9','h'));
	board.push_back(Card('3','h'));

	Hand own(Card('Q','h'), Card('Q','c'));

	river.printRange(v);

	PlayerRange range = river.getCallRaiseRange(VPIP, PFR, AF, stackSize, 0, 1, 0.04, potSize, flopPotSize, board, own, 25);
	range.printHS(board);

}

void testRiverRange3()
{
	int v[9];

	double potSize = 100*0.04;
	double stackSize = 114*0.04;
	double betSize = 100*0.04;
	int line = 1;
	double VPIP = 17.5;
	double PFR = 15.7;
	double AF = 2.5;
	double flopPotSize = 6.25*0.04;

	v[1] = normalizePotSize(4, potSize, 0.04);
	v[2] = normalizeStackSize(stackSize, 0.04);
	v[3] = normalizeBetSize(2, betSize, potSize, 0.04);
	v[4] = line;
	v[5] = normalizeVPIP(VPIP);
	v[6] = normalizePFR(PFR);
	v[7] = normalizeAF(AF);
	v[8] = normalizePotSize(2, flopPotSize, 0.04);

	printf("\n--------------------------------------River regular call range 3bet pot x, x, x--------------------------------------------\n");

	vector<Card> board;
	board.push_back(Card('6','h'));
	board.push_back(Card('5','c'));
	board.push_back(Card('A','d'));
	board.push_back(Card('Q','s'));
	board.push_back(Card('K','s'));

	Hand own(Card('A','s'), Card('6','d'));

	river.printRange(v);

	PlayerRange range = river.getRange(VPIP, PFR, AF, stackSize, line, betSize, 0.04, potSize, flopPotSize, board, own, 10);
	//PlayerRange range = river.getRange(v, board, own, 0);
	range.printHS(board);

}

void testRiverRange4()
{
	int v[9];

	double potSize = 50*0.04;
	double stackSize = 127*0.04;
	double betSize = 4;
	int line = 0;
	double VPIP = 18.4;
	double PFR = 15.1;
	double AF = 3.5;
	double flopPotSize = 5.75*0.04;

	v[1] = normalizePotSize(4, potSize, 0.04);
	v[2] = normalizeStackSize(stackSize, 0.04);
	v[3] = normalizeBetSize(2, betSize, potSize, 0.04);
	v[4] = line;
	v[5] = normalizeVPIP(VPIP);
	v[6] = normalizePFR(PFR);
	v[7] = normalizeAF(AF);
	v[8] = normalizePotSize(2, flopPotSize, 0.04);

	printf("\n--------------------------------------River regular call range 3bet pot x, x, x--------------------------------------------\n");

	vector<Card> board;
	board.push_back(Card('K','c'));
	board.push_back(Card('4','d'));
	board.push_back(Card('8','c'));
	board.push_back(Card('5','d'));
	board.push_back(Card('K','h'));

	Hand own(Card('K','s'), Card('9','c'));

	river.printRange(v);

	PlayerRange range = river.getCallRaiseRange(VPIP, PFR, AF, stackSize, line, betSize, 0.04, potSize, flopPotSize, board, own, 10);
	//PlayerRange range = river.getRange(v, board, own, 0);
	range.printHS(board);

}

void testRiverRange5()
{
	int v[9];

	double potSize = 101.25*0.04;
	double stackSize = 136.5*0.04;
	double betSize = 84.75*0.04;
	int line = 1;
	double VPIP = 16.6;
	double PFR = 12;
	double AF = 1.83;
	double flopPotSize = 8.75*0.04;

	v[1] = normalizePotSize(4, potSize, 0.04);
	v[2] = normalizeStackSize(stackSize, 0.04);
	v[3] = normalizeBetSize(2, betSize, potSize, 0.04);
	v[4] = line;
	v[5] = normalizeVPIP(VPIP);
	v[6] = normalizePFR(PFR);
	v[7] = normalizeAF(AF);
	v[8] = normalizePotSize(2, flopPotSize, 0.04);

	printf("\n--------------------------------------River regular call range 3bet pot x, x, x--------------------------------------------\n");

	vector<Card> board;
	board.push_back(Card('T','s'));
	board.push_back(Card('T','c'));
	board.push_back(Card('9','h'));
	board.push_back(Card('3','d'));
	board.push_back(Card('A','d'));

	Hand own(Card('T','h'), Card('2','h'));

	river.printRange(v);

	PlayerRange range = river.getRange(VPIP, PFR, AF, stackSize, line, betSize, 0.04, potSize, flopPotSize, board, own, 10);
	//PlayerRange range = river.getRange(v, board, own, 10);
	range.printHS(board);

}

void testRiverRange6()
{
	int v[9];

	double potSize = 1.28;
	double stackSize = 5.04;
	double betSize = 4.44;
	int line = 0;
	double VPIP = 18;
	double PFR = 15;
	double AF = 3.0;
	double flopPotSize = 0.24;

	v[1] = normalizePotSize(4, potSize, 0.04);
	v[2] = normalizeStackSize(stackSize, 0.04);
	v[3] = normalizeBetSize(2, betSize, potSize, 0.04);
	v[4] = line;
	v[5] = normalizeVPIP(VPIP);
	v[6] = normalizePFR(PFR);
	v[7] = normalizeAF(AF);
	v[8] = normalizePotSize(2, flopPotSize, 0.04);

	printf("\n--------------------------------------River regular call range 3bet pot x, x, x--------------------------------------------\n");

	vector<Card> board;
	board.push_back(Card('K','c'));
	board.push_back(Card('4','d'));
	board.push_back(Card('8','c'));
	board.push_back(Card('5','d'));
	board.push_back(Card('K','h'));

	Hand own(Card('K','s'), Card('T','c'));

	river.printRange(v);

	PlayerRange range = river.getRange(VPIP, PFR, AF, stackSize, line, betSize, 0.04, potSize, flopPotSize, board, own, 25);
	//PlayerRange range = river.getRange(v, board, own, 10);
	range.printHS(board);
}

void testRiverRange7()
{
	int v[8];

	double potSize = 27.75 * 0.04;
	double stackSize = 325.5 * 0.04;
	double betSize = 311.1 * 0.04;
	int line = 1;
	double VPIP = 20;
	double PFR = 15;
	double AF = 2.5;
	//double flopPotSize = 7.75 * 0.04;

	v[1] = normalizePotSize(2, potSize, 0.04);
	v[2] = normalizeStackSize(stackSize, 0.04);
	v[3] = normalizeBetSize(2, betSize, potSize, 0.04);
	v[4] = line;
	v[5] = normalizeVPIP(VPIP);
	v[6] = normalizePFR(PFR);
	v[7] = normalizeAF(AF);
	//v[8] = normalizePotSize(2, flopPotSize, 0.04);

	printf("\n--------------------------------------BLA--------------------------------------------\n");

	//PAIRED BOARD RIVER TUL LAZA RANGEK!

	vector<Card> board;
	board.push_back(Card('4','s'));
	board.push_back(Card('J','d'));
	board.push_back(Card('9','c'));
	/*board.push_back(Card('2','c'));
	board.push_back(Card('8','d'));*/

	Hand own;
	own.setHand(Card('K','s'),Card('K','c'));

	//river.printRange(v);

	PlayerRange range = flop.getRange(VPIP, PFR, AF, stackSize, line, betSize, 0.04, potSize, board, own, 500);
	range.printHS(board);
	range.printRange();

	PlayerRange base;
	base = base.create100();

	range = RangeUtils::mergeRange(base, range, board, own);
	range.printHS(board);

	EqCalculator calc;
	
	PlayerRange ownRange;
	ownRange.range.insert(make_pair(own, 1));
	vector<PlayerRange> ranges;
	ranges.push_back(ownRange);
	ranges.push_back(range);

	double EQ = calc.calculate(ranges, board, 5000);
	printf("EQ = %lf\n", EQ);

	
}

void testEvaluator1()
{
	printf("\n--------------------------------------River JJ on 8s3c8c4s3h--------------------------------------------\n");

	vector<Card> board;
	board.push_back(Card('8','s'));
	board.push_back(Card('3','c'));
	board.push_back(Card('8','c'));
	board.push_back(Card('4','s'));
	board.push_back(Card('3','h'));

	Hand hand;
	hand.setHand(Card('J','c'),Card('J','s'));

	printf("handstrength : %d", Evaluator::cardStrength(hand.getCard1(), hand.getCard2(), board));

	printf("--------------------------------------end of River JJ on 8s3c8c4s3h---------------------------------------\n");
}

void testEvaluator2()
{
	printf("\n--------------------------------------River AT on 7d4s4hTs8c--------------------------------------------\n");

	vector<Card> board;
	board.push_back(Card('7','d'));
	board.push_back(Card('4','s'));
	board.push_back(Card('4','h'));
	board.push_back(Card('T','s'));
	board.push_back(Card('8','c'));

	Hand hand;
	hand.setHand(Card('2','c'),Card('T','c'));

	printf("handstrength : %d", Evaluator::cardStrength(hand.getCard1(), hand.getCard2(), board));

	printf("--------------------------------------end of River AT on 7d4s4hTs8c---------------------------------------\n");
}

void testEvaluator3()
{
	printf("\n--------------------------------------FLOP 78 on 649--------------------------------------------\n");

	vector<Card> board;
	board.push_back(Card('T','s'));
	board.push_back(Card('7','s'));
	board.push_back(Card('7','c'));
	board.push_back(Card('7','h'));
	board.push_back(Card('9','h'));

	Hand hand;
	hand.setHand(Card('A','c'),Card('T','c'));

	printf("handstrength : %d", Evaluator::cardStrength(hand.getCard1(), hand.getCard2(), board));
	//printf("flopair : %d", Evaluator::flopTotalAir(hand, board));

	printf("--------------------------------------end of FLOP 78 on 649---------------------------------------\n");
}

void testEvaluator4()
{
	printf("\n--------------------------------------FLOP KT on KK485--------------------------------------------\n");

	vector<Card> board;
	board.push_back(Card('K','c'));
	board.push_back(Card('4','d'));
	board.push_back(Card('8','c'));
	board.push_back(Card('5','d'));
	board.push_back(Card('K','h'));

	Hand hand;
	hand.setHand(Card('J','s'),Card('J','c'));

	printf("handstrength : %d", Evaluator::cardStrength(hand.getCard1(), hand.getCard2(), board));

	printf("--------------------------------------end of FLOP KT on KK485---------------------------------------\n");
}

void testEvaluator5()
{
	printf("\n--------------------------------------FLOP 99 on JT2--------------------------------------------\n");

	vector<Card> board;
	board.push_back(Card('J','c'));
	board.push_back(Card('T','d'));
	board.push_back(Card('2','c'));

	Hand hand;
	hand.setHand(Card('9','s'),Card('9','c'));

	printf("handstrength : %d", Evaluator::cardStrength(hand.getCard1(), hand.getCard2(), board));

	printf("--------------------------------------end of FLOP 99 on JT2---------------------------------------\n");
}

void testEvaluator6()
{
	printf("\n--------------------------------------FLOP KT on KK485--------------------------------------------\n");

	vector<Card> board;
	board.push_back(Card('5','c'));
	board.push_back(Card('T','d'));
	board.push_back(Card('Q','h'));
	board.push_back(Card('K','s'));
	board.push_back(Card('Q','s'));

	Hand hand;
	hand.setHand(Card('A','h'),Card('J','d'));

	printf("handstrength : %d", Evaluator::cardStrength(hand.getCard1(), hand.getCard2(), board));

	printf("--------------------------------------end of FLOP KT on KK485---------------------------------------\n");
}

void testEvaluator7()
{
	printf("\n--------------------------------------RIVER KQ on T423K--------------------------------------------\n");

	vector<Card> board;
	board.push_back(Card('T','d'));
	board.push_back(Card('4','c'));
	board.push_back(Card('2','c'));
	board.push_back(Card('3','h'));
	board.push_back(Card('K','d'));

	Hand hand;
	hand.setHand(Card('K','c'),Card('Q','d'));

	printf("handstrength : %d", Evaluator::cardStrength(hand.getCard1(), hand.getCard2(), board));

	printf("--------------------------------------end of RIVER KQ on T423K---------------------------------------\n");
}

void testEvaluator8()
{
	printf("\n--------------------------------------TURN JJ on K5TA--------------------------------------------\n");

	vector<Card> board;
	board.push_back(Card('K','c'));
	board.push_back(Card('5','s'));
	board.push_back(Card('T','c'));
	board.push_back(Card('A','h'));

	Hand hand;
	hand.setHand(Card('J','s'),Card('J','h'));

	printf("handstrength : %d", Evaluator::cardStrength(hand.getCard1(), hand.getCard2(), board));

	printf("--------------------------------------end of TURN JJ on K5TA---------------------------------------\n");
}

void testEvaluator9()
{
	printf("\n--------------------------------------TURN Top pair on fucked up board----------------------------\n");

	vector<Card> board;
	board.push_back(Card('K','c'));
	board.push_back(Card('Q','s'));
	board.push_back(Card('9','c'));
	board.push_back(Card('T','h'));
	board.push_back(Card('2','h'));

	Hand hand;
	hand.setHand(Card('A','s'),Card('K','h'));

	printf("handstrength : %d", Evaluator::cardStrength(hand.getCard1(), hand.getCard2(), board));

	printf("--------------------------------------end of TURN Top pair on fucked up board------------------------\n");
}

void testEvaluator10()
{
	printf("\n--------------------------------------FLOP AA on 375--------------------------------------------\n");

	vector<Card> board;
	board.push_back(Card('3','h'));
	board.push_back(Card('7','c'));
	board.push_back(Card('5','c'));

	Hand hand;
	hand.setHand(Card('A','d'),Card('A','h'));

	printf("handstrength : %d", Evaluator::cardStrength(hand.getCard1(), hand.getCard2(), board));

	printf("--------------------------------------end of FLOP 35 on 375----------------------------------------\n");
}

void testEQCalculator()
{
	printf("\n--------------------------------------EQ calculation --------------------------------------------\n");

	PlayerRange range1, range2;
	range1.range.insert(make_pair(Hand(Card('A','d'),Card('J','h')), 1));
	range2.range.insert(make_pair(Hand(Card('A','c'),Card('T','s')), 1));

	vector<PlayerRange> ranges;
	ranges.push_back(range1);
	ranges.push_back(range2);

	vector<Card> board;
	board.push_back(Card('8','d'));
	board.push_back(Card('T','d'));
	board.push_back(Card('2','d'));
	board.push_back(Card('6','d'));

	EqCalculator calculator;

	double eq = calculator.calculate(ranges, board, 10000);

	printf("EQ of AdJh vs QdJd on 8dTd2d6d                  = %lf\n", eq);

	printf("--------------------------------------end of EQ calculation---------------------------------------\n");
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
		

		/*testPreflopFE();
		testFlopFE();
		testTurnFE();
		testRiverFE();*/

		/*
		testBoardType();
		testBoardType2();
		testBoardType3();*/
		//testBoardType4();

		//testEvaluator1();

		//testEvaluator3();

		//testEvaluator2();

		//testEvaluator6();

		//testEvaluator7();

		//testEvaluator8();

		//testEvaluator9();

		//testEvaluator10();

		//testEQCalculator();

		//testRiverRange1();
		//testRiverRange2();
		//testRiverRange3();
		//testRiverRange4();
		//testRiverRange5();
		//testRiverRange6();
		//testRiverRange7();

		vector<Card> board;
		board.push_back(Card('3', 'd'));
		board.push_back(Card('Q', 'h'));
		board.push_back(Card('8', 'c'));

		if (Evaluator::dryBoard(board))
		{
			printf("DRY\n");
		}
		else
		{
			printf("WET\n");
		}

		//testPreflopRange1();
		//testPreflopRange2();
    }
    catch (std::exception& e)
    {
        cout << "exception thrown: " << endl;
        cout << e.what() << endl;
        cout << "hit enter to terminate" << endl;
        cin.get();
    }
}