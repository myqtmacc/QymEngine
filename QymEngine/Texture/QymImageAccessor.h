#pragma once
#include "Windows.h"
#include <string>

class QymImageAccessor
{
public:
	QymImageAccessor(void);
	virtual ~QymImageAccessor(void);
	
	//һ������ļ��������ķ���,��Ϊ�����޹أ����Ծ�̬
	static bool checkFileExisting(const char *name);
	//һ������ļ��������ķ���,��Ϊ�����޹أ����Ծ�̬
	static bool checkFileExisting(const WCHAR *name);;

	//���ļ��ж�ȡͼƬ�������ݣ�����RGBA����ʽ��ʹ�ò�ͬ�Ĺ��߷�����ͬ�������Ǵ��麯������������ʵ��
	virtual bool	LoadByRGBA(const char *Filename) = 0;
	virtual bool	LoadByRGBA(const WCHAR *Filename) = 0;
	
	//��������ͼ���������ݣ��ڸ�����ʵ�ּ���
	bool getImageData(const unsigned char *p) const;
	bool getImageDataRevY(const unsigned char *p) const;

	//��ȡ�洢��������������������Ĵ�С��������ʵ�ּ���
	bool getSizeInByte(int &b) const;;

	int getWidth() const;

	int getHeight() const;

	int getBPP() const;
	
	bool getPixelColor(int x, int y, unsigned char &r, unsigned char &g, unsigned char &b) const;
	bool getPixelColor(int x, int y, unsigned char &r, unsigned char &g, unsigned char &b, unsigned char &a) const;

	static bool SaveImage(const std::string & path, BYTE * pBuffer, int width, int height);

//ʹ��protected���������⻹��private�����Ƕ�������ͱ���˿ɷ��ʵ�
protected:
    int sizeX, sizeY;
	int bpp;
    unsigned char *data;
	bool hasImage;
};
