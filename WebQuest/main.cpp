#include "Tokenizer.h"
#include "WebRequest.h"
#include "Parser.h"
#include "RunTime.h"
#include <iostream>
using namespace std;
void Request();
void Tokenize();
void Parse();
void Evaluate();
int main(int argc, char **argv)
{

	//Request();
	//Tokenize();
	//Parse();
	Evaluate();
	char wait;
	scanf_s(&wait);
	return 0;
}
void Evaluate()
{
	Runtime rt;
	try{
		//rt.Run("print('asdf')");
		//rt.Run("a='yea' b=true print(b)");
		//rt.Run("if true a='yes' else a='no' end print(a)");
		//rt.Run("a=0 while a<10  a=a+1 print(a) end");
		//rt.Run("aa=[234,234,234] append(aa,'aa') append(aa,'bb') append(aa,123)  print(aa)");
		//rt.Run("aa=['stringtest'] aa[0]=1 print(aa) ");
		//rt.Run("aa=0 aa=aa==2+2-4 print(aa)");
		//rt.Run("aa=1==2+5*3 print(aa)");
		//rt.Run("result=get(\"http://higavin.com\") print(result)");
		rt.Run("aa=1 aa+  =1 print(aa)");
		//rt.Run("if (0==4-4+2+1-3 && true==1) print('good!') else print('bad') end ");
		//rt.Run("a=1 while a<=10 b=2 a+=1 a+=1 print(a)  end print(a)");
		//rt.Run("a=1 a+=1 print(a)");
		//rt.Run("lsvar=[123,456,789] for a in lsvar print(a) end");
		//rt.Run("lsvar=[123,456,789] for a in lsvar print(a+' ') break end");
		//rt.Run("a=[123,456,7890.02] print(a[0:3]) print(a[0:-1])");
		// rt.Run("res=get_raw('http://google.com') print(len(res))");
		//rt.Run("res=get_raw('http://google.com') print(res) #print(res)");
	}
	catch(string &str){
		cout << "Line: " << rt.GetCurrentLineNumber() << endl;
	}

}
void Parse()
{

	Parser parser;
	//parser.Parse("ab=1+2\r\n");
	//parser.Parse("a=!3>2");
	//parser.Parse("if (a==3 && b==2) a=1 b=2 elseif a==43 accc=5 else b=2 end");
	//parser.Parse("if (a==3) a=1 else b=2 end");
	//parser.Parse("lsvar=[123,456,789] for a in lsvar print(a) end");
	parser.Parse("a=[123,456,789] print(a[12:])");
	parser.PrintTree();
}
void Tokenize()
{
	Tokenizer tker;
	tker.Tokenize("dd=12.2\r\naa={}\r\n");
	while (true)
	{
		Token* token = tker.NextToken();
		if (token == NULL)
			break;
		if (token->Type==TK_OPERATOR||token->Type==TK_STRING)
			printf((*token).Symbol->c_str());
		else if (token->Type == TK_INTEGER)
		{
			printf("%d", token->Integer);
		}
		else if (token->Type == TK_FLOAT)
		{
			printf("%f", token->Float);
		}
		printf("\n");
	}
}

void Request()
{
	WebRequest req;
	string response;
	map<string, string> params;
	try
	{
		//req.AddHeader("User-Agent","Mozilla/5.0 (Windows NT 6.2; WOW64) AppleWebKit/537.36 (KHTML, like Gecko) Chrome/44.0.2403.130 Safari/537.36");
		//response = req.PostBinary("http://posttestserver.com/post.php", "sa=a&ss=s&sdfa=asdfasdfasdfasdfasdfasdfasdf");

		params.insert(pair<string, string>("asdf", "d&s"));
		response = req.PostPairs("http://posttestserver.com/post.php", params);
		printf(response.c_str());
	}
	catch (const char* msg)
	{
		printf(msg);
	}
}