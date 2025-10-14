#pragma once

class GImage
{

public:

    enum IMAGE_LOAD_KIND
    {
        LOAD_RESOURCE = 0, LOAD_FILE,      // 리소스, 파일
        LOAD_EMPTY, LOAD_END            // 빈 비트맵, 로드 done

    };

    typedef struct tagImage
    {
        DWORD   resID;
        HDC     hMemDC;
        HBITMAP hBit;
        HBITMAP hOBit;
        float elpasedSec;
        float   x;                      // 이미지 x좌표
        float   y;                      // 이미지 y좌표
        int     width;                  // 이미지 가로 크기
        int     height;                 // 이미지 세로크기
        int     currentFrameX;          // 현재 프레임 x
        int     currentFrameY;          // 현재 프레임 y
        int     maxFrameX;              // 최대 프레임 x 갯수
        int     maxFrameY;              // 최대 프레임 y 갯수
        int     frameWidth;             // 1 프레임 가로 크기
        int     frameHeight;            // 1 프레임 가로 크기
        BYTE    loadType;               // 


        tagImage()
        {
            resID = 0;                          //long
            hMemDC = nullptr;                   // 메모리에 들어갈 메모리DC
            hBit = nullptr;                     // 메모리에 그릴 것과 화면에 그릴 것을 분리 화면이 지나가면 메모리에 있던것을 복사하겠다!
            hOBit = nullptr;
            elpasedSec = 0.0;
            x = y = 0;
            width = 0;
            height = 0;
            currentFrameX = 0;
            currentFrameY = 0;
            maxFrameX = 0;
            maxFrameY = 0;
            frameWidth = 0;
            frameHeight = 0;
            width = 0;
            height = 0;
            loadType = LOAD_RESOURCE;          // unsigned char
        }
    }IMAGE_INFO, * LPIMAGE_INFO;
    // 별도로 받아와서 찍어주는것

private:
    LPIMAGE_INFO   _imageInfo;
    char* _fileName;
    bool            _isTrans;
    COLORREF      _transColor;

    BLENDFUNCTION   _blendFunc;
    LPIMAGE_INFO    _blendImage;

    bool aniPlaying;

    COLORREF* mPixels;



public:

    HRESULT init(int width, int height);
    HRESULT init(const DWORD resId, int width, int height, bool isTrans = false, COLORREF transColor = RGB(0, 0, 0));
    HRESULT init(const char* fileName, int width, int height, bool isTrans = false, COLORREF transColor = RGB(0, 0, 0));
    HRESULT init(const char* fileName, float x, float y, int width, int height, bool isTrans = false, COLORREF transColor = RGB(0, 0, 0));
    HRESULT init(const char* fileName, int width, int height, int maxFrameX, int maxFrameY, bool isTrans = false, COLORREF transColor = RGB(0, 0, 0));
    HRESULT init(const char* fileName, float x, float y, int width, int height, int maxFrameX, int maxFrameY, bool isTrans = false, COLORREF transColor = RGB(0, 0, 0));
    HRESULT initForAlphaBlend(void);

    void setTransColor(bool isTrans, COLORREF transColor);

    void release(void);


    void play(float frameUpdateSec);
    inline void resume(void) { aniPlaying = true; }
    inline void pause(void) { aniPlaying = false; }

    // 랜더
    void render(HDC hdc);
    void render(HDC hdc, int destX, int destY);

    void render(HDC hdc, int destX, int destY, int sourX, int sourY, int sourwidth, int sourheight);
    void render(HDC hdc, int destX, int destY, int sourX, int sourY, int sizeX, int sizeY, int sourwidth, int sourheight);

    // 알바 랜더
    void alphaRender(HDC hdc, BYTE alpha);      // BYTE 자료형은 0~255 크기를 가진다 -> 알파값에 딱 맞음
    void alphaRender(HDC hdc, int destX, int destY, BYTE alpha);
    void alphaRender(HDC hdc, int destX, int destY, int sourX, int sourY, int sourwidth, int sourheight, BYTE alpha);

    //프레임 랜더
    void frameRender(HDC hdc, int destX, int destY);
    void frameRender(HDC hdc, int destX, int destY, int currentFrameX, int currentFrameY);
    void frameRender(HDC hdc, int destX, int destY, int currentFrameX, int currentFrameY, BYTE alpha);

    void frameAlphaRender(HDC hdc, int destX, int destY, int currentFrameX, int currentFrameY, BYTE alpha);

    // 루프 랜더
    void loopRender(HDC hdc, const LPRECT drawArea, int offsetX, int offsetY);
    void loopAlphaRender(HDC hdc, const LPRECT drawArea, int offsetX, int offsetY, BYTE alpha);

    void setTransImage(const char* fileName);

    // 인라인 함수
    inline HDC getMemDC(void) { return _imageInfo->hMemDC; }

    // 이미지 x좌표
    inline float getX(void) { return _imageInfo->x; }
    inline void setX(float x) { _imageInfo->x = x; }

    // 이미지 y좌표
    inline float getY(void) { return _imageInfo->y; }
    inline void setY(float y) { _imageInfo->y = y; }

    // 이미지 센터 좌표
    inline void setCenter(float x, float y)
    {
        _imageInfo->x = x - (_imageInfo->width / 2);
        _imageInfo->y = y - (_imageInfo->height / 2);
    }

    // 이미지 가로 / 세로 크기
    inline int getWidth(void) { return _imageInfo->width; }
    inline int getHeight(void) { return _imageInfo->height; }

    // 바운딩 박스(충돌용 렉트)
    inline RECT boundingBox(void)
    {
        RECT rc =
        {
            _imageInfo->x,
            _imageInfo->y,
            _imageInfo->x + _imageInfo->width,
            _imageInfo->y + _imageInfo->height
        };
        return rc;
    }

    // 프레임을 포함한 바운딩 박스(충돌용 렉트)
    inline RECT boundingBoxWithFrame(void)
    {
        RECT rc =
        {
            _imageInfo->x,
            _imageInfo->y,
            _imageInfo->x + _imageInfo->frameWidth,
            _imageInfo->y + _imageInfo->frameHeight
        };
        return rc;
    }

    // 프레임 X
    inline int getFrameX(void) { return _imageInfo->currentFrameX; }
    inline void setFrameX(int frameX)
    {
        _imageInfo->currentFrameX = frameX;

        if (frameX > _imageInfo->maxFrameX)
        {
            _imageInfo->currentFrameX = _imageInfo->maxFrameX;
        }

        _imageInfo->elpasedSec = 0.0f;  // <- 추가
    }

    // 프레임 Y
    inline int getFrameY(void) { return _imageInfo->currentFrameY; }
    inline void setFrameY(int frameY)
    {
        _imageInfo->currentFrameY = frameY;

        if (frameY > _imageInfo->maxFrameX)
        {
            _imageInfo->currentFrameY = _imageInfo->maxFrameY;
        }

        _imageInfo->elpasedSec = 0.0f;  // <- 추가
    }

    // 이미지 1프레임 가로 / 세로 크기
    inline int getFrameWidth(void) { return _imageInfo->frameWidth; }
    inline int getFrameHeight(void) { return _imageInfo->frameHeight; }

    // 최대 프레임 X / Y 갯수
    inline int getMaxFrameX(void) { return _imageInfo->maxFrameX; }
    inline int getMaxFrameY(void) { return _imageInfo->maxFrameY; }

    inline char* getFileName() { return _fileName; }
    inline bool getIsTrans() { return _isTrans; }
    inline COLORREF getTransColor() { return _transColor; }

    inline GImage* cloneImage()
    {
        if (!_imageInfo) return nullptr;

        GImage* clone = new GImage;

        clone->_imageInfo = new IMAGE_INFO(*_imageInfo);

        clone->_fileName = _fileName;
        clone->_isTrans = _isTrans;
        clone->_transColor = _transColor;

        clone->_blendFunc = _blendFunc;
        clone->_blendFunc = _blendFunc;
        clone->_blendImage = _blendImage;        
        clone->aniPlaying = aniPlaying;

        return clone;
    }

    GImage();
    ~GImage() {}
};