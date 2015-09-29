#include <string>
#include <map>
#include<vector>
#include<sstream>
using namespace std;
#ifndef WEBRESPONSE_H
#define WEBRESPONSE_H
class WebResponse
{
public:
	void ParseResponse(string response);
	map<string, string> ResponseHeaders;
	string RawResponseHeader;
	string ResponseBody;
	string ProcessChunkedBody(string& str);
	string Status;

};

#endif
