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
	WQObject* CreateObject();
	void DeleteObject(WQObject* obj);
	void IncreaseReference(WQObject* obj);
	void ReleaseReference(WQObject* obj);
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
			ReleaseReference( it->second);
		}
		TemporaryVariables.sort();
		TemporaryVariables.unique();
		list<WQObject*>::iterator tempit = TemporaryVariables.begin();
		for (; tempit != TemporaryVariables.end(); tempit++)
		{
			if ((*tempit)->ReferenceCounter < 1)
			{
				//if this object is referencing other objects object-obj2, also release obj2
				if ((*tempit)->IsReference)
					ReleaseReference((*tempit)->Reference);
				//delete *tempit;
			}
		}

		//bring the rest to the parent environment or delete them

		tempit = TemporaryVariables.begin();
		for (; tempit != TemporaryVariables.end(); tempit++)
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

	Environment* Parent;
	map<string, WQObject*> Variables;

	list<WQObject*> TemporaryVariables;
};


#endif