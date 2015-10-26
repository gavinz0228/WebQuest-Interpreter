#include<string>
#include<map>
#include "StringUtility.h"
#include <sstream>
#include <fstream>
#ifndef HTTPUTILITY_H
#define HTTPUTILITY_H
class HTTPUtility
{
public:
	static const string COOKIE_FILE_FORMAT;
	static void LoadCookies(map<string, string>& pairs,string &currenthost);
	static void LoadCookiesString(string& cookies, string &currenthost);
	static void SaveCookies(map<string, string>& pairs, string &currenthost);
};

#endif
