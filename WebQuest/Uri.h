#include <string>
#include <cctype>
#include <iomanip>
#include <sstream>
#include <string>
#include "WQException.h"
using namespace std;
#ifndef URI_H
#define URI_H

#define DEFAULT_PORT		"80"
class Uri
{
public:
	string Path, Protocol, Host, Port;
	static Uri Parse(const string &uri);
	static string URLEncode(const string& param);

};  

#endif