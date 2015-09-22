#include "GlobalFunctions.h"

void GlobalFunctions::Add(string name, WQFunction func)
{
	Functions->insert(pair<string,WQFunction>(name, func));
}
WQFunction GlobalFunctions::Get(string* name)
{
	map<string, WQFunction>::iterator it = Functions->find(*name);
	if (it == Functions->end())
	{
		return NULL;
	}
	else
	{
		return it->second;
	}
}


void func(WQState* state)
{
}

static void WQPrint(WQState* state)
{
	WQObject* obj = state->GetParam();
	printf(obj->ToString().c_str());
}
static void WQAppend(WQState* state)
{
	if (state->ParamSize!=2)
	{
		throw "Only accept 2 argument(list,element)";
	}
	else
	{
 		WQObject* ls = state->GetParam();
		ls->AppendListValue(*state->GetParam());
	}
}

void GlobalFunctions::LoadFunctions()
{
	Add("print", WQPrint);
	Add("append", WQAppend);
}