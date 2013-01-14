#include "foundation\src\evaluator.h"

void main(int argc, char *argv[])
{
	if (strlen(argv[1]) == 10)
	{
		Card c1,c2,b1,b2,b3;
		c1.rank = argv[1][0]; c1.suit = argv[1][1];
		c2.rank = argv[1][2]; c2.suit = argv[1][3];
		b1.rank = argv[1][4]; b1.suit = argv[1][5];
		b2.rank = argv[1][6]; b2.suit = argv[1][7];
		b3.rank = argv[1][8]; b3.suit = argv[1][9];
	
		cout << "Card strength : " << Evaluator::cardStrength(c1,c2,b1,b2,b3);
	}
	else if (strlen(argv[1]) == 12)
	{
		Card c1,c2,b1,b2,b3,b4;
		c1.rank = argv[1][0]; c1.suit = argv[1][1];
		c2.rank = argv[1][2]; c2.suit = argv[1][3];
		b1.rank = argv[1][4]; b1.suit = argv[1][5];
		b2.rank = argv[1][6]; b2.suit = argv[1][7];
		b3.rank = argv[1][8]; b3.suit = argv[1][9];
		b4.rank = argv[1][10]; b4.suit = argv[1][11];
	
		cout << "Card strength : " << Evaluator::cardStrength(c1,c2,b1,b2,b3,b4);
	}
	else if (strlen(argv[1]) == 14)
	{
		Card c1,c2,b1,b2,b3,b4,b5;
		c1.rank = argv[1][0]; c1.suit = argv[1][1];
		c2.rank = argv[1][2]; c2.suit = argv[1][3];
		b1.rank = argv[1][4]; b1.suit = argv[1][5];
		b2.rank = argv[1][6]; b2.suit = argv[1][7];
		b3.rank = argv[1][8]; b3.suit = argv[1][9];
		b4.rank = argv[1][10]; b4.suit = argv[1][11];
		b5.rank = argv[1][12]; b5.suit = argv[1][13];
	
		cout << "Card strength : " << Evaluator::cardStrength(c1,c2,b1,b2,b3,b4,b5);
	}
}