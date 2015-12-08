#include "WQRequest.h"
//bool WQRequest::ShowHeaders = true;
WQRequest::WQRequest()
{

}
size_t WQRequest::DataWrite(void* buf, size_t size, size_t nmemb, void* userp)
{
	if (userp)
	{
		std::ostream& os = *static_cast<std::ostream*>(userp);
		std::streamsize len = size * nmemb;
		if (os.write(static_cast<char*>(buf), len))
			return len;
	}
	return 0;
}


/**
* timeout is in seconds
**/
CURLcode WQRequest::HTTPSRequest(const string& url, ostream& os, long timeout)
{
	CURL *curl;
	CURLcode res;

	curl_global_init(CURL_GLOBAL_DEFAULT);

	curl = curl_easy_init();
	if (curl) {
		curl_easy_setopt(curl, CURLOPT_URL, url);

	//curl_easy_setopt(curl,CURLOPT_SSLCERT, "ca-bundle.crt");
	//curl_easy_setopt(curl,CURLOPT_CAINFO, "ca-bundle.crt");
//#ifdef SKIP_PEER_VERIFICATION
		/*
		* If you want to connect to a site who isn't using a certificate that is
		* signed by one of the certs in the CA bundle you have, you can skip the
		* verification of the server's certificate. This makes the connection
		* A LOT LESS SECURE.
		*
		* If you have a CA cert for the server stored someplace else than in the
		* default bundle, then the CURLOPT_CAPATH option might come handy for
		* you.
		*/
		curl_easy_setopt(curl, CURLOPT_SSL_VERIFYPEER, 0L);
//#endif

#ifdef SKIP_HOSTNAME_VERIFICATION
		/*
		* If the site you're connecting to uses a different host name that what
		* they have mentioned in their server certificate's commonName (or
		* subjectAltName) fields, libcurl will refuse to connect. You can skip
		* this check, but this will make the connection less secure.
		*/
		curl_easy_setopt(curl, CURLOPT_SSL_VERIFYHOST, 0L);
#endif
		
		/* Perform the request, res will get the return code */
		res = curl_easy_perform(curl);
		/* Check for errors */
		if (res != CURLE_OK)
			fprintf(stderr, "curl_easy_perform() failed: %s\n",
			curl_easy_strerror(res));

		/* always cleanup */
		curl_easy_cleanup(curl);
	}

	curl_global_cleanup();
	printf(GetCurrentDir().c_str());
	return res;

}
CURLcode WQRequest::GetResponse(const std::string& url, std::ostream& os, long timeout )
{
	CURLcode code(CURLE_FAILED_INIT);
	CURL* curl;
	
	curl = curl_easy_init();

	if (curl)
	{

		if (CURLE_OK == (code = curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, &WQRequest::DataWrite))
			&& CURLE_OK == (code = curl_easy_setopt(curl, CURLOPT_NOPROGRESS, 1L))
			&& CURLE_OK == (code = curl_easy_setopt(curl, CURLOPT_FOLLOWLOCATION, 1L))
			&& CURLE_OK == (code = curl_easy_setopt(curl, CURLOPT_FILE, &os))
			&& CURLE_OK == (code = curl_easy_setopt(curl, CURLOPT_TIMEOUT, timeout))
			&& CURLE_OK == (code = curl_easy_setopt(curl, CURLOPT_URL, url.c_str())))
		{
			code = curl_easy_perform(curl);
		}
		curl_easy_cleanup(curl);
	}
	return code;
}


string WQRequest::PairsToURLParameters(map<string, string>& params)
{
	string urlparam = "";
	map<string, string>::iterator it = params.begin();
	for (; it != params.end(); it++)
	{
		urlparam += Uri::URLEncode(it->first) + "=" + Uri::URLEncode(it->second);
	}
	return urlparam;
}
curl_slist *  WQRequest::SetHeaders(map<string, string>& headers, struct curl_slist* chunk)
{
	CURLcode res;
	map<string, string>::iterator it = headers.begin();
	for (; it != headers.end(); it++)
	{
		chunk=curl_slist_append(chunk,(it->first+": "+it->second).c_str() );
	}
	return chunk;
}

curl_slist* WQRequest::SetCookies(struct curl_slist *chunk,string& hostname)
{
	string cookies;
	HTTPUtility::LoadCookiesString(cookies,hostname);
	chunk = curl_slist_append(chunk, ("Cookie: " + cookies).c_str());
	return chunk;
}

CURLcode WQRequest::HTTPGet(const string& url, map<string, string>& headers, ostream& os, long timeout)
{
	CURLcode code(CURLE_FAILED_INIT);
	CURL* curl;
	URL = Uri::Parse(url);
	curl = curl_easy_init();
	CURLcode res;

	if (curl)
	{
		//if (CURLE_OK == (code = curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, &WQRequest::DataWrite))
		//	&& CURLE_OK == (code = curl_easy_setopt(curl, CURLOPT_NOPROGRESS, 1L))
		//	&& CURLE_OK == (code = curl_easy_setopt(curl, CURLOPT_FOLLOWLOCATION, 1L))
		//	&& CURLE_OK == (code = curl_easy_setopt(curl, CURLOPT_FILE, &os))
		//	&& CURLE_OK == (code = curl_easy_setopt(curl, CURLOPT_TIMEOUT, timeout))
		//	&& CURLE_OK == (code = curl_easy_setopt(curl, CURLOPT_URL, url)))
		//{
		//	code = curl_easy_perform(curl);
		//}
		res=curl_easy_setopt(curl, CURLOPT_URL, url);
		res=curl_easy_setopt(curl, CURLOPT_SSL_VERIFYPEER, 0L);
		res=curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, &WQRequest::DataWrite);
		res=curl_easy_setopt(curl, CURLOPT_FILE, &os);
		res = curl_easy_setopt(curl, CURLOPT_TIMEOUT, timeout);
		res = curl_easy_setopt(curl, CURLOPT_HEADER, 1);
		struct curl_slist * hlist = NULL;
		SetCookies(hlist, URL.Host);
		hlist=SetHeaders( headers,hlist);
		res = curl_easy_setopt(curl, CURLOPT_HTTPHEADER, hlist);
		res = curl_easy_perform(curl);
		curl_easy_cleanup(curl);
		curl_slist_free_all(hlist);;
	}
	return code;
}

CURLcode WQRequest::HTTPPostForm(const string& url, string& data, map<string, string>& headers, ostream& os, long timeout)
{
	CURL *curl;
	CURLcode res;
	URL = Uri::Parse(url);
	/* In windows, this will init the winsock stuff */
	curl_global_init(CURL_GLOBAL_ALL);

	/* get a curl handle */
	curl = curl_easy_init();
	if (curl) {
		/* First set the URL that is about to receive our POST. This URL can
		just as well be a https:// URL if that is what should receive the
		data. */
		curl_easy_setopt(curl, CURLOPT_URL, url);
		res = curl_easy_setopt(curl, CURLOPT_SSL_VERIFYPEER, 0L);
		res = curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, &WQRequest::DataWrite);
		res = curl_easy_setopt(curl, CURLOPT_FILE, &os);
		res = curl_easy_setopt(curl, CURLOPT_TIMEOUT, timeout);
		/* Now specify the POST data */
		res=curl_easy_setopt(curl, CURLOPT_POSTFIELDS, data.c_str());
		struct curl_slist * hlist = NULL;
		SetCookies(hlist, URL.Host);
		hlist = SetHeaders(headers,hlist);
		if (hlist!=NULL)
			res = curl_easy_setopt(curl, CURLOPT_HTTPHEADER, hlist);
		res = curl_easy_setopt(curl, CURLOPT_HEADER, 1);
		/* Perform the request, res will get the return code */
		res = curl_easy_perform(curl);
		/* Check for errors */
		if (res != CURLE_OK)
			fprintf(stderr, "curl_easy_perform() failed: %s\n",
			curl_easy_strerror(res));

		/* always cleanup */
		curl_easy_cleanup(curl);
		curl_slist_free_all(hlist);

	}
	curl_global_cleanup();
	return res;
}

CURLcode WQRequest::HTTPPostJSON(const string& url, string& data, map<string, string>& headers, ostream& os, long timeout)
{
	CURL *curl;
	CURLcode res;
	URL = Uri::Parse(url);
	/* In windows, this will init the winsock stuff */
	curl_global_init(CURL_GLOBAL_ALL);

	/* get a curl handle */
	curl = curl_easy_init();
	if (curl) {
		/* First set the URL that is about to receive our POST. This URL can
		just as well be a https:// URL if that is what should receive the
		data. */
		res = curl_easy_setopt(curl, CURLOPT_URL, url);
		res = curl_easy_setopt(curl, CURLOPT_SSL_VERIFYPEER, 0L);
		res = curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, &WQRequest::DataWrite);
		res = curl_easy_setopt(curl, CURLOPT_FILE, &os);
		res = curl_easy_setopt(curl, CURLOPT_TIMEOUT, timeout);
		/* Now specify the POST data */
		res = curl_easy_setopt(curl, CURLOPT_POSTFIELDS, data.c_str());
		struct curl_slist * hlist = NULL;
		hlist = curl_slist_append(hlist, "Accept: application/json");
		hlist = curl_slist_append(hlist, "Content-Type: application/json");
		hlist = curl_slist_append(hlist, "charsets: utf-8");
		SetCookies(hlist, URL.Host);
		hlist = SetHeaders(headers,hlist);
		if (hlist != NULL)
			res = curl_easy_setopt(curl, CURLOPT_HTTPHEADER, hlist);
		res = curl_easy_setopt(curl, CURLOPT_HEADER, 1);
		/* Perform the request, res will get the return code */
		res = curl_easy_perform(curl);
		/* Check for errors */
		if (res != CURLE_OK)
			fprintf(stderr, "curl_easy_perform() failed: %s\n",
			curl_easy_strerror(res));

		/* always cleanup */
		curl_easy_cleanup(curl);
		curl_slist_free_all(hlist);
	}
	curl_global_cleanup();
	return res;
}

CURLcode WQRequest::HTTPPut(const string& url, string& data, map<string, string>&headers, ostream& os, long timeout)
{
	CURL *curl;
	CURLcode res;
	URL = Uri::Parse(url);
	/* In windows, this will init the winsock stuff */
	curl_global_init(CURL_GLOBAL_ALL);
	curl = curl_easy_init();

	if (curl) {
		
		res = curl_easy_setopt(curl, CURLOPT_URL, url);
		res = curl_easy_setopt(curl, CURLOPT_SSL_VERIFYPEER, 0L);
		res = curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, &WQRequest::DataWrite);
		res = curl_easy_setopt(curl, CURLOPT_FILE, &os);
		res = curl_easy_setopt(curl, CURLOPT_TIMEOUT, timeout);
		/* Now specify the POST data */
		res = curl_easy_setopt(curl, CURLOPT_POSTFIELDS, data.c_str());
		struct curl_slist * hlist = NULL;
		hlist = curl_slist_append(hlist, "Content-Type: application/json");
		hlist = curl_slist_append(hlist, "charsets: utf-8");
		if (hlist != NULL)
			res = curl_easy_setopt(curl, CURLOPT_HTTPHEADER, hlist);
		
		curl_easy_setopt(curl, CURLOPT_CUSTOMREQUEST, "PUT"); /* !!! */

		res = curl_easy_perform(curl);

		curl_slist_free_all(hlist);
		curl_easy_cleanup(curl);
	}
	curl_global_cleanup();
	return res;
}
CURLcode WQRequest::HTTPDelete(const string& url, string& data, map<string, string>&headers, ostream& os, long timeout)
{
	CURL *curl;
	CURLcode res = CURLE_OK;

	curl = curl_easy_init();
	if (curl) {
		/* Set username and password */
		curl_easy_setopt(curl, CURLOPT_USERNAME, "user");
		curl_easy_setopt(curl, CURLOPT_PASSWORD, "secret");

		/* This is just the server URL */
		curl_easy_setopt(curl, CURLOPT_URL, "imap://imap.example.com");

		/* Set the DELETE command specifing the existing folder */
		curl_easy_setopt(curl, CURLOPT_CUSTOMREQUEST, "DELETE FOLDER");

		/* Perform the custom request */
		res = curl_easy_perform(curl);

		/* Check for errors */
		if (res != CURLE_OK)
			fprintf(stderr, "curl_easy_perform() failed: %s\n",
			curl_easy_strerror(res));

		/* Always cleanup */
		curl_easy_cleanup(curl);
	}

	return res;
}
string WQRequest::GetCurrentDir()
{
	char path[300];
	GetModuleFileName(GetModuleHandle(NULL),path,sizeof(path));
	
	size_t length = strlen(path);
	int i=0;
	for (i = length - 1; i >= 0; i--)
	{
		if (path[i] == '\\')
			break;
	}
	string str( path,i+1);
	return str;

}