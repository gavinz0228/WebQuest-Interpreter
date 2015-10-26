#include "HTTPUtility.h"
const string HTTPUtility::COOKIE_FILE_FORMAT = ".cookie";
void HTTPUtility::LoadCookies(map<string, string>& pairs, string &currenthost)
{
	string cookies;
	LoadCookiesString(cookies,currenthost);
	StringUtility::StringToPairs(cookies, pairs);
}
void HTTPUtility::LoadCookiesString(string& cookies, string &currenthost)
{
	ifstream input(currenthost + HTTPUtility::COOKIE_FILE_FORMAT, ifstream::in);
	if (input.is_open())
	{
		cookies.append((std::istreambuf_iterator<char>(input)), std::istreambuf_iterator<char>());
		//std::string str((std::istreambuf_iterator<char>(input)), std::istreambuf_iterator<char>());
	}
}
void HTTPUtility::SaveCookies(map<string, string>& pairs, string &currenthost)
{
	if (pairs.size() > 0)
	{
		string domain;
		if (pairs.find("domain") != pairs.end())
		{
			domain = pairs["domain"];
		}
		else
		{
			domain = currenthost;
		}
		ofstream output(domain + HTTPUtility::COOKIE_FILE_FORMAT, ofstream::out);
		if (output.is_open())
		{
			string cookiestr;
			StringUtility::PairsToString(pairs, cookiestr);
			output << cookiestr;
			output.close();
		}
	}
}