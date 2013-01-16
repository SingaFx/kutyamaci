#ifndef ACTION_H
#define ACTION_H

class Action
{
private:

    char type;
	double size;
public:

    Action();
    Action(char, double);
    ~Action();
    void setAction(char, double);
    const char getType();
    const double getSize();
};

#endif