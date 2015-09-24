#include <map>
#include <list>
#include "GlobalFunctions.h"
#include "WQObject.h"
#ifndef ENVIRONMENT_H
#define ENVIRONMENT_H
class Environment
{
public:
	WQObject* GetVariable(string& name);
	void AddVariable(string& name, WQObject* obj);
	void SetVariable(string& name, WQObject* obj);
	void SetVariable(WQObject* oldobj, WQObject* newobj);
	WQObject* CreateVariable(string& name);

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
	map<string, WQObject*> Variables;
	map<string, WQObject*> GarbageVariables;
	GlobalFunctions* Functions;
};


#endif