#pragma once

#include "updater.h"

//#include <boost/thread.hpp>
//#include <boost/date_time.hpp>

class LiveUpdater : public Updater
{
public:
	LiveUpdater(string folder, Database* database)
	{
		this->database = database;
		this->folder = folder;
	}

	//threadworker
	void worker()
	{
		//give it to the parser!
		//update the database according to the parser!
	}

	void  run()
	{
		//=> DELETE all the history files in the folder!! => zip them, and copy to a folder?
		//run a batch file for this?

		//get importing path
			//1a make a new thread for every history file there
		//update database
		while(1)
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
					_tprintf(TEXT("%s\n"), ffd.cFileName);
					//create a thread!
				}
			}
			while (FindNextFile(hFind, &ffd) != 0);
			FindClose(hFind);
			//some sleep
		}
	}
	/*
	void parseAndUpdate(string filename)
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
					database->setVPIP(100, player.player.name);
				else
					database->setVPIP(0, player.player.name);

				if (raised)
					database->setPFR(100, player.player.name);
				else
					database->setPFR(0, player.player.name);

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
			}
		}
	}
	*/
};