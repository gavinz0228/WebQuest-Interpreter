#include "WQGlobalFunctions.h"

void WQGlobalFunctions::Add(string name, WQFunction func)
{
	Functions->insert(pair<string,WQFunction>(name, func));
}
WQFunction WQGlobalFunctions::Get(string* name)
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
void WQGlobalFunctions::LoadFunctions()
{
	//basic
	Add("print", WQPrint);
	Add("append", WQAppend);
	Add("len", WQLen);
	Add("range", WQRange);
	Add("type", WQType);
	Add("deep_copy", WQDeepCopy);
	Add("str", WQStr);
	Add("int", WQInt);
	Add("float", WQFloat);
	//time
	Add("year", WQYear);
	Add("month", WQMonth);
	Add("day", WQDay);
	Add("hour", WQHour);
	Add("minute", WQMinute);
	Add("second", WQSecond);
	//utility
	Add("dump_json", WQDumpJson);
	Add("parse_json", WQParseJson);
	Add("milli", WQMilli);
	//web
	Add("get_raw", WQGetRaw);
	Add("parse_headers",WQParseHeader);
	Add("parse_body", WQParseBody);
	Add("parse_status", WQParseStatus);
	Add("get", WQGet);


}