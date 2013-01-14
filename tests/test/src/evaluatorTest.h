#include "cpptest.h"
#include "foundation\src\evaluator.h"
#include "foundation\src\handHistoryParser.h"

class FlopEvaluatorTestSuite : public Test::Suite
{
public:
	FlopEvaluatorTestSuite()
	{
		TEST_ADD(FlopEvaluatorTestSuite::flopStraightFlushTest);
	}
private:
	void flopStraightFlushTest()
	{
		Card c1('A','s');
		Card c2('K','s');
		vector<Card> cards;
		cards.push_back(Card('Q','s'));
		cards.push_back(Card('J','s'));
		cards.push_back(Card('T','s'));

		TEST_ASSERT(Evaluator::cardStrength(c1, c2, cards) == 0);
	}
};

class EvaluatorTestSuite : public Test::Suite
{
public:
	EvaluatorTestSuite()
	{
		this->add(auto_ptr<Test::Suite>(new FlopEvaluatorTestSuite));
	}
};