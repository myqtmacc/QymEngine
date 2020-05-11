#ifndef __CHARACTER_TOOLS_H__
#define __CHARACTER_TOOLS_H__

#include <windows.h>
#include <string>
#include <xstring>

#include "Common/QymCommonHeader.h"

DLL_EXPORT std::wstring UTF8ToUnicode(const std::string& str);
DLL_EXPORT std::string UnicodeToUTF8(const std::wstring& str);
DLL_EXPORT std::wstring ANSIToUnicode(const std::string& str);
DLL_EXPORT std::string UnicodeToANSI(const std::wstring& str);

#endif