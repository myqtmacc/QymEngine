#pragma once
#include "Windows.h"
#include <string>

class QymImageAccessor
{
public:
	QymImageAccessor(void);
	virtual ~QymImageAccessor(void);
	
	//一个检测文件存在与否的方法,因为与类无关，所以静态
	static bool checkFileExisting(const char *name);
	//一个检测文件存在与否的方法,因为与类无关，所以静态
	static bool checkFileExisting(const WCHAR *name);;

	//从文件中读取图片像素数据，按照RGBA的形式，使用不同的工具方法不同，所以是纯虚函数，交由子类实现
	virtual bool	LoadByRGBA(const char *Filename) = 0;
	virtual bool	LoadByRGBA(const WCHAR *Filename) = 0;
	
	//复制所有图像像素数据，在父类中实现即可
	bool getImageData(const unsigned char *p) const;
	bool getImageDataRevY(const unsigned char *p) const;

	//获取存储所有像素数据所需数组的大小，父类中实现即可
	bool getSizeInByte(int &b) const;;

	int getWidth() const;

	int getHeight() const;

	int getBPP() const;
	
	bool getPixelColor(int x, int y, unsigned char &r, unsigned char &g, unsigned char &b) const;
	bool getPixelColor(int x, int y, unsigned char &r, unsigned char &g, unsigned char &b, unsigned char &a) const;

	static bool SaveImage(const std::string & path, BYTE * pBuffer, int width, int height);

//使用protected，这样对外还是private，但是对与子类就变成了可访问的
protected:
    int sizeX, sizeY;
	int bpp;
    unsigned char *data;
	bool hasImage;
};
