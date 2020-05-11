#include "IOTools.h"
#include "stdio.h"

#ifdef _WIN32

HRESULT WriteBuffer2File(LPCWSTR filename, char * buffer, int size) {
	HANDLE hFile;
	DWORD dwBytesWritten = 0;
	BOOL bErrorFlag = FALSE;
	HRESULT hr;

	hFile = CreateFile(filename,                // name of the write
		GENERIC_WRITE,          // open for writing
		0,                      // do not share
		NULL,                   // default security
		CREATE_ALWAYS,          
		FILE_ATTRIBUTE_NORMAL,  // normal file
		NULL);                  // no attr. template

	if (hFile == INVALID_HANDLE_VALUE)
	{
		return S_FALSE;
	}

	bErrorFlag = WriteFile(
		hFile,           // open file handle
		buffer,      // start of data to write
		size,  // number of bytes to write
		&dwBytesWritten, // number of bytes that were written
		NULL);            // no overlapped structure

	if (FALSE == bErrorFlag)
	{
		hr = S_FALSE;
	}
	else
	{
		hr = S_OK;
	}

	CloseHandle(hFile);
	return hr;
}

HRESULT ReadBufferFromFile(LPCWSTR filename, char * buffer, int size, DWORD *readedSize) {
	HANDLE hFile;
	DWORD  dwBytesRead = 0;
	OVERLAPPED ol = { 0 };

	hFile = CreateFile(filename,               // file to open
		GENERIC_READ,          // open for reading
		FILE_SHARE_READ,       // share for reading
		NULL,                  // default security
		OPEN_EXISTING,         // existing file only
		FILE_ATTRIBUTE_NORMAL, // normal file
		NULL);                 // no attr. template

	if (hFile == INVALID_HANDLE_VALUE)
	{
		return S_FALSE;
	}

	// Read one character less than the buffer size to save room for
	// the terminating NULL character. 


	if (FALSE == ReadFile(hFile, buffer, size - 1, &dwBytesRead, NULL))
	{
		DWORD err = GetLastError();
		CloseHandle(hFile);
		return S_FALSE;
	}
	// This is the section of code that assumes the file is ANSI text. 
	// Modify this block for other data types if needed.

	if (dwBytesRead > 0 && dwBytesRead <= static_cast<DWORD>(size - 1))
	{
		buffer[dwBytesRead] = '\0'; // NULL character
	}
	else if (dwBytesRead == 0)
	{
	}

	// It is always good practice to close the open file handles even though
	// the app will exit here and clean up open handles anyway.

	CloseHandle(hFile);

	*readedSize = dwBytesRead;

	return S_OK;
}

#endif

bool ReadAllBytes(const char * filename, BYTE * buffer, int * readedSize) {
	FILE * pFile;
	errno_t err;
	long lSize;
	bool res = false;

	/* 若要一个byte不漏地读入整个文件，只能采用二进制方式打开 */
	if ((err = fopen_s(&pFile, filename, "rb")) != 0)
	{
		return false;
	}

	/* 获取文件大小 */
	fseek(pFile, 0, SEEK_END);
	lSize = ftell(pFile);
	rewind(pFile);

	/* 将文件拷贝到buffer中 */
	if (buffer != nullptr) {
		size_t result = fread(buffer, 1, lSize, pFile);
		if (result == lSize)
		{
			res = true;
		}
	}

	fclose(pFile);
	*readedSize = (int)lSize;

	return res;

}

bool WriteAllBytes(const char * filename, BYTE * buffer, int writeSize) {
	FILE * pFile;
	errno_t err;
	size_t result;
	bool res = false;

	if ((err = fopen_s(&pFile, filename, "wb")) != 0)
	{
		return false;
	}

	result = fwrite(buffer, 1, writeSize, pFile);
	if (result == writeSize)
	{
		res = true;
	}

	fclose(pFile);
	return res;
}

bool ReadAllBytes2(const char * filename, BYTE * buffer, int * readedSize)
{
	HANDLE hFile;
	DWORD  dwBytesRead = 0;
	OVERLAPPED ol = { 0 };
	bool res = false;

	hFile = CreateFileA(filename,               // file to open
		GENERIC_READ,          // open for reading
		FILE_SHARE_READ,       // share for reading
		NULL,                  // default security
		OPEN_EXISTING,         // existing file only
		FILE_ATTRIBUTE_NORMAL, // normal file
		NULL);                 // no attr. template

	if (hFile == INVALID_HANDLE_VALUE)
	{
		return false;
	}

	long lSize = GetFileSize(hFile, NULL);

	if (buffer != NULL) {
		if (FALSE != ReadFile(hFile, buffer, lSize, &dwBytesRead, NULL) && dwBytesRead == lSize)
		{
			res = true;
		}
	}
	else
	{
		dwBytesRead = lSize;
	}

	// It is always good practice to close the open file handles even though
	// the app will exit here and clean up open handles anyway.

	CloseHandle(hFile);

	*readedSize = dwBytesRead;

	return res;
}

bool WriteAllBytes2(const char * filename, BYTE * buffer, int writeSize) {
	HANDLE hFile;
	DWORD dwBytesWritten = 0;
	BOOL bErrorFlag = FALSE;
	bool res = false;

	hFile = CreateFileA(filename,                // name of the write
		GENERIC_WRITE,          // open for writing
		0,                      // do not share
		NULL,                   // default security
		CREATE_ALWAYS,
		FILE_ATTRIBUTE_NORMAL,  // normal file
		NULL);                  // no attr. template

	if (hFile == INVALID_HANDLE_VALUE)
	{
		return false;
	}

	bErrorFlag = WriteFile(
		hFile,           // open file handle
		buffer,      // start of data to write
		writeSize,  // number of bytes to write
		&dwBytesWritten, // number of bytes that were written
		NULL);            // no overlapped structure

	if (FALSE != bErrorFlag && writeSize == dwBytesWritten)
	{
		res = true;
	}
	
	// It is always good practice to close the open file handles even though
	// the app will exit here and clean up open handles anyway.

	CloseHandle(hFile);

	return res;
}