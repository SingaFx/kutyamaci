#include "stdafx.h"
#include "HoldemCalculator.h"

void main()
{
	double results[7];
	HoldemCalculator calc;
	calc.Calculate("AhKh|Td9s|QQ+,AQs+,AQo+|JJ-88|XxXx|XxXx|XxXx", "Ks7d4d", "", 100000, results);
}