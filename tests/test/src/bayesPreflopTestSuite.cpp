#include "bayesPreflopTestSuite.h"
#include "handHistoryGenerator.h"

BayesPreflopTestSuite::BayesPreflopTestSuite()
{
	TEST_ADD(BayesPreflopTestSuite::testHandTypes);
}

void BayesPreflopTestSuite::testHandTypes()
{
	TEST_ASSERT_EQUALS(preflopUser.preflopHandType(Hand(Card('K','s'),Card('K','c'))), preflopUser.preflopHandType(Hand(Card('A','s'), Card('A','c'))));
	TEST_ASSERT_EQUALS(preflopUser.preflopHandType(Hand(Card('5','s'),Card('5','c'))), preflopUser.preflopHandType(Hand(Card('9','s'), Card('9','c'))));
	TEST_ASSERT(preflopUser.preflopHandType(Hand(Card('5','s'),Card('5','c'))) != preflopUser.preflopHandType(Hand(Card('A','s'), Card('A','c'))));
}

void BayesPreflopTestSuite::testBayes1()
{
	//generate hand history
	PreflopSituation situation;
	situation.handknown = 0;
	generator.generateHandHistory("test1", 2, situation);

	//learn it with bayeslearn

	//test it with bayesuser
}