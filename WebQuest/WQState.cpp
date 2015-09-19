#include "WQState.h"

void WQState::ReturnNull()
{
	this->ReturnObject;
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