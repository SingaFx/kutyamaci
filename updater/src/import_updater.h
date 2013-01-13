#pragma once

#include "updater.h"
#include "handHistoryParser.h"
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

		OngameParser parser;
		vector<HandHistory> history =  parser.parse(filename);
		nrofhands += HandHistoryUtils::exportToFile(history,"hh.txt");
		return;
		//HandHistoryUtils::detailedExportToFile(history,"dhh.txt");

		for (int i = 0; i < history.size(); ++i)
		{
			if (database->isHand(history[i].id)) continue;
				database->insertHand(history[i].id);

			for (int j = 0; j < history[i].players.size(); ++j)
			{
				PlayerHistory player = history[i].players[j];
				/*
				if (!database->isUser(player.player.name))
				{
					database->insertUser(player.player.name);
				}
				*/
				if (Players->find(player.player.name) == Players->end())
				{
					Players->insert(player.player.name);
				}
				bool raised = false;
				bool called = false;
				for (int k = 0; k < player.preflopAction.size(); ++k)
				{
					if (player.preflopAction[k].type == 'r') raised = true;
					if (player.preflopAction[k].type == 'c') called = true;
				}
				
				if (called || raised)
				{
					//database->setVPIP(100, player.player.name);
					(*VPIP)[player.player.name] = ((*VPIP)[player.player.name] * (*handnr)[player.player.name] + 100) / ((*handnr)[player.player.name] + 1);
				}
				else
				{
					//database->setVPIP(0, player.player.name);
					(*VPIP)[player.player.name] = ((*VPIP)[player.player.name] * (*handnr)[player.player.name]) / ((*handnr)[player.player.name] + 1);
				}

				if (raised)
				{
					//database->setPFR(100, player.player.name);
					(*PFR)[player.player.name] = ((*PFR)[player.player.name] * (*handnr)[player.player.name] + 100) / ((*handnr)[player.player.name] + 1);
				}
				else
				{
					//database->setPFR(0, player.player.name);
					(*PFR)[player.player.name] = ((*PFR)[player.player.name] * (*handnr)[player.player.name]) / ((*handnr)[player.player.name] + 1);
				}

				int aggr = 0;
				int pass = 0;
				for (int k = 0; k < player.flopAction.size(); ++k)
				{
					if (player.flopAction[k].type == 'c') pass++;
					if (player.flopAction[k].type == 'r') aggr++;
					//if (player.flopAction[k].type == 'x') pass++;
				}
				for (int k = 0; k < player.turnAction.size(); ++k)
				{
					if (player.flopAction[k].type == 'c') pass++;
					if (player.flopAction[k].type == 'r') aggr++;
					//if (player.flopAction[k].type == 'x') pass++;
				}
				for (int k = 0; k < player.riverAction.size(); ++k)
				{
					if (player.flopAction[k].type == 'c') pass++;
					if (player.flopAction[k].type == 'r') aggr++;
					//if (player.flopAction[k].type == 'x') pass++;
				}

				//database->setAGGR(aggr, player.player.name);
				//database->setPASS(pass, player.player.name);
				(*aggrM)[player.player.name] += aggr;
				(*passM)[player.player.name] += pass;
				(*handnr)[player.player.name]++;
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
		//cout << "Parsing file: " << filename << endl;

		OngameParser parser;
		vector<HandHistory> history =  parser.parse(filename);
		HandHistoryUtils::exportToFile(history,"hh.txt");

		for (int i = 0; i < history.size(); ++i)
		{
			if (database->isHand(history[i].id)) continue;
			database->insertHand(history[i].id);

			for (int j = 0; j < history[i].players.size(); ++j)
			{
				PlayerHistory player = history[i].players[j];

				if (!database->isUser(player.player.name))
				{
					database->insertUser(player.player.name);
				}
				bool raised = false;
				bool called = false;
				for (int k = 0; k < player.preflopAction.size(); ++k)
				{
					if (player.preflopAction[k].type == 'r') raised = true;
					if (player.preflopAction[k].type == 'c') called = true;
				}
				
				if (called || raised)
					database->setVPIP(100, 1, player.player.name);
				else
					database->setVPIP(0, 1, player.player.name);

				if (raised)
					database->setPFR(100, 1, player.player.name);
				else
					database->setPFR(0, 1, player.player.name);

				int aggr = 0;
				int pass = 0;
				for (int k = 0; k < player.flopAction.size(); ++k)
				{
					if (player.flopAction[k].type == 'c') pass++;
					if (player.flopAction[k].type == 'r') aggr++;
					//if (player.flopAction[k].type == 'x') pass++;
				}
				for (int k = 0; k < player.turnAction.size(); ++k)
				{
					if (player.flopAction[k].type == 'c') pass++;
					if (player.flopAction[k].type == 'r') aggr++;
					//if (player.flopAction[k].type == 'x') pass++;
				}
				for (int k = 0; k < player.riverAction.size(); ++k)
				{
					if (player.flopAction[k].type == 'c') pass++;
					if (player.flopAction[k].type == 'r') aggr++;
					//if (player.flopAction[k].type == 'x') pass++;
				}

				database->setAGGR(aggr, player.player.name);
				database->setPASS(pass, player.player.name);
				database->incHandnr(1, player.player.name);
			}
		}
	}
};

