#include "WQState.h"
#include "WQRequest.h"
#include "WQResponse.h"

#include "JsonParser\JSON.h"
#include "JsonParser\JSONValue.h"

#include "Converter.h"
#include <regex>
#include <wchar.h>
#include <ctype.h>
#include <wctype.h>
#include <chrono>
#include <thread>
#include <ctime>
#ifndef WQLIBRARY_H
#define WQLIBRARY_H
 void WQDeepCopy(WQState* state);
 void WQDumpJson(WQState* state);

 void WQShowHeaders(WQState* state);
 void WQParseJson(WQState* state);
 void WQParseStatus(WQState* state);
 void WQParseBody(WQState* state);
 void WQParseHeader(WQState* state);
 void WQGet(WQState* state);
 void WQPost(WQState* state);
 void WQPostJSON(WQState* state);  
 void WQPut(WQState* state);
 void WQGetRaw(WQState* state);

 
 //-----------------------------------
 void WQStr(WQState* state);
 void WQFloat(WQState* state);
 void WQInt(WQState* state);
 void WQPrint(WQState* state);
 void WQRange(WQState* state);
 void WQLen(WQState* state);
 void WQType(WQState* state);
 void WQAppend(WQState* state);
 void WQContains(WQState* state);
 //-----------------------------------
 void WQSaveFile(WQState* state);
 void WQLoadFile(WQState* state);
 //-----------------------------------
 void WQSplit(WQState* state);
 void WQFind(WQState* state);
 void WQSubString(WQState* state);
 void WQRegexMatch(WQState* state);
 //-----------------------------------
 void WQYear(WQState* state);
 void WQMonth(WQState* state);
 void WQDay(WQState* state);
 void WQHour(WQState* state);
 void WQMinute(WQState* state);
 void WQSecond(WQState* state);
 void WQMilli(WQState* state);
 void WQSleep(WQState* state);

#endif
