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
map<string,WQObject*>* WQObject::GetDictionaryValue() const
{
	return (map<string, WQObject*>*)Data;
}
void WQObject::ClearValue()
{
	if (Data != NULL&&Type!=DT_NULL)
	{
		if (Type == DT_LIST)
		{
			//delete every element of the list
			vector<WQObject*>* ls = GetListValue();
			for (int i = 0; i < ls->size(); i++)
				delete ls->at(i);
		}
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
void WQObject::InitDictionary()
{
	ClearValue();
	Type = DT_DICTIONARY;
	Data = new map <string,WQObject* >;
	assigned = true;
}
void WQObject:: SetKeyValue(string key, WQObject& value)
{
	if (Type == DT_DICTIONARY)
	{
		map<string, WQObject*>* dict = GetDictionaryValue();
		WQObject* newvalue = new WQObject;
		newvalue->GetAssigned(&value);
		dict->insert(pair<string, WQObject*>(key, newvalue));
	}
	else
		throw RUNTIME_NON_DICT_APPENDING;
}
void WQObject::SetKeyValue(string key, long long value){
	if (Type == DT_DICTIONARY)
	{
		map<string, WQObject*>* dict = GetDictionaryValue();
		WQObject* newvalue = new WQObject;
		newvalue->SetIntValue(value);
		dict->insert(pair<string, WQObject*>(key, newvalue));
	}
	else
		throw RUNTIME_NON_DICT_APPENDING;
}
void WQObject::SetKeyValue(string key, string value)
{
	if (Type == DT_DICTIONARY)
	{
		map<string, WQObject*>* dict = GetDictionaryValue();
		WQObject* newvalue = new WQObject;
		newvalue->SetStringValue(value);
		//dict->insert(pair<string, WQObject*>(key, newvalue));
		(*dict)[key] = newvalue;
	}
	else
		throw RUNTIME_NON_DICT_APPENDING;
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
WQObject* WQObject::GetDictionaryElement(string& key)
{
	if (Type == DT_DICTIONARY)
	{
		map<string, WQObject*>* dict = GetDictionaryValue();
		map<string, WQObject*>::iterator it = dict->find(key);
		{
			if (it != dict->end())
			{
				return it->second;
			}
			else
			{
				WQObject* obj = new WQObject;
				obj->InitDictionary();
				(*dict)[key] = obj;
				return obj;
			}
		}
	}
	else
		throw RUNTIME_NON_DICT_VALUE_ASSIGNMENT;
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
	else if (obj->Type == DT_DICTIONARY)
	{
		ClearValue();
		InitDictionary();
		map<string, WQObject*>* newdict = GetDictionaryValue();
		map<string, WQObject*>* olddict = obj->GetDictionaryValue();
		map<string, WQObject*>::iterator it = olddict->begin();
		for (; it != olddict->end(); it++)
		{
			WQObject* obj=new WQObject;
			obj->GetAssigned(it->second);
			newdict->insert(pair<string, WQObject*>(it->first, obj));
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
		return to_string(GetFloatValue());
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
	else if (Type == DT_DICTIONARY)
	{
		string output="{";
		map<string, WQObject*>* dict = GetDictionaryValue();
		map<string, WQObject*>::iterator it = dict->begin();
		for (; it != dict->end(); it++)
		{
			output += "\"" + it->first + "\"" + ":";
			output += it->second->ToElementString();
			if (next(it) != dict->end())
				output += ",";
			
		}
		output += "}";
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
	else if (Type == DT_STRING||right.Type==DT_STRING)
	{
		//stringstream ss;
		////use string stream as a buffer
		//ss >> ToString() >> right.ToString();
		//ss.flush();
		//string cc = ss.str();
		SetStringValue(ToString()+ right.ToString());

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
	else
		throw RUNTIME_EXPECTING_NUMERIC;
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
	else
		throw RUNTIME_EXPECTING_NUMERIC;
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
	else
		throw RUNTIME_EXPECTING_NUMERIC;
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
	else
		throw RUNTIME_EXPECTING_NUMERIC;
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
void WQObject::GetSlicingWithLeftIndexValue(long start, WQObject* targetlist)
{
	if (Type == DT_LIST)
	{

		targetlist->InitList();
		vector<WQObject*>* original = GetListValue();
		long startindex;
		//if it's 0, start from the begingning
		if (start>= 0)
			startindex = start;
		else if (start < 0)
			startindex = original->size() + start;
		//make sure the start index is greater than 0
		if (startindex< 0)  startindex = 0;
		//copy values to new list
		for (int i = startindex; i <original->size(); i++)
		{
			targetlist->AppendListValue(*original->at(i));
		}
	}
	else if (Type == DT_STRING)
	{
		string original = ToString();
		string result;
		long startindex;
		//if it's 0, start from the begingning
		if (start >= 0)
			startindex = start;
		else if (start < 0)
			startindex = original.size() + start;
		//make sure the start index is greater than 0
		if (startindex< 0)  startindex = 0;
		result = original.substr(startindex, original.size() - startindex );
		targetlist->SetStringValue(result);
	}
	else
	{
		throw RUNTIME_SLICING_NON_LIST_VARIABLE;
	}
	
}
void WQObject::GetSlicingWithRightIndexValue(long end, WQObject* targetlist)
{
	if (Type == DT_LIST)
	{

		targetlist->InitList();
		vector<WQObject*>* original = GetListValue();
		long endindex;
		// if it's greater than 0 ,normal index
		//if it's 0, don't need slicing
		if (end >= 0)
			endindex = end;
		else if (end < 0)
			endindex = original->size() + end;
		//copy values to new list
		for (int i = 0; i < endindex&&i<original->size(); i++)
		{
			targetlist->AppendListValue(*original->at(i));
		}
		// otherwise ,return the empty list
	}
	
	else if (Type == DT_STRING)
	{
		string original = ToString();
		string result;
		long endindex;
		// if it's greater than 0 ,normal index
		//if it's 0, don't need slicing
		if (end >= 0)
			endindex = end;
		else if (end < 0)
			endindex = original.size() + end;
		result = original.substr(0, original.size() - endindex);
		targetlist->SetStringValue(result);
	}
	else
	{
		throw RUNTIME_SLICING_NON_LIST_VARIABLE;
	}
}
void WQObject::GetSlicing(long start, long end,WQObject* targetlist)
{
	if (Type == DT_LIST)
	{

		targetlist->InitList();
		vector<WQObject*>* original = GetListValue();
		long endindex;
		long startindex;
		//make sure the end index is good
		if (end >= 0)
			endindex = end<original->size()?end:original->size();
		else 
			//if (end < 0)
		{
			endindex = original->size() + end;
			if (endindex < 0) endindex = 0;
		}
		//make sure the start index is good
		if (start >= 0)
			startindex = start < original->size() ? start : original->size();
		else {
			//if (start<0)
			startindex = start + original->size();
			if (startindex < 0) startindex = 0;
		}
		//copy values to new list
		for (int i = startindex; i < endindex; i++)
		{
			targetlist->AppendListValue(*original->at(i));
		}

	}
	else if (Type == DT_STRING)
	{
		string original = ToString();
		long endindex;
		long startindex;
		//make sure the end index is good
		if (end >= 0)
			endindex = end<original.size() ? end : original.size();
		else
			//if (end < 0)
		{
			endindex = original.size() + end;
			if (endindex < 0) endindex = 0;
		}
		//make sure the start index is good
		if (start >= 0)
			startindex = start < original.size() ? start : original.size();
		else {
			//if (start<0)
			startindex = start + original.size();
			if (startindex < 0) startindex = 0;
		}
		string result = original.substr(startindex, endindex-startindex);
		targetlist->SetStringValue(result);

	}
	else
	{
		throw RUNTIME_SLICING_NON_LIST_VARIABLE;
	}
}