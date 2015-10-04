#include <winsock2.h>
#include <ws2tcpip.h>
#include <stdlib.h>
#include <stdio.h>
#include <sstream>
#include <string>
#include <stdlib.h>
#include <map>
#include "Uri.h"
#include "Exception.h"
using namespace std;
#ifndef WEBREQUEST_H
#define WEBREQUEST_H

// Need to link with Ws2_32.lib, Mswsock.lib, and Advapi32.lib
#pragma comment (lib, "Ws2_32.lib")
#pragma comment (lib, "Mswsock.lib")
#pragma comment (lib, "AdvApi32.lib")

#define DEFAULT_BUFLEN			512
#define HTTP_VERSION			"1.1"

#define METHOD_GET				"GET"
#define METHOD_POST				"POST"
#define METHOD_PUT				"PUT"
#define METHOD_HEAD				"HEAD"
#define METHOD_DELETE			"DELETE"
#define METHOD_OPTION			"OPTION"

class WebRequest
{
public:
	WebRequest();
	void AddHeader(string key,string value);
	string Get(string url);
	string PostBinary(string url, const char* data);
	string PostForm(string url, map<string, string> data);

private:
	string SendRequest(const char* hostName, const char* port, const char* data);
	string GetHeaders(string method,Uri uri);
	map<string, string> headers;
};;

#endif

