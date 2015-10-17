#include <string>
#include <map>
#include<vector>
#include<sstream>
#include <fstream>
#include <direct.h>
#include "WebRequest.h"
#include "StringUtility.h"
using namespace std;
#ifndef WEBRESPONSE_H
#define WEBRESPONSE_H

class WebResponse
{
public:
	static const string COOKIE_FILE_FORMAT ;
	WebResponse(){};
	WebResponse(WebRequest* request):Request(request){}
	
	void ParseResponse(string& response);
	map<string, string> ResponseHeaders;
	string RawResponseHeader;
	string ResponseBody;
	string ProcessChunkedBody(string& str);
	string Status;
	string GetCookies();
	void LoadCookies(map<string,string>& pairs);
	void SaveCookies(map<string, string>& pairs);
	WebRequest* Request;
	void StringToPairs(string& str, map<string, string>& pairs);
	void PairsToString(map<string, string>& pairs, string& str);
	void ProcessCookies();
};

#endif
