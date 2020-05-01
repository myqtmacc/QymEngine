#ifndef __CHARACTER_TOOLS_H__
#define __CHARACTER_TOOLS_H__

#include <windows.h>
#include <string>
#include <xstring>
using namespace std;


wstring UTF8ToUnicode(const string& str);
string UnicodeToUTF8(const wstring& str);
wstring ANSIToUnicode(const string& str);
string UnicodeToANSI(const wstring& str);

#endif