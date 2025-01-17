#include "WQObject.h"

WQObject::WQObject()
{
	Type = DT_NULL;
	Data = NULL;

}
WQObject::~WQObject()
{
	ClearValue();
	//if (IsReference)
	//	SetReference(NULL);
}
void WQObject::AssertCanAssign()
{
	//if (assigned == true)
	//	throw RUNTIME_REASSIGN_OBJECT_NOT_ALLOW;
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
		if (GetList()->size()==0)
			return false;
		else
			return true;
	}
	else if (Type == DT_DICTIONARY)
	{
		if (GetDictionary()->size() > 0)
			return true;
		else
			return false;
	}
	return false;

}
void* WQObject::GetUserFunctionValue()
{
	return Data;
}
void* WQObject::GetStandardFunctionValue()
{
	return Data;
}
vector<WQObject*>* WQObject::GetList() const
{
	return (vector<WQObject*>*)Data;
}
map<string,WQObject*>* WQObject::GetDictionary() const
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
			vector<WQObject*>* ls = GetList();
			for (size_t i = 0; i < ls->size(); i++)
			{
				ls->at(i)->ReferenceCounter--;
			}
		}
		if (Type != DT_STANDARD_FUNCTION && Type != DT_USER_FUNCTION)
		{
			delete Data;
			Data = NULL;
		}
		else
		{

		}
		


	}
}

bool WQObject::Assigned()
{
	return assigned;
}
void WQObject::SetUserFunctionValue(void* func)
{
	AssertCanAssign();
	Type = DT_USER_FUNCTION;
	Data = func;
	assigned = true;
}
void WQObject::SetStandardFunctionValue(void* func)
{
	AssertCanAssign();
	Type = DT_STANDARD_FUNCTION;
	Data = func;
	assigned = true;
}

void WQObject::SetFloatValue(long double value)
{
	AssertCanAssign();
	Type = DT_FLOAT;
	Data = new long double;
	*((long double*)Data) = value;
	assigned = true;
}
void WQObject::SetIntValue(long long value)
{
	AssertCanAssign();
	Type = DT_INTEGER;
	Data = new long long;
	*((long long*)Data) = value;
	assigned = true;
}
void WQObject::SetStringValue(const string& value)
{

	AssertCanAssign();
	Type = DT_STRING;
	Data = new string(value);
	//*((string*)Data) = value;
	assigned = true;
}
void WQObject::InitList()
{

	AssertCanAssign();
	Type = DT_LIST;
	Data = new vector < WQObject* > ;
	assigned = true;
}
void WQObject::InitDictionary()
{
	AssertCanAssign();
	Type = DT_DICTIONARY;
	Data = new map <string,WQObject* >;
	assigned = true;
}
WQObject* WQObject:: SetKeyValue(string key, WQObject* value)
{
	WQObject* oldobj = NULL;
	if (Type == DT_DICTIONARY)
	{
		map<string, WQObject*>* dict = GetDictionary();
		//WQObject* ref = new WQObject;
		//ref->SetReference(value);
		value->ReferenceCounter++;
		WQObject* oldobj = (*dict)[key];
		if (oldobj == NULL)
		{
			(*dict)[key] = value;
		}
		else
		{
			oldobj->ReferenceCounter--;
			oldobj = (*dict)[key];
			//change the value
			(*dict)[key] = value;
			
		}
	}
	else
		throw RUNTIME_NON_DICT_APPENDING;
	return oldobj;
}


//void WQObject::SetListValue(list<WQObject> &value)
//{
//	AssertCanAssign();
//	Type = DT_LIST;
//	Data = new list < WQObject > ;
//	*((list<WQObject>*)Data) = value; 
//	assigned = true;
//}


void WQObject::AppendList(WQObject* obj)
{
	if (Type == DT_LIST)
	{
		vector < WQObject* >* ls = (vector < WQObject* >*)Data;
		//WQObject* ref = new WQObject;
		//ref->ReferenceCounter++;
		//ref->SetReference(obj);
		obj->ReferenceCounter++;
		ls->push_back(obj);
	}
	else
	{
		throw RUNTIME_NON_LIST_APPENDING;
	}
}
WQObject* WQObject::GetListElement(long _index)
{

	if (Type == DT_LIST)
	{
		vector < WQObject* >* ls = (vector < WQObject* >*)Data;
		unsigned long index;
		if (_index >= 0)
		{
			if (_index >= ls->size())
				return NULL;
			index = (unsigned long)_index;
		}
		else
		{
			if (_index + ls->size() < 0)
				return NULL;
			index = (unsigned long)(_index + ls->size());
		}
		return ls->at(index);
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
		map<string, WQObject*>* dict = GetDictionary();
		map<string, WQObject*>::iterator it = dict->find(key);
		{
			if (it != dict->end())
			{
				return it->second;
			}
			else
			{
				WQObject* obj = new WQObject;
				(*dict)[key] = obj;
				return obj;
			}
		}
	}
	else
		throw RUNTIME_NON_DICT_VALUE_ASSIGNMENT;
}
WQObject* WQObject::SetListElement(long _index, WQObject* ele)
{
	unsigned long index = _index;
	WQObject* oldobj=NULL;
	if (Type == DT_LIST)
	{
		vector < WQObject* >* ls = (vector < WQObject* >*)Data;
		if (index < ls->size() && index >= 0)
		{
			oldobj= ls->at(index);
			oldobj->ReferenceCounter--;
			//WQObject* ref = WQObject::Create();
			//ref->SetReference(ele);
			ele->ReferenceCounter++;
			(*ls)[index] = ele;
			
		}
		else
			throw RUNTIME_INDEX_OUT_OF_BOUND;
	} 
	else
	{
		throw RUNTIME_NON_LIST_INDEXING;
	}
	return oldobj;
}
void WQObject::SetBoolValue(bool val)
{
	AssertCanAssign();
	Type = DT_BOOLEAN;
	Data = new bool;
	*((bool*)Data) = val;
	assigned = true;

}
void WQObject:: SetNull()
{
	AssertCanAssign();
	Type = DT_NULL;
}
void WQObject::DeepCopy(WQObject* obj)
{
	//if (IsReference)
	//{
	//	Reference->DeepCopy(obj);
	//	return;
	//}
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
		AssertCanAssign();
		InitList();
		vector < WQObject* >* newlist = GetList();
		vector < WQObject* >* income = obj->GetList();
		for (size_t i = 0; i < income->size(); i++)
		{
			WQObject* newobj = new WQObject;
			newobj->DeepCopy(income->at(i));
			newlist->push_back(newobj);
		}
	}
	else if (obj->Type == DT_DICTIONARY)
	{
		AssertCanAssign();
		InitDictionary();
		map<string, WQObject*>* newdict = GetDictionary();
		map<string, WQObject*>* olddict = obj->GetDictionary();
		map<string, WQObject*>::iterator it = olddict->begin();
		for (; it != olddict->end(); it++)
		{
			WQObject* obj=new WQObject;
			obj->DeepCopy(it->second);
			newdict->insert(pair<string, WQObject*>(it->first, obj));
		}
	}
	else if (obj->Type == DT_USER_FUNCTION)
	{
		SetUserFunctionValue(obj->GetUserFunctionValue());
	}
	else if (obj->Type == DT_STANDARD_FUNCTION)
	{
		SetStandardFunctionValue(obj->GetStandardFunctionValue());
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
			return GetIntValue()*1.0;
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
		return "null";
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
		stringstream ss;
		ss<< "[";
		vector<WQObject*>* ls = GetList();
		for (size_t i = 0; i < ls->size(); i++)
		{
			ss<< ls->at(i)->ToElementString();
			if (i != ls->size() - 1)
				ss << ",";
		}
		ss << "]";
		return ss.str();
	}
	else if (Type == DT_DICTIONARY)
	{
		stringstream ss;
		ss<<"{";
		map<string, WQObject*>* dict = GetDictionary();
		map<string, WQObject*>::iterator it = dict->begin();
		for (; it != dict->end(); it++)
		{
			ss<<"\"" <<it->first << "\"" << ":";
			ss << it->second->ToElementString();
			if (next(it) != dict->end())
				ss << ",";
		}
		ss<< "}";
		return ss.str();
	}
	return "none";
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
		return this == &right;

}
WQObject& WQObject::operator+=(WQObject& right)
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
		stringstream ss;
		//use string stream as a buffer
		//ss >> ToString() >> right.ToString();
		//ss.flush();
		
		SetStringValue(ToString()+right.ToString());

	}
	else
		throw "Not Implemented yet";

	return *this;

}
WQObject& WQObject::operator-=( WQObject& right)
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
WQObject& WQObject::operator*=(WQObject& right)
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
WQObject& WQObject::operator/=(WQObject& right)
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
WQObject& WQObject::operator%=(WQObject& right)
{

	if (IsNumeric() &&right.IsNumeric())
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

WQObject& operator+(WQObject& left, WQObject& right)
{
	left += right;
	return left;
}
WQObject& operator-(WQObject& left, WQObject& right)
{
	left -= right;
	return left;
}
WQObject& operator*(WQObject& left, WQObject& right)
{
	left *= right;
	return left;
}
WQObject& operator/(WQObject& left, WQObject& right)
{
	left /= right;
	return left;
}
WQObject& operator%(WQObject& left, WQObject& right)
{
	left %= right;
	return left;
}
void WQObject::GetSlicingWithLeftIndex(long start, WQObject* targetlist)
{

	if (Type == DT_LIST)
	{

		targetlist->InitList();
		vector<WQObject*>* original = GetList();
		long startindex;
		//if it's 0, start from the begingning
		if (start>= 0)
			startindex = start;
		else if (start < 0)
			startindex = original->size() + start;
		//make sure the start index is greater than 0
		if (startindex< 0)  startindex = 0;
		//copy values to new list
		for (size_t i = startindex; i <original->size(); i++)
		{
			targetlist->AppendList(original->at(i));
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
void WQObject::GetSlicingWithRightIndex(long end, WQObject* targetlist)
{
	if (Type == DT_LIST)
	{

		targetlist->InitList();
		vector<WQObject*>* original = GetList();
		unsigned long endindex;
		// if it's greater than 0 ,normal index
		//if it's 0, don't need slicing
		if (end >= 0)
			endindex = end;
		else if (end < 0)
			endindex = original->size() + end;
		//copy values to new list
		for (size_t i = 0; i < endindex&&i<original->size(); i++)
		{
			targetlist->AppendList(original->at(i));
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
//void WQObject::SetReference(WQObject* obj)
//{
//	//decrement the reference counter of the old object
//	if (Reference != NULL)
//		Reference->ReferenceCounter--;
//	Reference = obj;
//	//increase the counter of the obj being refrenced
//	obj->ReferenceCounter++;
//	IsReference = true;
//}
void WQObject::GetSlicing(long start, long end,WQObject* targetlist)
{
	if (Type == DT_LIST)
	{

		targetlist->InitList();
		vector<WQObject*>* original = GetList();
		unsigned long endindex;
		unsigned long startindex;
		//make sure the end index is good
		if (end >= 0)
			endindex = (unsigned long)end<original->size() ? end : original->size();
		else 
			//if (end < 0)
		{
			endindex = original->size() + end;
			if (endindex < 0) endindex = 0;
		}
		//make sure the start index is good
		if (start >= 0)
			startindex = (unsigned long)start < original->size() ? start : original->size();
		else {
			//if (start<0)
			startindex = start + original->size();
			if (startindex < 0) startindex = 0;
		}
		//copy values to new list
		for (size_t i = startindex; i < endindex; i++)
		{
			targetlist->AppendList(original->at(i));
		}

	}
	else if (Type == DT_STRING)
	{
		string original = ToString();
		unsigned long endindex;
		unsigned long startindex;
		//make sure the end index is good
		if (end >= 0)
			endindex = (unsigned long)end<original.size() ? end : original.size();
		else
			//if (end < 0)
		{
			endindex = original.size() + end;
			if (endindex < 0) endindex = 0;
		}
		//make sure the start index is good
		if (start >= 0)
			startindex = (unsigned long)start < original.size() ? (unsigned long)start : original.size();
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