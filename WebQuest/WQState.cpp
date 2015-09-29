#include "WQState.h"

WQObject* WQState::GetReturnObject()
{
	if (ReferencedObject == NULL)
	{
		return &ReturnObject;
	}
	else
	{
		WQObject* holder = ReferencedObject;
		ReferencedObject = NULL;
		return holder;
	}
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
void WQState::ReturnNull()
{
	this->ReturnObject;
}
void WQState::ReturnFloat(long double num)
{
	this->ReturnObject.SetFloatValue(num);
}
void WQState::ReturnInteger(long long num)
{
	this->ReturnObject.SetIntValue(num);
}
void WQState::ReturnString(string &str)
{
	this->ReturnObject.SetStringValue(str);
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
	//WQObject * param = new WQObject;
	//param->GetAssigned(&obj);
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