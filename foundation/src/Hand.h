#ifndef HAND_H
#define HAND_H

#include "Card.h"

class Hand
{
private:
    Card card1, card2;
public:
    Hand();
    Hand(Card, Card);
    ~Hand();

    void setHand(Card, Card);

    const Card& getCard1() const;
    Card& getCard1();

    const Card& getCard2() const;
    Card& getCard2();

    bool operator == (const Hand& h) const;
    bool operator < (const Hand& h) const;
};

#endif HAND_H