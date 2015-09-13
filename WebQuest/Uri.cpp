#include "Uri.h"

string ToLowerCase(const string &str);
Uri Uri::Parse(const string &uri)
{
	
	Uri result;
	size_t length = uri.length();
	string _uri=ToLowerCase(uri);
	//detect protocal
	size_t http_pos = uri.find("http://");
	size_t https_pos = uri.find("https://");
	//if it doesn't find the http protocal in the string, throw exception
	if (http_pos!= 0 &&https_pos !=0)
	{
		result.Protocol = "http://";
		throw "Uri must starts with http:// or https:// (case-insensitive)";
	}
	//after the protocal ,it's the beginning of the host name
	size_t host_pos = http_pos != string::npos ? 7 : 8;
	result.Protocol = uri.substr(0, host_pos);

	size_t slash_pos = uri.find("/",host_pos);
	size_t colon_pos = uri.find(":", host_pos);
	//if there is colon, it's the end of the hostname
	if (colon_pos != string::npos)
	{
		result.Host = uri.substr(host_pos, colon_pos - host_pos);
	}
	//if there is no colon, the first slash is the end of the host name
	else if (slash_pos != string::npos)
	{
		result.Host = uri.substr(host_pos, slash_pos - host_pos);
	}
	// if there is no colon and no slash, the end of the string is the end of the hostname
	else
	{
		result.Host = uri.substr(host_pos, length - host_pos);
		result.Port = DEFAULT_PORT;
		result.Path = "/";
		return result;
	}
	//look for the port, if it's not define, use the defualt one
	if (colon_pos == string::npos)
	{
		result.Port = DEFAULT_PORT;
	}
	else
	{
		size_t tem_pos = colon_pos + 1;
		char c = uri.at(tem_pos);
		string port;
		while (tem_pos < length&&isdigit(c))
		{
			port += c;
		}
		if (!port.empty())
		{
			result.Port = port.c_str();
		}
		else
			result.Port = DEFAULT_PORT;
		tem_pos++;
		c = uri.at(tem_pos);
	}
	//the rest is the path
	if (slash_pos != string::npos)
		result.Path = uri.substr(slash_pos, length -  slash_pos);
	else
		result.Path = "/";
	//result.Port = "80";
	//result.Host = "www.yahoo.com";
	//result.Path = "/";


	return result;
}
string Uri::URLEncode(const string & param)
{

		ostringstream escaped;
		escaped.fill('0');
		escaped << hex;

		for (string::const_iterator i = param.begin(), n = param.end(); i != n; ++i) {
			string::value_type c = (*i);

			// Keep alphanumeric and other accepted characters intact
			if (isalnum(c) || c == '-' || c == '_' || c == '.' || c == '~') {
				escaped << c;
				continue;
			}

			// Any other characters are percent-encoded
			escaped << '%' << setw(2) << int((unsigned char)c);
		}

		return escaped.str();

}

string ToLowerCase(const string &str)
{
	string newStr(str);
	for (size_t i = 0; i < newStr.size(); i++)
	{
		char ch = newStr[i];
		if (ch>0x40 && ch < 0x5B)
		{
			newStr[i] = ch + 0x20;
		}
	}
	return newStr;
}
