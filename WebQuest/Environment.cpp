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
void Environment::SetVariable(WQObject* oldobj, WQObject* newobj)
{

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
		WQObject* newvar=CreateVariable(name);
		//assign the value to the leftside
		newvar->GetAssigned(newobj);
	}
	else
	{

		WQObject* replacement = new WQObject;
		replacement->GetAssigned(newobj);
		GarbageVariables.insert(pair<string,WQObject*>(name, targetobj));
		tempevnt->Variables.erase(name);
		tempevnt-> Variables.insert(pair<string, WQObject*>(name, replacement));
	}
}
WQObject* Environment::CreateVariable(string& name)
{
	WQObject* newvar = new WQObject;
	AddVariable(name,newvar);
	return newvar;
}
