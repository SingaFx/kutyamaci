#pragma once

#include <iostream>

using namespace std;

#include "cpptest.h"
#include "bayestest\src\bayesUserFunctions.h"
#include "bayeslearn\src\bayesLearnFunctions.h"

class BayesPreflopTestSuite : public Test::Suite
{
public:
	BayesUserPreflop preflop;
public:
	BayesPreflopTestSuite();
private:
	void testHandTypes();
	void testBayes1();
};
