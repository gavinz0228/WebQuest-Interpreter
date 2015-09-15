#include <list>
#include <string>
#include <vector>
#include "Tokenizer.h"
#include "WQObject.h"
#include "Exception.h"
using namespace std;

enum ParseTreeNodeType{ NT_ASSIGNMENT, 
	NT_EXPRESSION,
	NT_FUNCTIONCALL ,
	NT_PROGRAM,
	NT_OPERATION,
	NT_FLOAT,
	NT_INTEGER,
	NT_STRING,
	NT_VARIABLE,
	NT_IF};
class NodeBase
{
public:
	static int* Type;
	NodeBase* Parent;
	//needs to have a virtual function in order to be a polymorphic type
	virtual int GetType() { return -1; };
	virtual bool IsTerminal(){ return false; };
	//virtual WQObject Evaulate();
};

class TerminalNodeBase :public NodeBase
{
public:
	bool IsTerminal(){ return true; };
};
class StringNode :public TerminalNodeBase
{
public:
	string* Value; 
	int GetType(){ return NT_STRING; }
};
class VariableNode :public TerminalNodeBase
{
public:
	string* Value;
	int GetType(){ return NT_VARIABLE; }
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

/*
Non-terminal Node
*/
class ProgramNode :public NodeBase
{
public:
	list<NodeBase*> Statements;
	static int Type;
	int GetType(){ return NT_PROGRAM; };
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
	list<ExpressionNode*>* Terms;
	list<string*>* Operators;
	int GetType(){ return NT_OPERATION; }
};
class AssignmentNode :public NodeBase
{
public:
	//AssignmentNode();

	VariableNode* LeftSide;
	ExpressionNode* RightSide;
	static int Type;
	int GetType(){ return NT_ASSIGNMENT; };
};

class FunctionCallNode :public NodeBase
{
public:
	string* FunctionName;
	list<ExpressionNode*>* Parameters;
	static int Type;
	int GetType(){ return NT_FUNCTIONCALL; };
};
class IfNode :public NodeBase
{
	ExpressionNode* Condition;
	list<NodeBase*>* True;
	list<NodeBase*>* NotTrue;
	int GetType(){ return NT_IF; }
};


class ParseTree
{
public:
	void Parse(string script);
	void PrintTree();
private:
	void ParseExpression(Tokenizer* tker, ExpressionNode* node);
	void ParseParameters(Tokenizer* tker, list<ExpressionNode*>* parameters);
	void ConsumeNewLine(Tokenizer* tker);
	void ParseTerm(Tokenizer* tker, ExpressionNode* exp);
	void ParseIf(Tokenizer* tker, IfNode* ifnode);
	volatile ProgramNode *program;
	void PrintTreeNode(NodeBase* node, int level);
	void TypesInit();
	Tokenizer* tker;

};

//
//class ParseTreeNode
//{
//public:
//
//	ParseTreeNode(ParseTreeNodeType type);
//	~ParseTreeNode();
//	void AddNode(ParseTreeNode* node);
//	void AddToken(Token* tk);
//	ParseTreeNodeType NodeType;
//	list<Token*>* Tokens;
//	ParseTreeNode* Parent;
//	ParseTreeNode* Next;
//	ParseTreeNode* Prev;
//	vector<ParseTreeNode*> Children;
//};
//
