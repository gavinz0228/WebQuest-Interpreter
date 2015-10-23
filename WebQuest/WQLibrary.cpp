#include "WQLibrary.h"

 void WQMilli(WQState* state)
{
	using namespace std::chrono;
	milliseconds ms = duration_cast< milliseconds >(
		system_clock::now().time_since_epoch()
		);
	state->ReturnInteger(ms.count());
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

 void WQGetRaw(WQState* state)
 {
	 WebRequest request;
	 string url = state->GetStringParam();
	 state->ReturnString(request.Get(url));
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

void WQGet(WQState* state)
{
	WebRequest request;
	
	if (state->ParamSize < 1 || state->ParamSize>3)
	{
		throw "The function only accept maximum 3 parameters and minimum 1 parameter";
	}
	
	if (state->ParamSize == 1)
	{
		WQRequest req;
		stringstream ss;
		string url = state->GetStringParam();
		//req.GetResponse("http://www.google.com", ss);
		req.HTTPSRequest(url, ss);
		printf(ss.str().c_str());  
		//WebResponse response(&request);
		//string result = request.Get(url);
		//response.ParseResponse(result);
		//response.ProcessCookies();
		//state->ReturnString(response.ResponseBody);
		//CURL *curl;
		//CURLcode res;
		//curl = curl_easy_init();
		//if (curl)
		//{
		//	curl_easy_setopt(curl, CURLOPT_URL, url);
		//	curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, &writeCallback);
		//	curl_easy_setopt(curl, CURLOPT_VERBOSE, 1L); //tell curl to output its progress

		//	res = curl_easy_perform(curl);
		//	
		//	if (res == CURLE_OK)
		//	{
		//		
		//	}
		//	curl_easy_cleanup(curl);
		//}
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
		state->ReturnString(ss.str());
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
		state->ReturnString(ss.str());
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
		state->ReturnString(ss.str());
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
		state->ReturnString(ss.str());
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
		state->ReturnString(ss.str());
	}
}
 void WQParseHeader(WQState* state)
{
	string str = state->GetStringParam();
	WebResponse response;
	response.ParseResponse(str);
	state->ReturnString(response.RawResponseHeader);
}
 void WQParseBody(WQState* state)
{
	string str = state->GetStringParam();
	WebResponse response;
	response.ParseResponse(str);
	state->ReturnString(response.ResponseBody);
}

 void WQParseStatus(WQState* state)
{
	string str = state->GetStringParam();
	WebResponse response;
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
	else
	{
		throw "len function can only used for string type and lsit type";
	}
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