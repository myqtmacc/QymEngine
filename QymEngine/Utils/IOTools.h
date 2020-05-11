#include "windows.h"
#include <fstream>

HRESULT WriteBuffer2File(LPCWSTR filename, char * buffer, int size);

HRESULT ReadBufferFromFile(LPCWSTR filename, char * buffer, int size, DWORD *readedSize);

bool ReadAllBytes(const char * filename, BYTE * buffer, int * readedSize);
bool WriteAllBytes(const char * filename, BYTE * buffer, int writeSize);

bool ReadAllBytes2(const char * filename, BYTE * buffer, int * readedSize);
bool WriteAllBytes2(const char * filename, BYTE * buffer, int writeSize);