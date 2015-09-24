#include "ParseTree.h"

ParseTree::~ParseTree()
{
	if (tker != NULL)
		delete tker;
	if (program != NULL)
		delete program;
}
void ParseTree::Parse(string script)
{
	if (tker == NULL)
		delete tker;
	tker = new Tokenizer;
	tker->Tokenize(script);
	CodeBlockNode* program = new CodeBlockNode;
	ParseCodeBlock(tker, program);
	this->program = program;
}
void ParseTree::ParseCodeBlock(Tokenizer* tker, CodeBlockNode* program)
{

	while (true)
	{
		if (tker->IsNextEndBlock())
			return;
		//------------------------------------------
		//Token* frsttk = tker->NextToken();
		if (tker->LookAhead() == NULL)
			break;
		if (tker->IsNextIfKeyword())
		{
			//skip if
			tker->NextToken();

			IfNode* ifnode = new IfNode;
			ExpressionNode* firstifcondition = new ExpressionNode;
			CodeBlockNode* firstifblock = new CodeBlockNode;
			//parse the condition
			ParseExpression(tker, firstifcondition);
			ParseCodeBlock(tker, firstifblock);
			ifnode->IfBlock->insert(pair<ExpressionNode*, CodeBlockNode*>(firstifcondition, firstifblock));
			if (tker->IsNextElseIfKeyword())
			{
				while (tker->IsNextElseIfKeyword())
				{
					//skip the elseif
					tker->NextToken();
					ExpressionNode* elseifcondition = new ExpressionNode;
					CodeBlockNode* elseifblock = new CodeBlockNode;
					//parse the condition
					ParseExpression(tker, elseifcondition);
					ParseCodeBlock(tker, elseifblock);
					ifnode->IfBlock->insert(pair<ExpressionNode*, CodeBlockNode*>(elseifcondition, elseifblock));

				}

			}
			if (tker->IsNextElseKeyword())
			{
				//skip the else keyword
				tker->NextToken();
				CodeBlockNode* elseblock = new CodeBlockNode;
				//parse the condition
				ParseCodeBlock(tker, ifnode->ElseBlock);

			}
			//after parsing if and else , expecting a 'end'
			if (!tker->IsNextEndBlock())
			{
				throw SYNTAX_EXPECTING_END;
			}
			program->Statements->push_back(ifnode);
			//skip endif
			tker->NextToken();
		}
		else if (tker->IsNextWhileKeyword())
		{
			//skip the while
			tker->NextToken();
			WhileNode* whilenode = new WhileNode;
			ParseExpression(tker, whilenode->Condition);
			ParseCodeBlock(tker, whilenode->CodeBlock);
			program->Statements->push_back(whilenode);
			if (!tker->IsNextEndBlock())
			{
				throw SYNTAX_EXPECTING_END;
			}
		}
		//frsttk->Type == TK_ELSE || frsttk->Type == TK_ELSEIF
		//else if (tker->IsNextElseKeyword()||tker->IsNextElseIfKeyword())
		if (tker->IsNextEndBlock())
		{
			//the end of the current block so just return
			return;
		}
		else if (tker->LookAhead()->Type == TK_VARIABLE)
		{
			ExpressionNode exp;
			ParseTerm(tker, &exp);
			//four possible situations

			//1. assignment?
			//2. function call?
			//3. list operation?
			//4. dictionary operation?
			
			if (tker->IsNextOperator() && *tker->LookAhead()->Symbol == OP_ASSIGN)
			{
				
				if (!exp.Expression->IsAssignable())
				{
					throw SYNTAX_CAN_NOT_ASSIGN;
				}
				//get rid of the OP_ASSIGN/=
				tker->NextToken();
				//it's a assignment
				AssignmentNode* assignment = new AssignmentNode;
				//assign the first side
				if (exp.ExpressionType == NT_VARIABLE)
				{
					assignment->LeftSideVariable = (VariableNode*)exp.Expression;
					assignment->TargetType = AT_VARIABLE;
					//char tp=assignment->LeftSide->GetAssignableType();
					//char b = tp;
				}
				else if (exp.ExpressionType == NT_ELEMENT)
				{
					assignment->LeftSideElement = (ElementNode*)exp.Expression;
					assignment->TargetType = AT_ELEMENT;
				}

				//assignment of  <variable>=[]
				if (tker->IsNextLeftBracket())
				{
					tker->NextToken();
					if (tker->IsNextRightBracket())
					{
						tker->NextToken();
						CreateListNode * createlist = new CreateListNode;
						assignment->RightSide->ExpressionType = NT_CREATELIST;
						assignment->RightSide->Expression = (NodeBase*)createlist;
					}
					else
					{
						CreateListNode * createlist = new CreateListNode;
						ParseParameters(tker, createlist->Parameters);
						assignment->RightSide->ExpressionType = NT_CREATELIST;
						assignment->RightSide->Expression = (NodeBase*)createlist;
						if (tker->IsNextRightBracket())
							tker->NextToken();
						else
							throw SYNTAX_EXPECTING_RIGHT_BRACKET;
					}
				}
				//assignment of  <variable>={} or <variable>={<exp>}
				else if (tker->IsNextLeftCurlyBracket())
				{
					tker->NextToken();
					if (tker->IsNextRightCurlyBracket())
					{
						//<variable>={}
						tker->NextToken();
						CreateDictionaryNode* createdict = new CreateDictionaryNode;

						assignment->RightSide->ExpressionType = NT_CREATEDICT;
						assignment->RightSide->Expression = (NodeBase*)createdict;
					}
					else
					{
						ExpressionNode* param = new ExpressionNode;
						ParseExpression(tker, param);
						if (tker->IsNextRightCurlyBracket())
						{
							CreateDictionaryNode* createdict = new CreateDictionaryNode;
							createdict->Parameter = param;

							assignment->RightSide->ExpressionType = NT_CREATEDICT;
							assignment->RightSide->Expression = (NodeBase*)createdict;
						}
						else
							throw SYNTAX_EXPECTING_RIGHT_BRACKET;
					}


					string* name = tker->NextToken()->Symbol;
					FunctionCallNode* createdict = new FunctionCallNode;
					createdict->FunctionName = name;
					assignment->RightSide->ExpressionType = NT_FUNCTIONCALL;
					assignment->RightSide->Expression = (NodeBase*)createdict;
				}
				else
					ParseExpression(tker, assignment->RightSide);

				program->Statements->push_back((NodeBase*)assignment);
				ConsumeNewLine(tker);

			}
			//function call
			else if (exp.ExpressionType==NT_FUNCTIONCALL)
			{

				program->Statements->push_back(exp.Expression);
			}
			else if (false)
			{
				//list or dictionary
			}
			else if (tker->LookAhead()==NULL)
			{
				// end of the program
				return;
			}
			else
			{
				//throw exception
				// syntax error ,unexpected token
			}
		}
	}
}

void ParseTree::ParseExpression(Tokenizer *tker, ExpressionNode* exp, bool parselogicnode)
{
	ExpressionNode frstexp ;
	if (tker->IsNextLeftParen())
	{
		// skip the left parenthesis
		tker->NextToken();
		ExpressionNode* wrapexp = new ExpressionNode;
		ParseExpression(tker, wrapexp);
		exp->Expression =(NodeBase*) wrapexp;
		exp->ExpressionType = NT_EXPRESSION;
		if (!tker->IsNextRightParen())
		{
			throw SYNTAX_EXPECTING_RIGHT_PAREN;
		}
		//skip the right parenthesis
		tker->NextToken();
		return;
	}
	//Two cases
	//1. !<Term>
	//2 <Term>
	if (tker->IsNextNotOperator())
	{
		LogicNode* notnode = new LogicNode;
		notnode->Operators->push_back(tker->NextToken()->Symbol);
		ExpressionNode* tmpexp = new ExpressionNode;
		ParseExpression(tker,tmpexp);
		notnode->Expressions->push_back(tmpexp);
		frstexp.Expression = (NodeBase*)notnode;
		frstexp.ExpressionType = NT_LOGIC;
	}
	else
	{
		ParseTerm(tker, &frstexp);
	}
	ExpressionNode* currentexp=&frstexp;
	//there is only one term in this expression
	//its sign is that the next token is right parenthesis or new line
	if (tker->IsNextRightParen()||tker->IsNextEndBlock()||tker->IsNextColon()||tker->IsNextRightBracket())
	{
		//exp->Expression = frstexp.Expression;
		//exp->ExpressionType = frstexp.ExpressionType;
		memcpy(exp, &frstexp,sizeof(frstexp));
		return;
	}

	//there is more than one term in this expression
	if (tker->IsNextArithmeticOperator())
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
		currentexp->Expression = opnode;
		currentexp->ExpressionType = NT_OPERATION;

	}
	//if it's a comparison , then this is a comparison Node
	if (tker->IsNextComparisonOperator())
	{
		Token* op = tker->NextToken();
		ComparisonNode* comnode = new ComparisonNode;
		//comnode->LeftSide = currentexp;
		memcpy(comnode->LeftSide, currentexp, sizeof(*currentexp));
		comnode->Operator = op->Symbol;
		ParseExpression(tker, comnode->RightSide,false);
		currentexp->Expression = (NodeBase*)comnode;
		currentexp->ExpressionType = NT_COMPARISON;
	}
	//if it finds a logic operator after the current expression:
	// currentnode && ...
	// then use a logic node to wrap the whole expression ,for example:
	// <currentnode> && aa==1 || bb==2
	if (tker->IsNextAndOperator() || tker->IsNextOrOperator())
	{
		if (parselogicnode == true)
		{
			LogicNode* lgnode = new LogicNode;
			ExpressionNode* whatihavenow = new ExpressionNode;
			whatihavenow->Expression = currentexp->Expression;
			whatihavenow->ExpressionType = currentexp->ExpressionType;
			lgnode->Expressions->push_back(whatihavenow);
			while (true)
			{
				if (!tker->IsNextAndOperator() && !tker->IsNextOrOperator())
				{
					//the end of this expression
					break;
				}
				lgnode->Operators->push_back(tker->NextToken()->Symbol);
				ExpressionNode* temexp = new ExpressionNode;
				ParseExpression(tker, temexp,false);
				lgnode->Expressions->push_back(temexp);
			}
			currentexp->Expression = (NodeBase*)lgnode;
			currentexp->ExpressionType = NT_LOGIC;
		}

	}

	exp->Expression = currentexp->Expression;
	exp->ExpressionType = currentexp->ExpressionType;

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
		strnode->Value = frsttk->Symbol;
		exp->Expression = (NodeBase*)strnode;
		exp->ExpressionType = NT_STRING;
	}
	else if (frsttk->Type == TK_BOOLEAN)
	{
		BooleanNode * boolnode = new BooleanNode;
		if (*frsttk->Symbol == KW_TRUE)
			boolnode->Value = true;
		else
			boolnode->Value = false;
		exp->Expression = (NodeBase*)boolnode;
		exp->ExpressionType = NT_BOOLEAN;
	}
	else if (frsttk->Type == TK_VARIABLE)
	{ //get the next token- variable or function name
		if (tker->IsNextLeftParen())
		{
			FunctionCallNode* function = new FunctionCallNode;
			//skip the left parenthesis
			tker->NextToken();
			ParseParameters(tker, function->Parameters);
			//right parenthesis
			tker->NextToken();

			function->FunctionName = frsttk->Symbol;
			
			exp->Expression = function;
			exp->ExpressionType = NT_FUNCTIONCALL;
		}
		else if (tker->IsNextLeftBracket())
		{
			//parse a list or a dictionary,for example,
			//<variable>[<exp>]
			//<variable>[<exp>:<exp>]
			//skip the [
			tker->NextToken();
			ExpressionNode* firstitem = new ExpressionNode;
			ParseExpression(tker, firstitem);
			if (tker->IsNextColon())
			{
				tker->NextToken();
				FunctionCallNode* slicing;
			}
			
			if (tker->IsNextRightBracket())
			{
				tker->NextToken();
				ElementNode * elenode = new ElementNode;
				elenode->Variable->Value = frsttk->Symbol;
				elenode->key = firstitem;
				exp->Expression = (NodeBase*)elenode;
				exp->ExpressionType = NT_ELEMENT;
			}
			else
			{
				throw SYNTAX_INVALID_EXPRESSION;
			}
		}

		else{
			VariableNode* varnode = new VariableNode();
			varnode->Value = frsttk->Symbol;
			exp->Expression = (NodeBase*)varnode;
			exp->ExpressionType = NT_VARIABLE;
		}
	}
	else if (tker->LookAhead() == NULL)
	{
		return;
	}
}

void ParseTree::ParseParameters(Tokenizer* tker, list<ExpressionNode*>* parameters)
{


	while (true)
	{


		ExpressionNode *exp = new ExpressionNode;
		ParseExpression(tker, exp);
		parameters->push_back(exp);
		if (tker->IsNextRightParen()||tker->IsNextRightBracket())
		{
			break;
		}
		else if (tker->IsNextComma())
		{
			//skip the comma
			tker->NextToken();
		}
		else
			throw SYNTAX_INVALID_FUNCTION_CALL;

	}
}


void ParseTree::PrintTree()
{
	printf("Program");
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
		if (assignment->TargetType == AT_VARIABLE)
		{
			VariableNode* var = assignment->LeftSideVariable;
			PrintTreeNode(var, level + 1);
		}
		else if (assignment->TargetType == AT_ELEMENT)
		{
			PrintTreeNode((NodeBase*)assignment->LeftSideElement, level + 1);
		}

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
	else if (node->GetType() == NT_BOOLEAN)
	{
		BooleanNode * boolnode = (BooleanNode*)node;
		if (boolnode->Value == true)
			printf("true");
		else
			printf("false");
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
	else if (node->GetType() == NT_IF)
	{
		IfNode* ifnode = (IfNode*)node;
		map<ExpressionNode*, CodeBlockNode*>::iterator it = ifnode->IfBlock->begin();
		if (it != ifnode->IfBlock->end())
		{
			printf(Padding(level).c_str());
			printf("\nIf Expression(");

			PrintTreeNode(it->first, level + 1);
			printf(") True:");
			printf(Padding(level).c_str());
			PrintTreeNode(it->second, level + 1);
		}
		it++;
		while (it != ifnode->IfBlock->end())
		{
			printf(Padding(level).c_str());
			printf("\nElse If Expression(");

			PrintTreeNode(it->first, level + 1);
			printf(") True:");
			printf(Padding(level).c_str());
			PrintTreeNode(it->second, level + 1);
			it++;
		}
		printf(Padding(level).c_str());
		printf("\nElse:");
		PrintTreeNode(ifnode->ElseBlock, level + 1);

	}
	else if (node->GetType() == NT_COMPARISON)
	{
		ComparisonNode* comnode = (ComparisonNode*)node;
		PrintTreeNode(comnode->LeftSide, level + 1);
		printf(comnode->Operator->c_str());
		PrintTreeNode(comnode->RightSide, level + 1);
		
	}
	else if (node->GetType() == NT_LOGIC)
	{
		LogicNode* lgnode = (LogicNode*)node;
		printf("Logic Node(Expressions:");
		for (list<ExpressionNode*>::iterator expit = lgnode->Expressions->begin();
			expit != lgnode->Expressions->end();
			expit++)
		{
			PrintTreeNode(*expit, level + 1);
		}
		printf(", Logic Operators:");
		for (list<string*>::iterator lgit = lgnode->Operators ->begin();
			lgit != lgnode->Operators->end();
			lgit++)
		{
			printf((*lgit)->c_str(), level + 1);
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
	else if (node->GetType() == NT_CODEBLOCK)
	{
		printf(Padding(level).c_str());
		CodeBlockNode* program = (CodeBlockNode*)node;
		for (list<NodeBase*>::iterator it = program->Statements->begin(); it != program->Statements->end(); it++)
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