
using namespace std;
#ifndef GLOBALFUNCTIONS_H
#define GLOBALFUNCTIONS_H
#include "WQState.h"
#include "WebRequest.h"
#include "WebResponse.h"
#include <chrono>
#include <list>
#include <map>
#include <ctime>
class WQState;
typedef void(*WQFunction)(WQState*);
class WQState;
class GlobalFunctions
{
public:
	GlobalFunctions()
	{
		Functions = new map< string, WQFunction >;
		LoadFunctions();
	}
	~GlobalFunctions()
	{
		delete Functions;
	}
	void Add(string, WQFunction);
	void LoadFunctions();
	WQFunction Get(string*);
private:
	map<string,WQFunction>* Functions;

	//void (*func)(WQState*);
};


#endif