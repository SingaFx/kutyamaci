#pragma once

#include "handHistoryParser.h"
#include "rangeFunctions.h"
#include "bayesUtils.h"

class BayesPreflop
{
public:
	enum preflop
	{
		PREFLOP_HAND_STRENGTH = 0,
		PREFLOP_PLAYER_STACK_SIZE = 1,
		PREFLOP_PLAYER_BET_SIZE = 2,
		PREFLOP_PLAYER_LINE = 3,
		PREFLOP_PLAYER_VPIP = 4,
		PREFLOP_PLAYER_PFR = 5,
		PREFLOP_PLAYER_POZ = 6
	};

	static const int PREFLOP_HAND_STRENGTH_NUM = 170;
	static const int PREFLOP_PLAYER_STACK_SIZE_NUM = 4;
	static const int PREFLOP_PLAYER_BET_SIZE_NUM = 10;
	static const int PREFLOP_PLAYER_LINE_NUM = 3;
	static const int PREFLOP_PLAYER_VPIP_NUM = 5;
	static const int PREFLOP_PLAYER_PFR_NUM = 5;
	static const int PREFLOP_PLAYER_POZ_NUM = 6;

	//static const int FOLD_EQUITY_NUM = 5;

	static const int preflop_node_number = 7;
	int preflop_nums[preflop_node_number];

	int probabilityPreflop[PREFLOP_HAND_STRENGTH_NUM][PREFLOP_PLAYER_STACK_SIZE_NUM][PREFLOP_PLAYER_BET_SIZE_NUM][PREFLOP_PLAYER_LINE_NUM][PREFLOP_PLAYER_VPIP_NUM][PREFLOP_PLAYER_PFR_NUM][PREFLOP_PLAYER_POZ_NUM];
	int totalPreflop[PREFLOP_PLAYER_STACK_SIZE_NUM][PREFLOP_PLAYER_BET_SIZE_NUM][PREFLOP_PLAYER_LINE_NUM][PREFLOP_PLAYER_VPIP_NUM][PREFLOP_PLAYER_PFR_NUM][PREFLOP_PLAYER_POZ_NUM];
	
	int probabilityFE[PREFLOP_PLAYER_STACK_SIZE_NUM][PREFLOP_PLAYER_BET_SIZE_NUM][PREFLOP_PLAYER_LINE_NUM][PREFLOP_PLAYER_VPIP_NUM][PREFLOP_PLAYER_PFR_NUM][PREFLOP_PLAYER_POZ_NUM];
	int totalFE[PREFLOP_PLAYER_STACK_SIZE_NUM][PREFLOP_PLAYER_BET_SIZE_NUM][PREFLOP_PLAYER_LINE_NUM][PREFLOP_PLAYER_VPIP_NUM][PREFLOP_PLAYER_PFR_NUM][PREFLOP_PLAYER_POZ_NUM];

};
class BayesFlop
{
public:
	enum node
	{
		HAND_STRENGTH = 0,
		FOLD_EQUITY = 1,
		POT_SIZE = 2,
		PLAYER_STACK_SIZE = 3,
		PLAYER_BET_SIZE = 4,
		PLAYER_LINE = 5,
		PLAYER_VPIP = 6,
		PLAYER_PFR = 7,
		PLAYER_AF = 8,
		BOARD_TYPE = 9,
	};

	static const int HAND_STRENGTH_NUM = 9;
	static const int FOLD_EQUITY_NUM = 1;
	static const int POT_SIZE_NUM = 7; 
	static const int PLAYER_STACK_SIZE_NUM = 4;
	static const int PLAYER_BET_SIZE_NUM = 8; 
	static const int PLAYER_LINE_NUM = 3;
	static const int PLAYER_VPIP_NUM = 5;
	static const int PLAYER_PFR_NUM = 5;
	static const int PLAYER_AF_NUM = 5;

	static const int node_number = 9;
	int nums[node_number];

	int probabilityHS[HAND_STRENGTH_NUM][POT_SIZE_NUM][PLAYER_STACK_SIZE_NUM][PLAYER_BET_SIZE_NUM][PLAYER_LINE_NUM][PLAYER_VPIP_NUM][PLAYER_PFR_NUM][PLAYER_AF_NUM];
	int probabilityFE[FOLD_EQUITY_NUM][POT_SIZE_NUM][PLAYER_STACK_SIZE_NUM][PLAYER_BET_SIZE_NUM][PLAYER_LINE_NUM][PLAYER_VPIP_NUM][PLAYER_PFR_NUM][PLAYER_AF_NUM];
	int totalS[POT_SIZE_NUM][PLAYER_STACK_SIZE_NUM][PLAYER_BET_SIZE_NUM][PLAYER_LINE_NUM][PLAYER_VPIP_NUM][PLAYER_PFR_NUM][PLAYER_AF_NUM];
	int totalFE[POT_SIZE_NUM][PLAYER_STACK_SIZE_NUM][PLAYER_BET_SIZE_NUM][PLAYER_LINE_NUM][PLAYER_VPIP_NUM][PLAYER_PFR_NUM][PLAYER_AF_NUM];
};
class BayesTurn
{
public:
	enum node
	{
		HAND_STRENGTH = 0,
		FOLD_EQUITY = 1,
		POT_SIZE = 2,
		PLAYER_STACK_SIZE = 3,
		PLAYER_BET_SIZE = 4,
		PLAYER_LINE = 5,
		PLAYER_VPIP = 6,
		PLAYER_PFR = 7,
		PLAYER_AF = 8,
		FLOP_POT_SIZE = 9
	};

	static const int HAND_STRENGTH_NUM = 9;
	static const int FOLD_EQUITY_NUM = 1;
	static const int POT_SIZE_NUM = 8; 
	static const int PLAYER_STACK_SIZE_NUM = 4;
	static const int PLAYER_BET_SIZE_NUM = 8;
	static const int PLAYER_LINE_NUM = 3; 
	static const int PLAYER_VPIP_NUM = 5;
	static const int PLAYER_PFR_NUM = 5;
	static const int PLAYER_AF_NUM = 5;
	static const int FLOP_POT_SIZE_NUM = 6;

	static const int node_number = 10;
	int nums[node_number];

	int probabilityHS[HAND_STRENGTH_NUM][POT_SIZE_NUM][PLAYER_STACK_SIZE_NUM][PLAYER_BET_SIZE_NUM][PLAYER_LINE_NUM][PLAYER_VPIP_NUM][PLAYER_PFR_NUM][PLAYER_AF_NUM][FLOP_POT_SIZE_NUM];
	int probabilityFE[FOLD_EQUITY_NUM][POT_SIZE_NUM][PLAYER_STACK_SIZE_NUM][PLAYER_BET_SIZE_NUM][PLAYER_LINE_NUM][PLAYER_VPIP_NUM][PLAYER_PFR_NUM][PLAYER_AF_NUM][FLOP_POT_SIZE_NUM];
	int totalS[POT_SIZE_NUM][PLAYER_STACK_SIZE_NUM][PLAYER_BET_SIZE_NUM][PLAYER_LINE_NUM][PLAYER_VPIP_NUM][PLAYER_PFR_NUM][PLAYER_AF_NUM][FLOP_POT_SIZE_NUM];
	int totalFE[POT_SIZE_NUM][PLAYER_STACK_SIZE_NUM][PLAYER_BET_SIZE_NUM][PLAYER_LINE_NUM][PLAYER_VPIP_NUM][PLAYER_PFR_NUM][PLAYER_AF_NUM][FLOP_POT_SIZE_NUM];
};
class BayesRiver
{
public:
	enum node
	{
		HAND_STRENGTH = 0,
		FOLD_EQUITY = 1,
		POT_SIZE = 2,
		PLAYER_STACK_SIZE = 3,
		PLAYER_BET_SIZE = 4,
		PLAYER_LINE = 5,
		PLAYER_VPIP = 6,
		PLAYER_PFR = 7,
		PLAYER_AF = 8,
		FLOP_POT_SIZE = 9
	};

	static const int HAND_STRENGTH_NUM = 9;
	static const int FOLD_EQUITY_NUM = 1;
	static const int POT_SIZE_NUM = 8; 
	static const int PLAYER_STACK_SIZE_NUM = 4;
	static const int PLAYER_BET_SIZE_NUM = 8;
	static const int PLAYER_LINE_NUM = 3; 
	static const int PLAYER_VPIP_NUM = 5;
	static const int PLAYER_PFR_NUM = 5;
	static const int PLAYER_AF_NUM = 5;
	static const int FLOP_POT_SIZE_NUM = 6;

	static const int node_number = 10;
	int nums[node_number];

	int probabilityHS[HAND_STRENGTH_NUM][POT_SIZE_NUM][PLAYER_STACK_SIZE_NUM][PLAYER_BET_SIZE_NUM][PLAYER_LINE_NUM][PLAYER_VPIP_NUM][PLAYER_PFR_NUM][PLAYER_AF_NUM][FLOP_POT_SIZE_NUM];
	int probabilityFE[FOLD_EQUITY_NUM][POT_SIZE_NUM][PLAYER_STACK_SIZE_NUM][PLAYER_BET_SIZE_NUM][PLAYER_LINE_NUM][PLAYER_VPIP_NUM][PLAYER_PFR_NUM][PLAYER_AF_NUM][FLOP_POT_SIZE_NUM];
	int totalS[POT_SIZE_NUM][PLAYER_STACK_SIZE_NUM][PLAYER_BET_SIZE_NUM][PLAYER_LINE_NUM][PLAYER_VPIP_NUM][PLAYER_PFR_NUM][PLAYER_AF_NUM][FLOP_POT_SIZE_NUM];
	int totalFE[POT_SIZE_NUM][PLAYER_STACK_SIZE_NUM][PLAYER_BET_SIZE_NUM][PLAYER_LINE_NUM][PLAYER_VPIP_NUM][PLAYER_PFR_NUM][PLAYER_AF_NUM][FLOP_POT_SIZE_NUM];
};