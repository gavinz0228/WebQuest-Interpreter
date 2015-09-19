#include "Runtime.h"
void Runtime::Run(char* script)
{
	ParseTree parser;
	parser.Parse(script);
	WQState state;
	Evaulate((NodeBase*)parser.program, &state);

}
void Runtime::Evaulate(NodeBase* node,WQState* state)
{
	if (node->GetType() == NT_EXPRESSION)
	{
		ExpressionNode* expnode = (ExpressionNode*)node;
		Evaulate(expnode->Expression, state);
	}
	else if (node->GetType() == NT_ASSIGNMENT)
	{
		AssignmentNode* assignment = (AssignmentNode*)node;
		printf("\n");
		printf("Assignment:");
		printf(assignment->LeftSide->Value->c_str());
		printf("=");
		Evaulate((NodeBase*)assignment->RightSide, state);
	}
	else if (node->GetType() == NT_VARIABLE)
	{
		VariableNode* varnode = (VariableNode*)node;
		//
		printf(varnode->Value->c_str());
	}
	else if (node->GetType() == NT_STRING)
	{
		StringNode* strnode = (StringNode*)node;
		state->ReturnObject.SetStringValue( *strnode->Value);

	}
	else if (node->GetType() == NT_OPERATION)
	{
		printf("Operation(  Terms: ");
		OperationNode* opnode = (OperationNode*)node;
		//
		for (list<ExpressionNode*>::iterator expit = opnode->Terms->begin();
			expit != opnode->Terms->end();
			expit++)
		{
			Evaulate((*expit),state);
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
			
			printf("\nIf Expression(");

			Evaulate(it->first,state);
			printf(") True:");
			
			Evaulate(it->second, state);
		}
		it++;
		while (it != ifnode->IfBlock->end())
		{
			
			printf("\nElse If Expression(");

			Evaulate(it->first, state);
			printf(") True:");
			
			Evaulate(it->second, state);
			it++;
		}
		
		printf("\nElse:");
		Evaulate(ifnode->ElseBlock, state);

	}
	else if (node->GetType() == NT_COMPARISON)
	{
		ComparisonNode* comnode = (ComparisonNode*)node;
		Evaulate(comnode->LeftSide, state);
		printf(comnode->Operator->c_str());
		Evaulate(comnode->RightSide, state);

	}
	else if (node->GetType() == NT_LOGIC)
	{
		LogicNode* lgnode = (LogicNode*)node;
		printf("Logic Node(Expressions:");
		for (list<ExpressionNode*>::iterator expit = lgnode->Expressions->begin();
			expit != lgnode->Expressions->end();
			expit++)
		{
			Evaulate(*expit, state);
		}
		printf(", Logic Operators:");
		for (list<string*>::iterator lgit = lgnode->Operators->begin();
			lgit != lgnode->Operators->end();
			lgit++)
		{
			printf((*lgit)->c_str());
		}
		printf(")");
	}
	else if (node->GetType() == NT_FLOAT)
	{
		FloatNode* fltnode = (FloatNode*)node;
		//
		printf(" ");
		printf("%f", fltnode->Value);
	}
	else if (node->GetType() == NT_INTEGER)
	{
		IntegerNode* intnode = (IntegerNode*)node;
		//
		printf(" ");
		printf("%d", intnode->Value);
	}
	else if (node->GetType() == NT_FUNCTIONCALL)
	{
		
		FunctionCallNode* funcnode = (FunctionCallNode*)node;
		WQFunction func = environment->Functions->Get(funcnode->FunctionName);
		if (func == NULL)
		{
			throw string(RUNTIME_FUNCTION_NOT_DEFINED) + ": "+*funcnode->FunctionName;
		}
		state->ParamSize = funcnode->Parameters->size();
		for (list<ExpressionNode*>::iterator it = funcnode->Parameters->begin(); it != funcnode->Parameters->end(); it++)
		{
			Evaulate(*it, state);
			state->AddParam(state->ReturnObject);
		}
		func(state);
	}
	else if (node->GetType() == NT_CODEBLOCK)
	{
		
		CodeBlockNode* program = (CodeBlockNode*)node;
		for (list<NodeBase*>::iterator it = program->Statements->begin(); it != program->Statements->end(); it++)
		{
			NodeBase* nxt = *it;
			Evaulate(*it, state);

		}
	}
	
}