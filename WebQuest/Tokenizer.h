#include <list>
#include <regex>
#include<iostream>
#include "Converter.h"
#include "Exception.h"
using namespace std;
#ifndef TOKENIZER_H
#define TOKENIZER_H
enum TokenType :char{
	TK_OPERATOR = 'O',
	TK_STRING = 'S',
	TK_VARIABLE = 'V',
	TK_INTEGER = 'I',
	TK_FLOAT = 'F',
	TK_BOOLEAN = 'B',
	TK_NEWLINE = 'L',
	TK_TAB = 'T',
	TK_WHILE = 'W',
	TK_IF = 'U',
	TK_ELSE = 'X',
	TK_ELSEIF = 'Y',
	TK_END = 'Z',
	TK_FOR = 'R',
	TK_IN = 'N',
	TK_BREAK = 'K'
	//,
	//TK_CREATELIST,
	//TK_CREATEDICT
};


class Token
{
public:
	Token(char* str,int length,char type,long lineno);
	Token(long long integer, long lineno);
	Token(long double number, long lineno);
	~Token();
	char Type;
	string* Symbol;
	long long Integer;
	long double Float;
	long lineno;
};
class Tokenizer
{
public:
	string Data;
	Tokenizer();
	~Tokenizer();
	list<Token*>* Tokenize(string script);
	bool IsOperator(char* script, int &charlen, char*& operatorstr,int& operatorlen);
	bool IsInteger(string::iterator startit, string::iterator endit, int &charlen);
	bool IsFloat(string::iterator startit, string::iterator endit, int &charlen);

	bool IsSpaceTabOrNewLine(char* script);
	int GetTab(string::iterator startit, string::iterator endit, int &charlen);
	int GetNewLine(string::iterator startit, string::iterator endit, int &charlen);
	static char* OPERATORS[];
	long GetNextLineNumber();
	void Clear();
	Token* NextToken();
	Token* LookAhead();
	void InitIterator();
	bool IsNextNumber();
	bool IsNextSymbol();
	bool IsNextOperator();
	bool IsNextNewLine();
	bool IsNextString();
	bool IsNextVariable();
	bool IsNextComma();
	bool IsNextRightParen();
	bool IsNextLeftParen();
	bool IsNextLeftBracket();
	bool IsNextRightBracket();
	bool IsNextRightCurlyBracket();
	bool IsNextLeftCurlyBracket();
	bool IsNextComparisonOperator();
	bool IsNextArithmeticOperator();
	bool IsNextColon();
	bool IsNextAndOperator();
	bool IsNextOrOperator();
	bool IsNextNotOperator();
	bool IsNextIfKeyword();
	bool IsNextElseKeyword();
	bool IsNextElseIfKeyword();
	bool IsNextEndKeyword();
	bool IsNextEndBlock();
	bool IsNextWhileKeyword();
	//+=,-=,*= etc..
	bool IsNextAssignment();
	bool IsNextForKeyword();
	bool IsNextInKeyword();
	bool IsNextBreakKeyword();
	//void MoveBack();
private:
	bool StartGettingToken;
	list<Token*>* Tokens;
	list<Token*>::iterator iterator;

};


#define				OP_COMMENT_SYMBOL			"#"
#define				OP_L_PAREN					"("
#define				OP_R_PAREN					")"
#define				OP_L_BRAC					"["
#define				OP_R_BRAC					"]"
#define				OP_L_CURLYBRAC				"{"
#define				OP_R_CURLYBRAC				"}"
#define				OP_L_ANGLEBRAC				"<"
#define				OP_R_ANGLEBRAC				">"
#define				OP_MULTIPLY					"*"
#define				OP_DEVIDE					"/"
#define				OP_PLUS						"+"
#define				OP_MINUS					"-"
#define				OP_MODULO					"%"
#define				OP_ASSIGN					"="
#define				OP_PLUSASSIGN				"+="
#define				OP_MINUSASSIGN				"-="
#define				OP_MULTIPLYASSIGN			"*="
#define				OP_DEVIDEASSIGN				"/="
#define				OP_MODULOASSIGN				"%="
#define				OP_AND						"&&"
#define				OP_OR						"||"
#define				OP_NOT						"!"
#define				OP_GREATER					">"
#define				OP_LESS						"<"
#define				OP_GREATEQUAL				">="
#define				OP_LESSEQUAL				"<="
#define				OP_EQUAL					"=="
#define				OP_COMMA					","
#define				OP_COLON					":"

#define				KW_TRUE						"true"
#define				KW_FALSE					"false"
#define				KW_WHILE					"while"
#define				KW_IF						"if"
#define				KW_ELSE						"else"
#define				KW_ELSEIF					"elseif"
#define				KW_END						"end"
#define				KW_CONTINUE					"continue"
#define				KW_BREAK					"break"
#define				KW_FOR						"for"
#define				KW_IN						"in"
#define				KW_BREAK					"break"



#endif
