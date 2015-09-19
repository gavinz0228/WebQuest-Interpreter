#include "WQObject.h"

WQObject::WQObject()
{
	Type = DT_NONE;
	data = NULL;
}
WQObject::~WQObject()
{
	ClearValue();
}
void WQObject::ClearValue()
{
	if (data != NULL)
	{
		delete data;
	}
}

bool WQObject::Assigned()
{
	return assigned;
}
void WQObject::SetFloatValue(long double &value)
{
	ClearValue();
	Type = DT_FLOAT;
	data = new long double;
	*((long double*)data) = value;
	assigned = true;
}
void WQObject::SetIntValue(long long &value)
{
	ClearValue();
	Type = DT_INTEGER;
	data = new long long;
	*((long long*)data) = value;
	assigned = true;
}
void WQObject::SetStringValue(string &value)
{
	ClearValue();
	Type = DT_STRING;
	data = new string;
	*((string*)data) = value;
	assigned = true;
}
void WQObject::SetListValue(list<WQObject> &value)
{
	Type = DT_LIST;
	data = new list < WQObject > ;
	*((list<WQObject>*)data) = value; 
	assigned = true;
}
void WQObject:: SetNull()
{
	ClearValue();
	Type = DT_NULL;
}
string WQObject::ToString()
{
	if (this->Type == DT_NONE)
		return "None";
	else if (Type == DT_INTEGER)
	{
		//Convertion::ToString(*((long long*)data));
		return "";
	}
	else if (Type == DT_STRING)
	{
		return *((string*)data);
	}
	else if (Type == DT_FLOAT)
		return "";
	return "None";
}
