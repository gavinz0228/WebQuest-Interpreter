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
//void WQState::ReturnNewReference(WQObject* ref)
//{
//	ReferencedObject = ref;
//	CurrentEnvironment->TemporaryVariables.push_back(ref);
//}
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
	WQObject* obj = CreateObject();
	obj->SetBoolValue(val);
	this->ReturnReference(obj);
}
void WQState::ReturnFloat(long double num)
{
	WQObject* obj = CreateObject();
	obj->SetFloatValue(num);
	this->ReturnReference(obj);
}
void WQState::ReturnInteger(long long num)
{
	WQObject* obj = CreateObject();
	obj->SetIntValue(num);
	this->ReturnReference(obj);
}
void WQState::ReturnString(string &str)
{
	WQObject* obj = CreateObject();
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
WQObject* WQState::CreateObject()
{
	WQObject* obj = WQObject::Create();
	CurrentEnvironment->TemporaryVariables.push_back(obj);
	return obj;
}
WQObject* WQState::CreateReferenceObject(WQObject* targetobj)
{
	WQObject* obj = WQObject::Create();
	obj->SetReference(targetobj);
	CurrentEnvironment->TemporaryVariables.push_back(obj);
	return obj;
}