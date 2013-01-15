#pragma once

#include <iostream>

using namespace std;

#include "cpptest.h"
#include "bayestest\src\bayesUserFunctions.h"
//#include "foundation\src\bayesUtils.h"
//#include "foundation\src\evaluator.h"

class BayesPreflopTestSuite : public Test::Suite
{
public:
	BayesUserPreflop preflop;
public:
	BayesPreflopTestSuite();
protected:
	virtual void setup();
	virtual void tear_down();
private:
	void testHandTypes();
};
