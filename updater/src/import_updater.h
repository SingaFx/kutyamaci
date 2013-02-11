#pragma once

#include "updater.h"
#include "OnGameParser.h"
#include "BWinPartyParser.h"
#include "HandHistoryUtils.h"
#include <set>
#include <map>

using namespace std;

class ImportUpdater : public Updater
{
	int nrofhands;
public:
	ImportUpdater(string folder, Database* database)
	{
		this->database = database;
		this->folder = folder;
		this->nrofhands = 0;
	}

	void run()
	{
		HANDLE hFind;
		WIN32_FIND_DATA ffd;

		string s = folder + "\\*";

		if ((hFind = FindFirstFile(s2ws(s).c_str(), &ffd)) == INVALID_HANDLE_VALUE)
		{
			printf("No files found!\n");
			DisplayErrorBox(TEXT("FindFirstFile"));
    		return;
		}

		do
		{
			if (ffd.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY)
			{
			}
			else
			{
				//_tprintf(TEXT("%s\n"), ffd.cFileName);

				//give it to the parser!
				//update the database according to the parser!
				char ch[1000];
				char DefChar = ' ';
				WideCharToMultiByte(CP_ACP,0,ffd.cFileName,-1,ch,1000,&DefChar,NULL);
				string s = folder + "\\";
				string s2 = ch;
				parseAndUpdate(s + s2);
			}
		}
		while (FindNextFile(hFind, &ffd) != 0);
		FindClose(hFind);
		cout << "Number of hands at showdown : " << this->nrofhands << endl;
	}

	void parseAndUpdate(string filename)
	{
		//cout << "Parsing file: " << filename << endl;

		set<string>* Players = new set<string>();
		map<string, double>* VPIP = new map<string, double>();
		map<string, double>* PFR = new map<string, double>();
		map<string, int>* aggrM = new map<string, int>();
		map<string, int>* passM = new map<string, int>();
		map<string, int>* handnr = new map<string, int>();

        ifstream fileHandle;

        BwinPartyParser parser(PARSER_TYPE::IMPORT_PARSER, fileHandle);
        parser.openFileForParsing(filename);
		vector<HandHistory> history =  parser.parse();
        parser.closeFileAfterParsing();
		nrofhands += HandHistoryUtils::exportToFile(history, "hh.txt");

		HandHistoryUtils::detailedExportToFile(history,"dhh.txt");

		for (int i = 0; i < history.size(); ++i)
		{
			if (database->isHand(history[i].getId())) continue;
				database->insertHand(history[i].getId());

			for (int j = 0; j < history[i].getPlayerHistories().size(); ++j)
			{
				PlayerHistory player = history[i].getPlayerHistories()[j];
				/*
				if (!database->isUser(player.getPlayerName()))
				{
					database->insertUser(player.getPlayerName());
				}
				*/
				if (Players->find(player.getPlayerName()) == Players->end())
				{
					Players->insert(player.getPlayerName());
				}
				bool raised = false;
				bool called = false;
				for (int k = 0; k < player.getPreflopAction().size(); ++k)
				{
					if (player.getPreflopAction()[k].getType() == 'r') raised = true;
					if (player.getPreflopAction()[k].getType() == 'c') called = true;
				}

				if (called || raised)
				{
					//database->setVPIP(100, player.getPlayerName());
					(*VPIP)[player.getPlayerName()] = ((*VPIP)[player.getPlayerName()] * (*handnr)[player.getPlayerName()] + 100) / ((*handnr)[player.getPlayerName()] + 1);
				}
				else
				{
					//database->setVPIP(0, player.getPlayerName());
					(*VPIP)[player.getPlayerName()] = ((*VPIP)[player.getPlayerName()] * (*handnr)[player.getPlayerName()]) / ((*handnr)[player.getPlayerName()] + 1);
				}

				if (raised)
				{
					//database->setPFR(100, player.getPlayerName());
					(*PFR)[player.getPlayerName()] = ((*PFR)[player.getPlayerName()] * (*handnr)[player.getPlayerName()] + 100) / ((*handnr)[player.getPlayerName()] + 1);
				}
				else
				{
					//database->setPFR(0, player.getPlayerName());
					(*PFR)[player.getPlayerName()] = ((*PFR)[player.getPlayerName()] * (*handnr)[player.getPlayerName()]) / ((*handnr)[player.getPlayerName()] + 1);
				}

				int aggr = 0;
				int pass = 0;
				for (int k = 0; k < player.getFlopAction().size(); ++k)
				{
					if (player.getFlopAction()[k].getType() == 'c') pass++;
					if (player.getFlopAction()[k].getType() == 'r') aggr++;
					//if (player.getFlopAction()[k].getType() == 'x') pass++;
				}
				for (int k = 0; k < player.getTurnAction().size(); ++k)
				{
					if (player.getTurnAction()[k].getType() == 'c') pass++;
					if (player.getTurnAction()[k].getType() == 'r') aggr++;
					//if (player.getFlopAction()[k].getType() == 'x') pass++;
				}
				for (int k = 0; k < player.getRiverAction().size(); ++k)
				{
					if (player.getRiverAction()[k].getType() == 'c') pass++;
					if (player.getRiverAction()[k].getType() == 'r') aggr++;
					//if (player.getFlopAction()[k].getType() == 'x') pass++;
				}

				//database->setAGGR(aggr, player.getPlayerName());
				//database->setPASS(pass, player.getPlayerName());
				(*aggrM)[player.getPlayerName()] += aggr;
				(*passM)[player.getPlayerName()] += pass;
				(*handnr)[player.getPlayerName()]++;
			}
		}

		for (set<string>::iterator it = Players->begin(); it != Players->end(); ++it)
		{
			if (!database->isUser(*it))
			{
				database->insertUser(*it);
			}
			database->setAGGR((*aggrM)[*it], *it);
			database->setPASS((*passM)[*it], *it);
			database->setPFR((*PFR)[*it], (*handnr)[*it], *it);
			database->setVPIP((*VPIP)[*it], (*handnr)[*it], *it);
			database->incHandnr((*handnr)[*it], *it);
		}

		delete Players;
		delete aggrM;
		delete passM;
		delete VPIP;
		delete PFR;
		delete handnr;
	}

	void parseAndUpdate2(string filename)
	{
		////cout << "Parsing file: " << filename << endl;

		//OnGameParser parser;
		//vector<HandHistory> history =  parser.parse();
		//HandHistoryUtils::exportToFile(history,"hh.txt");

		//for (int i = 0; i < history.size(); ++i)
		//{
		//	if (database->isHand(history[i].getId())) continue;
		//	database->insertHand(history[i].getId());

		//	for (int j = 0; j < history[i].getPlayerHistories().size(); ++j)
		//	{
		//		PlayerHistory player = history[i].getPlayerHistories()[j];

		//		if (!database->isUser(player.getPlayerName()))
		//		{
		//			database->insertUser(player.getPlayerName());
		//		}
		//		bool raised = false;
		//		bool called = false;
		//		for (int k = 0; k < player.getPreflopAction().size(); ++k)
		//		{
		//			if (player.getPreflopAction()[k].getType() == 'r') raised = true;
		//			if (player.getPreflopAction()[k].getType() == 'c') called = true;
		//		}

		//		if (called || raised)
		//			database->setVPIP(100, 1, player.getPlayerName());
		//		else
		//			database->setVPIP(0, 1, player.getPlayerName());

		//		if (raised)
		//			database->setPFR(100, 1, player.getPlayerName());
		//		else
		//			database->setPFR(0, 1, player.getPlayerName());

		//		int aggr = 0;
		//		int pass = 0;
		//		for (int k = 0; k < player.getFlopAction().size(); ++k)
		//		{
		//			if (player.getFlopAction()[k].getType() == 'c') pass++;
		//			if (player.getFlopAction()[k].getType() == 'r') aggr++;
		//			//if (player.getFlopAction()[k].getType() == 'x') pass++;
		//		}
		//		for (int k = 0; k < player.getTurnAction().size(); ++k)
		//		{
		//			if (player.getFlopAction()[k].getType() == 'c') pass++;
		//			if (player.getFlopAction()[k].getType() == 'r') aggr++;
		//			//if (player.getFlopAction()[k].getType() == 'x') pass++;
		//		}
		//		for (int k = 0; k < player.getRiverAction().size(); ++k)
		//		{
		//			if (player.getFlopAction()[k].getType() == 'c') pass++;
		//			if (player.getFlopAction()[k].getType() == 'r') aggr++;
		//			//if (player.getFlopAction()[k].getType() == 'x') pass++;
		//		}

		//		database->setAGGR(aggr, player.getPlayerName());
		//		database->setPASS(pass, player.getPlayerName());
		//		database->incHandnr(1, player.getPlayerName());
		//	}
		//}
	}
};
