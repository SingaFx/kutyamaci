#include "Action.h"

Action::Action()
{
    setAction('c', 0);
};

Action::Action(char t, double s)
{
    setAction(t, s);
};

Action::~Action()
{
}

void Action::setAction(char t, double s)
{
    type = t;
    size = s;
};

const char Action::getType()
{
    return type;
};

const double Action::getSize()
{
    return size;
};

std::string Action::toString()
{
	std::string res = "";
	res += type;

	return res;
}