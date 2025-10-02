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

    // cout << "PlayerPox.y : " << playerPos.y << endl;
    // cout << "camera.y : " << camera.y << endl;

    // 카메라 x좌표 고정
    if (camera.x < 0)
    {
        camera.x = 0;
        cameraLockX = true;
    }

    else if (camera.x > maxSize.x - WINSIZE_X)
    {
        camera.x = maxSize.x - WINSIZE_X;
        cameraLockX = true;
    }

    else cameraLockX = false;

    // 카메라 y좌표 고정
    if (camera.y < maxSize.y - 288 * 3)
    {
        camera.y = maxSize.y - 288 * 3;
        cameraLockY = true;
    }
    
    else if (camera.y > maxSize.y - WINSIZE_Y)
    {
        camera.y = maxSize.y - WINSIZE_Y;
        cameraLockY = true;
    }

    else cameraLockY = false;
}


