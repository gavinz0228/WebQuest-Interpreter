#include "WQState.h"

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
void WQState::AddParam(WQObject& obj)
{
	CallingParams.push_back(&obj);
}