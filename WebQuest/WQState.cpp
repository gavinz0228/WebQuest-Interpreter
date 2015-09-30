#include "WQState.h"


void WQState::EnterNewEnvironment()
{
	Environment* newevnt = new Environment;
	newevnt->Parent = CurrentEnvironment;
	CurrentEnvironment = newevnt;
}
void WQState::BackToParentEnvironment()
{
	Environment* parentevnt = CurrentEnvironment->Parent;
	delete CurrentEnvironment;
	CurrentEnvironment = parentevnt;
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
		throw RUNTIME_EXPECTING_A_STRING_PARAM;
}
void WQState::ReturnReference(WQObject* ref)
{
	ReferencedObject = ref;
}
void WQState::ReturnNewReference(WQObject* ref)
{
	ReferencedObject = ref;
	CurrentEnvironment->TemporaryVariables.push_back(ref);
}
WQObject* WQState::GetReturnedReference()
{
	return ReferencedObject;
}
void WQState::ReturnNull()
{
	ReferencedObject = NULL;
}
void WQState::ReturnBoolean(bool val)
{
	WQObject* obj = new WQObject;
	obj->SetBoolValue(val);
	this->ReturnReference(obj);
}
void WQState::ReturnFloat(long double num)
{
	WQObject* obj = new WQObject;
	obj->SetFloatValue(num);
	this->ReturnNewReference(obj);
}
void WQState::ReturnInteger(long long num)
{
	WQObject* obj = new WQObject;
	obj->SetIntValue(num);
	this->ReturnNewReference(obj);
}
void WQState::ReturnString(string &str)
{
	WQObject* obj = new WQObject;
	obj->SetStringValue(str);
	this->ReturnNewReference(obj);
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