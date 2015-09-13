#include <stdlib.h>
#include <string>
#ifndef EXCEPTION_H
#define EXCEPTION_H

#define		INVALID_FUNCTION_CALL					"Expecting a comma or right parenthesis"

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
