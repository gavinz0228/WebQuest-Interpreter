#include "WQObject.h"

WQObject::WQObject()
{
	type = DT_NONE;
	data = NULL;
}
WQObject::~WQObject()
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
	type = DT_FLOAT;
	data = new long double;
	*((long double*)data) = value;
	assigned = true;
}
void WQObject::SetIntValue(long long &value)
{
	type = DT_INTEGER;
	data = new long long;
	*((long long*)data) = value;
	assigned = true;
}
void WQObject::SetStringValue(string &value)
{
	type = DT_STRING;
	data = new string;
	*((string*)data) = value;
	assigned = true;
}
void WQObject::SetListValue(list<WQObject> &value)
{
	type = DT_LIST;
	data = new list < WQObject > ;
	*((list<WQObject>*)data) = value; 
	assigned = true;
}
string WQObject::ToString()
{
	if (this->type == DT_NONE)
		return "None";
	else if (type == DT_INTEGER)
	{
		//Convertion::ToString(*((long long*)data));
		return "";
	}
	else if (type == DT_FLOAT)
		return "";
	return "None";
}
