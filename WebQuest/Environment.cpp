#include "Environment.h"
WQObject* Environment::GetVariable(string& name)
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
	WQObject* oldvar = GetVariable(name);
	if ( oldvar== NULL)
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
		GarbageVariables.insert(pair<string,WQObject*>(name, oldvar));
		Variables.erase(name);	
		Variables.insert(pair<string, WQObject*>(name, replacement));
	}
}
WQObject* Environment::CreateVariable(string& name)
{
	WQObject* newvar = new WQObject;
	AddVariable(name,newvar);
	return newvar;
}