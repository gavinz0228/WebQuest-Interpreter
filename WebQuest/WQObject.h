#include <string>
#include <map>
#include "Converter.h"
using namespace std;
#ifndef WQOBJECT_H
#define WQOBJECT_H
enum DataType{ DT_FUNCTION, DT_INTEGER, DT_FLOAT, DT_STRING, DT_LIST, DT_DICTIONARY, DT_NONE ,DT_NULL};
class WQObject
{

public:

	WQObject();
	~WQObject();
	WQObject(DataType type);
	string ToString();
	void SetIntValue(long long &val);
	void SetFloatValue(long double &val);
	void SetStringValue(string &val);
	void SetListValue(list<WQObject> &val);
	void SetNull();
	long long GetIntValue();
	long double GetFloatValue();
	list<WQObject>* GetListValue();
	bool Assigned();

private:
	DataType Type;
	void * data;
	bool assigned;
	void ClearValue();
};

#endif
