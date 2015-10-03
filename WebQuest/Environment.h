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
		for (; it != Variables.end(); it++)
		{
			//decrement reference counter
			it->second->ReferenceCounter--;
		}
		TemporaryVariables.sort();
		TemporaryVariables.unique();
		list<WQObject*>::iterator tempit = TemporaryVariables.begin();
		for (; tempit != TemporaryVariables.end(); tempit++)
		{
			if ((*tempit)->ReferenceCounter<1)
				delete *tempit;
			else
			{
				//something else is referencing this object ,don't delete it but bring it to the parent level
				if (this->Parent != NULL)
				{
					this->Parent->TemporaryVariables.push_back(*tempit);
				}
				else
					//it's already the top level of envrionment,means the end of the program, just delete it anyways
					delete *tempit;
			}
		}
	}

	Environment* Parent;
	map<string, WQObject*> Variables;

	list<WQObject*> TemporaryVariables;
};


#endif