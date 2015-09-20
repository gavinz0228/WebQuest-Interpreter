#include "Runtime.h"
void Runtime::Run(char* script)
{
	ParseTree parser;
	parser.Parse(script);
	WQState state;
	Evaluate((NodeBase*)parser.program, &state);

}
void Runtime::Evaluate(NodeBase* node,WQState* state)
{
	if (node->GetType() == NT_EXPRESSION)
	{
		ExpressionNode* expnode = (ExpressionNode*)node;
		Evaluate(expnode->Expression, state);
	}
	else if (node->GetType() == NT_ASSIGNMENT)
	{
		AssignmentNode* assignment = (AssignmentNode*)node; 
		//Evaluate the right side
		Evaluate((NodeBase*)assignment->RightSide, state);

		if (assignment->LeftSide->GetAssignableType() == AT_VARIABLE)
		{
			VariableNode* var = (VariableNode*)((NodeBase*)assignment->LeftSide);
			environment->SetVariable(*var->Value, &(state->ReturnObject));

		}
		else if (assignment->LeftSide->GetAssignableType() == AT_ELEMENT)
		{
			Evaluate((NodeBase*)assignment->LeftSide,state);
		}
	}
	else if (node->GetType() == NT_VARIABLE)
	{
		VariableNode* varnode = (VariableNode*)node;
		//
		//printf(varnode->Value->c_str());
		WQObject* obj=environment->GetVariable(*varnode->Value);
		if (obj == NULL)
		{
			state->ReturnObject.GetAssigned(environment->CreateVariable(*(varnode->Value)));
		}
		else
		{
			state->ReturnObject.GetAssigned(obj);
		}

	}
	else if (node->GetType() == NT_STRING)
	{
		StringNode* strnode = (StringNode*)node;
		state->ReturnObject.SetStringValue( *strnode->Value);

	}
	else if (node->GetType() == NT_BOOLEAN)
	{
		BooleanNode * boolnode = (BooleanNode*)node;
		state->ReturnObject.SetBoolValue(boolnode->Value);

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
			Evaluate((*expit),state);
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
		bool anyifexecuted = false;
		if (it != ifnode->IfBlock->end())
		{
			
			Evaluate(it->first,state);
			if (state->ReturnObject.Type != DT_BOOLEAN)
			{
				throw RUNTIME_EXPECTING_BOOLEAN;
			}
			if (state->ReturnObject.GetBoolValue())
				Evaluate(it->second, state);
			anyifexecuted = true;
		}
		it++;
		if (anyifexecuted == false)
		{
			while (it != ifnode->IfBlock->end())
			{

				Evaluate(it->first, state);
				if (state->ReturnObject.Type != DT_BOOLEAN)
				{
					throw RUNTIME_EXPECTING_BOOLEAN;
				}
				if (state->ReturnObject.GetBoolValue())
				{
					Evaluate(it->second, state);
					break;
				}

				it++;
			}
		}
		
		if (anyifexecuted==false)
			Evaluate(ifnode->ElseBlock, state);
	}
	else if (node->GetType() == NT_WHILE)
	{
		WhileNode* whilenode = (WhileNode*)node;
		while (true)
		{
			Evaluate(whilenode->Condition, state);
			if (state->ReturnObject.Type != DT_BOOLEAN)
			{
				throw RUNTIME_EXPECTING_BOOLEAN;
			}
			if (state->ReturnObject.GetBoolValue() == false)
			{
				break;
			}
			Evaluate(whilenode->CodeBlock, state);
		}
	}
	else if (node->GetType() == NT_COMPARISON)
	{
		ComparisonNode* comnode = (ComparisonNode*)node;
		Evaluate(comnode->LeftSide, state);
		printf(comnode->Operator->c_str());
		Evaluate(comnode->RightSide, state);

	}
	else if (node->GetType() == NT_LOGIC)
	{
		LogicNode* lgnode = (LogicNode*)node;
		printf("Logic Node(Expressions:");
		for (list<ExpressionNode*>::iterator expit = lgnode->Expressions->begin();
			expit != lgnode->Expressions->end();
			expit++)
		{
			Evaluate(*expit, state);
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
		state->ReturnObject.SetFloatValue(fltnode->Value);
	}
	else if (node->GetType() == NT_INTEGER)
	{
		IntegerNode* intnode = (IntegerNode*)node;
		state->ReturnObject.SetIntValue(intnode->Value);
	}
	else if (node->GetType() == NT_FUNCTIONCALL)
	{
		FunctionCallNode* funcnode = (FunctionCallNode*)node;
		//retrive the target function from the function library
		WQFunction func = environment->Functions->Get(funcnode->FunctionName);
		if (func == NULL)
		{
			throw string(RUNTIME_FUNCTION_NOT_DEFINED) + ": "+*funcnode->FunctionName;
		}
		//check the parameter size of the function call
		state->ParamSize = funcnode->Parameters->size();
		//evaluate each of them and send them to the current state
		for (list<ExpressionNode*>::iterator it = funcnode->Parameters->begin(); it != funcnode->Parameters->end(); it++)
		{
			Evaluate(*it, state);
			string aa = state->ReturnObject.ToString();
			state->AddParam(state->ReturnObject);
		}
		//call the function 
		func(state);
	}
	else if (node->GetType() == NT_CODEBLOCK)
	{
		
		CodeBlockNode* program = (CodeBlockNode*)node;
		for (list<NodeBase*>::iterator it = program->Statements->begin(); it != program->Statements->end(); it++)
		{
			NodeBase* nxt = *it;
			Evaluate(*it, state);

		}
	}
	
}