#include <stdlib.h>
#include <string>
#ifndef EXCEPTION_H
#define EXCEPTION_H

#define		SYNTAX_INVALID_FUNCTION_CALL					"Expecting a comma or right parenthesis"
#define		SYNTAX_INVALID_EXPRESSION						"Invalid expression"
#define		SYNTAX_EXPECTING_END_IF							"Expecting an 'end' for the if block"
#define		SYNTAX_EXPECTING_RIGHT_PAREN					"Expecting an right parentheise"
#define		SYNTAX_STRING_MISSING_QUOTE						"Missing Quote at the end of the string"

#define		RUNTIME_FUNCTION_NOT_DEFINED					"Function is not define"


class Exception :std::exception
{
public:
	explicit Exception(const char* message)
	{
		this->message = message;
	}
	std::string GetMessage()
	{
		return this->message;
	}
protected:
	std::string message;
};
#endif
