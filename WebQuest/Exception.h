#include <stdlib.h>
#include <string>
#ifndef EXCEPTION_H
#define EXCEPTION_H

#define		SYNTAX_INVALID_FUNCTION_CALL					"Expecting a comma or right parenthesis"
#define		SYNTAX_INVALID_EXPRESSION						"Invalid expression"
#define		SYNTAX_EXPECTING_END							"Expecting an 'end' for the block"
#define		SYNTAX_EXPECTING_RIGHT_PAREN					"Expecting a right parentheise"
#define		SYNTAX_EXPECTING_RIGHT_BRACKET					"Expecting a right bracket"
#define		SYNTAX_STRING_MISSING_QUOTE						"Missing Quote at the end of the string"
#define		SYNTAX_INVALID_EXPRESSION						"Invalid expression"
#define		SYNTAX_CAN_NOT_ASSIGN							"Variable cannot be assigned"		
#define		SYNTAX_INVALID_OPERATOR							"Invalid operator"
#define		SYNTAX_INVALID_FOR_TEMP_VARIABLE				"Invalid temporary variable of a for loop"
#define		SYNTAX_NOT_IN_KEYWORD_IN_FOR					"Expecing a 'in' keyword in a for loop"
#define		SYNTAX_INVALID_SYMBOL							"Invalid symbol"
#define		SYNTAX_UNEXPECTING_SYMBOL						"Unexpected symbol"

#define		RUNTIME_FUNCTION_NOT_DEFINED					"Function is not define"
#define		RUNTIME_EXPECTING_BOOLEAN						"Expecting a boolean expression"
#define		RUNTIME_EXPECTING_NUMERIC						"Expecting a numeric value"
#define		RUNTIME_NON_NUMERIC_CASTING						"Non numeric variable cannot be casted to numeric value"
#define		RUNTIME_INVALID_TYPE_FOR_LESS_THAN				"Invalid type for < operator"
#define		RUNTIME_INVALID_TYPE_FOR_GREATER_THAN			"Invalid type for > operator"
#define		RUNTIME_INVALID_TYPE_FOR_LESS_EQUAL				"Invalid type for <= operator"
#define		RUNTIME_ITERATE_NON_LIST_VARIABLE				"Non-list variable cannot be iterated"
#define		RUNTIME_NON_INTERGER_INDEX						"Non-integer index for a list is not allowed"
#define		RUNTIME_SLICING_NON_LIST_VARIABLE				"Slicing a non-list variable is not allowed"

#define		RUNTIME_INVALID_TYPE_FOR_EQUAL					"Invalid type for == operator"
#define		RUNTIME_INVALID_TYPE_FOR_GREATER_EQUAL			"Invalid type for >= operator"

#define		RUNTIME_NON_LIST_APPENDING						"Cannot append value to a non-list variable"
#define		RUNTIME_NON_LIST_INDEXING						"Target variable is not a list"
#define		RUNTIME_INDEX_OUT_OF_BOUND						"Index out of bound"
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
