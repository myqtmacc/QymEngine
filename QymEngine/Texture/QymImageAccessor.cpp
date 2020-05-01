#include "Texture/QymImageAccessor.h"
#include <atlimage.h>
#include "stdio.h"
#include "Utils/CharacterTools.h"

QymImageAccessor::QymImageAccessor(void)
{
	hasImage = false;
	sizeX = 0;
	sizeY = 0;
	bpp = 0;
}


QymImageAccessor::~QymImageAccessor(void)
{
	//if(data!=NULL){
	//	delete [] data;
	//}
	//data = NULL;
}

bool QymImageAccessor::checkFileExisting(const char *name)
{
	FILE *File=NULL;									// File Handle
	if (!name)										// Make Sure A Filename Was Given
	{
		return false;									// If Not Return NULL
	}

	errno_t err;									// Check To See If The File Exists
	if ((err = fopen_s(&File, name, "r")) == 0)			// Does The File Exist?
	{
		fclose(File);
		return true;
	}
	return false;
}

bool QymImageAccessor::checkFileExisting(const WCHAR *name)
{
	FILE *File=NULL;									// File Handle
	if (!name)										// Make Sure A Filename Was Given
	{
		return false;									// If Not Return NULL
	}

	errno_t err;									// Check To See If The File Exists
	if ((err = _wfopen_s(&File, name, L"r")) == 0)		// Does The File Exist?
	{
		fclose(File);
		return true;
	}
	return false;
}

bool QymImageAccessor::getImageData(const unsigned char *p) const
{
	int size =-1;
	if(getSizeInByte(size)){
		memcpy((void*)p , data , size);
		return true;
	}
	return false;
}

bool QymImageAccessor::getImageDataRevY(const unsigned char *p) const
{
	//int size =-1;
	//if(getSizeInByte(size)){
	//	memcpy((void*)p , data , size);
	//	return true;
	//}
	for(int y=0;y<sizeY;y++)
	{
		int step = sizeX *4;
		memcpy((void*)(p+step*y),data+step*(sizeY-y-1),step);
	}
	return false;
}
bool QymImageAccessor::getSizeInByte(int &b) const
{
	if(hasImage){
		b = sizeX*sizeY*bpp;
		return true;
	}
	return false;
}

int QymImageAccessor::getWidth() const
{
	return sizeX;
}

int QymImageAccessor::getHeight() const
{
	return sizeY;
}

int QymImageAccessor::getBPP() const
{
	return bpp;
}

bool QymImageAccessor::getPixelColor(int x, int y, unsigned char &r, unsigned char &g, unsigned char &b) const
{
	if(x>=0&&x<sizeX&&y>=0&&y<sizeY&&hasImage){
		int index = ( x + sizeX * y) * bpp;
		r = data[index + 0];
		g = data[index + 1];
		b = data[index + 2];
		return true;
	}
	return false;
}

bool QymImageAccessor::getPixelColor(int x, int y, unsigned char &r, unsigned char &g, unsigned char &b, unsigned char &a) const
{
	if(x>=0&&x<sizeX&&y>=0&&y<sizeY&&hasImage){
		int index = ( x + sizeX * y) * bpp;
		r = data[index + 0];
		g = data[index + 1];
		b = data[index + 2];
		a = data[index + 3];
		return true;
	}
	return false;
}

bool QymImageAccessor::SaveImage(const std::string& path, BYTE* pBuffer, int width, int height) 
{
	CImage img;

	BOOL re = img.Create(width, height, 24);
	//int i, j;
	byte r, g, b; //c, a;

	for (int y = 0; y< height; y++)
	{
		int offset = y * width;
		for (int x = 0; x < width; ++x)
		{
			b = pBuffer[4 * (offset + x) + 0];
			g = pBuffer[4 * (offset + x) + 1];
			r = pBuffer[4 * (offset + x) + 2];
			//a=imageDataFe[4*(offset + x)+3];
			img.SetPixelRGB(x, y, r, g, b);

		}
	}

	//int widthImg = img.GetWidth();
	//int heightImg = img.GetHeight();

	std::wstring wpath = ANSIToUnicode(path);
	HRESULT hr = img.Save(wpath.c_str());

	return hr == S_OK;
}
