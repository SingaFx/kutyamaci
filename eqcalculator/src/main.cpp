#include "eqcalculator.h"

int main()
{
	PlayerRange range1,range2, range3;
	range1.range.insert(make_pair(Hand(Card('A','s'),Card('A','d')), 1));
	//range1.range.insert(make_pair(Hand(Card('2','s'),Card('7','d')),0.5));
	range2.range.insert(make_pair(Hand(Card('Q','s'),Card('Q','d')),0.33));
	range2.range.insert(make_pair(Hand(Card('A','s'),Card('A','d')),0.33));
	range2.range.insert(make_pair(Hand(Card('A','c'),Card('A','d')),0.34));
	range3.range.insert(make_pair(Hand(Card('K','s'),Card('K','d')),1));

	vector<PlayerRange> range;
	vector<Card> board;
	range.push_back(range1);
	range.push_back(range2);
	range.push_back(range3);

	EqCalculator calc;
	cout << calc.calculate(range,board,25000) << endl;

	return 0;
}