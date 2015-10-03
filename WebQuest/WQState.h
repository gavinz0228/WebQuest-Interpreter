
using namespace std;
#ifndef WQSTATE_H
#define WQSTATE_H
#include <list>
#include<stack>
#include "WQObject.h"
#include "Environment.h"
enum EnvironmentType{ET_ROOT,ET_FUNCTION,ET_IF,ET_LOOP};
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

	void EnterNewEnvironment(EnvironmentType nodeType);
	void BackToParentEnvironment();
	EnvironmentType GetCurrentEnvironmentType();
	void MoveVariableToParentEnvironment(WQObject* obj);
	int ParamSize;
	Environment* CurrentEnvironment;
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
