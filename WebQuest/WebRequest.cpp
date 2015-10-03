#include "WebRequest.h"

void WebRequest::AddHeader(string key, string value)
{
	//headers.insert(pair<string,string>( key, value));
	headers[key] = value;
}
string WebRequest::GetHeaders(string method, Uri uri)
{
	string data;
	data += method+" "+ uri.Path + " HTTP/" + HTTP_VERSION + "\r\n";
	data += "Host: " + uri.Host + "\r\n";
	
	int hsize = headers.size();
	if (hsize > 0)
	{
		for (std::map<string, string>::iterator it = headers.begin(); it != headers.end(); ++it)
		{
			data += it->first + ": "+ it->second+"\r\n";
		}
	}
	data += "\r\n";
	return data;
}
//WebRequest::WebRequest()
//{headers.insert("");}
//sends a get request
string WebRequest::Get(string url)
{
	Uri uri;
	//parse the uri, splits the components of the uri
	uri=Uri::Parse(url);
	return SendRequest(uri.Host.c_str(),uri.Port.c_str(),GetHeaders(METHOD_GET,uri).c_str());
}

string WebRequest::PostForm(string url, map<string, string> data)
{
	string param;
	if (data.size() > 0)
	{
		for (std::map<string, string>::iterator it = data.begin(); it != data.end(); ++it)
		{

			param += it->first + "=" + Uri::URLEncode(it->second) + "&";
		}
		param.pop_back();
	}


	return PostBinary(url, param.c_str());
}
string WebRequest::PostBinary(string url, const char* data)
{
	char buf[30];
	Uri uri = Uri::Parse(url);
	//AddHeader("Accept", "*/*");
	AddHeader("Content-Type", "application/x-www-form-urlencoded;");
	AddHeader("Content-Length",to_string(strlen(data)));
	
	string datatosend = GetHeaders(METHOD_POST, uri);
	datatosend.append(data);
	return SendRequest(uri.Host.c_str(), uri.Port.c_str(),datatosend.c_str() );
}


string WebRequest::SendRequest(const char* hostName, const char* port, const char* data)
{

	WSADATA wsaData;
	SOCKET ConnectSocket = INVALID_SOCKET;
	struct addrinfo *result = NULL,
		*ptr = NULL,
		hints;
	char recvbuf[DEFAULT_BUFLEN];
	char errorbuf[50];
	int iResult;
	int recvbuflen = DEFAULT_BUFLEN;

	// Initialize Winsock
	iResult = WSAStartup(MAKEWORD(2, 2), &wsaData);
	if (iResult != 0) {
		printf("WSAStartup failed with error: %d\n", iResult);

	}

	ZeroMemory(&hints, sizeof(hints));
	hints.ai_family = AF_UNSPEC;
	hints.ai_socktype = SOCK_STREAM;
	hints.ai_protocol = IPPROTO_TCP;

	// Resolve the server address and port
	iResult = getaddrinfo(hostName, port, &hints, &result);
	if (iResult != 0) {
		//printf("getaddrinfo failed with error: %d\n", iResult);
		sprintf_s(errorbuf, "getaddrinfo failed with error: %d\n", iResult);
		WSACleanup();
		throw errorbuf;
	}

	// Attempt to connect to an address until one succeeds
	for (ptr = result; ptr != NULL; ptr = ptr->ai_next) {

		// Create a SOCKET for connecting to server
		ConnectSocket = socket(ptr->ai_family, ptr->ai_socktype,
			ptr->ai_protocol);
		if (ConnectSocket == INVALID_SOCKET) {
			sprintf_s(errorbuf,"socket failed with error: %ld\n", WSAGetLastError());

			WSACleanup();
			throw errorbuf;
		}

		// Connect to server.
		iResult = connect(ConnectSocket, ptr->ai_addr, (int)ptr->ai_addrlen);
		if (iResult == SOCKET_ERROR) {
			closesocket(ConnectSocket);
			ConnectSocket = INVALID_SOCKET;
			continue;
		}
		break;
	}

	freeaddrinfo(result);

	if (ConnectSocket == INVALID_SOCKET) {
		WSACleanup();
		sprintf_s(errorbuf,"Unable to connect to server!\n");
		throw errorbuf;
	}

	// Send an initial buffer
	iResult = send(ConnectSocket, data, (int)strlen(data), 0);
	if (iResult == SOCKET_ERROR) {
		closesocket(ConnectSocket);
		WSACleanup();
		sprintf_s(errorbuf, "send failed with error: %d\n", WSAGetLastError());
		throw errorbuf;
	}

	//printf("Bytes Sent: %ld\n", iResult);

	// shutdown the connection since no more data will be sent
	
	iResult = shutdown(ConnectSocket, SD_SEND);
	if (iResult == SOCKET_ERROR) {


		closesocket(ConnectSocket);
		WSACleanup();
		sprintf_s(errorbuf,"shutdown failed with error: %d\n", WSAGetLastError());
		throw errorbuf;
	}
	string response;
	// Receive until the peer closes the connection
	do {

		iResult = recv(ConnectSocket, recvbuf, recvbuflen-1, 0);
		recvbuf[iResult] = 0;
		if (iResult>0)
			response +=string(recvbuf);
		//printf("%s", recvbuf);
		//if (iResult > 0)
		//	printf("Bytes received: %d\n", iResult);
		//else if (iResult == 0)
		//	printf("Connection closed\n");
		//else
		//	printf("recv failed with error: %d\n", WSAGetLastError());

	} while (iResult > 0);
	//printf("%s",response->c_str());
	// cleanup
	closesocket(ConnectSocket);
	WSACleanup();

	return response;
}