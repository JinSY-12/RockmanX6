#include "Stdafx.h"
#include "ImageManager.h"

HRESULT ImageManager::init(void)
{
	// ! Do Nothing

	return S_OK;
}

void ImageManager::release(void)
{
	this->deleteAll();
}

GImage* ImageManager::addImage(string strKey, int width, int height)
{
	GImage* img = findImage(strKey);

	if (img) return img;

	img = new GImage;

	if (FAILED(img->init(width, height)))
	{
		SAFE_DELETE(img);
		return NULL;
	}

	_mImageList.insert(make_pair(strKey, img));


	return img;
}

GImage* ImageManager::addImage(string strKey, const char* fileName, int width, int height, bool isTrans, COLORREF transColor)
{
	GImage* img = findImage(strKey);

	if (img) return img;

	img = new GImage;

	if (FAILED(img->init(fileName, width, height, isTrans, transColor)))
	{
		SAFE_DELETE(img);
		return NULL;
	}

	_mImageList.insert(make_pair(strKey, img));

	return img;
}

GImage* ImageManager::addImage(string strKey, const char* fileName, float x, float y, int width, int height, bool isTrans, COLORREF transColor)
{
	GImage* img = findImage(strKey);

	if (img) return img;

	img = new GImage;


	if (FAILED(img->init(fileName, x, y, width, height, isTrans, transColor)))
	{
		SAFE_DELETE(img);
		return NULL;
	}

	_mImageList.insert(make_pair(strKey, img));

	return img;
}

GImage* ImageManager::addFrameImage(string strKey, const char* fileName, int width, int height, int maxFrameX, int maxFrameY, bool isTrans, COLORREF transColor)
{
	GImage* img = findImage(strKey);

	if (img) return img;

	img = new GImage;

	if (FAILED(img->init(fileName, width, height, maxFrameX, maxFrameY, isTrans, transColor)))
	{
		SAFE_DELETE(img);
		return NULL;
	}

	_mImageList.insert(make_pair(strKey, img));

	
	if(strKey.find("X_") != string::npos) 
	{
		this->addAfterImage(strKey, BLUE);
		this->addAfterImage(strKey, SKYBLUE);
		this->addAfterImage(strKey, LIGHTBLUE);
	}

	else if (strKey.find("Zero_") != string::npos)
	{
		
	}
	

	return img;

}

GImage* ImageManager::newAddFrameImage(string strKey, const char* fileName, int width, int height, int maxFrameX, int maxFrameY, bool isTrans, COLORREF transColor)
{
	GImage* img = findImage(strKey);

	if (img) return img;

	img = new GImage;

	if (FAILED(img->init(fileName, width, height, maxFrameX, maxFrameY, isTrans, transColor)))
	{
		SAFE_DELETE(img);
		return NULL;
	}

	_mImageList.insert(make_pair(strKey, img));


	return img;
}

GImage* ImageManager::addAfterImage(const string& srcKey, COLORREF targetColor)
{
	GImage* srcImg = findImage(srcKey);
	if (!srcImg) return nullptr;

	HBITMAP hBmp = srcImg->getHBit(); // GImage에서 HBITMAP 얻는다고 가정
	if (!hBmp) return nullptr;

	BITMAP bmp;
	GetObject(hBmp, sizeof(BITMAP), &bmp);

	// 비트맵 정보 준비
	BITMAPINFO bmi = {};
	bmi.bmiHeader.biSize = sizeof(BITMAPINFOHEADER);
	bmi.bmiHeader.biWidth = bmp.bmWidth;
	bmi.bmiHeader.biHeight = -bmp.bmHeight; // top-down DIB
	bmi.bmiHeader.biPlanes = 1;
	bmi.bmiHeader.biBitCount = 32; // 32비트 ARGB
	bmi.bmiHeader.biCompression = BI_RGB;

	// 픽셀 버퍼 가져오기
	std::vector<DWORD> pixels(bmp.bmWidth * bmp.bmHeight);
	HDC hdc = GetDC(NULL);
	GetDIBits(hdc, hBmp, 0, bmp.bmHeight, pixels.data(), &bmi, DIB_RGB_COLORS);

	auto toARGB = [](COLORREF c) {
		BYTE r = GetRValue(c);
		BYTE g = GetGValue(c);
		BYTE b = GetBValue(c);
		return (0xFF << 24) | (r << 16) | (g << 8) | b;
		};

	DWORD pink = 0xFFFF00FF; // ARGB: A=255, R=255, G=0, B=255
	DWORD changeColor = toARGB(targetColor); // ARGB: A=255, R=0, G=0, B=255
	
	string newKey;

	switch (targetColor)
	{
	case BLUE:
		newKey = srcKey + "_Blue";
		break;
	case SKYBLUE:
		newKey = srcKey + "_SkyBlue";
		break;
	case LIGHTBLUE:
		newKey = srcKey + "_LightBlue";
		break;
	default:
		newKey = srcKey;
		break;
	}

	// 색상 변환
	for (auto& px : pixels)
	{
		BYTE r = GetRValue(px);
		BYTE g = GetGValue(px);
		BYTE b = GetBValue(px);

		// RGB(255,0,255) → 그대로 두고, 나머지는 RGB(0,0,255)로 변경
		if (!(r == 255 && g == 0 && b == 255))
		{
			px = changeColor; // RGB(0, 0, 255);
		}
		else px = pink;
	}

	// 새로운 HBITMAP 생성
	HBITMAP newBmp = CreateDIBitmap(hdc, &bmi.bmiHeader, CBM_INIT,
		pixels.data(), &bmi, DIB_RGB_COLORS);
	ReleaseDC(NULL, hdc);

	// 새로운 GImage 객체 생성 (GImage가 HBITMAP을 받아서 생성 가능하다고 가정)
	GImage* newImg = new GImage;
	newImg->initFromHBITMAP(newBmp, srcImg->getWidth(), srcImg->getHeight(), srcImg->getMaxFrameX()+1, srcImg->getMaxFrameY()+1, true, MAGENTA);

	// map에 저장
	_mImageList.insert(make_pair(newKey, newImg));
	
	return newImg;
}

GImage* ImageManager::addFrameImage(string strKey, const char* fileName, float x, float y, int width, int height, int maxFrameX, int maxFrameY, bool isTrans, COLORREF transColor)
{
	GImage* img = findImage(strKey);

	if (img) return img;

	img = new GImage;

	if (FAILED(img->init(fileName, x, y, width, height, maxFrameX, maxFrameY, isTrans, transColor)))
	{
		SAFE_DELETE(img);
		return NULL;
	}

	_mImageList.insert(make_pair(strKey, img));

	return img;
}




GImage* ImageManager::findImage(string strKey)
{
	auto key = _mImageList.find(strKey);

	if (key != _mImageList.end())
	{
		return key->second;
	}

	return nullptr;
}

/*
void ImageManager::addAfterImage(string strKey, COLORREF rgb)
{
	// 이미지 클론 생성
	GImage* newimg;
	newimg = new GImage;

	if (findImage(strKey) == nullptr) return;
	else newimg = findImage(strKey)->cloneImage();

	// 이미지 키 값 변경
	string newKey;
	if(DARKBLUE == rgb) newKey = strKey + "_DarkBlue";
	else if (MIDBLUE == rgb) newKey = strKey + "_MidBlue";
	else if (LIGHTBLUE == rgb) newKey = strKey + "_LightBlue";
	else return;
	
	// 이미지 색 변경
	const int w = newimg->getWidth();
	const int h = newimg->getHeight();
	COLORREF trans = newimg->getTransColor();
	
	for (int y = 0; y < h; ++y)
	{
		for (int x = 0; x < w; ++x)
		{
			COLORREF c = GetPixel(newimg->getMemDC(), x, y);
			if (c == trans) continue;
			SetPixel(newimg->getMemDC(), x, y, rgb);
		}
	}

	_mImageList.insert(make_pair(newKey, newimg));
}
*/

bool ImageManager::deleteImage(string strKey)
{
	auto key = _mImageList.find(strKey);

	if (key != _mImageList.end())
	{
		key->second->release();
		SAFE_DELETE(key->second);
		_mImageList.erase(key);

		return true;
	}

	return false;
}

bool ImageManager::deleteAll()
{
	auto iter = _mImageList.begin();

	for (; iter != _mImageList.end();)
	{
		if (iter->second != NULL)
		{
			iter->second->release();
			SAFE_DELETE(iter->second);
			iter = _mImageList.erase(iter);
		}

		else
		{
			++iter;
		}
	}
	_mImageList.clear();

	return true;

}

void ImageManager::render(string strKey, HDC hdc)
{
	GImage* img = findImage(strKey);

	if (img) img->render(hdc);
}

void ImageManager::render(string strKey, HDC hdc, int destX, int destY)
{
	GImage* img = findImage(strKey);

	if (img) img->render(hdc, destX, destY);
}

void ImageManager::render(string strKey, HDC hdc, int destX, int destY, int sourX, int sourY, int sourwidth, int sourheight)
{
	GImage* img = findImage(strKey);

	if (img) img->render(hdc, destX, destY, sourX, sourY, sourwidth, sourheight);
}

void ImageManager::render(string strKey, HDC hdc, int destX, int destY, int sourX, int sourY, int sizeX, int sizeY, int sourwidth, int sourheight)
{
	GImage* img = findImage(strKey);

	if (img) img->render(hdc, destX, destY, sourX, sourY, sizeX, sizeY, sourwidth, sourheight);

}


void ImageManager::alphaRender(string strKey, HDC hdc, BYTE alpha)
{
	GImage* img = findImage(strKey);

	if (img) img->alphaRender(hdc, alpha);
}

void ImageManager::alphaRender(string strKey, HDC hdc, int destX, int destY, BYTE alpha)
{
	GImage* img = findImage(strKey);

	if (img) img->alphaRender(hdc, destX, destY, alpha);
}

void ImageManager::alphaRender(string strKey, HDC hdc, int destX, int destY, int sourX, int sourY, int sourwidth, int sourheight, BYTE alpha)
{
	GImage* img = findImage(strKey);

	if (img) img->alphaRender(hdc, destX, destY, sourX, sourY, sourwidth, sourheight, alpha);
}

void ImageManager::frameRender(string strKey, HDC hdc, int destX, int destY)
{
	GImage* img = findImage(strKey);

	if (img) img->frameRender(hdc, destX, destY);
}

void ImageManager::frameRender(string strKey, HDC hdc, int destX, int destY, int currentFrameX, int currentFrameY)
{
	GImage* img = findImage(strKey);

	if (img) img->frameRender(hdc, destX, destY, currentFrameX, currentFrameY);
}

void ImageManager::frameAlphaRender(string strKey, HDC hdc, int destX, int destY, int currentFrameX, int currentFrameY, BYTE alpha)
{
	GImage* img = findImage(strKey);

	if (img) img->frameAlphaRender(hdc, destX, destY, currentFrameX, currentFrameY, alpha);
}


void ImageManager::loopRender(string strKey, HDC hdc, const LPRECT drawArea, int offsetX, int offsetY)
{
	GImage* img = findImage(strKey);
	if (img) img->loopRender(hdc, drawArea, offsetX, offsetY);
}

void ImageManager::loopAlphaRender(string strKey, HDC hdc, const LPRECT drawArea, int offsetX, int offsetY, BYTE alpha)
{
	GImage* img = findImage(strKey);
	if (img) img->loopAlphaRender(hdc, drawArea, offsetX, offsetY, alpha);
}
