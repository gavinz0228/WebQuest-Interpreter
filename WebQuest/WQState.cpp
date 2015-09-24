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
string WQState::GetStringParam()
{
	WQObject* obj=GetParam();
	return obj->ToString();
}
void WQState::ReturnReference(WQObject* ref)
{
	ReferencedObject = ref;
}
void WQState::ReturnNull()
{
	this->ReturnObject;
}
void WQState::ReturnFloat(long double& num)
{
	this->ReturnObject.SetFloatValue(num);
}
void WQState::ReturnInteger(long long& num)
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
		return NULL;
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