#include <list>
#include "Converter.h"
using namespace std;
#ifndef TOKENIZER_H
#define TOKENIZER_H
enum TokenType:char{TK_OPERATOR='O',TK_STRING='S',TK_VARIABLE='V',TK_INTEGER='I',TK_FLOAT='F',TK_NEWLINE='N',TK_TAB='T',TK_IF='U',TK_ELSE='X',TK_ELSEIF='Y',TK_END='Z'};

class Token
{
public:
	Token(char* str,int length,char type);
	Token(long long integer);
	Token(long double number);
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
	bool IsOperator(char* script, int &charlen);
	bool IsSpaceTabOrNewLine(char* script);
	int GetTab(char* script,int &charlen);
	int GetNewLine(char* script,int &charlen);
	static char* OPERATORS[];
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

	//void MoveBack();
private:
	bool StartGettingToken;
	list<Token*>* Tokens;
	list<Token*>::iterator iterator;

};



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

#define				KW_IF						"if"
#define				KW_ELSE						"else"
#define				KW_ELSEIF					"elseif"
#define				KW_END						"end"
#define				KW_CONTINUE					"continue"
#define				KW_BREAK					"break"
#endif
