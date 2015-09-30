#ifndef ENVIRONMENT_H
#define ENVIRONMENT_H
#include <map>
#include <list>
#include "WQObject.h"

class Environment
{
public:
	WQObject* GetVariable(string& name);
	WQObject* SearchVariableInCurrentEvironment(string& name);
	void AddVariable(string& name, WQObject* obj);
	void SetVariable(string& name, WQObject* obj);
	WQObject* CreateVariable(string& name);

	Environment()
	{
		Parent = NULL;

	}
	~Environment()
	{

		map<string, WQObject*>::iterator it = Variables.begin();
		//for (; it != Variables.end(); it++)
		//{
		//	TemporaryVariables.push_back(it->second);
		//}
		TemporaryVariables.sort();
		TemporaryVariables.unique();
		list<WQObject*>::iterator tempit = TemporaryVariables.begin();
		for (; tempit != TemporaryVariables.end(); tempit++)
		{
				delete *tempit;
		}
	}

	Environment* Parent;
	map<string, WQObject*> Variables;

	list<WQObject*> TemporaryVariables;
};


#endif