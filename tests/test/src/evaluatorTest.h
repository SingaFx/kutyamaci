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
		TEST_ADD(RiverEvaluatorTestSuite::riverOneCardStrongStraightTest);
		TEST_ADD(RiverEvaluatorTestSuite::riverOneCardStraightOnSuitedBoard);
		TEST_ADD(RiverEvaluatorTestSuite::riverOneCardStraightOnDoubleBoard);
		TEST_ADD(RiverEvaluatorTestSuite::riverOneCardStraightOnDoubleAndSuitedBoard);
		TEST_ADD(RiverEvaluatorTestSuite::riverStraightOnTripleBoard);
		TEST_ADD(RiverEvaluatorTestSuite::riverStraightOnDoublePairedBoard);
		TEST_ADD(RiverEvaluatorTestSuite::riverLowStraight);
		TEST_ADD(RiverEvaluatorTestSuite::riverLowStraightOnSuitedBoard);
		TEST_ADD(RiverEvaluatorTestSuite::riverLowStraightOnDoubleBoard);
		TEST_ADD(RiverEvaluatorTestSuite::riverStrongStraight);
		TEST_ADD(RiverEvaluatorTestSuite::riverStraightOnSuitedBoard);
		TEST_ADD(RiverEvaluatorTestSuite::riverStraightOnDoubleBoard);
		TEST_ADD(RiverEvaluatorTestSuite::riverStraightOnDoubleAndSuitedBoard);
		TEST_ADD(RiverEvaluatorTestSuite::riverStraightOnBoard);
		TEST_ADD(RiverEvaluatorTestSuite::riverTripsOnBoard);
		TEST_ADD(RiverEvaluatorTestSuite::riverSet);
		TEST_ADD(RiverEvaluatorTestSuite::riverDrillTopKicker);
		TEST_ADD(RiverEvaluatorTestSuite::riverDrillNoKicker);
	}
private:

	void riverFourColorStraightTest()
	{
		string s = "AcKdQsJsTs7s6c";
		vector<Card> cards = EvaluatorUtils::convertStringToCards(s);

		TEST_ASSERT_EQUALS(4, Evaluator::cardStrength(cards[0], cards[1], cards[2], cards[3], cards[4], cards[5], cards[6]));

		s = "9cKdQsJsTs7s6c";
		cards = EvaluatorUtils::convertStringToCards(s);

		TEST_ASSERT_EQUALS(4, Evaluator::cardStrength(cards[0], cards[1], cards[2], cards[3], cards[4], cards[5], cards[6]));

		s = "9c8dQsJsTs7s6c";
		cards = EvaluatorUtils::convertStringToCards(s);

		TEST_ASSERT_EQUALS(4, Evaluator::cardStrength(cards[0], cards[1], cards[2], cards[3], cards[4], cards[5], cards[6]));
	}

	void riverOneCardLowStraightTest()
	{
		string s = "8sAc9dTcJhQd2d";
		vector<Card> cards = EvaluatorUtils::convertStringToCards(s);

		TEST_ASSERT_EQUALS(3, Evaluator::cardStrength(cards[0], cards[1], cards[2], cards[3], cards[4], cards[5], cards[6]));

		s = "As8d2d3s4c5hKd";
		cards = EvaluatorUtils::convertStringToCards(s);

		TEST_ASSERT_EQUALS(3, Evaluator::cardStrength(cards[0], cards[1], cards[2], cards[3], cards[4], cards[5], cards[6]));

		s = "2s8d3d4s5c6h6d";
		cards = EvaluatorUtils::convertStringToCards(s);

		TEST_ASSERT_EQUALS(3, Evaluator::cardStrength(cards[0], cards[1], cards[2], cards[3], cards[4], cards[5], cards[6]));
	}

	void riverOneCardStrongStraightTest()
	{
		string s = "8s5c5d6s7c9dAc";
		vector<Card> cards = EvaluatorUtils::convertStringToCards(s);

		TEST_ASSERT_EQUALS(1, Evaluator::cardStrength(cards[0], cards[1], cards[2], cards[3], cards[4], cards[5], cards[6]));

		s = "As8dTsJcQdKh2h";
		cards = EvaluatorUtils::convertStringToCards(s);

		TEST_ASSERT_EQUALS(0, Evaluator::cardStrength(cards[0], cards[1], cards[2], cards[3], cards[4], cards[5], cards[6]));

		s = "9h2cAc3d4h5h8c";
		cards = EvaluatorUtils::convertStringToCards(s);

		TEST_ASSERT_EQUALS(1, Evaluator::cardStrength(cards[0], cards[1], cards[2], cards[3], cards[4], cards[5], cards[6]));

		s = "Jh2cAcQdKhTh8c";
		cards = EvaluatorUtils::convertStringToCards(s);

		TEST_ASSERT_EQUALS(0, Evaluator::cardStrength(cards[0], cards[1], cards[2], cards[3], cards[4], cards[5], cards[6]));
	}

	void riverOneCardStraightOnSuitedBoard()
	{
		string s = "8s5c5d6s7c9cAc";
		vector<Card> cards = EvaluatorUtils::convertStringToCards(s);

		TEST_ASSERT_EQUALS(2, Evaluator::cardStrength(cards[0], cards[1], cards[2], cards[3], cards[4], cards[5], cards[6]));

		s = "As8dTsJcQhKh2h";
		cards = EvaluatorUtils::convertStringToCards(s);

		TEST_ASSERT_EQUALS(1, Evaluator::cardStrength(cards[0], cards[1], cards[2], cards[3], cards[4], cards[5], cards[6]));

		s = "9h2cAc3d4h5h8h";
		cards = EvaluatorUtils::convertStringToCards(s);

		TEST_ASSERT_EQUALS(2, Evaluator::cardStrength(cards[0], cards[1], cards[2], cards[3], cards[4], cards[5], cards[6]));
	}

	void riverOneCardStraightOnDoubleBoard()
	{
		string s = "8s5c5d6s7c9c9d";
		vector<Card> cards = EvaluatorUtils::convertStringToCards(s);

		TEST_ASSERT_EQUALS(2, Evaluator::cardStrength(cards[0], cards[1], cards[2], cards[3], cards[4], cards[5], cards[6]));

		s = "As8dTsJcQhKhJd";
		cards = EvaluatorUtils::convertStringToCards(s);

		TEST_ASSERT_EQUALS(2, Evaluator::cardStrength(cards[0], cards[1], cards[2], cards[3], cards[4], cards[5], cards[6]));

		s = "9h2cAc3d4h5h3c";
		cards = EvaluatorUtils::convertStringToCards(s);

		TEST_ASSERT_EQUALS(2, Evaluator::cardStrength(cards[0], cards[1], cards[2], cards[3], cards[4], cards[5], cards[6]));
	}

	void riverOneCardStraightOnDoubleAndSuitedBoard()
	{
		string s = "8s5c5d6c7c9c9d";
		vector<Card> cards = EvaluatorUtils::convertStringToCards(s);

		TEST_ASSERT_EQUALS(3, Evaluator::cardStrength(cards[0], cards[1], cards[2], cards[3], cards[4], cards[5], cards[6]));

		s = "As8dTsJhQhKhJd";
		cards = EvaluatorUtils::convertStringToCards(s);

		TEST_ASSERT_EQUALS(3, Evaluator::cardStrength(cards[0], cards[1], cards[2], cards[3], cards[4], cards[5], cards[6]));

		s = "9h2cAc3h4h5h3c";
		cards = EvaluatorUtils::convertStringToCards(s);

		TEST_ASSERT_EQUALS(3, Evaluator::cardStrength(cards[0], cards[1], cards[2], cards[3], cards[4], cards[5], cards[6]));
	}

	void riverStraightOnTripleBoard()
	{
		string s = "6s7c5d8s9h9d9c";
		vector<Card> cards = EvaluatorUtils::convertStringToCards(s);

		TEST_ASSERT_EQUALS(4, Evaluator::cardStrength(cards[0], cards[1], cards[2], cards[3], cards[4], cards[5], cards[6]));

		s = "As5d2c3d4h2d2h";
		cards = EvaluatorUtils::convertStringToCards(s);

		TEST_ASSERT_EQUALS(4, Evaluator::cardStrength(cards[0], cards[1], cards[2], cards[3], cards[4], cards[5], cards[6]));
	}

	void riverStraightOnDoublePairedBoard()
	{
		string s = "6s7c5d8s9h9d8d";
		vector<Card> cards = EvaluatorUtils::convertStringToCards(s);

		TEST_ASSERT_EQUALS(3, Evaluator::cardStrength(cards[0], cards[1], cards[2], cards[3], cards[4], cards[5], cards[6]));

		s = "As5d2c3d4h2d4d";
		cards = EvaluatorUtils::convertStringToCards(s);

		TEST_ASSERT_EQUALS(3, Evaluator::cardStrength(cards[0], cards[1], cards[2], cards[3], cards[4], cards[5], cards[6]));
	}

	void riverLowStraight()
	{
		string s = "6s7c8d9hTcAd2c";
		vector<Card> cards = EvaluatorUtils::convertStringToCards(s);

		TEST_ASSERT_EQUALS(1, Evaluator::cardStrength(cards[0], cards[1], cards[2], cards[3], cards[4], cards[5], cards[6]));

		s = "As2d3c4h5h9dTd";
		cards = EvaluatorUtils::convertStringToCards(s);

		TEST_ASSERT_EQUALS(1, Evaluator::cardStrength(cards[0], cards[1], cards[2], cards[3], cards[4], cards[5], cards[6]));
	}

	void riverLowStraightOnSuitedBoard()
	{
		string s = "6s7c8d9hTcAc2c";
		vector<Card> cards = EvaluatorUtils::convertStringToCards(s);

		TEST_ASSERT_EQUALS(2, Evaluator::cardStrength(cards[0], cards[1], cards[2], cards[3], cards[4], cards[5], cards[6]));

		s = "As2d3c4h5d9dTd";
		cards = EvaluatorUtils::convertStringToCards(s);

		TEST_ASSERT_EQUALS(2, Evaluator::cardStrength(cards[0], cards[1], cards[2], cards[3], cards[4], cards[5], cards[6]));
	}

	void riverLowStraightOnDoubleBoard()
	{
		string s = "6s7c8d9cTcTd2c";
		vector<Card> cards = EvaluatorUtils::convertStringToCards(s);

		TEST_ASSERT_EQUALS(2, Evaluator::cardStrength(cards[0], cards[1], cards[2], cards[3], cards[4], cards[5], cards[6]));

		s = "As2d3c4d5d9d9h";
		cards = EvaluatorUtils::convertStringToCards(s);

		TEST_ASSERT_EQUALS(2, Evaluator::cardStrength(cards[0], cards[1], cards[2], cards[3], cards[4], cards[5], cards[6]));
	}

	void riverStrongStraight()
	{
		string s = "9dTd6s7s8h2cAd";
		vector<Card> cards = EvaluatorUtils::convertStringToCards(s);

		TEST_ASSERT_EQUALS(0, Evaluator::cardStrength(cards[0], cards[1], cards[2], cards[3], cards[4], cards[5], cards[6]));

		s = "3s4sAc2c5h9dTd";
		cards = EvaluatorUtils::convertStringToCards(s);

		TEST_ASSERT_EQUALS(0, Evaluator::cardStrength(cards[0], cards[1], cards[2], cards[3], cards[4], cards[5], cards[6]));

		s = "ThJhKdQdAh2c3c";
		cards = EvaluatorUtils::convertStringToCards(s);

		TEST_ASSERT_EQUALS(0, Evaluator::cardStrength(cards[0], cards[1], cards[2], cards[3], cards[4], cards[5], cards[6]));
	}

	void riverStraightOnSuitedBoard()
	{
		string s = "9dTd6s7s8s2cAd";
		vector<Card> cards = EvaluatorUtils::convertStringToCards(s);

		TEST_ASSERT_EQUALS(1, Evaluator::cardStrength(cards[0], cards[1], cards[2], cards[3], cards[4], cards[5], cards[6]));

		s = "3s4sAc2c5c9dTd";
		cards = EvaluatorUtils::convertStringToCards(s);

		TEST_ASSERT_EQUALS(1, Evaluator::cardStrength(cards[0], cards[1], cards[2], cards[3], cards[4], cards[5], cards[6]));

		s = "ThJhKdQdAd2c3c";
		cards = EvaluatorUtils::convertStringToCards(s);

		TEST_ASSERT_EQUALS(1, Evaluator::cardStrength(cards[0], cards[1], cards[2], cards[3], cards[4], cards[5], cards[6]));
	}

	void riverStraightOnDoubleBoard()
	{
		string s = "9dTd6s7s8h2c2d";
		vector<Card> cards = EvaluatorUtils::convertStringToCards(s);

		TEST_ASSERT_EQUALS(1, Evaluator::cardStrength(cards[0], cards[1], cards[2], cards[3], cards[4], cards[5], cards[6]));

		s = "3s4sAc2c5h9d9h";
		cards = EvaluatorUtils::convertStringToCards(s);

		TEST_ASSERT_EQUALS(1, Evaluator::cardStrength(cards[0], cards[1], cards[2], cards[3], cards[4], cards[5], cards[6]));

		s = "ThJhKdQdAh2c2s";
		cards = EvaluatorUtils::convertStringToCards(s);

		TEST_ASSERT_EQUALS(1, Evaluator::cardStrength(cards[0], cards[1], cards[2], cards[3], cards[4], cards[5], cards[6]));
	}

	void riverStraightOnDoubleAndSuitedBoard()
	{
		string s = "9dTd6s7s8s2c2d";
		vector<Card> cards = EvaluatorUtils::convertStringToCards(s);

		TEST_ASSERT_EQUALS(2, Evaluator::cardStrength(cards[0], cards[1], cards[2], cards[3], cards[4], cards[5], cards[6]));

		s = "3s4sAc2c5c9d9h";
		cards = EvaluatorUtils::convertStringToCards(s);

		TEST_ASSERT_EQUALS(2, Evaluator::cardStrength(cards[0], cards[1], cards[2], cards[3], cards[4], cards[5], cards[6]));

		s = "ThJhKdQdAd2c2s";
		cards = EvaluatorUtils::convertStringToCards(s);

		TEST_ASSERT_EQUALS(2, Evaluator::cardStrength(cards[0], cards[1], cards[2], cards[3], cards[4], cards[5], cards[6]));
	}

	void riverStraightOnBoard()
	{
		string s = "2h2c6s7s8d9dTc";
		vector<Card> cards = EvaluatorUtils::convertStringToCards(s);

		TEST_ASSERT_EQUALS(4, Evaluator::cardStrength(cards[0], cards[1], cards[2], cards[3], cards[4], cards[5], cards[6]));

		s = "JhQh6s7s8d9dTc";
		cards = EvaluatorUtils::convertStringToCards(s);

		TEST_ASSERT_EQUALS(0, Evaluator::cardStrength(cards[0], cards[1], cards[2], cards[3], cards[4], cards[5], cards[6]));

		s = "JhKh6s7s8d9dTc";
		cards = EvaluatorUtils::convertStringToCards(s);

		TEST_ASSERT_EQUALS(1, Evaluator::cardStrength(cards[0], cards[1], cards[2], cards[3], cards[4], cards[5], cards[6]));

		s = "Ah2hJdQdKcTc9s";
		cards = EvaluatorUtils::convertStringToCards(s);

		TEST_ASSERT_EQUALS(0, Evaluator::cardStrength(cards[0], cards[1], cards[2], cards[3], cards[4], cards[5], cards[6]));

		s = "JhQhTcJcQdKdAh";
		cards = EvaluatorUtils::convertStringToCards(s);

		TEST_ASSERT_EQUALS(8, Evaluator::cardStrength(cards[0], cards[1], cards[2], cards[3], cards[4], cards[5], cards[6]));

		s = "JhQhTcJcQdKdAd";
		cards = EvaluatorUtils::convertStringToCards(s);

		TEST_ASSERT_EQUALS(4, Evaluator::cardStrength(cards[0], cards[1], cards[2], cards[3], cards[4], cards[5], cards[6]));
	}

	void riverTripsOnBoard()
	{
		string s = "AhKc6d6s6c7d2h";
		vector<Card> cards = EvaluatorUtils::convertStringToCards(s);

		TEST_ASSERT_EQUALS(4, Evaluator::cardStrength(cards[0], cards[1], cards[2], cards[3], cards[4], cards[5], cards[6]));
	}

	void riverSet()
	{
		string s = "7d7c7s8s9sTsAd";
		vector<Card> cards = EvaluatorUtils::convertStringToCards(s);

		TEST_ASSERT_EQUALS(3, Evaluator::cardStrength(cards[0], cards[1], cards[2], cards[3], cards[4], cards[5], cards[6]));

		s = "7d7c7s8s9s2hAd";
		cards = EvaluatorUtils::convertStringToCards(s);

		TEST_ASSERT_EQUALS(2, Evaluator::cardStrength(cards[0], cards[1], cards[2], cards[3], cards[4], cards[5], cards[6]));

		s = "7d7c7s8s3s2hQd";
		cards = EvaluatorUtils::convertStringToCards(s);

		TEST_ASSERT_EQUALS(1, Evaluator::cardStrength(cards[0], cards[1], cards[2], cards[3], cards[4], cards[5], cards[6]));

		s = "7d7c7s8s3d2hQd";
		cards = EvaluatorUtils::convertStringToCards(s);

		TEST_ASSERT_EQUALS(0, Evaluator::cardStrength(cards[0], cards[1], cards[2], cards[3], cards[4], cards[5], cards[6]));
	}

	void riverDrillTopKicker()
	{
		string s = "Ad7s7d7h8h9hKh";
		vector<Card> cards = EvaluatorUtils::convertStringToCards(s);

		TEST_ASSERT_EQUALS(3, Evaluator::cardStrength(cards[0], cards[1], cards[2], cards[3], cards[4], cards[5], cards[6]));

		s = "Ad7s7d7h8h9hKc";
		cards = EvaluatorUtils::convertStringToCards(s);

		TEST_ASSERT_EQUALS(2, Evaluator::cardStrength(cards[0], cards[1], cards[2], cards[3], cards[4], cards[5], cards[6]));

		s = "Ad7s7d7h8h9dKc";
		cards = EvaluatorUtils::convertStringToCards(s);

		TEST_ASSERT_EQUALS(1, Evaluator::cardStrength(cards[0], cards[1], cards[2], cards[3], cards[4], cards[5], cards[6]));

		s = "Ad7s7d7h8h2dKc";
		cards = EvaluatorUtils::convertStringToCards(s);

		TEST_ASSERT_EQUALS(0, Evaluator::cardStrength(cards[0], cards[1], cards[2], cards[3], cards[4], cards[5], cards[6]));
	}

	void riverDrillNoKicker()
	{
		string s = "2d7s7d7h8h9hKh";
		vector<Card> cards = EvaluatorUtils::convertStringToCards(s);

		TEST_ASSERT_EQUALS(4, Evaluator::cardStrength(cards[0], cards[1], cards[2], cards[3], cards[4], cards[5], cards[6]));

		s = "2d7s7d7h8h9hKc";
		cards = EvaluatorUtils::convertStringToCards(s);

		TEST_ASSERT_EQUALS(3, Evaluator::cardStrength(cards[0], cards[1], cards[2], cards[3], cards[4], cards[5], cards[6]));

		s = "2d7s7d7h8h9dKc";
		cards = EvaluatorUtils::convertStringToCards(s);

		TEST_ASSERT_EQUALS(2, Evaluator::cardStrength(cards[0], cards[1], cards[2], cards[3], cards[4], cards[5], cards[6]));

		s = "3d7s7d7h8h2dKc";
		cards = EvaluatorUtils::convertStringToCards(s);

		TEST_ASSERT_EQUALS(1, Evaluator::cardStrength(cards[0], cards[1], cards[2], cards[3], cards[4], cards[5], cards[6]));
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