#include <list>
#include "WQObject.h"
using namespace std;
#ifndef WQSTATE_H
#define WQSTATE_H
class WQState
{
public:
	
	WQObject* GetParam();
	void AddParam(WQObject& obj);
	void ReturnInteger(long long&);
	void ReturnString(string& str);
	void ReturnFloat(long double&);
	void ReturnNull();
	WQObject ReturnObject;
	int ParamSize;
	//void SetReturnObject(WQObject& obj);
	//WQObject* GetReturnObject();
private:
	//void CleanReturnObject();
	
	list<WQObject*> CallingParams;
};
#endif
