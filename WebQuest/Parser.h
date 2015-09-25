#include <list>
#include <string>
#include <vector>
#include <map>
#include "Tokenizer.h"
#include "WQObject.h"
#include "Exception.h"
using namespace std;
#ifndef PARSERTREE_H
#define PARSERTREE_H

enum ParserNodeType{ NT_ASSIGNMENT, 
	NT_EXPRESSION,
	NT_FUNCTIONCALL ,
	NT_BOOLEAN,
	NT_COMPARISON,
	NT_LOGIC,
	NT_CODEBLOCK,
	NT_OPERATION,
	NT_FLOAT,
	NT_INTEGER,
	NT_STRING,
	NT_VARIABLE,
	NT_IF,
	NT_WHILE,
	NT_CREATELIST,
	NT_CREATEDICT,
	NT_ELEMENT};
enum AssignmentTargetType{AT_VARIABLE,AT_ELEMENT};
class NodeBase
{
public:
	static int* Type;
	NodeBase* Parent;
	//needs to have a virtual function in order to be a polymorphic type
	virtual int GetType() { return -1; }
	virtual bool IsTerminal(){ return false; }
	virtual bool IsAssignable(){ return false; }
	//virtual WQObject Evaulate();
};
class TerminalNodeBase :public NodeBase
{
public:
	bool IsTerminal(){ return true; };
};
class Assignable
{
public:
	bool IsAssignable(){ return true; }
	virtual char GetAssignableType(){ return 0; }
};
class StringNode :public  TerminalNodeBase
{
public:
	string* Value; 
	int GetType(){ return NT_STRING; }
};
class VariableNode :public TerminalNodeBase, public Assignable
{
public:
	string* Value;
	int GetType(){ return NT_VARIABLE; }
	char GetAssignableType(){ return AT_VARIABLE; }
	bool IsAssignable(){ return true; }
};


class IntegerNode :public TerminalNodeBase
{
public:
	long long Value;
	int GetType(){ return NT_INTEGER; }
};

class FloatNode :public TerminalNodeBase
{
public:
	long double Value;
	int GetType(){ return NT_FLOAT; }
};

class BooleanNode :public TerminalNodeBase
{
public:
	bool Value;
	int GetType(){ return NT_BOOLEAN; }
};

class ExpressionNode;
class ElementNode :public Assignable, public NodeBase
{
public:
	ElementNode(){
		Variable = new VariableNode;
	}
	~ElementNode()
	{
		delete Variable;
	}
	VariableNode* Variable;
	ExpressionNode* key;
	int GetType(){ return NT_ELEMENT; }
	char GetAssignableType(){ return AT_ELEMENT; }
	bool IsAssignable(){ return true; }
};
class SlicingNode:NodeBase
{
public:
	SlicingNode(){ Variable = new VariableNode; }
	~SlicingNode(){}
	VariableNode* Variable;
	long StartIndex;
	long EndIndex;
};
/*
Non-terminal Node
*/
//class ProgramNode :public NodeBase
//{
//public:
//	list<NodeBase*> Statements;
//	int GetType(){ return NT_PROGRAM; };
//};
class CodeBlockNode :public NodeBase
{
public:
	CodeBlockNode(){ Statements = new list < NodeBase* > ; }
	~CodeBlockNode(){ 
		while (!Statements->empty()) delete Statements->front(), Statements->pop_front();
		delete Statements;
	}
	list<NodeBase*>* Statements;
	int GetType(){ return NT_CODEBLOCK; };
};
class ExpressionNode :public NodeBase
{
public:
	//ExpressionNode();
	NodeBase* Expression;
	int ExpressionType;
	static int Type;
	int GetType(){ return NT_EXPRESSION; };
};

class OperationNode:public NodeBase
{
public:
	OperationNode(){ Terms = new list < ExpressionNode* > ; Operators = new list < string* > ; }
	~OperationNode(){ 
		while (!Terms->empty()) delete Terms->front(), Terms->pop_front();
		delete Terms; delete Operators;
	
	}
	list<ExpressionNode*>* Terms;
	list<string*>* Operators;
	int GetType(){ return NT_OPERATION; }
};

class AssignmentNode :public NodeBase
{
public:
	AssignmentNode(){ LeftSideElement = new ElementNode; LeftSideVariable = new VariableNode;  RightSide = new ExpressionNode; }
	~AssignmentNode(){ delete LeftSideVariable;delete LeftSideElement; delete RightSide; }

	ElementNode* LeftSideElement;
	VariableNode* LeftSideVariable;
	ExpressionNode* RightSide;
	AssignmentTargetType TargetType;
	static int Type;
	int GetType(){ return NT_ASSIGNMENT; };
};

class FunctionCallNode :public NodeBase
{
public:
	FunctionCallNode(){ Parameters = new  list < ExpressionNode* > ; };
	~FunctionCallNode(){ 
		while (!Parameters->empty()) delete Parameters->front(), Parameters->pop_front();
		delete Parameters;
	}
	string* FunctionName;
	list<ExpressionNode*>* Parameters;
	static int Type;
	int GetType(){ return NT_FUNCTIONCALL; };
};
class IfNode :public NodeBase
{
public:
	IfNode(){ IfBlock = new map < ExpressionNode*, CodeBlockNode* > ; ElseBlock = new CodeBlockNode; }
	~IfNode(){
		map< ExpressionNode*, CodeBlockNode* >::iterator it = IfBlock->begin();
		for (; it != IfBlock->end(); it++)
		{
			delete it->first;
			delete it->second;
		}
		delete IfBlock; delete ElseBlock;
	}
	CodeBlockNode* ElseBlock;
	map<ExpressionNode*, CodeBlockNode*>* IfBlock;
	int GetType(){ return NT_IF; }
};
class WhileNode :public NodeBase
{
public:
	WhileNode(){ CodeBlock = new CodeBlockNode; Condition = new ExpressionNode; }
	~WhileNode(){ delete CodeBlock; delete Condition; }
	CodeBlockNode* CodeBlock;
	ExpressionNode* Condition;
	int GetType(){ return NT_WHILE; }
};



/*
Logic Node
for holding data <exp> <operator> <exp>

forexamples:
<exp> and <exp>
<exp> or <exp>
not	<exp>
*/
class LogicNode :public NodeBase
{
public:
	LogicNode()
	{
		Expressions = new list<ExpressionNode*>;
		Operators = new list < string* >;
	}
	~LogicNode()
	{
		while (!Expressions->empty()){ delete Expressions->front(); Expressions->pop_front(); }
		delete Expressions;
		delete Operators;
	}
	list<ExpressionNode*> * Expressions;
	list<string*>* Operators;
	int GetType(){ return NT_LOGIC; }
};
/*
Comparison Node

for holding data <exp> > <exp>,<exp> >= <exp>, <exp> == <exp> ...
*/
class ComparisonNode :public NodeBase
{
public:
	ComparisonNode(){ LeftSide = new ExpressionNode; RightSide = new ExpressionNode; }
	ExpressionNode* LeftSide;
	string* Operator;
	ExpressionNode* RightSide;
	int GetType(){ return NT_COMPARISON; }
};


class CreateListNode :public NodeBase
{
public:
	CreateListNode(){ Parameters = new list < ExpressionNode* > ; }
	~CreateListNode(){
		while (!Parameters->empty())
		{
			delete Parameters->front();
			Parameters->pop_front();
		}
		delete Parameters;
	}
	list <ExpressionNode*>* Parameters;
	int GetType(){ return NT_CREATELIST; }
};
class CreateDictionaryNode :public NodeBase
{
public:
	CreateDictionaryNode(){ Parameter = new ExpressionNode; }
	~CreateDictionaryNode(){ delete Parameter; }
	ExpressionNode* Parameter;
	int GetType(){ return NT_CREATEDICT; }
};

/*
	Parser class
*/

class Parser
{
public:
	~Parser();
	void Parse(string script);
	void PrintTree();
	volatile CodeBlockNode *program;
private:
	void ParseCodeBlock(Tokenizer* tker, CodeBlockNode* codeblock);
	void ParseExpression(Tokenizer* tker, ExpressionNode* node,bool parselogicnode=true);
	void ParseParameters(Tokenizer* tker, list<ExpressionNode*>* parameters);
	void ConsumeNewLine(Tokenizer* tker);
	void ParseTerm(Tokenizer* tker, ExpressionNode* exp);
	void ParseIf(Tokenizer* tker, IfNode* ifnode);

	void PrintTreeNode(NodeBase* node, int level);
	Tokenizer* tker;

};
#endif
//
//class ParserNode
//{
//public:
//
//	ParserNode(Pars3eTreeNodeType type);
//	~ParserNode();
//	void AddNode(ParserNode* node);
//	void AddToken(Token* tk);
//	ParserNodeType NodeType;
//	list<Token*>* Tokens;
//	ParserNode* Parent;
//	ParserNode* Next;
//	ParserNode* Prev;
//	vector<ParserNode*> Children;
//};
//