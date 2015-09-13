#include <iostream>
#include <stdio.h>
#include <list>
#include <string>
#include <sstream>
#include <stdlib.h>
using namespace std;
#ifndef CONVERTER_H
#define CONVERTER_H
class Converter
{
public:
	static string IntegerToString(const long long& num);
	static bool StringToInteger(const string& str, long long& integer);
	static bool StringToInteger(const char* str, int len, long long& integer);
	static bool StringToFloat(const string& str, long double& number);
	static bool StringToFloat(char* str, int len, long double& number);
private:
	static bool ContainsDot(const char* str, int len);
};
#endif


