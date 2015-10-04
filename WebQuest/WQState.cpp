#include "WQState.h"


void WQState::EnterNewEnvironment(EnvironmentType et)
{
	WQEnvironment* newevnt = new WQEnvironment;
	newevnt->Parent = CurrentEnvironment;
	CurrentEnvironment = newevnt;
	EnvironmentTypeStack.push(et);
}
void WQState::BackToParentEnvironment()
{
	WQEnvironment* parentevnt = CurrentEnvironment->Parent;
	delete CurrentEnvironment;
	CurrentEnvironment = parentevnt;
	if (EnvironmentTypeStack.size() != 0)
		EnvironmentTypeStack.pop();
}
EnvironmentType WQState::GetCurrentEnvironmentType()
{
	if (EnvironmentTypeStack.size() == 0)
	{
		return ET_ROOT;
	}
	else
		return EnvironmentTypeStack.top();
}
long long WQState::GetIntegerParam()
{
	WQObject* param = GetParam();
	if (param->Type == DT_INTEGER)
	{

		return param->GetIntValue();
	}
	else
		throw RUNTIME_EXPECTING_A_INTEGER_PARAMETER;
}
string WQState::GetStringParam()
{
	WQObject* obj=GetParam();
	if (obj->Type == DT_STRING)
		return obj->ToString();
	else
		throw RUNTIME_EXPECTING_A_STRING_PARAMETER;
}

map<string, WQObject*>* WQState::GetDictionaryParam()
{
	WQObject* obj = GetParam();
	if (obj->Type == DT_DICTIONARY)
		return obj->GetDictionary();
	else
		throw RUNTIME_EXPECTING_A_DICTIONARY_PARAMETER;
	
}

void WQState::ReturnReference(WQObject* ref)
{
	ReferencedObject = ref;
}
//void WQState::ReturnNewReference(WQObject* ref)
//{
//	ReferencedObject = ref;
//	CurrentEnvironment->TemporaryVariables.push_back(ref);
//}
WQObject* WQState::CreateObject()
{
	return CurrentEnvironment->CreateObject();
}
WQObject* WQState::GetReturnedReference()
{
	return ReferencedObject;
}
void WQState::ReturnNull()
{
	ReferencedObject = CurrentEnvironment->CreateObject();
}
void WQState::ReturnBoolean(bool val)
{
	WQObject* obj = CurrentEnvironment->CreateObject();
	obj->SetBoolValue(val);
	this->ReturnReference(obj);
}
void WQState::ReturnFloat(long double num)
{
	WQObject* obj = CurrentEnvironment->CreateObject();
	obj->SetFloatValue(num);
	this->ReturnReference(obj);
}
void WQState::ReturnInteger(long long num)
{
	WQObject* obj = CurrentEnvironment->CreateObject();
	obj->SetIntValue(num);
	this->ReturnReference(obj);
}
void WQState::ReturnString(string &str)
{
	WQObject* obj = CurrentEnvironment->CreateObject();
	obj->SetStringValue(str);
	this->ReturnReference(obj);
}
WQObject* WQState::GetParam()
{
	if (CallingParams.size() > 0)
	{
		WQObject* param = CallingParams.front();
		CallingParams.pop_front();
		return param;
	}
	else
		throw RUNTIME_TOO_FEW_ARGUMENTS;;
}
void WQState::AddParam(WQObject* obj)
{
	CallingParams.push_back(obj);
}
void WQState::ClearParams()
{
	while (!CallingParams.empty())
	{
		//delete CallingParams.front();
		CallingParams.pop_front();
	}
}

//WQObject* WQState::CreateReferenceObject(WQObject* targetobj)
//{
//	WQObject* obj = WQObject::Create();
//	obj->SetReference(targetobj);
//	CurrentEnvironment->TemporaryVariables.push_back(obj);
//	return obj;
//}
void WQState::MoveVariableToParentEnvironment(WQObject* obj)
{
	list<WQObject*>::iterator it = CurrentEnvironment->TemporaryVariables.begin();
	for (; it != CurrentEnvironment->TemporaryVariables.end(); it++)
	{
		if (*it == obj)
		{
			CurrentEnvironment->Parent->TemporaryVariables.push_back(*it);
			CurrentEnvironment->TemporaryVariables.erase(it);
			break;
		}
	}
	
}