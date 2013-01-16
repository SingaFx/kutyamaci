#include "bayesPreflopTestSuite.h"
#include "handHistoryGenerator.h"

BayesPreflopTestSuite::BayesPreflopTestSuite()
{
	TEST_ADD(BayesPreflopTestSuite::testHandTypes);
}

void BayesPreflopTestSuite::testHandTypes()
{
	TEST_ASSERT_EQUALS(preflop.preflopHandType(Hand(Card('K','s'),Card('K','c'))), preflop.preflopHandType(Hand(Card('A','s'), Card('A','c'))));
	TEST_ASSERT_EQUALS(preflop.preflopHandType(Hand(Card('5','s'),Card('5','c'))), preflop.preflopHandType(Hand(Card('9','s'), Card('9','c'))));
	TEST_ASSERT(preflop.preflopHandType(Hand(Card('5','s'),Card('5','c'))) != preflop.preflopHandType(Hand(Card('A','s'), Card('A','c'))));
}

void BayesPreflopTestSuite::testBayes1()
{
	//generate hand history

	//learn it with bayeslearn

	//test it with bayesuser
}