#include "WQObject.h"

WQObject::WQObject()
{
	Type = DT_NULL;
	data = NULL;
}
WQObject::~WQObject()
{
	ClearValue();
}
long long WQObject::GetIntValue()
{
	return *(long long *)data;
}
long double WQObject::GetFloatValue()
{
	return *(long double *)data;
}
bool WQObject::GetBoolValue()
{
	return *(bool*)data;
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
void WQObject::SetFloatValue(long double value)
{
	ClearValue();
	Type = DT_FLOAT;
	data = new long double;
	*((long double*)data) = value;
	assigned = true;
}
void WQObject::SetIntValue(long long value)
{
	ClearValue();
	Type = DT_INTEGER;
	data = new long long;
	*((long long*)data) = value;
	assigned = true;
}
void WQObject::SetStringValue(string& value)
{
	ClearValue();
	Type = DT_STRING;
	data = new string(value);
	//*((string*)data) = value;
	assigned = true;
}
void WQObject::SetListValue(list<WQObject> &value)
{
	ClearValue();
	Type = DT_LIST;
	data = new list < WQObject > ;
	*((list<WQObject>*)data) = value; 
	assigned = true;
}
void WQObject::SetBoolValue(bool val)
{
	ClearValue();
	Type = DT_BOOLEAN;
	data = new bool;
	*((bool*)data) = val;
	assigned = true;

}
void WQObject:: SetNull()
{
	ClearValue();
	Type = DT_NULL;
}
void WQObject::GetAssigned(WQObject* obj)
{
	if (obj->Type!=DT_NULL)
		ClearValue();

	if (obj->Type == DT_INTEGER)
	{
		SetIntValue(obj->GetIntValue());
	}
	else if (obj->Type == DT_FLOAT)
	{
		SetFloatValue(obj->GetFloatValue());
	}
	else if (obj->Type == DT_STRING)
	{
		SetStringValue(obj->ToString());
	}
	else if (obj->Type == DT_BOOLEAN)
	{
		SetBoolValue(obj->GetBoolValue());
	}
	
}
string WQObject::ToString()
{
	if (Type == DT_STRING)
	{
		return *((string*)data);
	}

	else if (Type == DT_INTEGER)
	{
		//Convertion::ToString(*((long long*)data));
		return "";
	}
	else if (this->Type == DT_NULL)
		return "Null";
	else if (Type == DT_FLOAT)
		return "float";
	else if (Type==DT_BOOLEAN)
	{
		if (GetBoolValue() == true)
		{
			return "true";
		}
		else
			return "false";
	}
	return "None";
}
