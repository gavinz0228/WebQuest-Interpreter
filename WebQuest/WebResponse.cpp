#include "WebResponse.h"
string ToLowerCase(string& str);
string Trim(string& str);
int HexStrToInt(string& str);
void WebResponse::ParseResponse(string response)
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
			ResponseHeaders[ToLowerCase(Trim(hd.substr(0, colonindex)))] = hd.substr(colonindex + 1, hd.size() - colonindex - 1);
			start = end + delim.length();
			end = RawResponseHeader.find(delim, start);
		}
		//add the last line of header to it
		string hd = RawResponseHeader.substr(start, RawResponseHeader.size() - start);
		size_t colonindex = hd.find(":");
		ResponseHeaders[ToLowerCase(Trim(hd.substr(0, colonindex)))] = hd.substr(colonindex + 1, hd.size() - colonindex - 1);

		//check headers
		//---------------------
		map<string, string>::iterator findit = ResponseHeaders.find("transfer-encoding");
		if (findit != ResponseHeaders.end() && ToLowerCase(Trim(findit->second)) == "chunked")
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
		string line = Trim(str.substr(start, findres -start));
		//convert the hex string to integer
		int length = HexStrToInt(line);
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
string Trim(string& str)
{
	bool sawchar = false;
	size_t start = 0;
	size_t end = str.size() -1;
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
	return str.substr(start, end - start+1);
}
string ToLowerCase(string& str)
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
int HexStrToInt(string& str)
{
	int x;
	std::stringstream ss;
	ss << std::hex << str;
	ss >> x;
	return x;
}
std::vector<std::string> &split(const std::string &s, char delim, std::vector<std::string> &elems) {
	std::stringstream ss(s);
	std::string item;
	while (std::getline(ss, item, delim)) {
		elems.push_back(item);
	}
	return elems;
}
