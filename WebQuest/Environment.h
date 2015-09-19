#include <map>
#include <list>
#include "WQObject.h"
#include "GlobalFunctions.h"
#ifndef ENVIRONMENT_H
#define ENVIRONMENT_H
class Environment
{
public:
	Environment()
	{
		Parent = NULL;
		Functions = new GlobalFunctions;
	}
	~Environment()
	{
		delete Functions;
	}
	Environment* Parent;
	map<string, WQObject> Variables;
	GlobalFunctions* Functions;
};
#endif