#pragma once
#include "Action.h"
#include "rangeFunctions.h"
#include "currentGameInfo.h"

const string openRaiseUTG = "77,88,99,TT,JJ,QQ,KK,AA,AQ,AK,KQs,";
const string openRaiseMP = "77,88,99,TT,JJ,QQ,KK,AA,AJ,AQ,AK,KQ,";
const string openRaiseCOTight = "55,66,77,88,99,TT,JJ,QQ,KK,AA,AT,AJ,AQ,AK,KJ,KQ,QJ,JTs,QTs,KTs,A9s";
const string openRaiseCOLoose = "22,33,44,55,66,77,88,99,TT,JJ,QQ,KK,AA,A2,A3,A4,A5,A6,A7,A8,A9,AT,AJ,AQ,AK,KT,KJ,KQ,QT,QJ,JT,K2s,K3s,K4s,K5s,K6s,K7s,K8s,K9s,Q9s,J9s,T9s,98s,87s,76s,65s";
const string openRaiseBU = "22,33,44,55,66,77,88,99,TT,JJ,QQ,KK,AA,A2,A3,A4,A5,A6,A7,A8,A9,AT,AJ,AQ,AK,K2,K3,K4,K5,K6,K7,K8,K9,KT,KJ,KQ,QT,QJ,JT,Q7,Q8,Q9,J7,J8,J9,T7,T8,T9,97,98,87,Q2s,Q3s,Q4s,Q5s,Q6s,76,65,54,";

class SimpleDecision
{
private:

	static char calculateDecision(CurrentGameInfo&);
	static double calculateBetSize(CurrentGameInfo&);
	set<Hand> getRange(string&);
public:
	//sajat handje erdekli csak
	static Action makeDecision(CurrentGameInfo&);
};