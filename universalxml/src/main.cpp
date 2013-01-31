#include <memory>   // std::auto_ptr
#include <iostream> 
#include <fstream>

#include "universalpokerhand.hxx"

using namespace std;

void main(int argc, char* argv[])
{
    auto_ptr<handhistory_container_type> h(handhistory_container(argv[1]));
    //handhistory_t::id_type hand_id("Clsdkfg");
    //handhistory_t handhistory(hand_id);
    ofstream* generatedHandHistory = new ofstream();
    generatedHandHistory->open("generatedHH.txt");


    for (handhistory_container_type::handhistory_const_iterator handhistory_iterator = h->handhistory().begin(); handhistory_iterator != h->handhistory().end(); ++handhistory_iterator)
    {                
        (*generatedHandHistory) << "***** History for hand " << handhistory_iterator->id() << " *****" << endl;
        (*generatedHandHistory) << "Start hand: Wed Aug 24 17:59:45 EEST 2011" << endl;
        (*generatedHandHistory) << handhistory_iterator->table() << "[169617518]" << "(NO_LIMIT TEXAS_HOLDEM $0.02/$0.04, Real money)" << endl;
        (*generatedHandHistory) << "User: " << handhistory_iterator->user() << endl;
        (*generatedHandHistory) << "Button: seat " << handhistory_iterator->dealer() <<endl;
        (*generatedHandHistory) << "Players in round: " << handhistory_iterator->nrplayers() <<endl;

        for (handhistory_type::seat_const_iterator seat_iterator = handhistory_iterator->seat().begin(); seat_iterator != handhistory_iterator->seat().end(); ++seat_iterator)
        {
            (*generatedHandHistory) << "Seat " << seat_iterator->position() << ": " << seat_iterator->playername() << " ($" << seat_iterator->balance() << ")" << endl;
        }

        (*generatedHandHistory) << handhistory_iterator->smallblind() << " posts small blind ($0.02)" << endl;
        (*generatedHandHistory) << handhistory_iterator->bigblind() << " posts big blind ($0.04)" << endl;
        (*generatedHandHistory) << "---" << endl;
        (*generatedHandHistory) << "Dealing pocket cards" << endl;
        (*generatedHandHistory) << "Dealing to " << handhistory_iterator->user() << ": [" << handhistory_iterator->holecard1() << ", " << handhistory_iterator->holecard2() << "]" << endl;

        for (handhistory_type::preflopaction_const_iterator preflop_iterator = handhistory_iterator->preflopaction().begin(); preflop_iterator != handhistory_iterator->preflopaction().end(); ++preflop_iterator)
        {
            (*generatedHandHistory) << preflop_iterator->playername() ;
            string action = preflop_iterator->action();
            if (action == "f")
            {
                (*generatedHandHistory) << " folds" << endl;
            }
            else if (action == "c")
            {
                (*generatedHandHistory) << " calls $" << preflop_iterator->size() << endl;
            }
            else if (action == "x")
            {
                (*generatedHandHistory) << " checks" << endl;
            }
            else if (action == "r")
            {
                (*generatedHandHistory) << " raises $" << preflop_iterator->size() << " to $" << preflop_iterator->raisesto() << endl;
            }
        }

        (*generatedHandHistory) << "***** End of hand " <<  handhistory_iterator->id() << "*****" << endl;
    }

    generatedHandHistory->close();
}