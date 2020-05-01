#pragma once

#include <windows.h>
#include <stdlib.h>
#include <stdio.h>
#include <tchar.h>
#include <atlimage.h>
#include "QymImageAccessor.h"

class QymImageReader : public QymImageAccessor
{
public:
	QymImageReader();
	~QymImageReader();
	bool LoadByRGBA(const char *Filename);
	bool LoadByRGBA(const WCHAR *Filename);
};