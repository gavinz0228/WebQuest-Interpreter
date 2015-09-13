#include <string>
#include <map>
#include "Converter.h"
using namespace std;
#ifndef WQOBJECT_H
#define WQOBJECT_H
enum Type{ DT_FUNCTION, DT_INTEGER, DT_FLOAT, DT_STRING, DT_LIST, DT_DICTIONARY, DT_NONE };
class WQObject
{

public:

	WQObject();
	~WQObject();
	WQObject(Type type);
	string ToString();
	void SetIntValue(long long &val);
	void SetFloatValue(long double &val);
	void SetStringValue(string &val);
	void SetListValue(list<WQObject> &val);
	long long GetIntValue();
	long double GetFloatValue();
	list<WQObject>* GetListValue();
	bool Assigned();


private:
	Type type;
	void * data;
	bool assigned;
};

#endif
