
using namespace std;
#ifndef GLOBALFUNCTIONS_H
#define GLOBALFUNCTIONS_H
#include "WQState.h"
#include "WebRequest.h"
#include "WebResponse.h"
#include "WQLibrary.h"
#include <chrono>
#include <list>
#include <map>
#include <ctime>

typedef void(*WQFunction)(WQState*);
class WQGlobalFunctions
{
public:
	WQGlobalFunctions()
	{
		Functions = new map< string, WQFunction >;
		LoadFunctions();
	}
	~WQGlobalFunctions()
	{
		delete Functions;
	}
	void Add(string, WQFunction);
	void LoadFunctions();
	WQFunction Get(string*);
	map<string, WQFunction>* Functions;
//private:
	

	//void (*func)(WQState*);
};


#endif