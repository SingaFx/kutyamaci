#include "bayesPreflopTestSuite.h"

BayesPreflopTestSuite::BayesPreflopTestSuite()
{
	TEST_ADD(BayesPreflopTestSuite::testHandTypes);
}

void BayesPreflopTestSuite::setup()
{

}

void BayesPreflopTestSuite::tear_down()
{
}

void BayesPreflopTestSuite::testHandTypes()
{
	TEST_ASSERT_EQUALS(preflop.preflopHandType(Hand(Card('K','s'),Card('K','c'))), preflop.preflopHandType(Hand(Card('A','s'), Card('A','c'))));
}