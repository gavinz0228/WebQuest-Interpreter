#include "Runtime.h"
void Runtime::Run(char* script)
{
	Parser parser;
	parser.Parse(script);
	WQState state;
	Evaluate((NodeBase*)parser.program, &state);

}
void Calculate(WQObject& left, string* op, WQObject& right)
{
	if (*op == OP_PLUS)
	{
		left.GetAssigned(&(left + right));
	}
	else if (*op == OP_MINUS)
	{
		left.GetAssigned(&(left - right));
	}
	else if (*op == OP_DEVIDE)
	{
		left.GetAssigned(&(left / right));
	}
	else if (*op == OP_MULTIPLY)
	{
		left.GetAssigned(&(left * right));
	}
	else if (*op == OP_MODULO)
	{
		left.GetAssigned(&(left % right));
	}
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

		if (assignment->TargetType == AT_VARIABLE)
		{
			VariableNode* var = assignment->LeftSideVariable;
			environment->SetVariable(*var->Value, state->GetReturnObject());

		}
		else if (assignment->TargetType == AT_ELEMENT)
		{
			ElementNode* ele = assignment->LeftSideElement;
			Evaluate(ele->Variable, state);
			WQObject* lsobj = state->GetReturnObject();
			if (lsobj->Type == DT_LIST)
			{
				Evaluate(ele->key, state);
				//get the index
				int index = state->GetReturnObject()->GetIntValue();
				Evaluate(assignment->RightSide, state);
				lsobj->SetListElement(index,*state->GetReturnObject());
			}
			//environment->GetVariable()

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
			state->ReturnReference(environment->CreateVariable(*(varnode->Value)));
		}
		else
		{
			state->ReturnReference(obj);
		}
	}
	//else if (node->GetType() == NT_ELEMENT)
	//{
	//	ElementNode* elenode = (ElementNode*)node;
	//	Evaluate(elenode->key, state);
	//	int index = state->GetReturnObject()->GetIntValue();
	//	WQObject* obj = environment->GetVariable(*elenode->Variable->Value)->GetListElement(index);
	//	state->ReturnReference(obj);
	//}
	else if (node->GetType() == NT_STRING)
	{
		StringNode* strnode = (StringNode*)node;
		state->GetReturnObject()->SetStringValue( *strnode->Value);

	}
	else if (node->GetType() == NT_BOOLEAN)
	{
		BooleanNode * boolnode = (BooleanNode*)node;
		state->GetReturnObject()->SetBoolValue(boolnode->Value);

	}
	else if (node->GetType() == NT_OPERATION)
	{
		
		OperationNode* opnode = (OperationNode*)node;
		//
		list<ExpressionNode*>::iterator expit = opnode->Terms->begin();
		list<string*>::iterator opit = opnode->Operators->begin();
		stack<string*> lowops;
		stack<WQObject*> lowexps;
		WQObject left, right;
		if (opnode->Terms->size() - opnode->Operators->size() != 1)
		{
			throw SYNTAX_INVALID_EXPRESSION;
		}
		Evaluate(*expit, state);
		left.GetAssigned(state->GetReturnObject());
		expit++;
		WQObject* term;
		string* op;
		while (expit!=opnode->Terms->end())
		{
			op = *opit;
			Evaluate( *expit,state);
			//notice I new an object, needs to be delete later
			term = new WQObject;
			term->GetAssigned(state->GetReturnObject());
			//if sees - or +, always push , means save it for later
			if (*op == "-" || *op == "+" )
			{
				lowops.push(op);
				lowexps.push(term);
			}
			else
			{
				//if it's a * / %, pop the one element from the stack and evaluate, then push it back
				if (lowexps.size() > 0)
				{
					//
					WQObject* exp= lowexps.top();					
					Calculate(*exp, op,*term);
				}
				else
				{
					//the most left one, which is not in the stack
					Calculate(left, op, *term);
				}
			}
			//Evaluate(term, state);
			//Calculate(left, op, *state->GetReturnObject());

			++expit;
			++opit;
		}
		while (lowops.size() != 0)
		{
			WQObject* term= lowexps.top();
			string* op = lowops.top();
			Calculate(left, op, *term);
			delete term;
			lowexps.pop();
			lowops.pop();
		}
		state->GetReturnObject()->GetAssigned(&left);

	}
	else if (node->GetType() == NT_IF)
	{
		IfNode* ifnode = (IfNode*)node;
		map<ExpressionNode*, CodeBlockNode*>::iterator it = ifnode->IfBlock->begin();
		bool anyifexecuted = false;
		if (it != ifnode->IfBlock->end())
		{
			
			Evaluate(it->first,state);
			if (state->GetReturnObject()->Type != DT_BOOLEAN)
			{
				throw RUNTIME_EXPECTING_BOOLEAN;
			}
			if (state->GetReturnObject()->GetBoolValue())
				Evaluate(it->second, state);
			anyifexecuted = true;
		}
		it++;
		if (anyifexecuted == false)
		{
			while (it != ifnode->IfBlock->end())
			{

				Evaluate(it->first, state);
				if (state->GetReturnObject()->Type != DT_BOOLEAN)
				{
					throw RUNTIME_EXPECTING_BOOLEAN;
				}
				if (state->GetReturnObject()->GetBoolValue())
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
			if (state->GetReturnObject()->Type != DT_BOOLEAN)
			{
				throw RUNTIME_EXPECTING_BOOLEAN;
			}
			if (state->GetReturnObject()->GetBoolValue() == false)
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
		
		WQObject lhs;
		lhs.GetAssigned(state->GetReturnObject());

		Evaluate(comnode->RightSide, state);
		bool result=false;
		if (*comnode->Operator == OP_EQUAL)
		{
			result = lhs == *state->GetReturnObject();
		}
		else if (*comnode->Operator == OP_GREATER)
		{
			result = lhs > *state->GetReturnObject();
		}
		else if (*comnode->Operator ==OP_LESS)
		{
			result = lhs < *state->GetReturnObject();
		}
		else if (*comnode->Operator ==OP_GREATEQUAL)
		{
			result = lhs >= *state->GetReturnObject();
		}
		else if (*comnode->Operator == OP_LESSEQUAL)
		{
			result = lhs <= *state->GetReturnObject();
		}
		state->GetReturnObject()->SetBoolValue(result);
	}
	else if (node->GetType() == NT_LOGIC)
	{
		LogicNode* lgnode = (LogicNode*)node;
		stack<string> lowops;
		stack<bool> lowexps;
		bool left;
		list<string*>::iterator opit = lgnode->Operators->begin();
		list<ExpressionNode*>::iterator expit = lgnode->Expressions->begin();

		string* op;
		ExpressionNode* exp;
		exp = *expit;
		expit++;
		Evaluate(exp, state);
		left=(state->GetReturnObject()->GetBoolValue());
		while (expit != lgnode->Expressions->end())
		{
			op = *opit;
			exp = *expit;
			if (opit==lgnode->Operators->end())
			{
				throw SYNTAX_INVALID_EXPRESSION;
			}
			Evaluate(exp, state);
			if (*op == OP_NOT)
			{
				lowexps.push(!state->GetReturnObject()->GetBoolValue());
			}
			else if (*op == OP_AND)
			{
				//if there is something in the stack, deal with the stack first
				if (lowexps.size() > 0)
				{
					lowexps.top() = lowexps.top()&&state->GetReturnObject()->GetBoolValue();
				}
				else
					left = left&&state->GetReturnObject()->GetBoolValue();
			}
			//if it's ||, it has lower priority than &&, so push it to the stack for later
			else if (*op == OP_OR)
			{
				lowexps.push(state->GetReturnObject()->GetBoolValue());
				lowops.push(*op);
			}
			opit++;
			expit++;
		}
		while (lowexps.size() > 0)
		{
			string op = lowops.top();
			bool val = lowexps.top();
			//it must be OR operation!!
			//if (op == OP_OR)
			//{
			left = left || val;
			//}
			lowops.pop();
			lowexps.pop();
		}
		state->GetReturnObject()->SetBoolValue(left);
		/*
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
		*/
	}
	else if (node->GetType() == NT_FLOAT)
	{
		FloatNode* fltnode = (FloatNode*)node;
		state->GetReturnObject()->SetFloatValue(fltnode->Value);
	}
	else if (node->GetType() == NT_INTEGER)
	{
		IntegerNode* intnode = (IntegerNode*)node;
		state->GetReturnObject()->SetIntValue(intnode->Value);
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
			state->AddParam(state->GetReturnObject());
		}
		state->ReturnNull();
		//call the function 
		func(state);


		//clear the parameters in the list
		state->ClearParams();
	}
	else if (node->GetType() == NT_CREATELIST)
	{
		CreateListNode* lsnode = (CreateListNode*)node;
		WQObject result;
		result.InitList();
		for (list<ExpressionNode*>::iterator it = lsnode->Parameters->begin();
			it != lsnode->Parameters->end(); it++)
		{
			Evaluate(*it, state);
			result.AppendListValue(*state->GetReturnObject());
		}
		state->GetReturnObject()->GetAssigned(&result);

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

