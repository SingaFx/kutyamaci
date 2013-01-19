#pragma once

#include <iostream>

using namespace std;

#include "cpptest.h"
#include "bayesUserFunctions.h"
#include "bayeslearn\src\bayesLearnFunctions.h"
#include "handHistoryGenerator.h"

class BayesPreflopTestSuite : public Test::Suite
{
public:
	BayesLearnPreflop preflopLearn;
	BayesUserPreflop preflopUser;
	PreflopHandHistoryGenerator generator;
public:
	BayesPreflopTestSuite();
private:
	void testHandTypes();
	void testBayes1();
};
