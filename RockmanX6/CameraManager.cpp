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

    mBtype = BossType::None;

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

    setMaxCameraRange();
    cameraOffset();
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
    // ī�޶� x��ǥ ����
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
    // ī�޶� y��ǥ ����
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
    switch (mBtype)
    {
        // Zone ������ ȭ���� �� ���� �����Դϴ�.
        // �׷��� Zone�� ���ϰ� ���� ��ġ - 160�� ���ֽø� Zone ������ �� �� �ֽ��ϴ�.
        // ī�޶� ���� �� top�� �� ������ ���ϰ� �̹��� �ؿ��������� ���̸� ������ ��ǥ�� ������ �˴ϴ�.
        // ī�޶� ���� �� bottom�� ������ ������ �̹��� �ؿ��������� ���̸� WINSIZE_Y�� ���ϸ� ������ �˴ϴ�.
        // ī�޶� ���� �� left�� ���� ������ ���ϰ� ���� ��ǥ���� - 160�� �ϸ� ������ �˴ϴ�.
        // ī�޶� ���� �� right�� ������ ������ ���ϰ� ��ǥ���� - 320�� �ϸ� ������ �˴ϴ�.

        case BossType::Intro:            
            if (camera.x >= 0 * SCALE_FACTOR && camera.x < (3530 - 160) * SCALE_FACTOR)
            {
                cameraRange.top = 288 * SCALE_FACTOR;
                cameraRange.bottom = WINSIZE_Y;
                cameraRange.left = 0 * SCALE_FACTOR;
                cameraRange.right = mapSize.x;
                
                // cout << "Zone 1" << endl;
            }
            
            else if (camera.x >= (3530 - 160) * SCALE_FACTOR && camera.x < (4064 - 160) * SCALE_FACTOR)
            {
                cameraRange.top = 480 * SCALE_FACTOR;
                cameraRange.bottom = WINSIZE_Y;  
            
                cameraRange.left = 0 * SCALE_FACTOR;
                cameraRange.right = mapSize.x;

                // cout << "Zone 2" << endl;
            }

            else if (camera.x >= (4064 - 160) * SCALE_FACTOR && camera.x < (5168 - 160) * SCALE_FACTOR && camera.y >= 490 * SCALE_FACTOR)
            {
                cameraRange.left = 0 * SCALE_FACTOR;
                cameraRange.right = 5400 * SCALE_FACTOR;
                cameraRange.top = 480 * SCALE_FACTOR;
                cameraRange.bottom = WINSIZE_Y + 240 * SCALE_FACTOR;

                // cout << "Zone 3" << endl;
            }

            // ���� ������ Ȯ�� �Ϸ�
            else if (camera.x >= (5168 - 160) * SCALE_FACTOR && camera.x < (5376 - 160) * SCALE_FACTOR)
            {
                cameraRange.top = mapSize.y;

                // Zone6 ������ ���� ����
                float slopeStartX = (5184 - 160) * SCALE_FACTOR;
                float slopeEndX = (5376 - 160) * SCALE_FACTOR;
                float bottomStart = WINSIZE_Y + 533 * SCALE_FACTOR;; // ������ ���� bottom
                float bottomEnd = WINSIZE_Y + 700 * SCALE_FACTOR;  // ������ �� bottom

                float t = (camera.x - slopeStartX) / (slopeEndX - slopeStartX);
                t *= 1.5f;
                if (t > 1.0f) t = 1.0f;

                // ī�޶� �¿� ����
                if (camera.y <= (280 - 120) * SCALE_FACTOR)
                {
                    cameraRange.left = 5008 * SCALE_FACTOR;
                    cameraRange.right = mapSize.x;
                    cameraRange.bottom = bottomStart + (bottomEnd - bottomStart) * t;

                    // cout << "Zone 6" << endl;
                }
                
                else if (camera.y <= (480 - 120) * SCALE_FACTOR)
                {
                    cameraRange.left = 5008 * SCALE_FACTOR;
                    cameraRange.right = (5440 - 320) * SCALE_FACTOR;
                    cameraRange.bottom = WINSIZE_Y + 240 * SCALE_FACTOR;

                    // cout << "Zone 5" << endl;
                }

                else
                {
                    cameraRange.left = 0 * SCALE_FACTOR;
                    cameraRange.right = (5440 - 320) * SCALE_FACTOR;
                    cameraRange.bottom = WINSIZE_Y + 240 * SCALE_FACTOR;

                    // cout << "Zone 4" << endl;
                }
            }

            else if (camera.x >= (5376 - 160) * SCALE_FACTOR)
            {
                cameraRange.left = 0 * SCALE_FACTOR;
                cameraRange.right = mapSize.x - 320 * SCALE_FACTOR;
                cameraRange.bottom = WINSIZE_Y + 700 * SCALE_FACTOR;

                // cout << "Zone Boss" << endl;
            }
            break;
    }
}

void CameraManager::cameraTest(void)
{

}


