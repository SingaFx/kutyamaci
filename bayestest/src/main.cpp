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

		testPreflopFE();
		testFlopFE();
		testTurnFE();
		testRiverFE();

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