#include "cpptest.h"
#include "foundation\src\evaluator.h"
#include "foundation\src\handHistoryParser.h"

class EvaluatorUtils
{
public:
	static vector<Card> convertStringToCards(string &s)
	{
		vector<Card> result;

		for (int i = 0; i < s.length(); i = i + 2)
		{
			result.push_back(Card(s[i],s[i+1]));
		}

		return result;
	}
};

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

class RiverEvaluatorTestSuite : public Test::Suite
{
public:
	RiverEvaluatorTestSuite()
	{
		TEST_ADD(RiverEvaluatorTestSuite::riverFourColorStraightTest);
		TEST_ADD(RiverEvaluatorTestSuite::riverOneCardLowStraightTest);
	}
private:

	void riverFourColorStraightTest()
	{
		string s = "AcKdQsJsTs7s6c";
		vector<Card> cards = EvaluatorUtils::convertStringToCards(s);

		TEST_ASSERT(Evaluator::cardStrength(cards[0], cards[1], cards[2], cards[3], cards[4], cards[5], cards[6]) == 4);

		s = "9cKdQsJsTs7s6c";
		cards = EvaluatorUtils::convertStringToCards(s);

		TEST_ASSERT(Evaluator::cardStrength(cards[0], cards[1], cards[2], cards[3], cards[4], cards[5], cards[6]) == 4);

		s = "9c8dQsJsTs7s6c";
		cards = EvaluatorUtils::convertStringToCards(s);

		TEST_ASSERT(Evaluator::cardStrength(cards[0], cards[1], cards[2], cards[3], cards[4], cards[5], cards[6]) == 4);
	}

	void riverOneCardLowStraightTest()
	{
		string s = "8sAc9dTcJhQd2d";
		vector<Card> cards = EvaluatorUtils::convertStringToCards(s);

		TEST_ASSERT(Evaluator::cardStrength(cards[0], cards[1], cards[2], cards[3], cards[4], cards[5], cards[6]) == 3);

		s = "As8d2d3s4c5hKd";
		cards = EvaluatorUtils::convertStringToCards(s);

		TEST_ASSERT(Evaluator::cardStrength(cards[0], cards[1], cards[2], cards[3], cards[4], cards[5], cards[6]) == 3);

		s = "2s8d3d4s5c6h6d";
		cards = EvaluatorUtils::convertStringToCards(s);

		TEST_ASSERT(Evaluator::cardStrength(cards[0], cards[1], cards[2], cards[3], cards[4], cards[5], cards[6]) == 3);
	}
};

class EvaluatorTestSuite : public Test::Suite
{
public:
	EvaluatorTestSuite()
	{
		this->add(auto_ptr<Test::Suite>(new FlopEvaluatorTestSuite));
		this->add(auto_ptr<Test::Suite>(new RiverEvaluatorTestSuite));
	}
};