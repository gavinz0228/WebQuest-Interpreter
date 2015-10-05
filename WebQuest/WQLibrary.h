#include "WQState.h"
#include "WebRequest.h"
#include "WebResponse.h"
#include "JsonParser\JSON.h"
#include "JsonParser\JSONValue.h"
#include <wchar.h>
#include <ctype.h>
#include <wctype.h>
#ifndef WQLIBRARY_H
#define WQLIBRARY_H
 void WQDeepCopy(WQState* state);
 void WQDumpJson(WQState* state);
 void WQRange(WQState* state);
 void WQLen(WQState* state);
 void WQAppend(WQState* state);
 void WQPrint(WQState* state);
 void WQParseStatus(WQState* state);
 void WQParseBody(WQState* state);
 void WQParseHeader(WQState* state);
 void WQGet(WQState* state);
 void WQGetRaw(WQState* state);
 void WQMilli(WQState* state);
 void WQParseJson(WQState* state);
#endif
