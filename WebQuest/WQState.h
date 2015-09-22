#include <list>
#include "WQObject.h"
using namespace std;
#ifndef WQSTATE_H
#define WQSTATE_H
class WQState
{
public:
	WQState(){ ReferencedObject = NULL; }
	WQObject* GetParam();
	void AddParam(WQObject* obj);
	void ClearParams();
	void ReturnInteger(long long& integer);
	void ReturnString(string& str);
	void ReturnFloat(long double&);
	void ReturnNull();
	void ReturnReference(WQObject*);
	WQObject* GetReturnObject();
	
	int ParamSize;
	//void SetReturnObject(WQObject& obj);
	//WQObject* GetReturnObject();
private:
	WQObject* ReferencedObject;
	WQObject ReturnObject;
	//void CleanReturnObject();
	
	list<WQObject*> CallingParams;
};
#endif
