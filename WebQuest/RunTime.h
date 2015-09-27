#include "Environment.h"
#include "Parser.h"
#include <stack>
using namespace std;
#ifndef RUNTIME_H
#define RUNTIME_H
class Runtime{
public:
	Runtime(){
		environment = new Environment;
	}
	~Runtime(){
		delete environment;
	}
	void Evaluate(NodeBase* program,WQState* state);
	void Run(char* script);
	Environment* environment;
	long GetCurrentLineNumber(){ return CurrentLineNumber; }
private:
	void EnterNewEnvironment();
	void BackToParentEnvironment();
	long CurrentLineNumber;

};
#endif
/*
python version for evulating expressions

class Tokenizer:
tokens=None
index=0
length=0
def __init__(self,tks):
self.tokens=tks
self.length=len(tks)
def next():
if self.index<self.length:
return self.tokens[self.index]
else:
return None

def plus(left,right):
return int(left) + int(right)
def mul(left,right):
return int(left) * int(right)
def minus(left,right):
return int(left) - int(right)

def execute(left,op,right):
print(left,op,right)
if len(right)==1:
return calculate[op](left,right[0])
else:
if not op in ['*','/']:
nextleft=right[0]
nextop=right[1]
rest=right[2:]
return execute(left,op,[execute(nextleft,nextop,rest)])
else:
firstright=right[0]
left=calculate[op](left,firstright)
currentop=right[1]
return execute(left,currentop,right[2:])
exp="2 + 3 * 2 + 4 * 5"
tokens=exp.split(" ")
operators=['+','-','*','/']
calculate={"+":plus,"-":minus,"*":mul}
def main():
left=tokens[0]
op=tokens[1]
rest=tokens[2:]
print execute(left,op,rest)
main()

*/