#include "Tokenizer.h"
#include "WebRequest.h"
#include "Parser.h"
#include "WQRunTime.h"
#include <iostream>
#include <fstream>
using namespace std;
void Request();
void Tokenize();
void Parse();
void Execute(string& script);
int main(int argc, char **argv)
{
	//no parameter is passed in, then run test method
	if (argc == 1){
		//rt.Run("print('asdf')");
		//rt.Run("a='yea' b=true print(b)");
		//rt.Run("if true a='yes' else a='no' end print(a)");
		//rt.Run("a=0 while a<10  a=a+1 print(a) end");
		//rt.Run("aa=[234,234,234] append(aa,'aa') append(aa,'bb') append(aa,123)  print(aa)");
		//rt.Run("aa=['stringtest'] aa[0]=1 print(aa) ");
		//rt.Run("aa=0 aa=aa==2+2-4 print(aa)");
		//rt.Run("aa=1==2+5*3 print(aa)");
		//rt.Run("result=get(\"http://higavin.com\") print(result)");
		//rt.Run("变量=1 变量+ =1 print(变量)");
		//rt.Run("if (0==4-4+2+1-3 && true==1) print('good!') else print('bad') end ");
		//rt.Run("a=1 while a<=10 b=2 a+=1 a+=1 print(a)  end print(a)");
		//rt.Run("a=1 a+=1 print(a)");
		//rt.Run("lsvar=[123,456,789] for a in lsvar print(a) end");
		//rt.Run("lsvar=[123,456,789] for a in lsvar print(a+' ') break end");
		//rt.Run("a=[123] b=[] append(b,a)  append(a,'a') print(b)");
		// rt.Run("res=get_raw('http://google.com') print(len(res))");
		//rt.Run("res=get('http://www.google.com') print(res) #print(res)");
		//rt.Run("aa={} aa['key']='value' dump_json(aa)");
		//rt.Run("for i in range(0,500) print(i+' ') end");
		//rt.Run("for i in range(10) print(i) end");
		//rt.Run("res=get('https://maps.googleapis.com/maps/api/geocode/json?address=2900+bedford+avenue+brooklyn')\n js=parse_json(res)\n print(js['results'])");
		//rt.Run("res=get_raw('https://maps.googleapis.com/maps/api/geocode/json?address=2900+bedford+avenue+brooklyn') \
		//			   			    header=parse_headers(res) print(header)  \
		//											print('\n---------------------------------------\n') \
		//														");
		//rt.Run("print(milli())  str=' ' i=0 while i<=10000 str=str+'1' i+=1 end print(str) print('\n')  print(milli()) ");
		//rt.Run("a=[1,2,3] for i in a print(i+100) end");
		//rt.Run("print(1+1)");
		//rt.Run("a=100 begin a=range(100000) print(a) end print(a)");
		//rt.Run("a=[234,234,324] for i in a a[1]='asdfas' end print (a)");
		//rt.Run("a=2 a+=' ' print(a+'_')");
		//rt.Run("result=my_func('working') print(result) def my_func(param,second_param) print(param+' yay! function works!') return 'ok' end");
		//rt.Run("c=[] append(c,'s')  d=c[0] print(d)");
		//rt.Run("a={} a['sdfg']='dfff' res=get('http://requestb.in/14rhet81',a) \n \n print(res)");

		//rt.Run("js=parse_json('{\"aa\":\"asdfsf\",\"bb\":[123,456]}') print(js)");
		//string script = "aa='sdf' \r\n \r\n \r\n aa=get('https:/www.google.com') print(aa)";
		//string script = "print(milli()) range(10000) print(milli()) ";
		//string script = "a=-2 b=-2.1+2.1 print(a+b)";
		string script = "get('http://www.baidu.com')";
		//string script = "for i in range(1210) print(i + 100) end ";
		Execute(script);
	}
	else
		//read script from the file
	{
		string script;
		ifstream file(argv[1]);
		//ifstream file("C:\\Users\\Gavin\\OneDrive\\WebQuest\\Debug\\script.wq");
		file.seekg(0, std::ios::end);
		script.reserve(file.tellg());
		file.seekg(0, std::ios::beg);

		script.assign((std::istreambuf_iterator<char>(file)),
			std::istreambuf_iterator<char>());
		//printf(script.c_str());
		Execute(script);
	}
	char wait;
	scanf_s(&wait);
	//Request();
	//Tokenize();
	//Parse();
	//char wait;
	//scanf_s(&wait);
	return 0;
}
void Execute(string& script)
{
	WQRuntime rt;
	WQState state;
	try{

		rt.Run(script,&state);
	}
	catch (const std::exception& ex)
	{
		cout << "[Error: Line " << rt.GetCurrentLineNumber() << ":" << ex.what() <<"]"<< endl;
		rt.PrintCurrentLine();
	}
	catch (char* msg){
		cout << "[Error: Line " << rt.GetCurrentLineNumber() << ":" << msg << "]" << endl;
		rt.PrintCurrentLine();
	}
	catch (string &str){
		cout << "[Error: Line " << rt.GetCurrentLineNumber() << ":" << str << "]" << endl;
		rt.PrintCurrentLine();
	}
}
//void Parse()
//{
//
//	Parser parser;
//	//parser.Parse("ab=1+2\r\n");
//	//parser.Parse("a=!3>2");
//	//parser.Parse("if (a==3 && b==2) a=1 b=2 elseif a==43 accc=5 else b=2 end");
//	//parser.Parse("if (a==3) a=1 else b=2 end");
//	//parser.Parse("lsvar=[123,456,789] for a in lsvar print(a) end");
//	parser.Parse("a=[123,456,789] print(a[12:])");
//	parser.PrintTree();
//}
//void Tokenize()
//{
//	Tokenizer tker;
//	tker.Tokenize("dd=12.2\r\naa={}\r\n");
//	while (true)
//	{
//		Token* token = tker.NextToken();
//		if (token == NULL)
//			break;
//		if (token->Type==TK_OPERATOR||token->Type==TK_STRING)
//			printf((*token).Symbol->c_str());
//		else if (token->Type == TK_INTEGER)
//		{
//			printf("%d", token->Integer);
//		}
//		else if (token->Type == TK_FLOAT)
//		{
//			printf("%f", token->Float);
//		}
//		printf("\n");
//	}
//}
//
//void Request()
//{
//	WebRequest req;
//	string response;
//	map<string, string> params;
//	try
//	{
//		//req.AddHeader("User-Agent","Mozilla/5.0 (Windows NT 6.2; WOW64) AppleWebKit/537.36 (KHTML, like Gecko) Chrome/44.0.2403.130 Safari/537.36");
//		//response = req.PostBinary("http://posttestserver.com/post.php", "sa=a&ss=s&sdfa=asdfasdfasdfasdfasdfasdfasdf");
//
//		params.insert(pair<string, string>("asdf", "d&s"));
//		response = req.PostForm("http://posttestserver.com/post.php", params);
//		printf(response.c_str());
//	}
//	catch (const char* msg)
//	{
//		printf(msg);
//	}
//}