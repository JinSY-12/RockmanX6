#include "Stdafx.h"
#include "GImage.h"


GImage::GImage() : _imageInfo(nullptr), _fileName(nullptr), _isTrans(false), _transColor(RGB(0, 0, 0)), _blendImage(NULL)
{
}

HRESULT GImage::init(int width, int height)
{
    if (_imageInfo != nullptr) this->release();

    HDC hdc = GetDC(_hWnd);

    _imageInfo = new IMAGE_INFO;

    _imageInfo->loadType = LOAD_EMPTY;
    _imageInfo->resID = 0;
    _imageInfo->hMemDC = CreateCompatibleDC(hdc);
    _imageInfo->hBit = (HBITMAP)CreateCompatibleBitmap(hdc, width, height);
    _imageInfo->hOBit = (HBITMAP)SelectObject(_imageInfo->hMemDC, _imageInfo->hBit);
    _imageInfo->width = width;
    _imageInfo->height = height;


    _fileName = nullptr;

    _isTrans = false;
    _transColor = RGB(0, 0, 0);

    if (_imageInfo->hBit == 0)
    {
        release();
        return E_FAIL;
    }

    ReleaseDC(_hWnd, hdc);

    return S_OK;
}

HRESULT GImage::init(const DWORD resID, int width, int height, bool isTrans, COLORREF transColor)
{

    if (_imageInfo != nullptr) this->release();

    HDC hdc = GetDC(_hWnd);

    _imageInfo = new IMAGE_INFO;

    _imageInfo->loadType = LOAD_RESOURCE;
    _imageInfo->resID = resID;
    _imageInfo->hMemDC = CreateCompatibleDC(hdc);
    _imageInfo->hBit = (HBITMAP)LoadBitmap(_hInstance, MAKEINTRESOURCE(_imageInfo->resID)); // 한줄조사
    _imageInfo->hOBit = (HBITMAP)SelectObject(_imageInfo->hMemDC, _imageInfo->hBit);
    _imageInfo->width = width;
    _imageInfo->height = height;

    _fileName = nullptr;

    _isTrans = isTrans;
    _transColor = transColor;

    if (_imageInfo->hBit == 0)
    {
        release();
        return E_FAIL;
    }

    ReleaseDC(_hWnd, hdc);

    return S_OK;
}

HRESULT GImage::init(const char* fileName, int width, int height, bool isTrans, COLORREF transColor)
{
    if (_imageInfo != nullptr) this->release();

    HDC hdc = GetDC(_hWnd);

    _imageInfo = new IMAGE_INFO;

    _imageInfo->loadType = LOAD_RESOURCE;
    _imageInfo->resID = 0;
    _imageInfo->hMemDC = CreateCompatibleDC(hdc);
    _imageInfo->hBit = (HBITMAP)LoadImage(_hInstance, fileName, IMAGE_BITMAP, width, height, LR_LOADFROMFILE); // 한줄조사
    _imageInfo->hOBit = (HBITMAP)SelectObject(_imageInfo->hMemDC, _imageInfo->hBit);
    _imageInfo->width = width;
    _imageInfo->height = height;

    int len = strlen(fileName);

    _fileName = new char[len + 1];

    strcpy_s(_fileName, len + 1, fileName);

    _isTrans = isTrans;
    _transColor = transColor;

    if (_imageInfo->hBit == 0)
    {
        release();
        return E_FAIL;
    }

    ReleaseDC(_hWnd, hdc);

    return S_OK;
}


HRESULT GImage::init(const char* fileName, float x, float y, int width, int height, bool isTrans, COLORREF transColor)
{
    if (_imageInfo != nullptr) this->release();

    HDC hdc = GetDC(_hWnd);

    _imageInfo = new IMAGE_INFO;

    _imageInfo->loadType = LOAD_RESOURCE;
    _imageInfo->resID = 0;
    _imageInfo->hMemDC = CreateCompatibleDC(hdc);
    _imageInfo->hBit = (HBITMAP)LoadImage(_hInstance, fileName, IMAGE_BITMAP, width, height, LR_LOADFROMFILE); // 한줄조사
    _imageInfo->hOBit = (HBITMAP)SelectObject(_imageInfo->hMemDC, _imageInfo->hBit);
    _imageInfo->x = x;
    _imageInfo->y = y;
    _imageInfo->width = width;
    _imageInfo->height = height;

    int len = strlen(fileName);

    _fileName = new char[len + 1];

    strcpy_s(_fileName, len + 1, fileName);

    _isTrans = isTrans;
    _transColor = transColor;

    if (_imageInfo->hBit == 0)
    {
        release();
        return E_FAIL;
    }

    ReleaseDC(_hWnd, hdc);

    return S_OK;
}

HRESULT GImage::init(const char* fileName, int width, int height, int maxFrameX, int maxFrameY, bool isTrans, COLORREF transColor)
{
    if (_imageInfo != nullptr) this->release();

    HDC hdc = GetDC(_hWnd);

    _imageInfo = new IMAGE_INFO;

    _imageInfo->loadType = LOAD_RESOURCE;
    _imageInfo->resID = 0;
    _imageInfo->hMemDC = CreateCompatibleDC(hdc);
    _imageInfo->hBit = (HBITMAP)LoadImage(_hInstance, fileName, IMAGE_BITMAP, width, height, LR_LOADFROMFILE);
    _imageInfo->hOBit = (HBITMAP)SelectObject(_imageInfo->hMemDC, _imageInfo->hBit);
    _imageInfo->width = width;
    _imageInfo->height = height;
    _imageInfo->currentFrameX = 0;
    _imageInfo->currentFrameY = 0;
    _imageInfo->maxFrameX = maxFrameX - 1;
    _imageInfo->maxFrameY = maxFrameY - 1;
    _imageInfo->frameWidth = width / maxFrameX;          // 1프레임의 대한 넓이
    _imageInfo->frameHeight = height / maxFrameY;        // 1프레임의 대한 높이


    int len = strlen(fileName);

    _fileName = new char[len + 1];

    strcpy_s(_fileName, len + 1, fileName);

    _isTrans = isTrans;
    _transColor = transColor;

    if (_imageInfo->hBit == 0)
    {
        release();
        return E_FAIL;
    }

    ReleaseDC(_hWnd, hdc);

    return S_OK;
}

HRESULT GImage::init(const char* fileName, float x, float y, int width, int height, int maxFrameX, int maxFrameY, bool isTrans, COLORREF transColor)
{
    if (_imageInfo != nullptr) this->release();

    HDC hdc = GetDC(_hWnd);

    _imageInfo = new IMAGE_INFO;

    _imageInfo->loadType = LOAD_RESOURCE;
    _imageInfo->resID = 0;
    _imageInfo->hMemDC = CreateCompatibleDC(hdc);
    _imageInfo->hBit = (HBITMAP)LoadImage(_hInstance, fileName, IMAGE_BITMAP, width, height, LR_LOADFROMFILE);
    _imageInfo->hOBit = (HBITMAP)SelectObject(_imageInfo->hMemDC, _imageInfo->hBit);
    _imageInfo->x = x;
    _imageInfo->y = y;
    _imageInfo->width = width;
    _imageInfo->height = height;
    _imageInfo->currentFrameX = 0;
    _imageInfo->currentFrameY = 0;
    _imageInfo->maxFrameX = maxFrameX - 1;
    _imageInfo->maxFrameY = maxFrameY - 1;
    _imageInfo->frameWidth = width / maxFrameX;          // 1프레임의 대한 너비이
    _imageInfo->frameHeight = height / maxFrameY;        // 1프레임의 대한 높이

    int len = strlen(fileName);

    _fileName = new char[len + 1];

    strcpy_s(_fileName, len + 1, fileName);

    _isTrans = isTrans;
    _transColor = transColor;

    if (_imageInfo->hBit == 0)
    {
        release();
        return E_FAIL;
    }

    ReleaseDC(_hWnd, hdc);

    return S_OK;
}

HRESULT GImage::initForAlphaBlend(void)
{
    HDC hdc = GetDC(_hWnd);

    // 알파블랜드 옵션
    _blendFunc.BlendFlags = 0;              // 블랜드 속성값 -> 랜드에서 사용할거기 때문에 기본값
    _blendFunc.AlphaFormat = 0;             // 알파가 빠진뒤 상황을 설정 -> 0 = 후처리
    _blendFunc.BlendOp = AC_SRC_OVER;       // 블랜드 이미지 처리 방식을 설정 

    _blendImage = new IMAGE_INFO;
    _blendImage->loadType = LOAD_FILE;
    _blendImage->resID = 0;
    _blendImage->hMemDC = CreateCompatibleDC(hdc);
    _blendImage->hBit = (HBITMAP)CreateCompatibleBitmap(hdc, _imageInfo->width, _imageInfo->height); // 한줄조사
    _blendImage->hOBit = (HBITMAP)SelectObject(_blendImage->hMemDC, _blendImage->hBit);
    _blendImage->width = WINSIZE_X;
    _blendImage->height = WINSIZE_Y;

    ReleaseDC(_hWnd, hdc);

    return S_OK;
}

void GImage::setTransColor(bool isTrans, COLORREF transColor)
{
    _isTrans = isTrans;
    _transColor = transColor;
}

void GImage::release(void)
{
    if (_imageInfo)
    {
        SelectObject(_imageInfo->hMemDC, _imageInfo->hOBit);
        DeleteObject(_imageInfo->hBit);
        DeleteDC(_imageInfo->hMemDC);

        SAFE_DELETE(_imageInfo);
        SAFE_DELETE_ARRAY(_fileName);

        _isTrans = false;
        _transColor = RGB(0, 0, 0);

    }

    if (_blendImage)
    {
        SelectObject(_blendImage->hMemDC, _blendImage->hOBit);
        DeleteObject(_blendImage->hBit);
        DeleteDC(_blendImage->hMemDC);

        SAFE_DELETE(_blendImage);
    }

}

void GImage::play(float frameUpdateSec)
{
    _imageInfo->elpasedSec += TIMEMANAGER->getElapsedTime();

    if (_imageInfo->elpasedSec >= frameUpdateSec)
    {
        _imageInfo->currentFrameX++;

        if (_imageInfo->currentFrameX > _imageInfo->maxFrameX)
        {
            _imageInfo->currentFrameX = 0;
        }

        _imageInfo->elpasedSec -= frameUpdateSec;
    }
}

void GImage::render(HDC hdc)
{
    if (_isTrans)
    {
        GdiTransparentBlt
        (
            hdc,                                    // 복사할 장소의 DC (화면 DC)
            0,                                      // 복사 될 좌표 시작 X
            0,                                      // 복사 될 좌표 시작 Y
            _imageInfo->width,                      // 복사 될 이미지 가로 크기
            _imageInfo->height,                     // 복사 될 이미지 세로 크기
            _imageInfo->hMemDC,                     // 복사 될 대상 메모리 DC
            0, 0,                                   // 복사 시작지점 0, 0
            _imageInfo->width,                      // 복사 영역 가로 크기
            _imageInfo->height,                     // 복사 영역 세로 크기
            _transColor                             // 복사 할때 제외할 색상 (등록된 컬러 = 마젠타 색상)
        );

    }

    else
    {
        BitBlt(hdc, 0, 0, _imageInfo->width, _imageInfo->height,
            _imageInfo->hMemDC, 0, 0, SRCCOPY);
    }

}

void GImage::render(HDC hdc, int destX, int destY)
{

    if (_isTrans)
    {
        GdiTransparentBlt
        (
            hdc,                                    // 복사할 장소의 DC (화면 DC)
            destX,                                  // 복사 될 좌표 시작 X
            destY,                                  // 복사 될 좌표 시작 Y
            _imageInfo->width,                      // 복사 될 이미지 가로 크기
            _imageInfo->height,                     // 복사 될 이미지 세로 크기
            _imageInfo->hMemDC,                     // 복사 될 대상 메모리 DC
            0, 0,                                   // 복사 시작지점 0, 0
            _imageInfo->width,                      // 복사 영역 가로 크기
            _imageInfo->height,                     // 복사 영역 세로 크기
            _transColor                             // 복사 할때 제외할 색상 (등록된 컬러 = 마젠타 색상)
        );

    }

    else
    {
        BitBlt(hdc, destX, destY, _imageInfo->width, _imageInfo->height,
            _imageInfo->hMemDC, 0, 0, SRCCOPY);
    }
}

void GImage::render(HDC hdc, int destX, int destY, int sourX, int sourY, int sourwidth, int sourheight)
{

    if (_isTrans)
    {
        GdiTransparentBlt
        (
            hdc,                                    // 복사할 장소의 DC (화면 DC)
            destX,                                  // 복사 될 좌표 시작 X
            destY,                                  // 복사 될 좌표 시작 Y
            sourwidth,                              // 복사 될 이미지 가로 크기
            sourheight,                             // 복사 될 이미지 세로 크기
            _imageInfo->hMemDC,                     // 복사 될 대상 메모리 DC
            sourX, sourY,                           // 복사 시작지점 0, 0
            sourwidth,                              // 복사 영역 가로 크기
            sourheight,                             // 복사 영역 세로 크기
            _transColor                             // 복사 할때 제외할 색상 (등록된 컬러 = 마젠타 색상)
        );

    }

    else
    {
        BitBlt(hdc, destX, destY, sourwidth, sourheight,
            _imageInfo->hMemDC, sourX, sourY, SRCCOPY);
    }
}

void GImage::render(HDC hdc, int destX, int destY, int sourX, int sourY, int sizeX, int sizeY, int sourwidth, int sourheight)
{

    if (_isTrans)
    {
        GdiTransparentBlt
        (
            hdc,                                    // 복사할 장소의 DC (화면 DC)
            destX,                                  // 복사 될 좌표 시작 X
            destY,                                  // 복사 될 좌표 시작 Y
            sizeX,                                  // 복사 될 이미지 가로 크기
            sizeY,                                  // 복사 될 이미지 세로 크기
            _imageInfo->hMemDC,                     // 복사 될 대상 메모리 DC
            sourX, sourY,                           // 복사 시작지점 0, 0
            sourwidth,                              // 복사 영역 가로 크기
            sourheight,                             // 복사 영역 세로 크기
            _transColor                             // 복사 할때 제외할 색상 (등록된 컬러 = 마젠타 색상)
        );

    }

    else
    {
        BitBlt(hdc, destX, destY, sourwidth, sourheight,
            _imageInfo->hMemDC, sourX, sourY, SRCCOPY);
    }
}

void GImage::alphaRender(HDC hdc, BYTE alpha)
{
    if (!_blendImage) initForAlphaBlend();

    _blendFunc.SourceConstantAlpha = alpha;

    if (_isTrans)
    {
        BitBlt
        (
            _blendImage->hMemDC,
            0, 0,
            _imageInfo->width,
            _imageInfo->height,
            hdc,
            0, 0,
            SRCCOPY
        );

        GdiTransparentBlt
        (
            _blendImage->hMemDC,
            0, 0,
            _imageInfo->width,
            _imageInfo->height,
            _imageInfo->hMemDC,
            0, 0,
            _imageInfo->width,
            _imageInfo->height,
            _transColor
        );

        AlphaBlend
        (
            hdc,
            0, 0,
            _imageInfo->width,
            _imageInfo->height,
            _blendImage->hMemDC,
            0, 0,
            _imageInfo->width,
            _imageInfo->height,
            _blendFunc
        );


    }

    else
    {
        AlphaBlend(hdc, 0, 0, _imageInfo->width, _imageInfo->height, _imageInfo->hMemDC, 0, 0, _imageInfo->width, _imageInfo->height, _blendFunc);
    }
}


void GImage::alphaRender(HDC hdc, int destX, int destY, BYTE alpha)
{
    if (!_blendImage) initForAlphaBlend();

    _blendFunc.SourceConstantAlpha = alpha;

    if (_isTrans)
    {
        BitBlt
        (
            _blendImage->hMemDC,
            0, 0,
            _imageInfo->width,
            _imageInfo->height,
            hdc,
            destX, destY,
            SRCCOPY
        );

        GdiTransparentBlt
        (
            _blendImage->hMemDC,
            0, 0,
            _imageInfo->width,
            _imageInfo->height,
            _imageInfo->hMemDC,
            0, 0,
            _imageInfo->width,
            _imageInfo->height,
            _transColor
        );

        AlphaBlend
        (
            hdc,
            destX, destY,
            _imageInfo->width,
            _imageInfo->height,
            _blendImage->hMemDC,
            0, 0,
            _imageInfo->width,
            _imageInfo->height,
            _blendFunc
        );
    }

    else
    {
        AlphaBlend(hdc, destX, destY, _imageInfo->width, _imageInfo->height, _imageInfo->hMemDC, 0, 0, _imageInfo->width, _imageInfo->height, _blendFunc);
    }

}

void GImage::alphaRender(HDC hdc, int destX, int destY, int sourX, int sourY, int sourwidth, int sourheight, BYTE alpha)
{
    if (!_blendImage) initForAlphaBlend();

    _blendFunc.SourceConstantAlpha = alpha;

    if (_isTrans)
    {
        BitBlt
        (
            _blendImage->hMemDC,
            0, 0,
            _imageInfo->width,
            _imageInfo->height,
            hdc,
            destX, destY,
            SRCCOPY
        );

        GdiTransparentBlt
        (
            _blendImage->hMemDC,
            0, 0,
            _imageInfo->width,
            _imageInfo->height,
            _imageInfo->hMemDC,
            0, 0,
            _imageInfo->width,
            _imageInfo->height,
            _transColor
        );


        AlphaBlend
        (
            hdc,
            destX, destY,
            sourwidth,
            sourheight,
            _blendImage->hMemDC,
            sourX, sourY,
            sourwidth, sourheight,
            _blendFunc
        );
    }

    else
    {
        AlphaBlend(hdc, destX, destY, _imageInfo->width, _imageInfo->height, _imageInfo->hMemDC, 0, 0, _imageInfo->width, _imageInfo->height, _blendFunc);
    }
}

void GImage::frameRender(HDC hdc, int destX, int destY)
{
    if (_isTrans)
    {
        GdiTransparentBlt
        (
            hdc,
            destX,
            destY,
            _imageInfo->frameWidth,
            _imageInfo->frameHeight,
            _imageInfo->hMemDC,
            _imageInfo->currentFrameX * _imageInfo->frameWidth,
            _imageInfo->currentFrameY * _imageInfo->frameHeight,
            _imageInfo->frameWidth,
            _imageInfo->frameHeight,
            _transColor
        );
    }
    else
    {
        BitBlt(hdc, destX, destY, _imageInfo->frameWidth, _imageInfo->frameHeight,
            _imageInfo->hMemDC, _imageInfo->currentFrameX * _imageInfo->frameWidth, _imageInfo->currentFrameY * _imageInfo->frameHeight, SRCCOPY);
    }
}

void GImage::frameRender(HDC hdc, int destX, int destY, int currentFrameX, int currentFrameY)
{
    _imageInfo->currentFrameX = currentFrameX;
    _imageInfo->currentFrameY = currentFrameY;

    if (currentFrameX > _imageInfo->maxFrameX)
    {
        _imageInfo->currentFrameX = _imageInfo->maxFrameX;
    }

    if (currentFrameY > _imageInfo->maxFrameY)
    {
        _imageInfo->currentFrameY = _imageInfo->maxFrameY;
    }

    if (_isTrans)
    {
        GdiTransparentBlt
        (
            hdc,
            destX,
            destY,
            _imageInfo->frameWidth,
            _imageInfo->frameHeight,
            _imageInfo->hMemDC,
            _imageInfo->currentFrameX * _imageInfo->frameWidth,
            _imageInfo->currentFrameY * _imageInfo->frameHeight,
            _imageInfo->frameWidth,
            _imageInfo->frameHeight,
            _transColor
        );

    }

    else
    {
        BitBlt(hdc, destX, destY, _imageInfo->frameWidth, _imageInfo->frameHeight,
            _imageInfo->hMemDC, _imageInfo->currentFrameX * _imageInfo->frameWidth, _imageInfo->currentFrameY * _imageInfo->frameHeight, SRCCOPY);
    }
}

// 알파 프레임 랜더
void GImage::frameRender(HDC hdc, int destX, int destY, int currentFrameX, int currentFrameY, BYTE alpha)
{
    if (!_blendImage) initForAlphaBlend();

    _blendFunc.SourceConstantAlpha = alpha;

    _imageInfo->currentFrameX = currentFrameX;
    _imageInfo->currentFrameY = currentFrameY;

    if (currentFrameX > _imageInfo->maxFrameX)
    {
        _imageInfo->currentFrameX = _imageInfo->maxFrameX;
    }

    if (currentFrameY > _imageInfo->maxFrameY)
    {
        _imageInfo->currentFrameY = _imageInfo->maxFrameY;
    }

    if (_isTrans)
    {
        BitBlt
        (
            //hdc,
            _blendImage->hMemDC,
            destX, destY,
            //0,0,
            //_imageInfo->frameWidth,
            //_imageInfo->frameHeight,
            _imageInfo->width,
            _imageInfo->height,
            hdc,
            //_imageInfo->currentFrameX * _imageInfo->frameWidth,
            //_imageInfo->currentFrameY * _imageInfo->frameHeight,
            //_imageInfo->frameWidth,
            //_imageInfo->frameHeight,
            destX, destY,
            SRCCOPY
        );

        GdiTransparentBlt
        (
            // hdc,
            _blendImage->hMemDC,
            //_imageInfo->hMemDC,
            0, 0,
            //destX, destY,
            _imageInfo->frameWidth,
            _imageInfo->frameHeight,
            //_imageInfo->width,
            //_imageInfo->height,
            _imageInfo->hMemDC,
            //_imageInfo->hMemDC,
            _imageInfo->currentFrameX * _imageInfo->frameWidth,
            _imageInfo->currentFrameY * _imageInfo->frameHeight,
            //_imageInfo->width,
            //_imageInfo->height,
            //destX,destY,
            _imageInfo->frameWidth,
            _imageInfo->frameHeight,
            _transColor
        );

        AlphaBlend
        (
            hdc,
            //_blendImage->hMemDC,
            destX, destY,
            //_imageInfo->width,
            //_imageInfo->height,
            _imageInfo->frameWidth,
            _imageInfo->frameHeight,
            // hdc,
            _blendImage->hMemDC,
            //_imageInfo->hMemDC,
            //0,0,
            _imageInfo->currentFrameX * _imageInfo->frameWidth,
            _imageInfo->currentFrameY * _imageInfo->frameHeight,
            //_imageInfo->width,
            //_imageInfo->height,
            _imageInfo->frameWidth,
            _imageInfo->frameHeight,
            _blendFunc
        );
    }

    else
    {
        /*BitBlt(hdc, destX, destY, _imageInfo->frameWidth, _imageInfo->frameHeight,
            _imageInfo->hMemDC, _imageInfo->currentFrameX * _imageInfo->frameWidth, _imageInfo->currentFrameY * _imageInfo->frameHeight, SRCCOPY);*/

        AlphaBlend(hdc, destX, destY, _imageInfo->frameWidth, _imageInfo->frameHeight,
            _imageInfo->hMemDC, _imageInfo->currentFrameX * _imageInfo->frameWidth, _imageInfo->currentFrameY * _imageInfo->frameHeight, _imageInfo->frameWidth, _imageInfo->frameHeight, _blendFunc);
    }
}

void GImage::frameAlphaRender(HDC hdc, int destX, int destY, int currentFrameX, int currentFrameY, BYTE alpha)
{
    // 알파블렌드를 처음 사용하면 초기화
    if (!_blendImage) initForAlphaBlend();

    _blendFunc.SourceConstantAlpha = alpha;

    // 이미지 예외처리
    _imageInfo->currentFrameX = currentFrameX;
    _imageInfo->currentFrameY = currentFrameY;

    if (currentFrameX > _imageInfo->maxFrameX)
    {
        _imageInfo->currentFrameX = _imageInfo->maxFrameX;
    }

    if (currentFrameY > _imageInfo->maxFrameY)
    {
        _imageInfo->currentFrameY = _imageInfo->maxFrameY;
    }

    if (_isTrans)
    {
        // 1. 출력해야 할 DC에 그려져 있는 내용을 블렌드 이미지에 그린다. (복사)
        BitBlt
        (
            _blendImage->hMemDC,
            0,
            0,
            _imageInfo->frameWidth,
            _imageInfo->frameHeight,
            hdc,
            destX,
            destY,
            SRCCOPY
        );

        // 2. 원본 이미지의 배경을 없앤 후 블렌드 이미지에 그린다.
        GdiTransparentBlt
        (
            _blendImage->hMemDC,                                        // 복사할 장소의 DC (화면 DC)
            0, 0,                                                       // 복사될 좌표 시작
            _imageInfo->frameWidth,                                     // 복사될 이미지 가로 크기
            _imageInfo->frameHeight,                                    // 복사될 이미지 세로 크기
            _imageInfo->hMemDC,                                         // 복사될 대상 메모리 DC
            _imageInfo->currentFrameX * _imageInfo->frameWidth,         // 복사 시작지점 X
            _imageInfo->currentFrameY * _imageInfo->frameHeight,        // 복사 시작지점 Y
            _imageInfo->frameWidth,                                     // 복사 영역 가로 크기
            _imageInfo->frameHeight,                                    // 복사 영역 세로 크기
            _transColor                                                 // 복사할 때 제외할 색상 (마젠타)
        );


        // 3. 블렌드 이미지를 화면에 그린다.
        AlphaBlend
        (
            hdc,
            destX, destY,
            _imageInfo->frameWidth,
            _imageInfo->frameHeight,
            _blendImage->hMemDC,
            0,
            0,
            _imageInfo->frameWidth,
            _imageInfo->frameHeight,
            _blendFunc
        );
    }

    else
    {
        AlphaBlend(hdc,
            destX, destY,
            _imageInfo->frameWidth,
            _imageInfo->frameHeight,
            _imageInfo->hMemDC,
            _imageInfo->currentFrameX * _imageInfo->frameWidth,
            _imageInfo->currentFrameY * _imageInfo->frameHeight,
            _imageInfo->frameWidth,
            _imageInfo->frameHeight,
            _blendFunc);
    }

}


void GImage::loopRender(HDC hdc, const LPRECT drawArea, int offsetX, int offsetY)
{
    if (offsetX < 0)
    {
        offsetX = _imageInfo->width + (offsetX % _imageInfo->width);
    }

    if (offsetY < 0)
    {
        offsetY = _imageInfo->height + (offsetY % _imageInfo->height);
    }

    RECT rcSour;
    int sourWidth;
    int sourHeight;

    RECT rcDest;

    int drawAreaX = drawArea->left;
    int drawAreaY = drawArea->top;
    int drawAreaW = drawArea->right - drawArea->left;
    int drawAreaH = drawArea->bottom - drawArea->top;

    for (int y = 0; y < drawAreaH; y += sourHeight)
    {
        rcSour.top = (y + offsetY) % _imageInfo->height;
        rcSour.bottom = _imageInfo->height;
        sourHeight = rcSour.bottom - rcSour.top;

        if (y + sourHeight > drawAreaH)
        {
            rcSour.bottom -= (y + sourHeight) - drawAreaH;
            sourHeight = rcSour.bottom - rcSour.top;
        }

        rcDest.top = y + drawAreaY;
        rcDest.bottom = rcDest.top + sourHeight;

        for (int x = 0; x < drawAreaW; x += sourWidth)
        {

            rcSour.left = (x + offsetX) % _imageInfo->width;
            rcSour.right = _imageInfo->width;
            sourWidth = rcSour.right - rcSour.left;

            if (x + sourWidth > drawAreaW)
            {

                rcSour.right -= (x + sourWidth) - drawAreaW;
                sourWidth = rcSour.right - rcSour.left;
            }

            rcDest.left = x + drawAreaX;
            rcDest.right = rcDest.left + sourWidth;

            render(hdc, rcDest.left, rcDest.top,
                rcSour.left, rcSour.top,
                sourWidth, sourHeight);

        }

    }

}

// 알파값 예외를 추가 하고 랜더 과정에서 알파 랜더를 쓰면 된다 -> 직접 만들어
void GImage::loopAlphaRender(HDC hdc, const LPRECT drawArea, int offsetX, int offsetY, BYTE alpha)
{
    if (offsetX < 0)
    {
        offsetX = _imageInfo->width + (offsetX % _imageInfo->width);
    }

    if (offsetY < 0)
    {
        offsetY = _imageInfo->height + (offsetY % _imageInfo->height);
    }

    RECT rcSour;
    int sourWidth;
    int sourHeight;

    RECT rcDest;

    int drawAreaX = drawArea->left;
    int drawAreaY = drawArea->top;
    int drawAreaW = drawArea->right - drawArea->left;
    int drawAreaH = drawArea->bottom - drawArea->top;

    for (int y = 0; y < drawAreaH; y += sourHeight)
    {
        rcSour.top = (y + offsetY) % _imageInfo->height;
        rcSour.bottom = _imageInfo->height;
        sourHeight = rcSour.bottom - rcSour.top;

        if (y + sourHeight > drawAreaH)
        {
            rcSour.bottom -= (y + sourHeight) - drawAreaH;
            sourHeight = rcSour.bottom - rcSour.top;
        }

        rcDest.top = y + drawAreaY;
        rcDest.bottom = rcDest.top + sourHeight;

        for (int x = 0; x < drawAreaW; x += sourWidth)
        {

            rcSour.left = (x + offsetX) % _imageInfo->width;
            rcSour.right = _imageInfo->width;
            sourWidth = rcSour.right - rcSour.left;

            if (x + sourWidth > drawAreaW)
            {

                rcSour.right -= (x + sourWidth) - drawAreaW;
                sourWidth = rcSour.right - rcSour.left;
            }

            rcDest.left = x + drawAreaX;
            rcDest.right = rcDest.left + sourWidth;

            alphaRender(hdc, rcDest.left, rcDest.top,
                rcSour.left, rcSour.top,
                sourWidth, sourHeight, alpha);

        }

    }
}

void GImage::setTransImage(const char* fileName)
{
    int len = strlen(fileName);

    _fileName = new char[len + 1];

    strcpy_s(_fileName, len + 1, fileName);
}
