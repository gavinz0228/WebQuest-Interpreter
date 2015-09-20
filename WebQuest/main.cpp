#include "Tokenizer.h"
#include "WebRequest.h"
#include "ParseTree.h"
#include "RunTime.h"

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
	//rt.Run("print('asdf')");
	//rt.Run("a='yea' b=true print(b)");
	rt.Run("if true a='yes' else a='no' end print(a)");
}
void Parse()
{

	ParseTree parser;
	//parser.Parse("ab=1+2\r\n");
	//parser.Parse("a=!3>2");
	parser.Parse("if a==3 && b==2 a=1 b=2 elseif a==43 accc=5 else b=2 end");
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