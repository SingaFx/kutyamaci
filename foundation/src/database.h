#pragma once
#pragma comment(lib, "libmysql")

#include <string>
#include <iostream>
#include <sstream>
#include <Windows.h>
#include <mysql.h>

using namespace std;

class Database
{
	struct connection_details
	{
		char *server;
		char *user;
		char *password;
		char *database;
	};

	FILE* logfile;
	MYSQL* conn;

public:
	Database(char* server, char* user, char* password, char* database)
	{
		connection_details mysql_details;
		/*
		mysql_details.server = "192.168.1.102";
		mysql_details.user = "root";
		mysql_details.password = "masterlord";
		mysql_details.database = "kutya";
		*/
		mysql_details.server = server;
		mysql_details.user = user;
		mysql_details.password = password;
		mysql_details.database = database;
		conn = mysql_connection_setup(mysql_details);

		logfile = fopen("databaselog", "w");
	}

	string query(string s)
	{
		string result="";

		MYSQL_RES *res;
		MYSQL_ROW row;
		res = mysql_perform_query(conn, (char *) s.c_str());
		if (res == NULL)
		{
			mysql_free_result(res);
			return result;
		}
		/*
		while ((row = mysql_fetch_row(res))!=NULL)
		{
			printf("%s\n", row[0]);
			//result.append(row[0]);
		}
		*/
		row = mysql_fetch_row(res);
		if (row == NULL)
		{
			mysql_free_result(res);
			return result;
		}
		result = row[0];
		mysql_free_result(res);

		return result;
	}

	bool isHand(string name)
	{
		string q = "SELECT * FROM hands WHERE id='" + name + "'";
		string result = query(q);

		return result.size() > 0;
	}

	void insertHand(string hand)
	{
		query("INSERT INTO hands(id) VALUES ('" + hand + "')");
	}

	bool isUser(string name)
	{
		string q = "SELECT * FROM user WHERE name='" + name + "'";
		string result = query(q);

		return result.size() > 0;
	}
	void insertUser(string name)
	{
		query("INSERT INTO user(name, handnr, VPIP, PFR, aggr, pass) VALUES ('" + name + "','0','0','0','0','0')");
	}

	//TODO:ha nincsen ilyen user exception!
	int getHandnr(string name)
	{
		string q = "SELECT handnr FROM user WHERE name='" + name + "'";
		string result = query(q);

		istringstream input(result);
		int handnr;
		input >> handnr;
		return handnr;
	}
	double getVPIP(string name)
	{
		string q = "SELECT VPIP FROM user WHERE name='" + name + "'";
		string result = query(q);

		istringstream input(result);
		double vpip;
		input >> vpip;
		if (vpip > 100 || vpip < 0)
		{
			printf("ERROR VPIP\n");
			fprintf(logfile,"DATABASE ERROR VPIP: %lf\n", vpip);
			fprintf(logfile,"DATABASE q: %s\n", q.c_str());
			fprintf(logfile,"DATABASE result: %s\n", result.c_str());
			fprintf(logfile,"NAME: %s\n", name.c_str());
			//exit(1);
		}
		return vpip;
	}
	double getPFR(string name)
	{
		string q = "SELECT PFR FROM user WHERE name='" + name + "'";
		string result = query(q);

		istringstream input(result);
		double pfr;
		input >> pfr;
		return pfr;
	}

	int getPASS(string name)
	{
		string q = "SELECT pass FROM user WHERE name='" + name + "'";
		string result = query(q);

		istringstream input(result);
		int pass;
		input >> pass;
		return pass;
	}

	int getAGGR(string name)
	{
		string q = "SELECT aggr FROM user WHERE name='" + name + "'";
		string result = query(q);

		istringstream input(result);
		int aggr;
		input >> aggr;
		return aggr;
	}

	double getAF(string name)
	{
		int pass = getPASS(name);
		int aggr = getAGGR(name);

		if (pass == 0) return -2;

		double af = (double)aggr/pass;
		return af;
	}

	void incHandnr(int nr, string name)
	{
		int handnr = getHandnr(name);
		handnr += nr;
		stringstream s;
		s << handnr;
		string hand;
		s >> hand;
		query("UPDATE user SET handnr="+hand+" WHERE name='"+name+"'");
	}
	void setVPIP(double VPIP, int newhandnr, string name)
	{
		double oldVPIP = getVPIP(name);

		int handnr = getHandnr(name);
		oldVPIP = (oldVPIP * handnr + VPIP * newhandnr) / (handnr + newhandnr);
		stringstream s;
		s << oldVPIP;
		string vpip_str;
		s >> vpip_str;
		query("UPDATE user SET VPIP="+vpip_str+" WHERE name='"+name+"'");
	}
	void setPFR(double PFR, int newhandnr, string name)
	{
		double oldPFR = getPFR(name);
		int handnr = getHandnr(name);
		oldPFR = (oldPFR * handnr + PFR * newhandnr) / (handnr + newhandnr);
		stringstream s;
		s << oldPFR;
		string pfr_str;
		s >> pfr_str;
		query("UPDATE user SET PFR="+pfr_str+" WHERE name='"+name+"'");
	}

	//@deprecated
	void setAF(double AF, string name)
	{
		if (AF == 0)
		{
			int pass = getPASS(name);
			pass = pass + 1;
			stringstream s;
			s << pass;
			string pass_str;
			s >> pass_str;
			query("UPDATE user SET pass="+pass_str+" WHERE name='"+name+"'");
		}
		else
		{
			int aggr = getAGGR(name);
			aggr = aggr + 1;
			stringstream s;
			s << aggr;
			string aggr_str;
			s >> aggr_str;
			query("UPDATE user SET aggr="+aggr_str+" WHERE name='"+name+"'");
		}
	}

	void setAGGR(int aggrU, string name)
	{
			int aggr = getAGGR(name);
			aggr = aggr + aggrU;
			stringstream s;
			s << aggr;
			string aggr_str;
			s >> aggr_str;
			query("UPDATE user SET aggr="+aggr_str+" WHERE name='"+name+"'");
	}

	void setPASS(int passU, string name)
	{
			int pass = getPASS(name);
			pass = pass + passU;
			stringstream s;
			s << pass;
			string pass_str;
			s >> pass_str;
			query("UPDATE user SET pass="+pass_str+" WHERE name='"+name+"'");
	}

	void test_database()
	{
		printf("%d\n", this->getHandnr("bla"));
		printf("%lf\n", this->getVPIP("bla"));
	}

private:
	MYSQL* mysql_connection_setup(struct connection_details mysql_details)
	{
		 // first of all create a mysql instance and initialize the variables within
		MYSQL *connection = mysql_init(NULL);

		// connect to the database with the details attached.
		if (!mysql_real_connect(connection,mysql_details.server, mysql_details.user, mysql_details.password, mysql_details.database, 0, NULL, 0)) {
		  printf("Conection error : %s\n", mysql_error(connection));
		  exit(1);
		}
		return connection;
	}

	MYSQL_RES* mysql_perform_query(MYSQL *connection, char *sql_query)
	{
	   // send the query to the database
	   if (mysql_query(connection, sql_query))
	   {
		  printf("MySQL query error : %s\n", mysql_error(connection));
		  exit(1);
	   }

	   return mysql_use_result(connection);
	}
};