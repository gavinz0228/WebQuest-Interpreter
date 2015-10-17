
#include <ostream>
#include <sstream>
#include <string>
#include <stdio.h> 
#include <curl/curl.h> 
#ifndef WQREQUEST_H
#define WQREQUEST_H
using namespace std;
/*
#if _DEBUG
#pragma comment(lib, "Library/debug_libcurl.lib") 
#pragma comment(lib, "Library/debug_libssh2.lib") 
#pragma comment(lib, "Library/debug_libeay32.lib") 
#pragma comment(lib, "Library/debug_ssleay32.lib") 
#pragma comment(lib, "Library/debug_zlib.lib") 

#else

#pragma comment(lib, "Library/release_libcurl.lib") 
#pragma comment(lib, "Library/release_libssh2.lib") 
#pragma comment(lib, "Library/release_libeay32.lib") 
#pragma comment(lib, "Library/release_ssleay32.lib") 
#pragma comment(lib, "Library/release_zlib.lib") 

#endif
#pragma comment(lib, "wldap32.lib") 
*/


class WQRequest
{
public:
	static size_t DataWrite(void* buf, size_t size, size_t nmemb, void* userp);
	CURLcode GetResponse(const std::string& url, std::ostream& os, long timeout = 30);
	CURLcode HTTPSRequest(const string& url, ostream& os,long timeout=30);
private:
	string GetCurrentDir();
};
#endif
