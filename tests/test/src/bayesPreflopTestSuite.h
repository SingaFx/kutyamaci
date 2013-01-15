#include <iostream>

using namespace std;

#include "cpptest.h"
#include "bayestest\src\bayesUserFunctions.h"

class BayesPreflopTestSuite : public Test::Suite
{
public:
	BayesPreflopTestSuite()
	{
	}
protected:
	virtual void setup();
	virtual void tear_down();
private:
	void testHandTypes();
};