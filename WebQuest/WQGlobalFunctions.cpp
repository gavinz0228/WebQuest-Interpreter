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
	Add("print", WQPrint);
	Add("append", WQAppend);
	Add("len", WQLen);
	Add("range", WQRange);
	Add("dump_json", WQDumpJson);
	Add("parse_json", WQParseJson);
	Add("deep_copy", WQDeepCopy);
	Add("milli", WQMilli);
	Add("get_raw", WQGetRaw);
	Add("parse_headers",WQParseHeader);
	Add("parse_body", WQParseBody);
	Add("parse_status", WQParseStatus);
	Add("get", WQGet);


}