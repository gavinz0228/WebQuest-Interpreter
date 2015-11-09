#include "WQLibrary.h"

 void WQMilli(WQState* state)
{
	using namespace std::chrono;
	milliseconds ms = duration_cast< milliseconds >(
		system_clock::now().time_since_epoch()
		);
	state->ReturnInteger(ms.count());
}
void WQSleep(WQState* state)
{
	 state->ExpectParams(1);
	 long long val = state->GetIntegerParam();
	 std::this_thread::sleep_for(std::chrono::milliseconds(val));
}
 void WQType(WQState* state)
 {

	 auto obj = state->GetParam();
	 string output = "";
	 if (obj->Type == DT_BOOLEAN)
		 output = "bool";
	 else if (obj->Type == DT_STRING)
		 output = "string";
	 else if (obj->Type == DT_FLOAT)
		 output = "float";
	 else if (obj->Type == DT_INTEGER)
		 output = "int";
	 else if (obj->Type == DT_LIST)
		 output = "list";
	 else if (obj->Type == DT_DICTIONARY)
		 output = "dict";
	 else if (obj->Type == DT_NULL)
		 output = "null";
	 state->ReturnString(output);
 }

 void WQMonth(WQState* state)
 {
	 time_t  timev;
	 time(&timev);
	 struct tm * now = localtime(&timev);
	 state->ReturnInteger(now->tm_mon+1);
 }
 void WQDay(WQState* state)
 {
	 time_t  timev;
	 time(&timev);
	 struct tm * now = localtime(&timev);
	 state->ReturnInteger(now->tm_mday);

 }
 void WQYear(WQState* state)
 {
	 time_t  timev;
	 time(&timev);
	 struct tm * now = localtime(&timev);
	 state->ReturnInteger(now->tm_year+1900);
 }
 void WQHour(WQState* state)
 {
	 time_t  timev;
	 time(&timev);
	 struct tm * now = localtime(&timev);
	 state->ReturnInteger(now->tm_hour);
 }
 void WQMinute(WQState* state)
 {
	 time_t  timev;
	 time(&timev);
	 struct tm * now = localtime(&timev);
	 state->ReturnInteger(now->tm_min);
 }
 void WQSecond(WQState* state)
 {
	 time_t  timev;
	 time(&timev);
	 struct tm * now = localtime(&timev);
	 state->ReturnInteger(now->tm_sec);
 }
 void WQStr(WQState* state)
 {
	 auto obj=state->CreateObject();
	 obj->SetStringValue(state->GetParam()->ToString());
 }
 void WQInt(WQState* state)
 {
	 auto val = state->GetStringParam();
	 long long intval ;
	 Converter::StringToInteger(val, intval);
	 state->ReturnInteger(intval);
 }
 void WQFloat(WQState* state)
 {
	 auto val = state->GetStringParam();
	 long double floatval;
	 Converter::StringToFloat(val, floatval);
	 state->ReturnFloat(floatval);
 }
 void WQSplit(WQState* state)
 {
	 state->ExpectParams(2);
	 string str=state->GetStringParam();
	 string delim = state->GetStringParam();
	 WQObject* obj = state->CreateObject();
	 obj->InitList();
	 vector<WQObject*>* strlist = obj->GetList();

	 if (str.length() == 0)
	 {
		 state->ReturnReference(obj);
		 return;
	 }
	 size_t startindex = 0;
	 while (true)
	 {
		 size_t targetindex = str.find(delim, startindex);
		 if (targetindex == string::npos)
		 {
			 WQObject* newitem=state->CreateObject();
			 newitem->SetStringValue(str.substr(startindex, targetindex - startindex));
			 obj->AppendList(newitem);
			 break;
		 }
		 WQObject* newitem = state->CreateObject();
		 newitem->SetStringValue(str.substr(startindex, targetindex - startindex));
		 obj->AppendList(newitem);
		 startindex = targetindex + 1;
	 }
	 state->ReturnReference(obj);
 }
 void WQFind(WQState* state)
 {
	 state->ExpectParams(2);
	 string str = state->GetStringParam();
	 string target = state->GetStringParam();
	 size_t index = str.find(target);
	 if (index == string::npos)
		 state->ReturnInteger(-1);
	 else
		 state->ReturnInteger(index);
 }
 void WQSubString(WQState* state)
 {
	 if (state->ParamSize <= 1)
	 {
		 throw "this function accepts 2 or 3 elements";
	 }
	 else if (state->ParamSize == 2)
	 {
		 string str = state->GetStringParam();
		 long long startindex = abs(state->GetIntegerParam());
		 if (startindex < str.length())
		 {
			 string returnstr=str.substr(startindex);
			 state->ReturnString(returnstr);
		 }
		 else
		 {
			 string emptystr = "";
			 state->ReturnString(emptystr);
		 }
	 }
	 else if (state->ParamSize == 3)
	 {
		 string str = state->GetStringParam();
		 unsigned long long startindex = abs(state->GetIntegerParam());
		 unsigned long long endindex = abs(state->GetIntegerParam());
		 if (startindex < str.length() && endindex < str.length())
		 {
			 string returnstr = str.substr(startindex,endindex);
			 state->ReturnString(returnstr);
		 }
		 else
		 {
			 string emptystr = "";
			 state->ReturnString(emptystr);
		 }
	 }
	 else{
		 throw "function accepts 2 or 3 elements";
	 }

 }
 void WQRegexMatch(WQState* state)
 {
	 state->ExpectParams(2);
	 string source = state->GetStringParam();
	 string pattern = state->GetStringParam();
	 regex reg_exp(pattern);
	 regex_iterator<string::iterator> regex(source.begin(), source.end(), reg_exp, regex_constants::match_continuous);
	 std::regex_iterator<string::iterator> end;
	 WQObject* lstresult = state->CreateObject();
	 lstresult->InitList();
	 for (; regex != end;regex++)
	 {
		 WQObject* res = state->CreateObject();
		 res->SetStringValue(regex->str());
		 lstresult->AppendList(res);
	 }
	 state->ReturnReference(lstresult);
 }
 void WQGetRaw(WQState* state)
 {
	 //WebRequest request;
	 //string url = state->GetStringParam();
	 //state->ReturnString(request.Get(url));
 }
 string PairsToURLParameters(map<string, WQObject*>* params)
 {
	 if (params == NULL)
		 return "";
	 string urlparam = "";
	 map<string, WQObject*>::iterator it = params->begin();
	 for (; it != params->end(); it++)
	 {
		 urlparam += Uri::URLEncode(it->first) + "=" + Uri::URLEncode(it->second->ToString());
		 if (next(it) != params->end())
			 urlparam += "&";
	 }
	 return urlparam;
 }
 string PairsToStringPairs(map<string, WQObject*>* params, map<string,string>* result)
 {
	 if (params == NULL)
		 return "";
	 string urlparam = "";
	 map<string, WQObject*>::iterator it = params->begin();
	 for (; it != params->end(); it++)
	 {
		 (*result)[it->first] = it->second->ToString();
	 }
	 return urlparam;
 }

 void WQShowHeaders(WQState* state)
 {
	 bool val = state->GetBooleanParam();
	 WQResponse::ShowHeaders=val;

 }
void WQGet(WQState* state)
{
	
	if (state->ParamSize < 1 || state->ParamSize>3)
	{
		throw "The function only accept maximum 3 parameters and minimum 1 parameter";
	}
	
	if (state->ParamSize == 1)
	{
		WQRequest request;
		
		map<string, string> headers;
		stringstream ss;
		string url = state->GetStringParam();
		request.HTTPGet(url, headers, ss);
		//
		WQResponse response(&request);
		string output;
		response.ParseResponse(ss.str(),output);
		response.ProcessCookies();
		state->ReturnString(output);
	}
	else if (state->ParamSize == 2)
	{
		WQRequest req;
		stringstream ss;
		map<string, string> headers;
		string url = state->GetStringParam();
		auto dict = state->GetDictionaryParam();
		if (dict==NULL)
			CURLcode code = req.HTTPGet(url, headers, ss);
		else
			CURLcode code= req.HTTPGet(url+"?"+PairsToURLParameters(dict), headers, ss);
		//cout << result<<endl;
		//
		WQResponse response(&req);
		string output;
		response.ParseResponse(ss.str(), output);
		response.ProcessCookies();
		state->ReturnString(output);
	}
	else if (state->ParamSize == 3)
	{
		WQRequest req;
		stringstream ss;
		map<string, string> headers;
		string url = state->GetStringParam();
		auto params = state->GetDictionaryParam();
		auto heads = state->GetDictionaryParam();
		string stringparams = "?" + PairsToURLParameters(params);
		if (heads != NULL)
		{
			map<string, WQObject*>::iterator it = heads->begin();
			for (; it != heads->end(); it++)
			{
				headers.insert(pair<string, string>(it->first, it->second->ToString()));
			}
		}
		CURLcode code = req.HTTPGet(url+stringparams , headers, ss);
		//cout << result<<endl;
		//
		WQResponse response(&req);
		string output;
		response.ParseResponse(ss.str(), output);
		response.ProcessCookies();
		state->ReturnString(output);
	}
}
void WQPost(WQState* state)
{
	WQRequest request;
	map<string, string> headers;
	stringstream ss;
	if (state->ParamSize < 1 || state->ParamSize>3)
	{
		throw "The function only accept maximum 3 parameters and minimum 1 parameter";
	}
	if (state->ParamSize==1)
	{
		string url = state->GetStringParam();
		request.HTTPPostForm(url, string(), headers, ss);
		//---
		WQResponse response(&request);
		string output;
		response.ParseResponse(ss.str(), output);
		response.ProcessCookies();
		state->ReturnString(output);

	}
	else if (state->ParamSize == 2)
	{
		string url = state->GetStringParam();
		map<string, WQObject*>* params = state->GetDictionaryParam();
		string postdata;
		if (url.length() == 0)
			throw "The url cannot be empty";
		
		if (params != NULL)
			postdata = PairsToURLParameters(params);
		request.HTTPPostForm(url,postdata , headers, ss);
		//---
		WQResponse response(&request);
		string output;
		response.ParseResponse(ss.str(), output);
		response.ProcessCookies();
		state->ReturnString(output);
	}
	else
	{
		string postdata;
		map<string, string> headerslist;
		string url = state->GetStringParam();
		if (url.length() == 0)
			throw "The url cannot be empty";
		map<string, WQObject*>* params = state->GetDictionaryParam();
		map<string, WQObject*>* headermap = state->GetDictionaryParam();

		PairsToStringPairs(headermap, &headerslist);
		if (params != NULL)
			postdata = PairsToURLParameters(params);
		request.HTTPPostForm(url, postdata, headerslist, ss);
		//---
		WQResponse response(&request);
		string output;
		response.ParseResponse(ss.str(), output);
		response.ProcessCookies();
		state->ReturnString(output);
	}
}

void WQPostJSON(WQState* state)
{
	WQRequest request;
	map<string, string> headers;
	stringstream ss;
	if (state->ParamSize < 1 || state->ParamSize>3)
	{
		throw "The function only accept maximum 3 parameters and minimum 1 parameter";
	}
	if (state->ParamSize == 1)
	{
		string url = state->GetStringParam();
		request.HTTPPostJSON(url, string(), headers, ss);
		//---
		WQResponse response(&request);
		string output;
		response.ParseResponse(ss.str(), output);
		response.ProcessCookies();
		state->ReturnString(output);
	}
	else if (state->ParamSize == 2)
	{
		string postdata;
		string url = state->GetStringParam();
		if (url.length() == 0)
			throw "The url cannot be empty";
		WQObject* dictobj = state->GetParam();
		if (dictobj->Type != DT_NULL)
			postdata = dictobj->ToString();
		request.HTTPPostJSON(url, postdata, headers, ss);
		//---
		WQResponse response(&request);
		string output;
		response.ParseResponse(ss.str(), output);
		response.ProcessCookies();
		state->ReturnString(output);
	}
	else
	{
		string postdata;
		map<string, string> headerslist;
		string url = state->GetStringParam();
		if (url.length() == 0)
			throw "The url cannot be empty";
		map<string, WQObject*>* params = state->GetDictionaryParam();
		map<string, WQObject*>* headermap = state->GetDictionaryParam();

		PairsToStringPairs(headermap, &headerslist);
		if (params != NULL)
			postdata = PairsToURLParameters(params);
		request.HTTPPostJSON(url, postdata, headerslist, ss);
		//---
		WQResponse response(&request);
		string output;
		response.ParseResponse(ss.str(), output);
		response.ProcessCookies();
		state->ReturnString(output);

		//string postdata;
		//map<string, string> headerslist;
		//string url = state->GetStringParam();
		//if (url.length() == 0)
		//	throw "The url cannot be empty";
		//map<string, WQObject*>* params = state->GetDictionaryParam();
		//map<string, WQObject*>* headermap = state->GetDictionaryParam();

		//PairsToStringPairs(headermap, &headerslist);
		//if (params != NULL)
		//	postdata = PairsToURLParameters(params);
		//request.HTTPPostForm(url, postdata, headerslist, ss);
		//state->ReturnString(ss.str());
	}
}

void WQPostData(WQState* state)
{

}
 void WQParseHeader(WQState* state)
{
	string str = state->GetStringParam();
	WQResponse response;
	response.ParseResponse(str);
	state->ReturnString(response.RawResponseHeader);
}
 void WQParseBody(WQState* state)
{
	string str = state->GetStringParam();
	WQResponse response;
	response.ParseResponse(str);
	state->ReturnString(response.ResponseBody);
}

 void WQParseStatus(WQState* state)
{
	string str = state->GetStringParam();
	WQResponse response;
	response.ParseResponse(str);
	state->ReturnString(response.Status);
}

 void WQPrint(WQState* state)
{
	WQObject* obj = state->GetParam();
	//printf(obj->ToString().c_str());
	cout << obj->ToString();
	cout.flush();
}
 void WQAppend(WQState* state)
{
	if (state->ParamSize != 2)
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
	else if (param->Type == DT_DICTIONARY)
	{
		state->ReturnInteger(param->GetDictionary()->size());
	}
	else
	{
		throw "len function can only used for string type and lsit type";
	}
}
 void WQContains(WQState* state)
 {
	 state->ExpectParams(2);
	 auto source = state->GetParam();
	 auto target = state->GetParam();
	 if (source->Type == DT_LIST)
	 {
		 bool result = false;
		 auto lst = source->GetList();
		 for (int i = 0; i < lst->size(); i++)
		 {
			 if (*(lst->at(i)) == *target)
			 {
				 result = true;
			 }
		 }
		 state->ReturnBoolean(result);
	 }
	 else if (source->Type == DT_DICTIONARY)
	 {
		 auto dict = source->GetDictionary();
		 string key = target->ToString();
		 map<string, WQObject*>::iterator it = dict->find(key);
		 if (it == dict->end())
		 {
			 state->ReturnBoolean(false);
		 }
		 else
			 state->ReturnBoolean(true);
	 }
	 else
		 state->ReturnBoolean(false);
 }
 void WQSaveFile(WQState* state)
 {
	 state->ExpectParams(2);
	 auto data = state->GetParam();
	 auto path = state->GetStringParam();
	 ofstream os;
	 os.open(path);
	 if (os.is_open())
	 {
		 auto strdata = data->ToString();
		 os.write(strdata.c_str(),strdata.length());
		 os.close();
	 }
	 else
	 {
		 throw "Failed to open the file : " + path;
	 }
 }
 void WQLoadFile(WQState* state)
 {
	 state->ExpectParams(1);
	 auto path = state->GetStringParam();
	 ifstream is;
	 is.open(path);
	 if (is.is_open())
	 {
		std::string str((std::istreambuf_iterator<char>(is)),
		std::istreambuf_iterator<char>());
		return state->ReturnString(str);
	 }
	 else
		 throw "Failed to open the file : " + path;
 }
 void WQRange(WQState* state)
{
	WQObject* ls = state->CreateObject();
	ls->InitList();
	if (state->ParamSize == 1)
	{
		long long endindex = state->GetIntegerParam();
		for (long long i = 0; i < endindex; i++)
		{
			WQObject* obj = state->CreateObject();
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
			WQObject* obj = WQObject::Create();
			obj->SetIntValue(i);
			ls->AppendList(obj);
		}
	}
	state->ReturnReference(ls);
}
 void WQDumpJson(WQState* state)
{
	//WQObject obj;
	//obj.InitDictionary();
	auto obj = state->GetParam();
	//obj->SetKeyValue("key", "val");
	//obj->SetKeyValue("numkey", 123);
	printf(obj->ToString().c_str());
	state->ReturnString(obj->ToString());
}
 void WQCreateNode(WQObject* parent, JSONValue* js,WQState* state)
 {
	 if (js == NULL)
		 return;
	 if (js->IsObject())
	 {
		 parent->InitDictionary();
		 auto obj = js->AsObject();
		 auto it = obj.begin();
		 for (; it != obj.end(); it++)
		 {
			 auto kv=state->CreateObject();
			 WQCreateNode(kv, it->second, state);
			 auto strval = it->first;
			 parent->SetKeyValue(string(strval.begin(),strval.end()),kv);
		 }
	 }
	 else if (js->IsString())
	 {
		 auto strval = js->AsString();
		 parent->SetStringValue(string(strval.begin(),strval.end()));
	 }
	 else if (js->IsBool())
	 {
		 parent->SetBoolValue(js->AsBool());
	 }
	 else if (js->IsNumber())
	 {
		 parent->SetFloatValue(js->AsNumber());
	 }
	 else if (js->IsArray())
	 {
		 parent->InitList();
		 auto arr = js->AsArray();
		 for (size_t i = 0; i < arr.size(); i++)
		 {
			 auto obj=state->CreateObject();
			 WQCreateNode(obj, arr[i], state);
			 parent->AppendList(obj);
		 }
	 }
	 else if (js->IsNull())
	 {

	 }
	 else
		 throw "Invalid json string";
	 //for (size_t i = 0; i < js->ObjectKeys().size(); i++)
	 //{
		// if (js->IsString())
		// {
		//	 (*parent->GetDictionary())[js->k]
		// }
	 //}
 }

 void WQParseJson(WQState* state)
 {
	 string raw = state->GetParam()->ToString();
	 auto dict=state->CreateObject();
	 JSONValue* js = JSON::Parse(raw.c_str());
	 WQCreateNode(dict, js,state);
	 state->ReturnReference(dict);
 }
 void WQDeepCopy(WQState* state)
{
	auto obj = state->GetParam();
	WQObject* newobj = state->CreateObject();
	newobj->DeepCopy(obj);
	state->ReturnReference(newobj);
}