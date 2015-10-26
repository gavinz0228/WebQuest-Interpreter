#include<string>
#include<map>
#include "WQRequest.h"
#include "StringUtility.h"
#include "HTTPUtility.h"
#include <sstream>
#include <fstream>
using namespace std;
class WQResponse
{
public:
	
	WQResponse(){};
	WQResponse(WQRequest* request) :Request(request){}

	void ParseResponse(string& response,string& output);
	map<string, string> ResponseHeaders;
	string RawResponseHeader;
	string ResponseBody;
	string ProcessChunkedBody(string& str);
	string Status;
	string GetCookies();

	WQRequest* Request;
	void ProcessCookies();
	static bool ShowHeaders;
};