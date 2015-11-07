#include "WebResponse.h"

const string WebResponse::COOKIE_FILE_FORMAT = ".cookie";


void WebResponse::ParseResponse(string& response)
{
	if (response.size() != 0)
	{
		string beginSign = "\r\n\r\n";
		size_t bodybegin = response.find(beginSign);
		RawResponseHeader = response.substr(0, bodybegin);
		ResponseBody = response.substr(bodybegin + beginSign.size(), response.size() - bodybegin - beginSign.size());

		//parse headers
		//------------------------------
		std::string delim = "\r\n";
		int delimlen = delim.size();
		auto start = RawResponseHeader.find(delim);
		Status = RawResponseHeader.substr(0, start);
		start += delimlen;
		auto end = RawResponseHeader.find(delim, start);
		while (end != std::string::npos)
		{
			//gets a line of header
			string hd = RawResponseHeader.substr(start, end - start);
			size_t colonindex= hd.find(":");
			ResponseHeaders[StringUtility::Trim(hd.substr(0, colonindex))] = hd.substr(colonindex + 1, hd.size() - colonindex - 1);
			start = end + delim.length();
			end = RawResponseHeader.find(delim, start);
		}
		//add the last line of header to it
		string hd = RawResponseHeader.substr(start, RawResponseHeader.size() - start);
		size_t colonindex = hd.find(":");
		ResponseHeaders[StringUtility::Trim(hd.substr(0, colonindex))] = hd.substr(colonindex + 1, hd.size() - colonindex - 1);

		//check headers
		//---------------------
		map<string, string>::iterator findit = ResponseHeaders.find("Transfer-Encoding");
		if (findit != ResponseHeaders.end() && StringUtility::Trim(findit->second)=="Chunked")
		{
			ResponseBody = ProcessChunkedBody(ResponseBody);
		}
	}
}

string WebResponse::ProcessChunkedBody(string& str)
{
	stringstream ss;
	//get the first line of the body
	string newline = "\r\n";
	int start = 0;
	int newlinelen = newline.length();

	size_t findres = str.find(newline,start);
	if (findres == string::npos)
		return str;
	while (findres != string::npos)
	{
		findres = str.find(newline, start);
		//get the line and remove extra spaces
		string line = StringUtility::Trim(str.substr(start, findres -start));
		//convert the hex string to integer
		int length = StringUtility::HexStrToInt(line);
		if (length == 0)
			break;
		//got a chunk of data, write it to the buffer
		ss << str.substr(findres + newlinelen, length);
		string sstr = str.substr(findres + newlinelen, length);
		//printf(str.substr(findres + newlinelen, length).c_str());
		start = findres + newlinelen + length + newlinelen;
	}
	return ss.str();
	
}
string WebResponse::GetCookies()
{
	string cookie = ResponseHeaders["Set-Cookie"];

	return cookie;
		
}
void WebResponse::ProcessCookies()
{
	map<string, string>  newvalue;
	map<string, string> oldvalue;
	LoadCookies(oldvalue);
	StringToPairs(GetCookies(), newvalue);
	map<string, string>::iterator newit = newvalue.begin();
	for (; newit != newvalue.end(); newit++)
	{
		oldvalue[newit->first] = newit->second;
	}
	SaveCookies(oldvalue);
}
void WebResponse::StringToPairs(string& str, map<string, string>& pairs)
{
	vector<string> items;
	StringUtility::split(str, ';', items);
	StringUtility::ListToPairs('=', items, pairs);
}
void WebResponse::PairsToString(map<string, string>& pairs, string& str)
{
	stringstream sbuffer;
	map<string, string>::iterator pairit = pairs.begin();
	for (; pairit != pairs.end(); pairit++)
	{
		sbuffer << pairit->first << "=" << pairit->second << ";";
	}
	str = sbuffer.str();
	
}

void WebResponse::SaveCookies(map<string, string>& pairs)
{
	ofstream output(Request->Host + WebResponse::COOKIE_FILE_FORMAT, ofstream::out);
	if (output.is_open())
	{
		string cookiestr;
		PairsToString(pairs, cookiestr);
		output << cookiestr;
		output.close();
	}
	
}
void WebResponse::LoadCookies(map<string,string>& pairs)
{
	ifstream input(Request->Host + WebResponse::COOKIE_FILE_FORMAT, ifstream::in);
	if (input.is_open())
	{
		std::string str((std::istreambuf_iterator<char>(input)), std::istreambuf_iterator<char>());
		StringToPairs(str, pairs);
	}
}
