#include "Runtime.h"
void Runtime::Run(char* script)
{
	Parser parser;
	parser.Parse(script);
	WQState state;
	Evaluate((NodeBase*)parser.program, &state);

}
void Runtime::Run(string& script)
{
	Parser parser;
	parser.Parse(script.c_str());
	WQState state;
	Evaluate((NodeBase*)parser.program, &state);

}
void Runtime::Calculate(WQObject* left, string* op, WQObject* right,WQState* state)
{
	WQObject* result = state->CreateObject();
	if (*op == OP_PLUS)
	{
		result->DeepCopy(&(*left + *right));
	}
	else if (*op == OP_MINUS)
	{
		result->DeepCopy(&(*left - *right));
	}
	else if (*op == OP_DEVIDE)
	{
		result->DeepCopy(&(*left / *right));
	}
	else if (*op == OP_MULTIPLY)
	{
		result->DeepCopy(&(*left * *right));
	}
	else if (*op == OP_MODULO)
	{
		result->DeepCopy(&(*left % *right));
	}
	state->ReturnReference(result);
}
void PerformAssignment(WQObject* left, string *optr,WQObject* right,WQState* state)
{
	WQObject* result = state->CreateObject();
	if (*optr == OP_ASSIGN)
	{
		state->ReturnReference(right);
	}
	else if (*optr == OP_PLUSASSIGN)
	{
		result->DeepCopy(&((*left) + (*right)));
		state->ReturnReference(result);
	}
	else if (*optr == OP_MINUSASSIGN)
	{
		result->DeepCopy(&((*left) - (*right)));
		state->ReturnReference(result);
	}
	else if (*optr == OP_MULTIPLYASSIGN)
	{
		result->DeepCopy(&((*left) * (*right)));
		state->ReturnReference(result);
	}
	else if (*optr == OP_DEVIDEASSIGN)
	{
		result->DeepCopy(&((*left) / (*right)));
		state->ReturnReference(result);
	}
	else if (*optr == OP_MODULOASSIGN)
	{
		result->DeepCopy(&((*left) % (*right)));
		state->ReturnReference(result);
	}
	else
	{
		throw SYNTAX_INVALID_OPERATOR;
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
		

		CurrentLineNumber = assignment->GetLineNumber();
		
		if (assignment->TargetType == AT_VARIABLE)
		{
			VariableNode* var = assignment->LeftSideVariable;
			Evaluate(var, state);
			WQObject* left =state->GetReturnedReference();
			//Evaluate the right side
			Evaluate(assignment->RightSide, state);
			PerformAssignment(left, assignment->AssignmentOperator, state->GetReturnedReference(),state);
			state->CurrentEnvironment->SetVariable(*var->Value, state->GetReturnedReference());

		}
		else if (assignment->TargetType == AT_ELEMENT)
		{
			ElementNode* ele = assignment->LeftSideElement;
			Evaluate(ele->Variable, state);
			WQObject* lsobj = state->GetReturnedReference();
			if (lsobj->Type == DT_LIST)
			{
				Evaluate(ele->key, state);
				//get the index
				long index = state->GetReturnedReference()->GetIntValue();

				WQObject *left = lsobj->GetListElement(index);
				Evaluate(assignment->RightSide, state);
				PerformAssignment(left, assignment->AssignmentOperator, state->GetReturnedReference(), state);

				lsobj->SetListElement(index, state->GetReturnedReference());
				//WQObject* oldobj=lsobj->SetListElement(index,state->GetReturnedReference());
				//state->CurrentEnvironment->TemporaryVariables.push_back(oldobj);
			}
			else if (lsobj->Type == DT_DICTIONARY)
			{

				Evaluate(ele->key, state);
				//get the index
				string key = state->GetReturnedReference()->ToString();

				WQObject *left = lsobj->GetDictionaryElement(key);
				Evaluate(assignment->RightSide, state);
				PerformAssignment(left, assignment->AssignmentOperator, state->GetReturnedReference(),state);
				lsobj->SetKeyValue(key, state->GetReturnedReference());
				
			}
			//environment->GetVariable()

		}
	}
	else if (node->GetType() == NT_VARIABLE)
	{
		VariableNode* varnode = (VariableNode*)node;
		//
		//printf(varnode->Value->c_str());
		WQObject* obj=state->CurrentEnvironment->GetVariable(*varnode->Value);
		if (obj == NULL)
		{
			state->ReturnReference(state->CurrentEnvironment->CreateVariable(*(varnode->Value)));
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
		state->ReturnString( *strnode->Value);

	}
	else if (node->GetType() == NT_BOOLEAN)
	{
		BooleanNode * boolnode = (BooleanNode*)node;
		state->ReturnBoolean(boolnode->Value);

	}
	else if (node->GetType() == NT_OPERATION)
	{
		
		OperationNode* opnode = (OperationNode*)node;
		//
		list<ExpressionNode*>::iterator expit = opnode->Terms->begin();
		list<string*>::iterator opit = opnode->Operators->begin();
		stack<string*> lowops;
		stack<WQObject*> lowexps;
		WQObject* left = state->CreateObject();
		if (opnode->Terms->size() - opnode->Operators->size() != 1)
		{
			throw SYNTAX_INVALID_EXPRESSION;
		}
		Evaluate(*expit, state);
		left->DeepCopy(state->GetReturnedReference());
		expit++;
		WQObject* term;
		string* op;
		while (expit!=opnode->Terms->end())
		{
			op = *opit;
			Evaluate( *expit,state);
			//notice I new an object, needs to be delete later
			term = state->GetReturnedReference();
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
					lowexps.pop();
					Calculate(exp, op,term,state);
					lowexps.push(state->GetReturnedReference());
				}
				else
				{
					//the most left one, which is not in the stack
					Calculate(left, op, term,state);
					left = state->GetReturnedReference();
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
			Calculate(left, op, term,state);
			left = state->GetReturnedReference();
			lowexps.pop();
			lowops.pop();
		}
		state->ReturnReference(left);

	}
	else if (node->GetType() == NT_IF)
	{
		IfNode* ifnode = (IfNode*)node;
		map<ExpressionNode*, CodeBlockNode*>::iterator it = ifnode->IfBlock->begin();
		bool anyifexecuted = false;
		if (it != ifnode->IfBlock->end())
		{
			
			Evaluate(it->first,state);
			if (state->GetReturnedReference()->Type != DT_BOOLEAN)
			{
				throw RUNTIME_EXPECTING_BOOLEAN;
			}
			if (state->GetReturnedReference()->GetBoolValue())
			{
				state->EnterNewEnvironment();
				Evaluate(it->second, state);
				state->BackToParentEnvironment();
			}
			anyifexecuted = true;
		}
		it++;
		if (anyifexecuted == false)
		{
			while (it != ifnode->IfBlock->end())
			{

				Evaluate(it->first, state);
				if (state->GetReturnedReference()->Type != DT_BOOLEAN)
				{
					throw RUNTIME_EXPECTING_BOOLEAN;
				}
				if (state->GetReturnedReference()->GetBoolValue())
				{
					state->EnterNewEnvironment();
					Evaluate(it->second, state);
					state->BackToParentEnvironment();
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
			if (state->GetReturnedReference()->Type != DT_BOOLEAN)
			{
				throw RUNTIME_EXPECTING_BOOLEAN;
			}
			if (state->GetReturnedReference()->GetBoolValue() == false)
			{
				break;
			}
			state->BreakOccurred = false;
			state->EnterNewEnvironment();
			Evaluate(whilenode->CodeBlock, state);
			state->BackToParentEnvironment();
			if (state->BreakOccurred)
				break;
		}
	}
	else if (node->GetType() == NT_BEGIN)
	{
		BeginNode* bgnode = (BeginNode*)node;
		state->EnterNewEnvironment();
		Evaluate(bgnode->CodeBlock, state);
		state->BackToParentEnvironment();

	}
	else if (node->GetType() == NT_COMPARISON)
	{
		
		ComparisonNode* comnode = (ComparisonNode*)node;
		Evaluate(comnode->LeftSide, state);
		
		WQObject* left=state->GetReturnedReference();

		Evaluate(comnode->RightSide, state);
		bool result=false;
		if (*comnode->Operator == OP_EQUAL)
		{
			result = *left == *state->GetReturnedReference();
		}
		else if (*comnode->Operator == OP_GREATER)
		{
			result = *left > *state->GetReturnedReference();
		}
		else if (*comnode->Operator ==OP_LESS)
		{
			result = *left< *state->GetReturnedReference();
		}
		else if (*comnode->Operator ==OP_GREATEQUAL)
		{
			result = *left >= *state->GetReturnedReference();
		}
		else if (*comnode->Operator == OP_LESSEQUAL)
		{
			result = *left <= *state->GetReturnedReference();
		}
		state->ReturnBoolean(result);
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
		left = (state->GetReturnedReference()->GetBoolValue());
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
				lowexps.push(!state->GetReturnedReference()->GetBoolValue());
			}
			else if (*op == OP_AND)
			{
				//if there is something in the stack, deal with the stack first
				if (lowexps.size() > 0)
				{
					lowexps.top() = lowexps.top() && state->GetReturnedReference()->GetBoolValue();
				}
				else
					left = left&&state->GetReturnedReference()->GetBoolValue();
			}
			//if it's ||, it has lower priority than &&, so push it to the stack for later
			else if (*op == OP_OR)
			{
				lowexps.push(state->GetReturnedReference()->GetBoolValue());
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
		state->GetReturnedReference()->SetBoolValue(left);
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
		state->ReturnFloat(fltnode->Value);
	}
	else if (node->GetType() == NT_INTEGER)
	{
		IntegerNode* intnode = (IntegerNode*)node;
		state->ReturnInteger(intnode->Value);
	}
	else if (node->GetType() == NT_FUNCTIONCALL)
	{
		FunctionCallNode* funcnode = (FunctionCallNode*)node;
		CurrentLineNumber = funcnode->GetLineNumber();
		//retrive the target function from the function library
		WQFunction func = Functions.Get(funcnode->FunctionName);
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
			state->AddParam(state->GetReturnedReference());
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
		WQObject* result = state->CreateObject();
		result->InitList();
		for (list<ExpressionNode*>::iterator it = lsnode->Parameters->begin();
			it != lsnode->Parameters->end(); it++)
		{
			Evaluate(*it, state);
			result->AppendList(state->GetReturnedReference());
		}
		state->ReturnReference(result);
	}
	else if (node->GetType() == NT_CREATEDICT)
	{
		CreateDictionaryNode* dictnode = (CreateDictionaryNode*)node;
		WQObject* result = state->CreateObject();
		result->InitDictionary();
		state->ReturnReference(result);
	}
	else if (node->GetType() == NT_FOR)
	{
		ForNode* fornode = (ForNode*)node;
		Evaluate(fornode->IterableVariable, state);
		WQObject* iterable=state->GetReturnedReference();

		if (iterable->Type != DT_LIST)
			throw RUNTIME_ITERATE_NON_LIST_VARIABLE;
		vector<WQObject*>* iterablelist = iterable->GetList();
		for (int i = 0; i < iterablelist->size(); i++)
		{
			
			state->EnterNewEnvironment();
			state->CurrentEnvironment->SetVariable(*fornode->TempVariable->Value, iterablelist->at(i));
			Evaluate(fornode->CodeBlock, state);
			state->BackToParentEnvironment();
			if (state->BreakOccurred)
			{
				break;
			}
		}
	}
	else if (node->GetType() == NT_SLICING)
	{
		SlicingNode* slicenode = (SlicingNode*)node;
		//has start index
		if (slicenode->HasStartIndex)
		{
			Evaluate(slicenode->StartIndex, state);
			if (state->GetReturnedReference()->Type != DT_INTEGER)
			{
				throw RUNTIME_NON_INTERGER_INDEX;
			}
			long startIndex = (long)state->GetReturnedReference()->GetIntValue();
			if (slicenode->HasEndIndex)
			{
				
				Evaluate(slicenode->EndIndex, state);
				if (state->GetReturnedReference()->Type != DT_INTEGER)
				{
					throw RUNTIME_NON_INTERGER_INDEX;
				}
				long endIndex = (long)state->GetReturnedReference()->GetIntValue();
				//<variable>[<exp>:<exp>]
				Evaluate(slicenode->Variable, state);
				WQObject *lsvar = state->GetReturnedReference();
				WQObject* result = state->CreateObject();
				lsvar->GetSlicing(startIndex,endIndex,result);
				state->ReturnReference(result);
			}
			else
			{
				//<variable>[<exp>:]
				Evaluate(slicenode->Variable, state);
				WQObject *lsvar = state->GetReturnedReference();
				WQObject* result = state->CreateObject();
				lsvar->GetSlicingWithLeftIndex(startIndex,result);
				state->ReturnReference(result);
			}
		}
		//no start index
		else
		{
			if (slicenode->HasEndIndex)
			{
				Evaluate(slicenode->EndIndex, state);
				if (state->GetReturnedReference()->Type != DT_INTEGER)
				{
					throw RUNTIME_NON_INTERGER_INDEX;
				}
				long endIndex = (long)state->GetReturnedReference()->GetIntValue();
				//<variable>[:<exp>]
				Evaluate(slicenode->Variable, state);
				WQObject *lsvar = state->GetReturnedReference();
				WQObject* result = state->CreateObject();
				lsvar->GetSlicingWithRightIndex(endIndex,result);
				state->ReturnReference(result);
			}
			else
			{
				//<variable>[:]
				//just copy the list
				Evaluate(slicenode->Variable, state);

			}
		}

		

	}
	else if (node->GetType() == NT_CODEBLOCK)
	{
		
		CodeBlockNode* program = (CodeBlockNode*)node;

		for (list<NodeBase*>::iterator it = program->Statements->begin(); it != program->Statements->end(); it++)
		{
			NodeBase* nxt = *it;
			if (nxt->GetType() == NT_BREAK)
			{
				state->BreakOccurred = true;
				break;
			}
			Evaluate(*it, state);
		}

	}
}



