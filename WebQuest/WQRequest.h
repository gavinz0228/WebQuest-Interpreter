
#include <ostream>
#include <sstream>
#include <string>
#include <stdio.h> 
#include <curl/curl.h> 
#include "Uri.h"
#include "HTTPUtility.h"
#include <map>


#ifndef WQREQUEST_H
#define WQREQUEST_H
using namespace std;

class WQRequest
{
public:

	WQRequest();
	static size_t DataWrite(void* buf, size_t size, size_t nmemb, void* userp);
	CURLcode GetResponse(const std::string& url, std::ostream& os, long timeout = 30);
	CURLcode HTTPSRequest(const string& url, ostream& os,long timeout=30);
	CURLcode HTTPGet(const string& url, map<string, string>& headers, ostream& os, long timeout = 30);
	CURLcode HTTPPostForm(const string& url,string& data, map<string, string>& headers, ostream& os, long timeout = 30);
	CURLcode HTTPPostJSON(const string& url, string& data, map<string, string>& headers, ostream& os, long timeout = 30);
	CURLcode HTTPPut(const string& url, string& data, map<string, string>&headers, ostream& os, long timeout = 30);
	CURLcode HTTPDelete(const string& url, string& data, map<string, string>&headers, ostream& os, long timeout = 30);
	string PairsToURLParameters(map<string, string>& params);
	curl_slist * SetHeaders(map<string, string>& headers, struct curl_slist *chunk);
	curl_slist* SetCookies( struct curl_slist *chunk,string& hostname);
	Uri URL;

	
private:
	//static bool ShowHeaders;
	string GetCurrentDir();
};

#endif

