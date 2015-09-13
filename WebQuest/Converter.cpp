#include "Converter.h"

string Converter::IntegerToString(const long long &num)
{
	ostringstream stream;
	stream << num;
	return stream.str();
}
bool Converter::StringToInteger(const string& str,long long& integer)
{
	try{
		if (str.find('.') != string::npos)
			return false;
		integer = stoi(str);
		return true;
	}
	catch (const invalid_argument& ia){
		return false;
	}
}
bool Converter::StringToInteger(const char* str, int len, long long& integer)
{
	try{
		if (ContainsDot(str, len))
			return false;
		integer= stoi(string(str, len));
		return true;
	}
	catch (const invalid_argument& ia){
		return false;
	}
}

bool Converter::StringToFloat(const string& str, long double& number)
{
	
	try{
		number = stold(str);
		return true;
	}
	catch (const invalid_argument& ia){
		return false;
	}
	
}

bool Converter::StringToFloat(char* str,int len, long double& number)
{

	try{
		number = stold(string(str,len));
		return true;
	}
	catch (const invalid_argument& ia){
		return false;
	}

}
bool Converter::ContainsDot(const char* str, int len)
{
	for (int i = 0; i < len; i++)
	{
		if (*(str + i) == '.')
			return true;
	}
	return false;
}