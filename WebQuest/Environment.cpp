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

void Environment::SetVariable(string& name, WQObject* obj)
{
	WQObject* var = GetVariable(name);
	if ( var== NULL)
	{
		//the variable doesn't exist yet,
		//create that variable
		WQObject* newobj=CreateVariable(name);
		//assign the value to the leftside
		newobj->GetAssigned(obj);
	}
	else
	{
		GarbageVariables.insert(pair<string,WQObject*>(name, var));
		Variables.erase(name);	
	}
}
WQObject* Environment::CreateVariable(string& name)
{
	WQObject* newvar = new WQObject;
	AddVariable(name,newvar);
	return newvar;
}