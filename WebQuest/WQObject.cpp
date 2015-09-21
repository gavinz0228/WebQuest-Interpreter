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
long long WQObject::GetIntValue() const
{
	return *(long long *)data;
}
long double WQObject::GetFloatValue() const
{
	return *(long double *)data;
}
bool WQObject::GetBoolValue() const
{
	return *(bool*)data;
}
void WQObject::ClearValue()
{
	if (data != NULL&&Type!=DT_NULL)
	{
		delete data;
		data = NULL;
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
	else if (obj->Type == DT_NULL)
	{
		SetNull();
	}
}
bool WQObject::IsNumeric() const
{
	if (Type == DT_FLOAT || Type == DT_INTEGER)
		return true;
	else
		return false;
}
long long WQObject::ToInteger() const
{
	if (IsNumeric())
	{
		if (Type == DT_INTEGER)
			return GetIntValue();
		else
		{
			long long var =(long long) GetFloatValue();
			return var;
		}
	}
	else
	{
		throw RUNTIME_NON_NUMERIC_CASTING;
	}
}

long double WQObject::ToFloat() const
{
	if (IsNumeric())
	{
		if (Type == DT_FLOAT)
			return GetFloatValue();
		else
		{
			return GetIntValue();;
		}
	}
	else
	{
		throw RUNTIME_NON_NUMERIC_CASTING;
	}
}


string WQObject::ToString() const
{
	if (Type == DT_STRING)
	{
		return *((string*)data);
	}

	else if (Type == DT_INTEGER)
	{
		//Convertion::ToString(*((long long*)data));
		return Converter::IntegerToString(*((long long*)data));
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


bool WQObject::operator < (const WQObject& right)
{
	//check if they are both numeric
	if (IsNumeric() && right.IsNumeric())
	{
		//try to use integer comparison, instead of double which is just a approximation
		if (Type == DT_INTEGER&&right.Type == DT_INTEGER)
			return ToInteger() < right.ToInteger();
		else
			return ToFloat() < right.ToFloat();
	}
	else
		throw RUNTIME_INVALID_TYPE_FOR_LESS_THAN;
	return false;
}
bool WQObject::operator > (const WQObject& right)
{
	if (IsNumeric() && right.IsNumeric())
	{
		if (Type == DT_INTEGER&&right.Type == DT_INTEGER)
			return ToInteger() > right.ToInteger();
		else
			return ToFloat() > right.ToFloat();
	}
	else
		throw RUNTIME_INVALID_TYPE_FOR_GREATER_THAN;

	return false;
}
bool WQObject::operator <= (const WQObject& right)
{

	if (IsNumeric() && right.IsNumeric())
	{
		if (Type == DT_INTEGER&&right.Type == DT_INTEGER)
			return ToInteger() <= right.ToInteger();
		else
			return ToFloat() <= right.ToFloat();
	}
	else
		throw RUNTIME_INVALID_TYPE_FOR_LESS_EQUAL;

	return false;
}
bool WQObject::operator >= (const WQObject& right)
{

	if (IsNumeric() && right.IsNumeric())
	{
		if (Type == DT_INTEGER&&right.Type == DT_INTEGER)
			return ToInteger() >= right.ToInteger();
		else
			return ToFloat() >= right.ToFloat();
	}
	else
		throw RUNTIME_INVALID_TYPE_FOR_GREATER_EQUAL;

	return false;
}
bool WQObject::operator == (const WQObject& right)
{

	if (IsNumeric() && right.IsNumeric())
	{
		if (Type == DT_INTEGER&&right.Type == DT_INTEGER)
			return ToInteger() == right.ToInteger();
		else
			return ToFloat() == right.ToFloat();
	}

	else if (Type == DT_STRING)
	{
		return ToString().compare(right.ToString()) == 0;
	}
	else
		throw RUNTIME_INVALID_TYPE_FOR_EQUAL;

	return false;

}
WQObject& WQObject::operator+=(const WQObject& right)
{
	if (IsNumeric() && right.IsNumeric())
	{
		if (Type == DT_INTEGER&&right.Type == DT_INTEGER)
		{
			SetIntValue(ToInteger() + right.ToInteger());
			return *this;
		}
		else
		{
			SetFloatValue(ToFloat() + right.ToFloat());
			return *this;
		}
	}
	else if (Type == DT_STRING)
	{
		stringstream ss;
		//use string stream as a buffer
		ss >> ToString() >> right.ToString();
		SetStringValue(ss.str());

	}
	else
		throw "Not Implemented yet";

	return *this;

}
WQObject& operator+(WQObject& left, const WQObject& right)
{
	left += right;
	return left;
}
