
using namespace std;
#ifndef WQSTATE_H
#define WQSTATE_H
#include <list>
#include "WQObject.h"
#include "Environment.h"
class Environment;
class WQState
{
public:

	WQState(){
		ReferencedObject = NULL; 
		BreakOccurred = false;
		CurrentEnvironment = new Environment;
	}
	~WQState(){
		Environment* current = CurrentEnvironment;
		while (current != NULL)
		{
			//delete its parent environment until the parent is null
			Environment* temp = current->Parent;
			delete current;
			current = temp;
		}
	}
	WQObject* CreateObject();
	WQObject* CreateReferenceObject(WQObject* targetobj);
	WQObject* GetParam();
	string GetStringParam();
	long long GetIntegerParam();
	long double GetFloatParam();
	void AddParam(WQObject* obj);
	void ClearParams();
	void ReturnInteger(long long integer);
	void ReturnString(string& str);
	void ReturnFloat(long double);
	void ReturnNull();
	void ReturnBoolean(bool val);
	void ReturnReference(WQObject*);
	//void ReturnNewReference(WQObject* ref);
	WQObject* GetReturnedReference();
	//WQObject* GetReturnObject();
	bool BreakOccurred;

	void EnterNewEnvironment();
	void BackToParentEnvironment();

	int ParamSize;
	Environment* CurrentEnvironment;
	//void SetReturnObject(WQObject& obj);
	//WQObject* GetReturnObject();
private:
	
	WQObject ReturnObject;
	//void CleanReturnObject();
	WQObject* ReferencedObject;
	list<WQObject*> CallingParams;
};
#endif
