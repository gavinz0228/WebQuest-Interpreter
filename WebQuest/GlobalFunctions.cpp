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
static void WQGetRaw(WQState* state)
{
	WebRequest request;
	string url =state-> GetStringParam();
	state->GetReturnObject()->SetStringValue( request.Get(url));
}
static void WQGet(WQState* state)
{
	WebRequest request;
	string beginSign = "\r\n\r\n";
	string url = state->GetStringParam();
	string result=request.Get(url);

	size_t bodybegin = result.find(beginSign);
	if (bodybegin != string::npos)
	{
		string body = result.substr(bodybegin + beginSign.size(), result.size() - bodybegin - beginSign.size());
		//printf(body.c_str());
		state->GetReturnObject()->SetStringValue(body);
	}
	else
	{
		state->GetReturnObject()->SetStringValue(string(""));
	}
	

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
void WQLen(WQState* state)
{
	WQObject* param = state->GetParam();
	if (param->Type == DT_STRING)
	{
		state->ReturnInteger(param->ToString().size());
	}
	else if (param->Type == DT_LIST)
	{
		state->ReturnInteger(param->GetListValue()->size());
	}
	else
	{
		throw "len function can only used for string type and lsit type";
	}
}

void GlobalFunctions::LoadFunctions()
{
	Add("print", WQPrint);
	Add("append", WQAppend);
	Add("len", WQLen);
	Add("get_raw", WQGetRaw);
	Add("get", WQGet);
}