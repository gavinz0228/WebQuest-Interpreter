#include "Tokenizer.h"

Token::Token(char* str, int length, char type, long lineno)
{
	this->lineno = lineno;
	Symbol =new string( str,length);
	Type = type;
}
Token::Token(long long integer, long lineno)
{
	this->lineno = lineno;
	Integer = integer;
	Type = TK_INTEGER;
}
Token::Token(long double number, long lineno)
{
	this->lineno = lineno;
	Float = number;
	Type = TK_FLOAT;
}
Token::~Token()
{
	if (Symbol != NULL)
		delete Symbol;
}
//-----------------------------------------------------------------------------------------------------------------
//Rule Number 1. this array ends with a NULL
//Rule Number 2. Longer item should  be placed before the shorter ones.
// for example:  <= before < 
//== before =
//etc...
char*  Tokenizer::OPERATORS[] {OP_COMMA,OP_AND, OP_OR, OP_NOT, OP_GREATEQUAL, OP_LESSEQUAL, OP_EQUAL, OP_GREATER, OP_LESS,
OP_PLUSASSIGN, OP_MINUSASSIGN, OP_MULTIPLYASSIGN, OP_DEVIDEASSIGN, OP_MODULOASSIGN,
	OP_L_PAREN, OP_R_PAREN, OP_L_BRAC, OP_R_BRAC, OP_L_CURLYBRAC, OP_R_CURLYBRAC, OP_L_ANGLEBRAC, OP_R_ANGLEBRAC,
	OP_COLON,
OP_MULTIPLY, OP_DEVIDE, OP_PLUS, OP_MINUS, OP_MODULO, OP_ASSIGN,


 NULL};

Tokenizer::Tokenizer()
{
	Tokens = new list <Token*> ;
	StartGettingToken = false;
	CurrentLineNumber = 0;
}
Tokenizer::~Tokenizer()
{
	this->Clear();
}
list<Token*>* Tokenizer::Tokenize(string& script)
{
	
	this->Clear();
	long long integer;
	long double floatno;
	char* operatorStart=NULL;
	int operatorlen;
	CurrentLineNumber = 1;
	string::iterator it = script.begin();
	int charlen;

	while (it!=script.end())
	{
		if (*it==' ')
		{
			//just to skip the space
			it++;
		}
		else if (*it == '\t')
		{
			GetTab(it,script.end(), charlen);
			//Token* tk = new Token(&(*it), charlen, TK_TAB, lineno);
			//Tokens->push_back(tk);
			while (charlen>0)
			{
				it++;
				charlen--;
			}
			
		}
		else if (*it == '\r' || *it == '\n')
		{
			int num = GetNewLine(it, script.end(),charlen);
			//Token* tk = new Token(&(*it), charlen, TK_NEWLINE, lineno);
			//Tokens->push_back(tk);
			CurrentLineNumber += num;
			while (charlen>0)
			{
				it++;
				charlen--;
			}
		}
		else if (*it == '#')
		{
			//skip this line of comment
			while (it != script.end()&&*it != '\n' && *it != '\r')
			{
				it++;
			}
		}
		//return true if it's operator, operatorlen outputs the length of the operator


		else if (IsOperator(&(*it), charlen, operatorStart, operatorlen)
			|| IsFloat(it, script.end(), charlen)
			|| IsInteger(it, script.end(), charlen)
			)
		{
			//situation like (-3) ,a==-2, a=-4, [-3:-5]
			//in this case first parse number
			if (!Tokens->empty()
				&& Tokens->back()->Type == TK_OPERATOR
				&& (
				*(Tokens->back()->Symbol) == OP_ASSIGN
				|| *(Tokens->back()->Symbol) == OP_EQUAL
				|| *(Tokens->back()->Symbol) == OP_L_PAREN
				|| *(Tokens->back()->Symbol) == OP_ASSIGN)
				)
			{

				if (IsFloat(it, script.end(), charlen))
				{
					Converter::StringToFloat(&(*it), charlen, floatno);

					Token* tk = new Token(floatno, CurrentLineNumber);
					Tokens->push_back(tk);
					while (charlen > 0)
					{
						it++;
						charlen--;
					}
				}
				else if (IsInteger(it, script.end(), charlen))
				{
					Converter::StringToInteger(&(*it), charlen, integer);

					Token* tk = new Token(integer, CurrentLineNumber);
					Tokens->push_back(tk);
					while (charlen > 0)
					{
						it++;
						charlen--;
					}
				}
				else if (IsOperator(&(*it), charlen, operatorStart, operatorlen))
				{
					Token* tk = new Token(operatorStart, operatorlen, TK_OPERATOR, CurrentLineNumber);
					Tokens->push_back(tk);
					while (charlen > 0)
					{
						it++;
						charlen--;
					}
				}
			}
			//otherwise first parse operators
			else
			{
				if (IsOperator(&(*it), charlen, operatorStart, operatorlen))
				{
					Token* tk = new Token(operatorStart, operatorlen, TK_OPERATOR, CurrentLineNumber);
					Tokens->push_back(tk);
					while (charlen > 0)
					{
						it++;
						charlen--;
					}
				}
				else if (IsFloat(it, script.end(), charlen))
				{
					Converter::StringToFloat(&(*it), charlen, floatno);

					Token* tk = new Token(floatno, CurrentLineNumber);
					Tokens->push_back(tk);
					while (charlen > 0)
					{
						it++;
						charlen--;
					}
				}
				else if (IsInteger(it, script.end(), charlen))
				{
					Converter::StringToInteger(&(*it), charlen, integer);

					Token* tk = new Token(integer, CurrentLineNumber);
					Tokens->push_back(tk);
					while (charlen > 0)
					{
						it++;
						charlen--;
					}
				}
			}
		}
		//else if (IsFloat(it, script.end(), charlen))
		//{
		//	Converter::StringToFloat(&(*it), charlen, floatno);

		//	Token* tk = new Token(floatno, CurrentLineNumber);
		//	Tokens->push_back(tk);
		//	while (charlen>0)
		//	{
		//		it++;
		//		charlen--;
		//	}
		//}
		//else if (IsInteger(it, script.end(), charlen))
		//{
		//	Converter::StringToInteger(&(*it), charlen, integer);

		//	Token* tk = new Token(integer, CurrentLineNumber);
		//	Tokens->push_back(tk);
		//	while (charlen>0)
		//	{
		//		it++;
		//		charlen--;
		//	}
		//}

		//must be variable name,string, or numbers
		else
		{
			char* start = &(*it);
			char* ch = &(*it);
			bool gettingString = false;
			bool finishedString = false;
			char quotechar = '\"';
			int len = 0;
			if (*start == '\'' || *start == '\"')
			{
				gettingString = true;
				quotechar = *start;
				ch++;
				it++;
			}
			//not a operator , then see it as a symbol at this stage

			while (it != script.end())
			{	

				if (gettingString == false && (IsOperator(ch, charlen,operatorStart,operatorlen) || IsSpaceTabOrNewLine(ch)))
					break;
				else if (gettingString == true)
				{
					if (*ch == quotechar)
					{
						ch++;
						it++;
						finishedString = true;
						break;
					}
				}
				ch++;
				it++;

			}

			len = ch - start;
			Token* tk;
			//if the first character is a slash, then it's a string
			if (gettingString==true )
			{
				if (finishedString==true)
				{
					//skip the quote at the begining and the end;
					tk = new Token(start + 1, len - 2, TK_STRING, CurrentLineNumber);
				}
				else
					throw SYNTAX_STRING_MISSING_QUOTE;
			}

			else if (len == strlen(KW_ELSEIF)&&strncmp(start, KW_ELSEIF, len) == 0)
			{
				tk = new Token(start, len, TK_ELSEIF, CurrentLineNumber);
			}
			else if (len == strlen(KW_IF) && strncmp(start, KW_IF, len) == 0)
			{
				tk = new Token(start, len, TK_IF, CurrentLineNumber);
			}
			else if (len == strlen(KW_ELSE) && strncmp(start, KW_ELSE, len) == 0)
			{
				tk = new Token(start, len, TK_ELSE, CurrentLineNumber);
			}
			else if (len == strlen(KW_END) && strncmp(start, KW_END, len) == 0)
			{
				tk = new Token(start, len, TK_END, CurrentLineNumber);
			}
			else if (len == strlen(KW_WHILE) && strncmp(start, KW_WHILE, len) == 0)
			{
				tk = new Token(start, len, TK_WHILE, CurrentLineNumber);
			}
			else if (len == strlen(KW_FOR) && strncmp(start, KW_FOR, len) == 0)
			{
				tk = new Token(start, len, TK_FOR, CurrentLineNumber);
			}
			else if (len == strlen(KW_IN) && strncmp(start, KW_IN, len) == 0)
			{
				tk = new Token(start, len, TK_IN, CurrentLineNumber);
			}
			else if (len == strlen(KW_DEF) && strncmp(start, KW_DEF, len) == 0)
			{
				tk = new Token(start, len, TK_DEF, CurrentLineNumber);
			}
			else if (len == strlen(KW_BREAK) && strncmp(start, KW_BREAK, len) == 0)
			{
				tk = new Token(start, len, TK_BREAK, CurrentLineNumber);
			}
			else if (len == strlen(KW_CONTINUE) && strncmp(start, KW_CONTINUE, len) == 0)
			{
				tk = new Token(start, len, TK_CONTINUE, CurrentLineNumber);
			}
			else if (len == strlen(KW_NULL) && strncmp(start, KW_NULL, len) == 0)
			{
				tk = new Token(start, len, TK_NULL, CurrentLineNumber);
			}
			else if (len == strlen(KW_RETURN) && strncmp(start, KW_RETURN, len) == 0)
			{
				tk = new Token(start, len, TK_RETURN, CurrentLineNumber);
			}
			else if ((len == strlen(KW_TRUE) && strncmp(start, KW_TRUE, len) == 0) ||
				len == strlen(KW_FALSE) && strncmp(start, KW_FALSE, len) == 0
				)
			{
				tk = new Token(start, len, TK_BOOLEAN, CurrentLineNumber);
			}
			//else if (Converter::StringToInteger(start, len, integer))
			//{
			//	tk = new Token(integer);
			//}
			//else if (Converter::StringToFloat(start, len, floating))
			//{
			//	tk = new Token(floating);
			//}
			//else if (len == strlen(KW_CREATEDICT) && strncmp(start, KW_CREATEDICT, len) == 0)
			//{
			//	tk = new Token(start, len, TK_CREATEDICT);
			//}
			//else if (len == strlen(KW_CREATELIST) && strncmp(start, KW_CREATELIST, len) == 0)
			//{
			//	tk = new Token(start, len, TK_CREATELIST);
			//}
			else
			{
				tk = new Token(start, len, TK_VARIABLE, CurrentLineNumber);
			}
			//while (len > 0)
			//{
			//	it++;
			//	len--;
			//}

			Tokens->push_back(tk);
		}
	}
	return Tokens;
}
long Tokenizer::GetNextLineNumber()
{
	Token* next = LookAhead();
	if (next == NULL)
		return 0;
	else
		return next->lineno;
}
bool Tokenizer::IsInteger(string::iterator startit, string::iterator endit, int &charlen)
{
	regex integer("^(\\+|-)?[[:digit:]]+");
	regex_iterator<std::string::iterator> wordstart(startit, endit, integer, regex_constants::match_continuous);
	std::regex_iterator<std::string::iterator> wordend;
	if (wordstart != wordend)
	{
		charlen=wordstart->str().length();
		//charlen = distance(words_begin, words_end);
		return true;
	}
	else
		return false;
}
bool Tokenizer::IsFloat(string::iterator startit, string::iterator endit, int &charlen)
{
	regex floatexp("^(\\+|-)?[\\d]*[.]{1}\\d+");
	regex_iterator<std::string::iterator> wordstart(startit, endit, floatexp, regex_constants::match_continuous);
	std::regex_iterator<std::string::iterator> wordend;
	if (wordstart != wordend)
	{
		charlen = wordstart->str().length();
		//charlen = distance(words_begin, words_end);
		return true;
	}
	else
		return false;
}
void Tokenizer::InitIterator()
{
	if (StartGettingToken == false)
	{
		StartGettingToken = true;
		iterator = Tokens->begin();
	}
}
Token* Tokenizer::NextToken()
{
	InitIterator();
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
	InitIterator();
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
int Tokenizer::GetTab(string::iterator startit, string::iterator endit, int &charlen)
{
	charlen = 0;
	while (startit != endit&&*startit == '\t')
	{
		charlen++;
		startit++;
	}
	return charlen;
}
int Tokenizer::GetNewLine(string::iterator startit, string::iterator endit, int &charlen)
{
	charlen = 0;
	int linenum=0;
	while (startit != endit&&(*startit == '\r' || *startit == '\n'))
	{
		charlen++;
		if (*startit == '\n')
			linenum++;
		startit++;
	}
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

bool Tokenizer::IsOperator(char* script, int &scriptlen, char*& operatorstr, int& operatorlen)
{
	bool matched ;
	for (char** op = Tokenizer::OPERATORS; *op != NULL; op++)
	{
		matched = true;
		int oplen = strlen(*op);
		int scriptindex = 0;
		for (int i = 0; i < oplen; i++,scriptindex++)
		{
			while (*(script + scriptindex)==' ')
			{
				scriptindex++;
			}
			if (*(*op + i) != *(script + scriptindex))
			{
				matched = false;
				break;
			}
		}
		if (matched)
		{
			scriptlen = scriptindex;
			operatorstr = *op;
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
	return next != NULL&&next->Type==TK_OPERATOR&& *next->Symbol==OP_COMMA;
}
bool Tokenizer::IsNextLeftParen()
{
	Token* next = LookAhead();
	return next != NULL&&next->Type==TK_OPERATOR&& *next->Symbol ==OP_L_PAREN;
}
bool Tokenizer::IsNextLeftBracket()
{
	Token* next = LookAhead();
	return next != NULL&&next->Type == TK_OPERATOR&&*next->Symbol == OP_L_BRAC;
}
bool Tokenizer::IsNextRightBracket()
{
	Token* next = LookAhead();
	return next != NULL&&next->Type == TK_OPERATOR&&*next->Symbol == OP_R_BRAC;
}
bool Tokenizer::IsNextRightParen()
{
	Token* next = LookAhead();
	return next != NULL&&next->Type==TK_OPERATOR&& *next->Symbol == OP_R_PAREN;
}
bool Tokenizer::IsNextColon()
{
	Token* next = LookAhead();
	return next != NULL&&next->Type==TK_OPERATOR&&*next->Symbol == OP_COLON;

}
bool Tokenizer::IsNextArithmeticOperator()
{
	Token* next = LookAhead();
	return next != NULL &&next->Type==TK_OPERATOR && (*next->Symbol == OP_PLUS ||
		*next->Symbol == OP_MINUS||
		*next->Symbol == OP_MULTIPLY||
		*next->Symbol == OP_DEVIDE||
		*next->Symbol== OP_MODULO);
}
bool Tokenizer::IsNextComparisonOperator()
{
	Token* next = LookAhead();
	return next != NULL&&next->Type == TK_OPERATOR&&(*next->Symbol == OP_GREATER ||
		*next->Symbol == OP_GREATEQUAL ||
		*next->Symbol == OP_EQUAL ||
		*next->Symbol == OP_LESS ||
		*next->Symbol == OP_LESSEQUAL);

}
bool Tokenizer::IsNextIfKeyword()
{
	Token* next = LookAhead();
	return next != NULL&&next->Type==TK_IF;
}
bool Tokenizer::IsNextElseKeyword()
{
	Token* next = LookAhead();
	return next != NULL&&next->Type == TK_ELSE;
}
bool Tokenizer::IsNextElseIfKeyword()
{
	Token* next = LookAhead();
	//char b = TK_END;
	//char a =  TK_ELSEIF;
	//char c = TK_ELSE;
	return next != NULL&&(next->Type==TK_ELSEIF);
}
bool Tokenizer::IsNextEndKeyword()
{
	Token* next = LookAhead();
	return next != NULL&&(next->Type == TK_END);
}
bool Tokenizer::IsNextEndBlock()
{
	return  IsNextEndKeyword() || IsNextElseKeyword()||IsNextElseIfKeyword();
}
bool Tokenizer::IsNextAndOperator()
{
	Token* next = LookAhead();
	return next != NULL && next->Type == TK_OPERATOR&&*next->Symbol == OP_AND;
}
bool Tokenizer::IsNextOrOperator()
{
	Token* next = LookAhead();
	return next != NULL && next->Type == TK_OPERATOR&&*next->Symbol == OP_OR;
}
bool Tokenizer::IsNextNotOperator()
{
	Token* next = LookAhead();
	return next != NULL &&next->Type==TK_OPERATOR&& (*next->Symbol == OP_NOT);
}
bool Tokenizer::IsNextWhileKeyword()
{
	Token* next = LookAhead();
	return next != NULL&&next->Type == TK_WHILE;
}
bool Tokenizer::IsNextLeftCurlyBracket()
{
	Token* next = LookAhead();
	return next != NULL&&next->Type == TK_OPERATOR&&(*next->Symbol==OP_L_CURLYBRAC);
}
bool Tokenizer::IsNextRightCurlyBracket()
{
	Token* next = LookAhead();
	return next != NULL&&next->Type == TK_OPERATOR && (*next->Symbol == OP_R_CURLYBRAC);
}
bool Tokenizer::IsNextAssignment()
{
	Token* next = LookAhead();
	return next != NULL&&next->Type == TK_OPERATOR && (*next->Symbol ==OP_ASSIGN ||
		*next->Symbol == OP_PLUSASSIGN ||
		*next->Symbol == OP_MINUSASSIGN ||
		*next->Symbol == OP_DEVIDEASSIGN ||
		*next->Symbol == OP_MULTIPLYASSIGN ||
		*next->Symbol == OP_MODULOASSIGN);
}
bool Tokenizer::IsNextForKeyword()
{
	Token* next = LookAhead();
	return next != NULL&&next->Type == TK_FOR;
}
bool Tokenizer::IsNextInKeyword()
{
	Token* next = LookAhead();
	return next != NULL&&next->Type == TK_IN;
}
bool Tokenizer::IsNextBreakKeyword()
{
	Token* next = LookAhead();
	return next != NULL&&next->Type == TK_BREAK;
}
bool Tokenizer::IsNextDefKeyword()
{
	Token* next = LookAhead();
	return next != NULL&&next->Type == TK_DEF;
}
bool Tokenizer::IsNextReturnKeyword()
{
	Token* next = LookAhead();
	return next != NULL&&next->Type == TK_RETURN;
}
bool Tokenizer::IsNextContinueKeyword()
{
	Token* next = LookAhead();
	return next != NULL&&next->Type == TK_CONTINUE;
}