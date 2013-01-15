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
    Logger& logger = Logger::getLogger();            
    logger.logExp(string("[Processing query] : ").append(pquery).c_str());

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
    Logger& logger = Logger::getLogger();
    logger.logExp("[Processing state] : ");
	if (pstate!=NULL)
    {
      m_holdem_state[ (++m_ndx)&0xff ] = *pstate;

      // logging the state      
      logger.logExp(string("-> m_title : ").append(pstate->m_title).c_str());
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