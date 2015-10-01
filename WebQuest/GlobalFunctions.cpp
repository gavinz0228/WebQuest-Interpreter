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
static void WQMilli(WQState* state)
{
	long milli = time(0) * 1000;
	return state->ReturnInteger(milli);
}
static void WQGetRaw(WQState* state)
{
	WebRequest request;
	string url =state-> GetStringParam();
	state->ReturnString( request.Get(url));
}
static void WQGet(WQState* state)
{
	WebRequest request;
	WebResponse response;
	string url = state->GetStringParam();
	string result=request.Get(url);
	response.ParseResponse(result);
	state->ReturnString(response.ResponseBody);
}
static void WQParseHeader(WQState* state)
{
	string str = state->GetStringParam();
	WebResponse response;
	response.ParseResponse(str);
	state->ReturnString(response.RawResponseHeader);
}
static void WQParseBody(WQState* state)
{
	string str = state->GetStringParam();
	WebResponse response;
	response.ParseResponse(str);
	state->ReturnString(response.ResponseBody);
}
static void WQParseStatus(WQState* state)
{
	string str = state->GetStringParam();
	WebResponse response;
	response.ParseResponse(str);
	state->ReturnString(response.Status);
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
		ls->AppendList(state->GetParam());
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
		state->ReturnInteger(param->GetList()->size());
	}
	else
	{
		throw "len function can only used for string type and lsit type";
	}
}
static void WQRange(WQState* state)
{
	WQObject* ls = state->CreateObject();
	ls->InitList();
	if (state->ParamSize == 1)
	{
		long long endindex=state->GetIntegerParam();
		for (long long i = 0; i < endindex;i++)
		{
			WQObject* obj = new WQObject;
			obj->SetIntValue(i);
			ls->AppendList(obj);
		}
	}
	else if (state->ParamSize == 2)
	{
		long long startindex = state->GetIntegerParam();
		long long endindex = state->GetIntegerParam();
		for (long long i = startindex; i < endindex; i++)
		{
			WQObject* obj = new WQObject;
			obj->SetIntValue(i);
			ls->AppendList(obj);
		}
	}
	state->ReturnReference(ls);
}
static void WQDumpJson(WQState* state)
{
	//WQObject obj;
	//obj.InitDictionary();
	auto obj = state->GetParam();
	//obj->SetKeyValue("key", "val");
	//obj->SetKeyValue("numkey", 123);
 	printf(obj->ToString().c_str());
	state->ReturnString(obj->ToString());
}
static void WQDeepCopy(WQState* state)
{
	auto obj = state->GetParam();
	WQObject* newobj = state->CreateObject();
	newobj->DeepCopy(obj);
	state->ReturnReference(newobj);
}
void GlobalFunctions::LoadFunctions()
{
	Add("print", WQPrint);
	Add("append", WQAppend);
	Add("len", WQLen);
	Add("range", WQRange);
	Add("dump_json", WQDumpJson);
	Add("deep_copy", WQDeepCopy);
	Add("milli", WQMilli);
	Add("get_raw", WQGetRaw);
	Add("parse_headers",WQParseHeader);
	Add("parse_body", WQParseBody);
	Add("parse_status", WQParseStatus);
	Add("get", WQGet);


}