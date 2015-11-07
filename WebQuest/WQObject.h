#include <string>
#include <map>
#include <vector>
#include "WQException.h"
#include "Converter.h"

#include <sstream>
using namespace std;
#ifndef WQOBJECT_H
#define WQOBJECT_H

enum DataType{ DT_INTEGER, DT_FLOAT, DT_STRING, DT_LIST, DT_DICTIONARY ,DT_BOOLEAN,DT_STANDARD_FUNCTION,DT_USER_FUNCTION,DT_NULL};
class WQObject
{
public:

	~WQObject();
	WQObject(DataType type);
	static WQObject* Create(){ return new WQObject; }
	string ToString() const;
	string ToElementString() const;
	
	//void SetReference(WQObject* obj);

	void SetIntValue(long long val);
	void SetFloatValue(long double val);
	void SetStringValue(const string& val);
	void SetBoolValue(bool val);

	void SetUserFunctionValue(void* func);
	void SetStandardFunctionValue(void* func);

	void SetNull();
	void InitList();
	void InitDictionary();
	void AppendList(WQObject* obj);
	WQObject* GetListElement(long index);
	WQObject* GetDictionaryElement(string& key);
	WQObject* SetListElement(long index, WQObject* ele);


	WQObject* SetKeyValue(string key, WQObject* value);
	void SetKeyValue(string key, string value);
	void SetKeyValue(string key, long long value);

	void DeepCopy(WQObject* obj);

	void* GetUserFunctionValue();
	void* GetStandardFunctionValue();

	long long GetIntValue() const;
	long double GetFloatValue() const;
	bool GetBoolValue() const;
	vector<WQObject*>* GetList() const;
	map<string, WQObject*>* GetDictionary() const;
	void GetSlicingWithLeftIndex(long start, WQObject* targetlist);
	void GetSlicingWithRightIndex(long end, WQObject* targetlist);
	void GetSlicing(long start, long end,WQObject* targetlist);

	bool Assigned();
	bool IsNumeric() const;
	long long ToInteger() const;
	long double ToFloat() const;
	DataType Type;
	bool operator < ( const WQObject& right);
	bool operator > (const WQObject& right);
	bool operator <= (const WQObject& right);
	bool operator >= (const WQObject& right);
	bool operator == (const WQObject& right);
	WQObject& operator+=( WQObject& right);
	WQObject& operator-=(WQObject& right);
	WQObject& operator*=(WQObject& right);
	WQObject& operator/=(WQObject& right);
	WQObject& operator%=(WQObject& right);
	//bool IsReference = false;
	int ReferenceCounter = 0;
	//WQObject* Reference;
private:
	WQObject();

	void * Data;
	bool assigned;
	void ClearValue();
	void AssertCanAssign();
	
};

WQObject& operator+(WQObject& left, WQObject& right);
WQObject& operator-(WQObject& left, WQObject& right);
WQObject& operator*(WQObject& left, WQObject& right);
WQObject& operator/(WQObject& left, WQObject& right);
WQObject& operator%(WQObject& left, WQObject& right);
#endif
