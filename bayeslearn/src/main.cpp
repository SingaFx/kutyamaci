#include <iostream>
#include <fstream>
#include <vector>
#include <cstdio>

#include "handHistoryParser.h"
#include "bayesLearnFunctions.h"
#include "database.h"

//using namespace dlib;
;
using namespace std;
//using namespace bayes_node_utils;

FILE* logfile;
Database* database;
//directed_graph<bayes_node>::kernel_1a_c bn[4]; //Preflop, Flop, Turn, River
//assignment parent_state;

/*
hand strength -> 5
	0. FOLD
	1. WEAK DRAW
	2. DRAW
	3. WEAK MADE HAND
	4. NORMAL MADE HAND
	5. STRONG MADE HAND
pot size -> 3
	0. small pot (limped)
	1. normal pot
	2. big pot
stack size -> 5 (effective stack size?)
	0. 0 - 30bb
	1. 30 - 70bb
	2. 70 - 100bb
	3. 100 - 150bb
	4. 150+
bet size -> 3
	0. small bet
	1. normal bet
	2. overbet
player line -> 4
	1. WEAK (c, c, c)
	2. NORMAL (r/f, r)
	3. STRONG (r/c, r/r, x/r)
(player type -> 3
	0. passive FISH
	1. aggressive FISH
	2. REG)
(VPIP(4), PFR(3), AF(3))
------------------
540
*/

BayesLearnPreflop* preflop;
BayesLearnFlop* flop;
BayesLearnTurn* turn;

int total;


void parseHands(string filename)
{
	int total = 0;
	printf("Begin parsing hands\n");
	
	FILE *f = fopen(filename.c_str(),"r");
	char s[100];

	int historyNumber;
	while (fscanf(f,"%d\n", &historyNumber) == 1 && historyNumber != 0)
	{
		std::vector<HandHistory> history = HandHistoryUtils::importFromFile(f, historyNumber);
		total += history.size();
		printf("Total number of parsed hands = %d\n", total);
		for (int i = 0; i < history.size(); ++i)
		{
			preflop->updateProbabilities(history[i]);
			flop->updateProbabilities(history[i]);
			turn->updateProbabilities(history[i]);
		}
	}
}


/*
void test()
{
	//set_node_value(bn, C, 1);
    //set_node_as_evidence(bn, C);
	//bayesian_network_join_tree solution_with_evidence(bn, join_tree);
	//gibbs sampling if needed?
}
*/
/*
void testPreflop(directed_graph<bayes_node>::kernel_1a_c& bn)
{
	typedef dlib::set<unsigned long>::compare_1b_c set_type;
    typedef graph<set_type, set_type>::kernel_1a_c join_tree_type;
    join_tree_type join_tree;

	create_moral_graph(bn, join_tree);
    create_join_tree(join_tree, join_tree);

	//megfigyelesek

	double VPIP = 20;
	double PFR = 10;
	double betsize = 0.2;
	int poz = 0;
	double stacksize = 2;
	int line = 0;

//[PREFLOP_PLAYER_STACK_SIZE_NUM][PREFLOP_PLAYER_BET_SIZE_NUM][PREFLOP_PLAYER_LINE_NUM][PREFLOP_PLAYER_VPIP_NUM][PREFLOP_PLAYER_PFR_NUM][PREFLOP_PLAYER_POZ_NUM];


	int nStackSize = 2;
	int nBetSize = 2;
	int nLine = 0;
	int nVPIP = 2;
	int nPFR = 0;
	poz = 1;

	printf("TEST2\n");

	//
	set_node_value(bn, PREFLOP_PLAYER_VPIP, nVPIP);
	set_node_as_evidence(bn, PREFLOP_PLAYER_VPIP);
	set_node_value(bn, PREFLOP_PLAYER_PFR, nPFR);
	set_node_as_evidence(bn, PREFLOP_PLAYER_PFR);
	set_node_value(bn, PREFLOP_PLAYER_BET_SIZE, nBetSize);
	set_node_as_evidence(bn, PREFLOP_PLAYER_BET_SIZE);
	set_node_value(bn, PREFLOP_PLAYER_POZ, poz);
	set_node_as_evidence(bn, PREFLOP_PLAYER_POZ);
	set_node_value(bn, PREFLOP_PLAYER_STACK_SIZE, nStackSize);
	set_node_as_evidence(bn, PREFLOP_PLAYER_STACK_SIZE);
	set_node_value(bn, PREFLOP_PLAYER_LINE, line);
	set_node_as_evidence(bn, PREFLOP_PLAYER_LINE);

	/*
	bayesian_network_join_tree solution(bn, join_tree);

	printf("TEST\n");

	for (int i = 0; i < PREFLOP_HAND_STRENGTH_NUM; ++i)
	{
		cout << solution.probability(0)(i) << endl;
	}
	*/

/*
	set_node_value(bn, 0, 0);
	bayesian_network_gibbs_sampler sampler;

	unsigned long A[PREFLOP_HAND_STRENGTH_NUM];
	memset(A, 0, sizeof(A));
	const long rounds = 2000;
    for (long i = 0; i < rounds; ++i)
    {
        sampler.sample_graph(bn);
		A[node_value(bn, 0)]++;
    }

	for (int i = 0; i < PREFLOP_HAND_STRENGTH_NUM; ++i)
	{
		cout << "p(A= " << i << ") = " << (double)A[i]/(double)rounds << endl;
	}
}
*/

/*
void setPostFlop()
{
	//set postflop
		for (int i = 1; i < 4; ++i)
			bn[i].set_number_of_nodes(node_number);
		
		//setup edges
		//keep it simple now, just    all-{1}->0
		for (int i = 1; i < 4; ++i)
		{
			for (int j = 2; j < node_number; ++j) //2..node_number
				bn[i].add_edge(j, 0);
		}

		//all->1
		for (int i = 1; i < 4; ++i)
		{
			for (int j = 2; j < node_number; ++j) //2..node_number
				bn[i].add_edge(j, 1);
			bn[i].add_edge(0, 1);
		}

		//setup node parameters	
		for (int i = 1; i < 4; ++i)
		{
			set_node_num_values(bn[i], HAND_STRENGTH, HAND_STRENGTH_NUM);
			set_node_num_values(bn[i], FOLD_EQUITY, FOLD_EQUITY_NUM);
			set_node_num_values(bn[i], POT_SIZE, POT_SIZE_NUM);
			set_node_num_values(bn[i], PLAYER_STACK_SIZE, PLAYER_STACK_SIZE_NUM);
			set_node_num_values(bn[i], PLAYER_BET_SIZE, PLAYER_BET_SIZE_NUM); 
			set_node_num_values(bn[i], PLAYER_LINE, PLAYER_LINE_NUM);
			set_node_num_values(bn[i], PLAYER_VPIP, PLAYER_VPIP_NUM);
			set_node_num_values(bn[i], PLAYER_PFR, PLAYER_PFR_NUM);
			set_node_num_values(bn[i], PLAYER_AF, PLAYER_AF_NUM);
		}

	//learn parameters
		
		//!read from a file the data
		//update node probabilities -> probability[...]!
		
		//p(a1), p(a2) -> if not observed!
		
		
		for (int i = 1; i < 4; ++i)
			for (int j = 2; j < node_number; ++j) //2..nodenumber
			{
				set_node_probability(bn[i], j, 0, parent_state, 1);
				for (int k = 1; k < nums[j]; ++k)
				{
					set_node_probability(bn[i], j, k, parent_state, 0);
				}
			}
		

		//p(0|all - {1}) + p(1|all) probabilities
		//p(0|all - {1})
		//parent_state(mindegyik 0, 1, 2, 3...) => back()
		//backHS(0);
		//p(1|all);
		//backFE(0);
		
	//TESTING
		//test();
	//save network
		//serialize(bn, f);
		//deserialize(bn, f);
			preflop->write("bayes");
}
*/

int main(int argc, char* argv[])
{
	try
	{
		preflop = new BayesLearnPreflop();
		flop = new BayesLearnFlop();
		turn = new BayesLearnTurn();
		parseHands("d:\\pokerbot\\hh.txt");
		//parseHands("..\\tests\\resource\\bayes\\testhh.txt");
		
		preflop->write("preflopBayes");
		flop->write("flopBayes");
		turn->write("turnBayes");

		/*BAYES NETWORK
		//==============SET preflop network=======================
		bn[0].set_number_of_nodes(preflop_node_number);
		
		for (int j = 1; j < preflop_node_number; ++j)
		{
			bn[0].add_edge(j, 0);
		}
		set_node_num_values(bn[0], PREFLOP_HAND_STRENGTH, PREFLOP_HAND_STRENGTH_NUM);
		set_node_num_values(bn[0], PREFLOP_PLAYER_STACK_SIZE, PREFLOP_PLAYER_STACK_SIZE_NUM);
		set_node_num_values(bn[0], PREFLOP_PLAYER_BET_SIZE, PREFLOP_PLAYER_BET_SIZE_NUM); 
		set_node_num_values(bn[0], PREFLOP_PLAYER_LINE, PREFLOP_PLAYER_LINE_NUM);
		set_node_num_values(bn[0], PREFLOP_PLAYER_VPIP, PREFLOP_PLAYER_VPIP_NUM);
		set_node_num_values(bn[0], PREFLOP_PLAYER_PFR, PREFLOP_PLAYER_PFR_NUM);
		set_node_num_values(bn[0], PREFLOP_PLAYER_POZ, PREFLOP_PLAYER_POZ_NUM);

		for (int j = 1; j < preflop_node_number; ++j)
		{
			set_node_probability(bn[0], j, 0, parent_state, 1);
			for (int k = 1; k < preflop_nums[j]; ++k)
			{
				set_node_probability(bn[0], j, k, parent_state, 0);
			}
		}

		printf("Back begin\n");
		backPreflopHS(1);
		parent_state.clear();
		printf("Back end\n");

		std::ofstream myf;
		myf.open("bla");

		serialize(bn[0], myf);
		*/
	}
	catch (exception& e)
    {
        cout << "exception thrown: " << e.what() << endl;
        return 1;
    }
	return 0;
}