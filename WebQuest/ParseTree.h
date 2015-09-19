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

enum ParseTreeNodeType{ NT_ASSIGNMENT, 
	NT_EXPRESSION,
	NT_FUNCTIONCALL ,
	//NT_PROGRAM,
	NT_COMPARISON,
	NT_LOGIC,
	NT_CODEBLOCK,
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
	AssignmentNode(){ LeftSide = new VariableNode; RightSide = new ExpressionNode; }
	~AssignmentNode(){ delete LeftSide; delete RightSide; }
	VariableNode* LeftSide;
	ExpressionNode* RightSide;
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



/*
	ParseTree class
*/

class ParseTree
{
public:
	~ParseTree();
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
//class ParseTreeNode
//{
//public:
//
//	ParseTreeNode(Pars3eTreeNodeType type);
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
