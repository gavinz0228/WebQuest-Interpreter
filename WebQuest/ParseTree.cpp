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
	///for testing!!!!!
	if (tker->IsNextNumber())
	{
		Token* numtk = tker->NextToken();
		if (numtk->Type == TK_FLOAT)
		{
			FloatNode* fltnode = new FloatNode;
			fltnode->Value = numtk->Float;
			exp->Expression = (NodeBase*)fltnode;
			exp->ExpressionType = NT_FLOAT;
		}
		else if (numtk->Type == TK_INTEGER)
		{
			IntegerNode* intnode = new IntegerNode;
			intnode->Value = numtk->Integer;
			exp->Expression = (NodeBase*)intnode;
			exp->ExpressionType = NT_INTEGER;
		}
		
	}
	else if (tker->IsNextVariable())
	{ //get the next token- variable or function name
		Token* tkvar = tker->NextToken();
		if (tker->IsNextLeftParen())
		{
			FunctionCallNode* function = new FunctionCallNode;
			list<ExpressionNode*>* params = new list < ExpressionNode* >;
			ParseParameters(tker, params);

			function->FunctionName = tkvar->Symbol;
			function->Parameters = params;
			exp->Expression = function;
			exp->ExpressionType = NT_FUNCTIONCALL;
		}
		else{
			VariableNode* varnode = new VariableNode();
			varnode->Value = tkvar->Symbol;
			exp->Expression = (NodeBase*)varnode;
			exp->ExpressionType = NT_VARIABLE;
		}
	}
	else if (tker->IsNextString())
	{

		StringNode* strnode = new StringNode;
		strnode->Value = tker->NextToken()->Symbol;
		exp->Expression = (NodeBase*)strnode;
		exp->ExpressionType = NT_INTEGER;
	}


	return;

	while (true)
	{
		if (tker->IsNextOperator())
		{
			Token* op = tker->NextToken();
			if (tker->IsNextNumber() || tker->IsNextOperator())
			{
				Token* symbol = tker->NextToken();

			}
		}
		else
			break;

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

	printf("\n");
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
		printf(Padding(level).c_str());
		printf(varnode->Value->c_str());
	}
	else if (node->GetType() == NT_STRING)
	{
		StringNode* strnode = (StringNode*)node;
		printf(Padding(level).c_str());
		printf("%s", strnode->Value);
	}
	else if (node->GetType() == NT_OPERATION)
	{
		OperationNode* opnode = (OperationNode*)node;
		printf(Padding(level).c_str());
		printf("%s", opnode->Value);
	}
	else if (node->GetType()==NT_FLOAT)
	{
		FloatNode* fltnode = (FloatNode*)node;
		printf(Padding(level).c_str());
		printf("%f", fltnode->Value);
	}
	else if (node->GetType() == NT_INTEGER)
	{
		IntegerNode* intnode = (IntegerNode*)node;
		printf(Padding(level).c_str());
		printf("%d", intnode->Value);
	}
	else if (node->GetType() == NT_FUNCTIONCALL)
	{

		FunctionCallNode* funcnode=(FunctionCallNode*)node;
		printf(Padding(level).c_str());
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