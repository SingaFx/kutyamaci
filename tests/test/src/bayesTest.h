#pragma once

#include "bayesPreflopTestSuite.h"

class BayesFlopTestSuite : public Test::Suite
{
};

class BayesTestSuite : public Test::Suite
{
public:
	BayesTestSuite()
	{
		this->add(auto_ptr<Test::Suite>(new BayesPreflopTestSuite));
	}
};