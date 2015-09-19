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
static void WQPrint(WQState* state);
void GlobalFunctions::LoadFunctions()
{
	Add("print", WQPrint);
}
void func(WQState* state)
{
}

static void WQPrint(WQState* state)
{
	WQObject* obj = state->GetParam();
	printf(obj->ToString().c_str());

	state->ReturnNull();
}