#include "Stdafx.h"
#include "CameraManager.h"

HRESULT CameraManager::init(void)
{

    _blackImage = IMAGEMANAGER->findImage("Black");
    _whiteImage = IMAGEMANAGER->findImage("White");
    _blackAlpha = 0;
    _whiteAlpha = 0;

    _padeTime = 0.0f;

    _isBlackPadeIn = false;
    _isBlackPadeOut = false;
    _isWhitePadeIn = false;
    _isWhitePadeOut = false;
    _isPadeResult = false;

    stageType = -1;

    return S_OK;
}

void CameraManager::release(void)
{
   // SAFE_DELETE(_blackImage);
}

void CameraManager::update(void)
{
    // 페이드-인
    if (_isBlackPadeIn)
    {
        _whiteAlpha = 0;
        _blackAlpha -= 255.0f / (_padeTime * TIMEMANAGER->getFrameRate());

        if (_blackAlpha <= 0)
        {
            _isBlackPadeIn = false;
            _blackAlpha = 0;
        }
    }

    // 페이드-아웃
    if (_isBlackPadeOut)
    {
        _whiteAlpha = 0;
        _blackAlpha += 255.0f / (_padeTime * TIMEMANAGER->getFrameRate());

        if (_blackAlpha >= 255)
        {
            _isBlackPadeOut = false;
            _isPadeResult = true;
            _blackAlpha = 255;
        }
    }

    if (_isWhitePadeIn)
    {
        _blackAlpha = 0;
        _whiteAlpha -= 255.0f / (_padeTime * TIMEMANAGER->getFrameRate());

        if (_whiteAlpha <= 0)
        {
            _isWhitePadeIn = false;
            _whiteAlpha = 0;
        }
    }

    if (_isWhitePadeOut)
    {
        _blackAlpha = 0;
        _whiteAlpha += 255.0f / (_padeTime * TIMEMANAGER->getFrameRate());

        if (_whiteAlpha >= 255)
        {
            _isWhitePadeOut = false;
            _isPadeResult = true;
            _whiteAlpha = 255;
        }
    }

    cameraOffset();
    setMaxCameraRange();
    cameraTest();
}

void CameraManager::render(HDC hdc)
{
    _blackImage->alphaRender(hdc, _blackAlpha);
    _whiteImage->alphaRender(hdc, _whiteAlpha);
}

void CameraManager::padeIn(float padeTime)
{
    _blackAlpha = 255.0f;
    _padeTime = padeTime;
    _isPadeResult = false;
    _isBlackPadeIn = true;
}

void CameraManager::padeOut(float padeTime)
{
    _blackAlpha = 0.0f;
    _padeTime = padeTime;
    _isPadeResult = false;
    _isBlackPadeOut = true;
}

void CameraManager::whiteIn(float padeTime)
{
    _whiteAlpha = 255.0f;
    _padeTime = padeTime;
    _isPadeResult = false;
    _isWhitePadeIn = true;
}

void CameraManager::whiteOut(float padeTime)
{
    _whiteAlpha = 0.0f;
    _padeTime = padeTime;
    _isPadeResult = false;
    _isWhitePadeOut = true;
}

void CameraManager::cameraOffset(void)
{
    camera.x = playerPos.x - WINSIZE_X / 2;
    camera.y = playerPos.y - WINSIZE_Y / 2;
    
    /////////////////////////
    // 카메라 x좌표 고정
    /////////////////////////

    if (camera.x <= cameraRange.left)
    {
        camera.x = cameraRange.left;
        cameraLockX = true;
    }

    else if (camera.x >= cameraRange.right)
    {
        camera.x = cameraRange.right;
        cameraLockX = true;
    }

    else cameraLockX = false;
    
    /////////////////////////
    // 카메라 y좌표 고정
    /////////////////////////

    if (camera.y <= mapSize.y - cameraRange.top)
    {
        camera.y = mapSize.y - cameraRange.top;
        cameraLockY = true;
    }

    else if (camera.y >= mapSize.y - cameraRange.bottom)
    {
        camera.y = mapSize.y - cameraRange.bottom;
        cameraLockY = true;
    }

    else cameraLockY = false;
}

void CameraManager::setMaxCameraRange()
{
    switch (stageType)
    {
        case 0:            
            if (camera.x >= 0 * SCALE_FACTOR && camera.x < 3530 * SCALE_FACTOR)
            {
                cameraRange.top = 288 * SCALE_FACTOR;
                cameraRange.bottom = WINSIZE_Y;
                cameraRange.left = 0 * SCALE_FACTOR;
                cameraRange.right = mapSize.x;

                cout << "Zone1" << endl;
            }
            
            else if (camera.x >= 3530 * SCALE_FACTOR && camera.x < 4060 * SCALE_FACTOR - WINSIZE_X)
            {
                cameraRange.top = 480 * SCALE_FACTOR;
                cameraRange.bottom = WINSIZE_Y;
            
                cameraRange.left = 0 * SCALE_FACTOR;
                cameraRange.right = mapSize.x;

                cout << "Zone2" << endl;
            }

            else if (playerPos.x >= 4060 * SCALE_FACTOR - WINSIZE_X && playerPos.x < 5000 * SCALE_FACTOR)
            {
                if (playerPos.y < 1400)
                {
                    cameraRange.top = mapSize.y * SCALE_FACTOR;
                    cameraRange.left = 5024 * SCALE_FACTOR;
                    cout << "Zone4" << endl;
                }

                else
                {
                    cameraRange.top = 480 * SCALE_FACTOR;
                    cout << "Zone3" << endl;
                }

                cameraRange.left = 0 * SCALE_FACTOR;
                cameraRange.right = 5400 * SCALE_FACTOR;
                cameraRange.bottom = WINSIZE_Y + 240 * SCALE_FACTOR;
            }

            else if (playerPos.x >= 5200 * SCALE_FACTOR && playerPos.x < 5400 * SCALE_FACTOR)
            {
                cameraRange.top = mapSize.y * SCALE_FACTOR;
                cameraRange.bottom = WINSIZE_Y + 240 * SCALE_FACTOR;

                cameraRange.left = 5024 * SCALE_FACTOR;
                cameraRange.right = 5400 * SCALE_FACTOR;

                cout << "Zone5" << endl;
            }

            else if (playerPos.x >= 5400 * SCALE_FACTOR && playerPos.x < mapSize.x * SCALE_FACTOR)
            {
                cameraRange.top = mapSize.y * SCALE_FACTOR;

                cameraRange.bottom = WINSIZE_Y + (688 * SCALE_FACTOR);

                cameraRange.left = 5024 * SCALE_FACTOR;
                cameraRange.right = mapSize.x;
                cout << "Zone6" << endl;
            }

            break;
    }
}

void CameraManager::cameraTest(void)
{
    if (KEYMANAGER->isStayKeyDown('J'))
    {
        camera.x -= 2;
    }
    if (KEYMANAGER->isStayKeyDown('L'))
    {
        camera.x += 2;
    }
    if (KEYMANAGER->isStayKeyDown('I'))
    {
        camera.y -= 2;
    }
    if (KEYMANAGER->isStayKeyDown('K'))
    {
        camera.y += 2;
    }
}


