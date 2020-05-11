#include "windows.h"
#include <fstream>

#include "Common/QymCommonHeader.h"

DLL_EXPORT HRESULT WriteBuffer2File(LPCWSTR filename, char * buffer, int size);

DLL_EXPORT HRESULT ReadBufferFromFile(LPCWSTR filename, char * buffer, int size, DWORD *readedSize);

DLL_EXPORT bool ReadAllBytes(const char * filename, BYTE * buffer, int * readedSize);
DLL_EXPORT bool WriteAllBytes(const char * filename, BYTE * buffer, int writeSize);

DLL_EXPORT bool ReadAllBytes2(const char * filename, BYTE * buffer, int * readedSize);
DLL_EXPORT bool WriteAllBytes2(const char * filename, BYTE * buffer, int writeSize);