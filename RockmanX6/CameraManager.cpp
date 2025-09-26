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
    // ���̵�-��
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

    // ���̵�-�ƿ�
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

    if(_isWhitePadeOut)
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

    // �׽�Ʈ�Դϴ�

    if (KEYMANAGER->isStayKeyDown(VK_RIGHT))
    {
        CAMERAMANAGER->setPos(4,0);
    }

    else if (KEYMANAGER->isStayKeyDown(VK_LEFT))
    {
        if (camera.x >= 4 )
        {
            CAMERAMANAGER->setPos(-4, 0);
        }
    }

    if (KEYMANAGER->isStayKeyDown(VK_UP))
    {
        CAMERAMANAGER->setPos(0, -4);
    }

    else if (KEYMANAGER->isStayKeyDown(VK_DOWN))
    {
        if (camera.y <= 2880 - 772)
        {
            CAMERAMANAGER->setPos(0, 4);
         
        }
    }
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

void CameraManager::cameraShake(float intensity, float duration)
{
}
