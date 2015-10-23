#include "WQRequest.h"
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

	curl_easy_setopt(curl,CURLOPT_SSLCERT, "ca-bundle.crt");
	curl_easy_setopt(curl,CURLOPT_CAINFO, "ca-bundle.crt");
#ifdef SKIP_PEER_VERIFICATION
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
#endif

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
curl_slist *  WQRequest::SetHeaders( map<string, string>& headers)
{
	CURLcode res;
	struct curl_slist *chunk = NULL;
	map<string, string>::iterator it = headers.begin();
	for (; it != headers.end(); it++)
	{
		chunk=curl_slist_append(chunk,(it->first+": "+it->second).c_str() );
	}
	return chunk;
}

CURLcode WQRequest::HTTPGet(const string& url, map<string, string>& headers, ostream& os, long timeout)
{
	CURLcode code(CURLE_FAILED_INIT);
	CURL* curl;
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
		res=curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, &WQRequest::DataWrite);
		res=curl_easy_setopt(curl, CURLOPT_FILE, &os);
		res = curl_easy_setopt(curl, CURLOPT_TIMEOUT, timeout);
		struct curl_slist * hlist = NULL;
		hlist=SetHeaders( headers);
		res = curl_easy_setopt(curl, CURLOPT_HTTPHEADER, hlist);
		res = curl_easy_perform(curl);
		curl_easy_cleanup(curl);
	}
	return code;
}

CURLcode WQRequest::HTTPPostForm(const string& url, string& data, map<string, string>& headers, ostream& os, long timeout)
{
	CURL *curl;
	CURLcode res;

	/* In windows, this will init the winsock stuff */
	curl_global_init(CURL_GLOBAL_ALL);

	/* get a curl handle */
	curl = curl_easy_init();
	if (curl) {
		/* First set the URL that is about to receive our POST. This URL can
		just as well be a https:// URL if that is what should receive the
		data. */
		curl_easy_setopt(curl, CURLOPT_URL, url);
		res = curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, &WQRequest::DataWrite);
		res = curl_easy_setopt(curl, CURLOPT_FILE, &os);
		/* Now specify the POST data */
		res=curl_easy_setopt(curl, CURLOPT_POSTFIELDS, data.c_str());
		struct curl_slist * hlist = NULL;
		hlist = SetHeaders(headers);
		if (hlist!=NULL)
			res = curl_easy_setopt(curl, CURLOPT_HTTPHEADER, hlist);
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