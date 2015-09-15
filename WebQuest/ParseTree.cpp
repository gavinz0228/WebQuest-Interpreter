#include "ParseTree.h"
void ParseTree::TypesInit()
{

}
void ParseTree::Parse(string script)
{
	if (tker == NULL)
		delete tker;
	tker = new Tokenizer;
	tker->Tokenize(script);
	ProgramNode* program = new ProgramNode;
	while (true)
	{
		//------------------------------------------
		Token* frsttk = tker->NextToken();
		if (frsttk == NULL)
			break;
		if (frsttk->Type == TK_VARIABLE)
		{
			//starts with symbol
			//1. assignment?
			//2. function call?
			//3. list operation?
			//4. dictionary operation?
			bool ss = tker->IsNextOperator();
			Token* tk = tker->LookAhead();
			if (tker->IsNextOperator()&&*tker->LookAhead()->Symbol == OP_ASSIGN)
			{
				//get rid of the OP_ASSIGN/=
				tker->NextToken();
				//it's a assignment
				AssignmentNode* assignment = new AssignmentNode;
				ExpressionNode* rightside = new ExpressionNode;
				VariableNode* leftside = new VariableNode;
				leftside->Value=frsttk->Symbol;
				assignment->LeftSide = leftside;
				ParseExpression(tker, rightside);
				assignment->RightSide = rightside;
				program->Statements.push_back((NodeBase*)assignment);
				ConsumeNewLine(tker);

			}
			//function call
			else if (tker->IsNextOperator()&&*tker->LookAhead()->Symbol == OP_L_PAREN)
			{

				FunctionCallNode* function = new FunctionCallNode;
				list<ExpressionNode*>* params = new list < ExpressionNode* > ;
				ParseParameters(tker, params);

				function->FunctionName = frsttk->Symbol;
				function->Parameters = params;
				program->Statements.push_back(function);
			}
			else if (tker->IsNextOperator()&&*tker->LookAhead()->Symbol == OP_L_BRAC)
			{
				//list or dictionary
			}

			else
			{
				// not any of them, throw error
			}
		}
	}
	this->program = program;
}


void ParseTree::ParseExpression(Tokenizer *tker, ExpressionNode* exp)
{
	ExpressionNode frstexp ;
	ParseTerm(tker, &frstexp);
	//there is only one term in this expression
	//its sign is that the next token is right parenthesis or new line
	if (tker->IsNextNewLine() || tker->IsNextRightParen())
	{
		exp->Expression = frstexp.Expression;
		exp->ExpressionType = frstexp.ExpressionType;
		
	}
	//there is more than one term in this expression
	else if (tker->IsNextArithmeticOperator())
	{
		/* 
		evaluation for this parsing solution
		3-4*1+2
		if it sees - or +, evaluate the right first
		3-(4*1+2)
		if it sees * or / evaluation the current one first
		//do this recursively
		*/
		//copy the first node over to a new dynamically allocated expression node
		OperationNode* opnode = new OperationNode;
		opnode->Operators = new list < string* > ;
		opnode->Terms = new list < ExpressionNode* > ;
		ExpressionNode* firstterm = new ExpressionNode;
		firstterm->Expression = frstexp.Expression;
		firstterm->ExpressionType = frstexp.ExpressionType;
		opnode->Terms->push_back(firstterm);
		//copy the rest
		while (true)
		{
			if (!tker->IsNextArithmeticOperator())
				break;
			opnode->Operators->push_back(tker->NextToken()->Symbol);
			ExpressionNode* newterm = new ExpressionNode;
			ParseTerm(tker, newterm);
			opnode->Terms->push_back(newterm);
		}
		
		exp->Expression = opnode;
		exp->ExpressionType = NT_OPERATION;
	}

	// the next one is neither arithmetic operator nor terminal node, nor  new line or right paren
	//throw exception
	else
	{
		throw INVALID_EXPRESSION;
	}

}


void ParseTree::ParseTerm(Tokenizer* tker, ExpressionNode* exp)
{
	Token* frsttk=tker->NextToken();
	if (frsttk->Type == TK_FLOAT)
	{
		FloatNode* fltnode = new FloatNode;
		fltnode->Value = frsttk->Float;
		exp->Expression = (NodeBase*)fltnode;
		exp->ExpressionType = NT_FLOAT;
	}
	else if (frsttk->Type == TK_INTEGER)
	{
		IntegerNode* intnode = new IntegerNode;
		intnode->Value = frsttk->Integer;
		exp->Expression = (NodeBase*)intnode;
		exp->ExpressionType = NT_INTEGER;
	}
	else if (frsttk->Type == TK_STRING)
	{

		StringNode* strnode = new StringNode;
		strnode->Value = tker->NextToken()->Symbol;
		exp->Expression = (NodeBase*)strnode;
		exp->ExpressionType = NT_STRING;
	}
	else if (frsttk->Type == TK_VARIABLE)
	{ //get the next token- variable or function name
		if (tker->IsNextLeftParen())
		{
			FunctionCallNode* function = new FunctionCallNode;
			list<ExpressionNode*>* params = new list < ExpressionNode* >;
			ParseParameters(tker, params);

			function->FunctionName = frsttk->Symbol;
			function->Parameters = params;
			exp->Expression = function;
			exp->ExpressionType = NT_FUNCTIONCALL;
		}
		else{
			VariableNode* varnode = new VariableNode();
			varnode->Value = frsttk->Symbol;
			exp->Expression = (NodeBase*)varnode;
			exp->ExpressionType = NT_VARIABLE;
		}
	}
}

void ParseTree::ParseParameters(Tokenizer* tker, list<ExpressionNode*>* parameters)
{
	//skip the parenthesis
	tker->NextToken();

	while (true)
	{
		if (tker->IsNextRightParen())
		{
			//skipt the right parenthesis
			tker->NextToken();
			break;
		}
		ExpressionNode *exp = new ExpressionNode;
		ParseExpression(tker, exp);
		parameters->push_back(exp);
		if (!tker->IsNextComma() && !tker->IsNextRightParen())
			throw INVALID_FUNCTION_CALL;

	}
}


void ParseTree::PrintTree()
{
	PrintTreeNode((NodeBase*)program, 0);
}
void ParseTree::ConsumeNewLine(Tokenizer* tker)
{
	while (true)
	{
		//skip the consequtive newlines
		if (tker->IsNextNewLine())
			tker->NextToken();
		else
			return;
	}
}
string Padding(int num)
{
	string pd;
	for (int i = 0; i < num; i++)
	{
		pd += "  ";
	}
	return pd;
}
void ParseTree::PrintTreeNode(NodeBase* node,int level)
{


	if (node->GetType() == NT_EXPRESSION)
	{
		ExpressionNode* expnode = (ExpressionNode*)node;
		PrintTreeNode(expnode->Expression, level + 1);	
	}
	else if (node->GetType() == NT_ASSIGNMENT)
	{
		AssignmentNode* assignment = (AssignmentNode*)node;
		printf("\n");
		printf(Padding(level).c_str());
		printf("Assignment:");
		printf(assignment->LeftSide->Value->c_str());
		printf("=");
		PrintTreeNode((NodeBase*)assignment->RightSide, level + 1);
	}
	else if (node->GetType() == NT_VARIABLE)
	{
		VariableNode* varnode = (VariableNode*)node;
		//printf(Padding(level).c_str());
		printf(varnode->Value->c_str());
	}
	else if (node->GetType() == NT_STRING)
	{
		StringNode* strnode = (StringNode*)node;
		//printf(Padding(level).c_str());
		printf("%s", strnode->Value);
	}
	else if (node->GetType() == NT_OPERATION)
	{
		printf("Operation(  Terms: ");
		OperationNode* opnode = (OperationNode*)node;
		//printf(Padding(level).c_str());
		for (list<ExpressionNode*>::iterator expit = opnode->Terms->begin();
			expit != opnode->Terms->end();
			expit++)
		{
			PrintTreeNode((*expit), level + 1);
		}
		printf("  Operators: ");
		for (list<string*>::iterator opit = opnode->Operators->begin();
			opit != opnode->Operators->end();
			opit++)
		{
			printf("%s ", (*opit)->c_str());
		}
		printf(")");

	}
	else if (node->GetType()==NT_FLOAT)
	{
		FloatNode* fltnode = (FloatNode*)node;
		//printf(Padding(level).c_str());
		printf(" ");
		printf("%f", fltnode->Value);
	}
	else if (node->GetType() == NT_INTEGER)
	{
		IntegerNode* intnode = (IntegerNode*)node;
		//printf(Padding(level).c_str());
		printf(" ");
		printf("%d", intnode->Value);
	}
	else if (node->GetType() == NT_FUNCTIONCALL)
	{

		FunctionCallNode* funcnode=(FunctionCallNode*)node;
		//printf(Padding(level).c_str());
		printf("Function Call:");
		
		printf(funcnode->FunctionName->c_str());
		
		printf(" Parameters:");
		for (list<ExpressionNode*>::iterator it = funcnode->Parameters->begin(); it != funcnode->Parameters->end(); it++)
		{
			PrintTreeNode(*it, level + 1);
			printf(" ");
		}
	}
	else if (node->GetType() == NT_PROGRAM)
	{
		printf(Padding(level).c_str());
		printf("Program");
		ProgramNode* program = (ProgramNode*)node;
		for (list<NodeBase*>::iterator it = program->Statements.begin(); it != program->Statements.end(); it++)
		{
			NodeBase* nxt = *it;
			PrintTreeNode(*it, level + 1);

		}
	}
}
//
//void ParseTreeNode::AddNode(ParseTreeNode* node)
//{
//	node->Parent = this;
//	Children.push_back(node);
//
//}
//void ParseTreeNode::AddToken(Token* tk)
//{
//	Tokens->push_back(tk);
//}
//ParseTreeNode::ParseTreeNode(ParseTreeNodeType type):
//NodeType(type)
//{
//	Tokens = new list < Token* > ;
//}
//ParseTreeNode::~ParseTreeNode()
//{
//	if (Tokens != NULL)
//		delete Tokens;
//}


//
//Token* optk = tker->NextToken();
//OperationNode* opnode = new OperationNode;
//ExpressionNode* leftside = new ExpressionNode;
//leftside->Expression = frstexp.Expression;
//leftside->ExpressionType = frstexp.ExpressionType;
//
//ExpressionNode* rightside = new ExpressionNode;
//opnode->Operation = optk->Symbol;
//ParseTerm(tker, rightside);
//
//opnode->LeftSide = leftside;
//opnode->RightSide = rightside;