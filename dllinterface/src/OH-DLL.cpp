#define WHUSER_EXPORTS 

#include "OH-DLL.h"
#include <windows.h>
#include "logger.h"
#include <string>
using namespace std;
///////////////////////////////////// 
//card macros 
#define RANK(c)	         ((c>>4)&0x0f) 
#define SUIT(c)          ((c>>0)&0x0f) 
#define ISCARDBACK(c)	 (c==0xff) 
#define ISUNKNOWN(c)	 (c==0) 
///////////////////////////////////// 

//////////////////////////////////// 
//consecutive states 
holdem_state    m_holdem_state[256]; 
unsigned char   m_ndx; 
////////////////////////////////////

//////////////////////////////////// 
//versus list & prwin
phl1k_t m_phl1k;
pp13 prw1326;
////////////////////////////////////

//////////////////////////////////// 
//WH symbols
pfgws_t m_pget_winholdem_symbol;
////////////////////////////////////

double gws(int chair, const char* name, bool& iserr)
{ 
	return (*m_pget_winholdem_symbol)(chair,name,iserr); 
} 

double gws(const char* name)
{ 
	bool iserr; 
	int mychair = (int)gws(0,"userchair",iserr); 
	return gws(mychair,name,iserr); 
}

bool hlset( int rank0, int rank1, int listnum, bool onoff )
{
   return ((*m_phl1k)[listnum][rank0-2][rank1-2] = onoff);
}

bool hlget( int rank0, int rank1, int listnum )
{
   return ((*m_phl1k)[listnum][rank0-2][rank1-2]);
}

double process_query(const char* pquery)
{
    //Logger& logger = Logger::getLogger(DLL_INTERFACE_LOGGER);            
    //logger.logExp(string("[Processing query] : ").append(pquery).c_str());

	if(pquery==NULL)
    {
		return 0;
    }
	//example
	if(strcmp(pquery,"dll$test")==0)
    {
		return 101;
    }

	return 0;

}

double process_state(holdem_state* pstate)
{
    Logger& logger = Logger::getLogger(DLL_INTERFACE_LOGGER);
    logger.logExp("[Processing state] : ");

    double balance0 = gws("balance0");
    double balance1 = gws("balance1");
    double balance2 = gws("balance2");
    double balance3 = gws("balance3");
    double balance4 = gws("balance4");
    double balance5 = gws("balance5");

    logger.logExp("-> balance0 : ", balance0);
    logger.logExp("-> balance1 : ", balance1);
    logger.logExp("-> balance2 : ", balance2);
    logger.logExp("-> balance3 : ", balance3);
    logger.logExp("-> balance4 : ", balance4);
    logger.logExp("-> balance5 : ", balance5);

    char buffer[100];
    for (int idx = 0; idx < 5; ++idx)
    {        
        holdem_player& hp = pstate->m_player[idx];

        
        if (hp.m_name_known & 0x01)
        {
            sprintf(buffer, "holdem_player[%d] -> player name : ", idx);
            logger.logExp(buffer, hp.m_name);
        }
        if (hp.m_balance_known & 0x01 )
        {
            sprintf(buffer, "holdem_player[%d] -> balance : ", idx);
            logger.logExp(buffer, hp.m_balance);
        }

        sprintf(buffer, "holdem_player[%d] -> currentbet : ", idx);
        logger.logExp(buffer, hp.m_currentbet);
        //logger.logExp(buffer, hp.m_cards);
        sprintf(buffer, "holdem_player[%d] -> cards : ", idx);
        char rank = RANK(hp.m_cards[0]);
        char suit = SUIT(hp.m_cards[0]);
        char card[3];
        sprintf(card, "%c%c", rank, suit);
        logger.logExp(buffer, card);
    }

	if (pstate!=NULL)
    {
      m_holdem_state[ (++m_ndx)&0xff ] = *pstate;

      
      // logging the state      
      logger.logExp("-> m_title : ", pstate->m_title);
      logger.logExp("-> m_title : ", pstate->m_pot[0]);
      //logger.logExp(string("-> m_title : ").append(pstate->m_title).c_str());
      //logger.logExp(string("-> m_pot[0] : ").append(pstate->m_pot[0]).c_str());
      //logger.logExp(string("-> m_cards : ").append(pstate->m_cards[0]).c_str());
    }

	return 0;

}
///////////////////////////////////////////////////// 
//WINHOLDEM RUNTIME ENTRY POINT 
///////////////////////////////////////////////////// 
WHUSER_API double process_message(const char* pmessage, const void* param){

	if (pmessage == NULL)
    {
		return 0;
	}

	if (param == NULL)
    {
		return 0;
	}
	if(strcmp(pmessage,"state") == 0)
    {
		return process_state( (holdem_state*)param );
	}
	if(strcmp(pmessage,"phl1k") == 0){   
		m_phl1k = (phl1k_t)param;
		return 0;
	}
	if(strcmp(pmessage,"prw1326") == 0)
    {
		prw1326 = (pp13)param;
		return 0;
	}
	if(strcmp(pmessage,"query") == 0)
    { 
		return process_query((const char*)param);
	}
	if(strcmp(pmessage,"pfgws") == 0)
    {
		m_pget_winholdem_symbol = (pfgws_t)param;
		return 0;
	}

	return 0;

}
///////////////////////////////// 
//DLLMAIN 
///////////////////////////////// 
BOOL APIENTRY DllMain( HANDLE hModule, DWORD ul_reason_for_call, LPVOID lpReserved){

	switch(ul_reason_for_call)
    {
		case DLL_PROCESS_ATTACH:
			break; 
		case DLL_THREAD_ATTACH:
			break; 
		case DLL_THREAD_DETACH:
			break; 
		case DLL_PROCESS_DETACH:
			break;
	}

	return TRUE;

}