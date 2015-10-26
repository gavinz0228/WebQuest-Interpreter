
using namespace std;
#ifndef WQSTATE_H
#define WQSTATE_H
#include <list>
#include<stack>
#include "WQObject.h"
#include "WQEnvironment.h"
enum EnvironmentType{ET_ROOT,ET_FUNCTION,ET_IF,ET_LOOP};
class WQEnvironment;
class WQState
{
public:
	WQState(){
		ReferencedObject = NULL; 
		BreakOccurred = false;
		CurrentEnvironment = new WQEnvironment;
	}
	~WQState(){
		WQEnvironment* current = CurrentEnvironment;
		while (current != NULL)
		{
			//delete its parent environment until the parent is null
			WQEnvironment* temp = current->Parent;
			delete current;
			current = temp;
		}
	}

	WQObject* CreateReferenceObject(WQObject* targetobj);
	WQObject* GetParam();
	string GetStringParam();
	bool GetBooleanParam();
	long long GetIntegerParam();
	long double GetFloatParam();
	map<string, WQObject*>* GetDictionaryParam();
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

	void EnterNewEnvironment(EnvironmentType nodeType);
	void BackToParentEnvironment();
	EnvironmentType GetCurrentEnvironmentType();
	void MoveVariableToParentEnvironment(WQObject* obj);
	int ParamSize;
	WQEnvironment* CurrentEnvironment;
	WQObject* CreateObject();
	volatile long CurrentLineNumber;
	//void SetReturnObject(WQObject& obj);
	//WQObject* GetReturnObject();
private:
	
	//void CleanReturnObject();
	//it's to store the node chain;
	stack<EnvironmentType> EnvironmentTypeStack;
	WQObject* ReferencedObject;
	list<WQObject*> CallingParams;
};
#endif
