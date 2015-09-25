#include "WQObject.h"

WQObject::WQObject()
{
	Type = DT_NULL;
	Data = NULL;
}
WQObject::~WQObject()
{
	ClearValue();
}
long long WQObject::GetIntValue() const
{
	return *(long long *)Data;
}
long double WQObject::GetFloatValue() const
{
	return *(long double *)Data;
}
bool WQObject::GetBoolValue() const
{
	if (Type == DT_BOOLEAN)
		return *(bool*)Data;
	else if (Type == DT_NULL)
		return false;
	else if (Type == DT_FLOAT)
	{
		if (GetFloatValue() == 0)
			return false;
		else
			return true;
	}
	else if (Type == DT_INTEGER)
	{
		if (GetIntValue() == 0)
			return false;
		else
			return true;
	}
	else if (Type == DT_LIST)
	{
		if (GetListValue()->size()==0)
			return false;
		else
			return true;
	}
	else if (Type == DT_DICTIONARY)
	{
		return true;
	}

}
vector<WQObject*>* WQObject::GetListValue() const
{
	return (vector<WQObject*>*)Data;
}
void WQObject::ClearValue()
{
	if (Data != NULL&&Type!=DT_NULL)
	{
		delete Data;
		Data = NULL;
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
	Data = new long double;
	*((long double*)Data) = value;
	assigned = true;
}
void WQObject::SetIntValue(long long value)
{
	ClearValue();
	Type = DT_INTEGER;
	Data = new long long;
	*((long long*)Data) = value;
	assigned = true;
}
void WQObject::SetStringValue(string& value)
{
	ClearValue();
	Type = DT_STRING;
	Data = new string(value);
	//*((string*)Data) = value;
	assigned = true;
}
void WQObject::InitList()
{
	ClearValue();
	Type = DT_LIST;
	Data = new vector < WQObject* > ;
	assigned = true;
}
//void WQObject::SetListValue(list<WQObject> &value)
//{
//	ClearValue();
//	Type = DT_LIST;
//	Data = new list < WQObject > ;
//	*((list<WQObject>*)Data) = value; 
//	assigned = true;
//}

void WQObject::AppendListValue(WQObject& obj)
{
	if (Type == DT_LIST)
	{
		vector < WQObject* >* ls = (vector < WQObject* >*)Data;
		WQObject* newobj = new WQObject;
		newobj->GetAssigned(&obj);
		ls->push_back(newobj);
	}
	else
	{
		throw RUNTIME_NON_LIST_APPENDING;
	}
}
WQObject* WQObject::GetListElement(long index)
{
	if (Type == DT_LIST)
	{
		vector < WQObject* >* ls = (vector < WQObject* >*)Data;
		if (index < ls->size() && index >= 0)
		{
			return ls->at(index);
		}
		else
			return NULL;
	}
	else
	{
		throw RUNTIME_NON_LIST_INDEXING;
	}
	return NULL;
}
void WQObject::SetListElement(long index, WQObject& ele)
{
	if (Type == DT_LIST)
	{
		vector < WQObject* >* ls = (vector < WQObject* >*)Data;
		if (index < ls->size() && index >= 0)
		{
			delete ls->at(index);
			WQObject* newObj = new WQObject;
			newObj->GetAssigned(&ele);
			(*ls)[index] = newObj;
		}
		else
			throw RUNTIME_INDEX_OUT_OF_BOUND;
	}
	else
	{
		throw RUNTIME_NON_LIST_INDEXING;
	}
}
void WQObject::SetBoolValue(bool val)
{
	ClearValue();
	Type = DT_BOOLEAN;
	Data = new bool;
	*((bool*)Data) = val;
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
	else if (obj->Type == DT_LIST)
	{
		ClearValue();
		InitList();
		vector < WQObject* >* newlist = GetListValue();
		vector < WQObject* >* income = obj->GetListValue();
		for (int i = 0; i < income->size(); i++)
		{
			WQObject* newobj = new WQObject;
			newobj->GetAssigned(income->at(i));
			newlist->push_back(newobj);
		}
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

string WQObject::ToElementString() const
{
	if (Type == DT_STRING)
	{
		return "\"" + *((string*)Data) + "\"";
	}
	else
	{
		return ToString();
	}
}

string WQObject::ToString() const
{
	if (Type == DT_STRING)
	{
		return *((string*)Data);
	}

	else if (Type == DT_INTEGER)
	{
		//Convertion::ToString(*((long long*)Data));
		return Converter::IntegerToString(*((long long*)Data));
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
	else if (Type == DT_LIST)
	{
		string output;
		output = "[";
		vector<WQObject*>* ls = GetListValue();
		for (int i = 0; i < ls->size(); i++)
		{
			output += ls->at(i)->ToElementString();
			if (i != ls->size() - 1)
				output += ",";
		}
		output += "]";
		return output;
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
	else if (Type == DT_BOOLEAN)
	{
		return GetBoolValue() == right.GetBoolValue();
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
WQObject& WQObject::operator-=( const WQObject& right)
{
	if (IsNumeric() && right.IsNumeric())
	{
		if (Type == DT_INTEGER&&right.Type == DT_INTEGER)
		{
			SetIntValue(ToInteger() - right.ToInteger());
			return *this;
		}
		else
		{
			SetFloatValue(ToFloat() - right.ToFloat());
			return *this;
		}
	}
}
WQObject& WQObject::operator*=(const WQObject& right)
{
	if (IsNumeric() && right.IsNumeric())
	{
		if (Type == DT_INTEGER&&right.Type == DT_INTEGER)
		{
			SetIntValue(ToInteger() * right.ToInteger());
			return *this;
		}
		else
		{
			SetFloatValue(ToFloat() * right.ToFloat());
			return *this;
		}
	}
}
WQObject& WQObject::operator/=(const WQObject& right)
{
	if (IsNumeric() && right.IsNumeric())
	{
		if (Type == DT_INTEGER&&right.Type == DT_INTEGER)
		{
			SetIntValue(ToInteger() / right.ToInteger());
			return *this;
		}
		else
		{
			SetFloatValue(ToFloat() / right.ToFloat());
			return *this;
		}
	}
}
WQObject& WQObject::operator%=(const WQObject& right)
{
	if (IsNumeric() && right.IsNumeric())
	{
		if (Type == DT_INTEGER&&right.Type == DT_INTEGER)
		{
			SetIntValue(ToInteger() % right.ToInteger());
			return *this;
		}
		else
		{
			SetFloatValue(fmod(ToFloat() , right.ToFloat()));
			return *this;
		}
	}
}

WQObject& operator+(WQObject& left, const WQObject& right)
{
	left += right;
	return left;
}
WQObject& operator-(WQObject& left, const WQObject& right)
{
	left -= right;
	return left;
}
WQObject& operator*(WQObject& left, const WQObject& right)
{
	left *= right;
	return left;
}
WQObject& operator/(WQObject& left, const WQObject& right)
{
	left /= right;
	return left;
}
WQObject& operator%(WQObject& left, const WQObject& right)
{
	left %= right;
	return left;
}