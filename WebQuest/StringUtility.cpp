#include "StringUtility.h"

string StringUtility::Trim(string& str)
{
	bool sawchar = false;
	size_t start = 0;
	size_t end = str.size() - 1;
	for (; start < str.length(); start++)
	{
		if (str.at(start) != ' ')
			break;
	}
	for (; end >= start; end--)
	{
		if (str.at(end) != ' ')
			break;
	}
	return str.substr(start, end - start + 1);
}
string StringUtility::ToLowerCase(string& str)
{
	string output;
	for (size_t i = 0; i < str.length(); i++)
	{
		char c = str.at(i);
		if (c >= 65 && c <= 90)
		{
			output += c + 32;
		}
		else
		{
			output += c;
		}

	}
	return output;
}
int StringUtility::HexStrToInt(string& str)
{
	int x;
	std::stringstream ss;
	ss << std::hex << str;
	ss >> x;
	return x;
}
vector<std::string>& StringUtility::split(const string &s, char delim, vector<std::string> &elems) {
	stringstream ss(s);
	string item;
	while (getline(ss, item, delim)) {
		elems.push_back(item);
	}
	return elems;
}

map<string, string>& StringUtility::ListToPairs(char delim, vector<string>& sourcelist, map<string, string>& pairs)
{
	stringstream sbuffer;
	vector<string>::iterator it = sourcelist.begin();
	for (; it != sourcelist.end(); it++)
	{
		sbuffer.str(string());
		string* item = &(*it);
		string::iterator strit = item->begin();
		for (; strit != item->end(); strit++)
		{
			if ((*strit) == '=')
			{
				strit++;
				break;
			}
			sbuffer << (*strit);
		}
		string key = sbuffer.str();
		if (strit == item->end())
		{
			pairs[key] = "";
		}
		else
		{
			sbuffer.str(std::string());
			for (; strit != item->end(); strit++)
			{
				sbuffer << (*strit);
			}
			pairs[key] = sbuffer.str();
		}
	}
	return pairs;
}