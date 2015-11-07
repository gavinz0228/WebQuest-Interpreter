#include <string>
#include <map>
#include <vector>
#include <iostream>
#include <sstream>
using namespace std;
#ifndef STRING_UTILITY_H
#define STRING_UTILITY_H
class StringUtility
{
public:
	static map<string, string>& ListToPairs(char delim, vector<string>& sourcelist, map<string, string>& pairs);
	static vector<string> &split(const string &s, char delim, vector<std::string> &elems);
	static vector<string>&splitByString(const string& s, string& delim, vector<string>);
	static void StringToPairs(string& str, map<string, string>& pairs);
	static void PairsToString(map<string, string>& pairs, string& str);
	static int HexStrToInt(string& str);
	static string ToLowerCase(string& str);
	static string Trim(string& str);

};
#endif
