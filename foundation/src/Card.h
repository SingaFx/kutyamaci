#ifndef CARD_H
#define CARD_H

class Card
{
private:

    char rank, suit;
public:

    Card();
    Card(char, char);
    ~Card();

    void setRank(char);
    char getRank();
    char getRank() const;

    void setSuit(char);
    char getSuit() const;
    char getSuit();

    void setCard(char, char);

    bool operator == (const Card& c) const;
    bool operator < (const Card& c) const;
};

#endif