#include <string>
#include <map>
#include <vector>
#include "Exception.h"
#include "Converter.h"
#include <sstream>
using namespace std;
#ifndef WQOBJECT_H
#define WQOBJECT_H
enum DataType{ DT_FUNCTION, DT_INTEGER, DT_FLOAT, DT_STRING, DT_LIST, DT_DICTIONARY ,DT_BOOLEAN,DT_NULL};
class WQObject
{

public:

	WQObject();
	~WQObject();
	WQObject(DataType type);
	string ToString() const;
	string ToElementString() const;
	void SetIntValue(long long val);
	void SetFloatValue(long double val);
	void SetStringValue(string& val);
	
	void SetBoolValue(bool val);
	void SetNull();
	void InitList();

	//void AppendListValue(long long val);
	//void AppendListValue(long double val);
	//void AppendListValue(string &val);
	//void AppendListvalue(bool val);
	void AppendListValue(WQObject& obj);
	WQObject* GetListElement(long index);
	void SetListElement(long index, WQObject& ele);

	void AppendKeyValue(string& key, string& value);
	void AppendKeyValue(string& key, long long value);

	void GetAssigned(WQObject* obj);
	long long GetIntValue() const;
	long double GetFloatValue() const;
	bool GetBoolValue() const;
	vector<WQObject*>* GetListValue() const;

	bool Assigned();
	bool IsNumeric() const;
	long long ToInteger() const;
	long double ToFloat() const;
	DataType Type;
	bool operator < ( const WQObject& right) ;
	bool operator > (const WQObject& right);
	bool operator <= (const WQObject& right);
	bool operator >= (const WQObject& right);
	bool operator == (const WQObject& right);
	WQObject& operator+=(const WQObject& right);
	WQObject& operator-=(const WQObject& right);
	WQObject& operator*=(const WQObject& right);
	WQObject& operator/=(const WQObject& right);
	WQObject& operator%=(const WQObject& right);
private:
	
	void * Data;
	bool assigned;
	void ClearValue();
};

WQObject& operator+(WQObject& left, const WQObject& right);
WQObject& operator-(WQObject& left, const WQObject& right);
WQObject& operator*(WQObject& left, const WQObject& right);
WQObject& operator/(WQObject& left, const WQObject& right);
WQObject& operator%(WQObject& left, const WQObject& right);
#endif
