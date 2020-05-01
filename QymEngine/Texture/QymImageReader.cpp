#include "QymImageReader.h"

QymImageReader::QymImageReader(){
	this->data = NULL;
}

QymImageReader::~QymImageReader(){
	if(this->data != NULL){
		delete[] this->data;
	}
}

bool QymImageReader::LoadByRGBA(const char *Filename)
{
	//字符转换
	int dwNum ;  
	bool r = false;
	dwNum = MultiByteToWideChar (CP_ACP, 0, Filename,-1, NULL, 0) ;  
	WCHAR *filenamew = new WCHAR[dwNum];
	if(filenamew != NULL)
	{
		MultiByteToWideChar (CP_ACP, 0, Filename, -1, filenamew, dwNum) ;  
		r = LoadByRGBA(filenamew);
		delete[] filenamew;
	}
	return r;
}
bool QymImageReader::LoadByRGBA(const WCHAR *Filename)
{
	if (checkFileExisting(Filename))											// Does The File Exist?
	{

		CImage img0;  
		byte *img0Data;

		img0.Load(Filename);    
		img0Data=(byte*)img0.GetBits(); 
		/**
		这里上面的指正并不是像素数据的起始位置
		*/
		int width = img0.GetWidth();
		int height = img0.GetHeight();

		int pit=img0.GetPitch(); // 每个bitmap line的长度，由于对齐的原因与width不一定相同 
		int byteCount=img0.GetBPP()/8;

		int paddedWidth = (abs(pit) / byteCount);

		//变换一下才是
		if(pit<0)
			img0Data=(BYTE*)img0Data+(pit*(height-1));
		else
			img0Data=(BYTE*)img0Data;

		this->sizeX = width;
		this->sizeY = height;
		this->bpp = 4;
		this->data = new unsigned char[ width * height * 4];
		
		byte r, g, b, a;   //COLORREF pixel;
		if(byteCount==4){
			for (int y=0; y<height; y++) {  
				int lineS = (height - y - 1) * abs(pit);//y *width;//
				int lineD = y *width;
				for (int x=0; x<width; x++) {  
					b = img0Data[lineS + (x * 4) + 0];
					g = img0Data[lineS + (x * 4) + 1];
					r = img0Data[lineS + (x * 4) + 2];
					a = img0Data[lineS + (x * 4) + 3];
				   
					this->data[(lineD+x)*4+0] = r;
					this->data[(lineD+x)*4+1] = g;
					this->data[(lineD+x)*4+2] = b;
					this->data[(lineD+x)*4+3] = a;
				}
			}
			//memcpy(imgData, img0Data, width * height * 4);
			//memcpy(this->data , img0Data , width * height * byteCount);
			
		}else{
			for(int y = 0; y < height; y++) {  
				int lineS = (height - y - 1) * abs(pit);//y *width;//
				int lineD = y *width;
				for (int x=0; x<width; x++) {  
					   /*pixel = img0.GetPixel(x, y);
					   r = GetRValue(pixel);
					   g = GetGValue(pixel);
					   b = GetBValue(pixel);*/
					b = img0Data[lineS + (x * 3) + 0];
					g = img0Data[lineS + (x * 3) + 1];
					r = img0Data[lineS + (x * 3) + 2];

					this->data[(lineD + x) * 4 + 0] = r;
					this->data[(lineD + x) * 4 + 1] = g;
					this->data[(lineD + x) * 4 + 2] = b;
					this->data[(lineD + x) * 4 + 3] = 255;
				}
			}
		}
		hasImage = true;
		return true;
	}
	return false;
}