#include "WQEnvironment.h"
WQObject* WQEnvironment::GetVariable(string& name)
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
			WQEnvironment* tempevnt = Parent;
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
WQObject*  WQEnvironment::SearchVariableInCurrentEvironment(string& name)
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
void WQEnvironment::AddVariable(string& name, WQObject* obj)
{
	string varname = name;
	Variables.insert(pair<string, WQObject*>(varname, obj));
}

void WQEnvironment::SetVariable(string& name, WQObject* newobj)
{
	WQObject* targetobj=NULL;

	WQEnvironment* tempevnt = this;
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
		IncreaseReference(newobj);
	}
	else
	{
		//GarbageVariables.insert(pair<string,WQObject*>(name, newobj));
		//TemporaryVariables.push_back(targetobj);
		tempevnt->Variables.erase(name);
		//object is not refrenced by the variable, so decrement the counter
		ReleaseReference( targetobj);
		tempevnt-> Variables.insert(pair<string, WQObject*>(name, newobj));
		//increment the counter of the new obj
		IncreaseReference( newobj);
	}
}
WQObject* WQEnvironment::CreateVariable(string& name)
{
	WQObject* newvar = WQObject::Create();
	TemporaryVariables.push_back(newvar);
	AddVariable(name,newvar);
	return newvar;
}
void WQEnvironment::IncreaseReference(WQObject* obj)
{
	obj->ReferenceCounter++;
}
void WQEnvironment::ReleaseReference(WQObject* obj)
{
	obj->ReferenceCounter--;
	//if this object is not being reference any more
	//delete the object right now, before that also check if it's a reference
	//if yes, also release the object it's referencing
	if (obj->ReferenceCounter < 1)
	{
		//if (obj->IsReference)
		//	ReleaseReference(obj->Reference);
		DeleteObject(obj);
	}
}
void WQEnvironment::DeleteObject(WQObject* obj)
{
	list<WQObject*>::iterator tempit = TemporaryVariables.begin();
	for (; tempit != TemporaryVariables.end(); tempit++)
	{
		if (*tempit == obj)
		{
			delete obj;
			TemporaryVariables.erase(tempit);
			return;
		}
	}
}
WQObject* WQEnvironment::CreateObject()
{
	WQObject* obj = WQObject::Create();
	TemporaryVariables .push_back(obj);
	return obj;
}