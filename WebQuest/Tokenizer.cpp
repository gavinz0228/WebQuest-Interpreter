#include "Tokenizer.h"

Token::Token(char* str, int length, char type)
{
	Symbol =new string( str,length);
	Type = type;
}
Token::Token(long long integer)
{
	Integer = integer;
	Type = TK_INTEGER;
}
Token::Token(long double number)
{
	Float = number;
	Type = TK_FLOAT;
}
Token::~Token()
{
	if (Symbol != NULL)
		delete Symbol;
}
//-----------------------------------------------------------------------------------------------------------------
char*  Tokenizer::OPERATORS[] { OP_L_PAREN, OP_R_PAREN, OP_L_BRAC, OP_R_BRAC, OP_L_CURLYBRAC, OP_R_CURLYBRAC, OP_L_ANGLEBRAC, OP_R_ANGLEBRAC,
OP_MULTIPLY, OP_DEVIDE, OP_PLUS, OP_MINUS, OP_MODULO, OP_ASSIGN,
OP_PLUSASSIGN, OP_MINUSASSIGN, OP_MULTIPLYASSIGN, OP_DEVIDEASSIGN, OP_MODULOASSIGN,
OP_AND, OP_OR, OP_GREATER, OP_LESS, OP_GREATEQUAL, OP_LESSEQUAL, OP_EQUAL, NULL};

Tokenizer::Tokenizer()
{
	Tokens = new list <Token*> ;
	StartGettingToken = false;
}
Tokenizer::~Tokenizer()
{
	this->Clear();
}
list<Token*>* Tokenizer::Tokenize(string script)
{
	this->Clear();

	char * ch = (char*)script.c_str();
	char * end = ch + script.length();
	int charlen;
	while (ch < end)
	{
		if (*ch==' ')
		{
			//just to skip the space
			ch++;
		}
		else if (*ch == '\t')
		{
			GetTab(ch, charlen);
			Token* tk = new Token(ch,charlen, TK_TAB);
			Tokens->push_back(tk);
			ch += charlen;
		}
		else if (*ch == '\r' || *ch == '\n')
		{
			int num = GetNewLine(ch, charlen);
			Token* tk = new Token(ch, charlen, TK_NEWLINE);
			Tokens->push_back(tk);
			ch += charlen;
		}

		//return true if it's operator, operatorlen outputs the length of the operator
		else if (IsOperator(ch, charlen))
		{

			Token* tk=new Token(ch, charlen,TK_OPERATOR);
			Tokens->push_back(tk);
			ch += charlen;
		}
		//must be variable name,string, or numbers
		else
		{
			char* start = ch;
			//not a operator , then see it as a symbol at this stage
			while (!IsOperator(ch, charlen) && !IsSpaceTabOrNewLine(ch)&& ch < end)
			{
				ch++;
			}
			long long integer;
			long double floating;
			Token* tk;
			//if the first character is a slash, then it's a string
			if (*start=='\"'||*start=='\'')
			{
				tk = new Token(start, ch - start, TK_STRING);
			}
			else if (Converter::StringToInteger(start, ch - start, integer))
			{
				tk = new Token(integer);
			}
			else if (Converter::StringToFloat(start, ch - start, floating))
			{
				tk = new Token(floating);
			}
			else
			{
				tk = new Token(start, ch - start, TK_VARIABLE);
			}
			
			Tokens->push_back(tk);
		}
	}
	return Tokens;
}
Token* Tokenizer::NextToken()
{
	if (StartGettingToken == false)
	{
		StartGettingToken = true;
		iterator = Tokens->begin();
	}
	if (iterator == Tokens->end())
		return NULL;
	else
	{
		Token* tk = *iterator;
		iterator++;
		return tk;
	}
}
//void Tokenizer::MoveBack()
//{
//	if (iterator != Tokens->begin())
//		iterator--;
//}
Token* Tokenizer::LookAhead()
{

	//list<Token*>::iterator temp_it = next(iterator);
	if (iterator != Tokens->end())
		return *iterator;
	else
		return NULL;
}
void Tokenizer::Clear()
{
	if (this->Tokens != NULL)
	{
		for (list<Token*>::iterator it = Tokens->begin(); it != Tokens->end(); it++)
			delete *it;
	}
	Tokens->clear();
}
int Tokenizer::GetTab(char* script,int& charlen)
{
	char* pos = script;
	while (*pos == '\t')
	{
		pos++;
	}
	charlen = pos - script;
	return charlen;
}
int Tokenizer::GetNewLine(char* script, int& charlen)
{
	char * pos = script;
	int linenum = 0;
	while (*pos == '\r' || *pos == '\n')
	{
		pos++;
		if (*pos == '\n')
			linenum++;
	}
	charlen = pos - script;
	return linenum;
}
bool Tokenizer::IsSpaceTabOrNewLine(char *script)
{
	switch (*script)
	{
		case '\n':
		case '\r':
		case '\t':
		case ' ':
			return true;
		default:
			return false;
		break;
	}
}

bool Tokenizer::IsOperator(char* script, int &operatorlen)
{
	bool matched ;
	for (char** op = Tokenizer::OPERATORS; *op != NULL; op++)
	{
		matched = true;
		int oplen = strlen(*op);
		for (int i = 0; i < oplen; i++)
		{
			if (*(*op+i) != *(script + i))
				matched=false;
		}
		if (matched)
		{
			operatorlen = oplen;
			return true;
		}
		//string onechar = string(*script, 1);
		//if (onechar == *op)
		//{
		//	operatorlen = 1;
		//	return true;
		//}
		//string twochar = string(*script, 2);
		//if (twochar == *op)
		//{
		//	operatorlen = 2;
		//	return true;
		//}
	}
	return false;
}
bool Tokenizer::IsNextNumber()
{
	Token* next=LookAhead();
	return next!=NULL&&(next->Type == TK_FLOAT || next->Type == TK_INTEGER);
}
bool Tokenizer::IsNextOperator()
{
	Token* next = LookAhead();
	return next != NULL && (next->Type == TK_OPERATOR);
}
bool Tokenizer::IsNextVariable()
{
	Token* next = LookAhead();
	return next != NULL&& next->Type == TK_VARIABLE;
}
bool Tokenizer::IsNextNewLine()
{
	Token* next = LookAhead();
	return next != NULL&& next->Type == TK_NEWLINE;
}
bool Tokenizer::IsNextString()
{
	Token* next = LookAhead();
	return next != NULL&& next->Type == TK_STRING;
}
bool Tokenizer::IsNextComma()
{
	Token* next = LookAhead();
	return next != NULL&& *next->Symbol==OP_COMMA;
}
bool Tokenizer::IsNextLeftParen()
{
	Token* next = LookAhead();
	return next != NULL&& *next->Symbol ==OP_L_PAREN;
}
bool Tokenizer::IsNextRightParen()
{
	Token* next = LookAhead();
	return next != NULL&& *next->Symbol == OP_R_PAREN;
}
bool Tokenizer::IsNextArithmeticOperator()
{
	Token* next = LookAhead();
	return next != NULL && (*next->Symbol==OP_PLUS||
		*next->Symbol == OP_MINUS||
		*next->Symbol == OP_MULTIPLY||
		*next->Symbol == OP_DEVIDE||
		*next->Symbol== OP_MODULO);
}
