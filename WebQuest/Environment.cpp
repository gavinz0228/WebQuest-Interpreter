#include "Environment.h"
WQObject* Environment::GetVariable(string& name)
{
	WQObject* var = SearchVariableInCurrentEvironment(name);
	if (var!=NULL)
	{
		return var;
	}
	else
	{	//if it's not in the current environment
		//search the the parent environment
		if (Parent != NULL)
		{
			Environment* tempevnt = Parent;
			while (tempevnt != NULL)
			{
				WQObject* tempobj = tempevnt->SearchVariableInCurrentEvironment(name);
				if (tempobj != NULL)
					return tempobj;
				tempevnt =tempevnt-> Parent;
			}
			return NULL;
		}
		//it's already the top environment, and it couldn't find it, then return NULL
		else
			return NULL;
	}

}
WQObject*  Environment::SearchVariableInCurrentEvironment(string& name)
{
	map<string, WQObject*>::iterator it = Variables.find(name);
	if (it != Variables.end())
	{
		return it->second;
	}
	else
	{
		return NULL;
	}
}
void Environment::AddVariable(string& name, WQObject* obj)
{
	string varname = name;
	Variables.insert(pair<string, WQObject*>(varname, obj));
}

void Environment::SetVariable(string& name, WQObject* newobj)
{
	WQObject* targetobj=NULL;

	Environment* tempevnt = this;
	while (tempevnt != NULL)
	{
		WQObject* tempobj =tempevnt-> SearchVariableInCurrentEvironment(name);
		if (tempobj != NULL){
			targetobj = tempobj;
			break;
		}
		tempevnt = tempevnt->Parent;
	}


	if ( targetobj== NULL)
	{
		//the variable doesn't exist yet,
		//create that variable
		AddVariable(name, newobj);
		//the new object is being refrenced, so increment the counter
		newobj->ReferenceCounter++;
	}
	else
	{
		//GarbageVariables.insert(pair<string,WQObject*>(name, newobj));
		//TemporaryVariables.push_back(targetobj);
		tempevnt->Variables.erase(name);
		//object is not refrenced by the variable, so decrement the counter
		targetobj->ReferenceCounter--;
		tempevnt-> Variables.insert(pair<string, WQObject*>(name, newobj));
		//increment the counter of the new obj
		newobj->ReferenceCounter++;
	}
}
WQObject* Environment::CreateVariable(string& name)
{
	WQObject* newvar = new WQObject;
	TemporaryVariables.push_back(newvar);
	AddVariable(name,newvar);
	return newvar;
}
