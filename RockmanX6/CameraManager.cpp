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
    _isPadeInResult = false;
    _isPadeOutResult = false;

    mBtype = BossType::None;

	isAutoMove = false;
	targetX = 0.0f;
    targetY = 0.0f;
	lerpSpeed = 0.05f;

    cameraMove = false;
    cameraMoving = false;
    lerpCompleteX = false;
    lerpCompleteY = false;
    locationTestX = 0;
    locationTestRight = 0;
    cameraMoveEnd = true;

    locationTestY = 0;
    locationTestBottom = 0;

    timer = 0.0f;

    moveDir = None;

    checkPointSave.x = 0;
    checkPointSave.y = 0;

    zoneNumber = 0;
    prevZoneNumber = -1;

    test = false;

    return S_OK;
}

void CameraManager::release(void)
{
   // SAFE_DELETE(_blackImage);
}

void CameraManager::update(void)
{
#pragma region 페이드 인/아웃 처리
    // 페이드-인
    if (_isBlackPadeIn)
    {
        _whiteAlpha = 0;
        _blackAlpha -= 255.0f / (_padeTime * TIMEMANAGER->getFrameRate());

        if (_blackAlpha <= 0)
        {
            _isBlackPadeIn = false;
            _isPadeInResult = true;
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
            _isPadeOutResult = true;
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
            _isPadeInResult = true;
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
            _isPadeOutResult = true;
            _whiteAlpha = 255;
        }
    }

#pragma endregion

    if (isAutoMove)
    {
        camera.x += (targetX - camera.x) * lerpSpeed;

        if(useLerpY) camera.y += (targetY - camera.y) * lerpSpeed;

		bool reachedX = fabs(camera.x - targetX) < 1.0f;
		bool reachedY = !useLerpY || fabs(camera.y - targetY) < 1.0f;

        // 거의 다 도착했으면 멈춤
        if (reachedX && reachedY)
        {
            camera.x = targetX;
			if (useLerpY) camera.y = targetY;
            isAutoMove = false;
        }

        return;
    }

    if (KEYMANAGER->isOnceKeyDown('Y'))
    {
        cameraMove = (cameraMove == true) ? false : true; 
    }

    // 보스전 카메라 무빙 테스트

    

    if (cameraMoving)
    {
        if (forceCameraMove(locationTestX, locationTestBottom, locationTestRight, locationTestY))
        {
            cameraMoving = false;
            timer = TIMEMANAGER->getWorldTime();
            cameraMove = false;
            cameraMoveEnd = false;
        }
    }

    if (!cameraMoveEnd)
    {
        if (TIMEMANAGER->getWorldTime() - timer >= 1.0f)
        {
            cameraMoveEnd = true;
        }
    }
    
    if(!test) setMaxCameraRange();

    if (KEYMANAGER->isOnceKeyDown('1'))
    {
        test = !test;
        cameraRange.top = 0 * SCALE_FACTOR;
        cameraRange.bottom = mapSize.y;
        cameraRange.left = 0 * SCALE_FACTOR;
        cameraRange.right = mapSize.x;
    }

    cameraOffset();
}

void CameraManager::render(HDC hdc)
{
    _blackImage->alphaRender(hdc, _blackAlpha);
    _whiteImage->alphaRender(hdc, _whiteAlpha);

    if (UIMANAGER->getIsDebugMode() == true)
    {
        TEXTMANAGER->drawTextColor(hdc, WINSIZE_X / 50 * 40, 0.85 * WINSIZE_Y + 50, "카매라 X", "DNF_M_18", RGB(0, 255, 255));
        TEXTMANAGER->drawTextColor(hdc, WINSIZE_X / 50 * 40, 0.85 * WINSIZE_Y + 70, to_string(camera.x / SCALE_FACTOR), "DNF_M_18", RGB(0, 255, 255));
        
        TEXTMANAGER->drawTextColor(hdc, WINSIZE_X / 50 * 40 + 80, 0.85 * WINSIZE_Y + 50, "카매라 Y", "DNF_M_18", RGB(0, 255, 255));
        TEXTMANAGER->drawTextColor(hdc, WINSIZE_X / 50 * 40 + 80, 0.85 * WINSIZE_Y + 70, to_string(camera.y / SCALE_FACTOR), "DNF_M_18", RGB(0, 255, 255));
    }
}

#pragma region 카메라 효과
void CameraManager::padeIn(float padeTime)
{
    _blackAlpha = 255.0f;
    _padeTime = padeTime;
    _isPadeInResult = false;
    _isBlackPadeIn = true;
}

void CameraManager::padeOut(float padeTime)
{
    _blackAlpha = 0.0f;
    _padeTime = padeTime;
    _isPadeOutResult = false;
    _isBlackPadeOut = true;
}

void CameraManager::whiteIn(float padeTime)
{
    _whiteAlpha = 255.0f;
    _padeTime = padeTime;
    _isPadeInResult = false;
    _isWhitePadeIn = true;
}

void CameraManager::whiteOut(float padeTime)
{
    _whiteAlpha = 0.0f;
    _padeTime = padeTime;
    _isPadeOutResult = false;
    _isWhitePadeOut = true;
}
#pragma endregion

void CameraManager::cameraOffset(void)
{
    if (!cameraMove)
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

        else if (camera.x + WINSIZE_X >= cameraRange.right)
        {
            camera.x = cameraRange.right - WINSIZE_X;
            cameraLockX = true;
        }

        else cameraLockX = false;
    
    /////////////////////////
    // 카메라 y좌표 고정
    /////////////////////////
        if (camera.y <= cameraRange.top)
        {
            camera.y = cameraRange.top;
            cameraLockY = true;
        }

        else if (camera.y + WINSIZE_Y >= cameraRange.bottom)
        {
            camera.y = cameraRange.bottom - WINSIZE_Y;
            cameraLockY = true;
        }
        
        else cameraLockY = false;
    }
}

void CameraManager::setMaxCameraRange()
{
    switch (mBtype)
    {
        // Zone 범위와 camera.x는 화면의 맨 왼쪽 기준입니다.
        // Zone 범위와 camera.y는 캐릭터 바닥에서 위로 WINSIZE_Y/2 만큼입니다. 즉 보이는 화면의 최상단입니다.
        // (캐릭터 X좌표 - 160)을 해주시면 Zone의 X축 범위를 설정을 할 수 있습니다.
        // (캐릭터 Y좌표 - 120)을 해주시면 Zone의 Y축 범위를 설정을 할 수 있습니다.
        // cameraRange left와 right는 포토샵 기준 가로 좌표를 측정해서 입력해주면 됩니다.
        // cameraRange top과 bottom은 포토샵 기준 세로 좌표를 측정해서 입력해주면 됩니다.
        // right와 bottom은 체크 방법을 바꿔뒀으니 right와 bottom을 설정하고 싶은 좌표 그대로 입력하시면 됩니다.

        case BossType::Intro:            
            if (camera.x >= 0 * SCALE_FACTOR && camera.x < 1600 * SCALE_FACTOR)
            {
                cameraRange.top = 682 * SCALE_FACTOR;
                cameraRange.bottom = mapSize.y;
                cameraRange.left = 0 * SCALE_FACTOR;
                cameraRange.right = mapSize.x;
                
                zoneNumber = 0;
                zoneName = "Zone 1-1";

                setCheckPoint(160 * SCALE_FACTOR, 682 * SCALE_FACTOR, zoneNumber);
            }

            if (camera.x >= 1965 * SCALE_FACTOR && camera.x < 3370 * SCALE_FACTOR)
            {
                cameraRange.top = 682 * SCALE_FACTOR;
                cameraRange.bottom = mapSize.y;
                cameraRange.left = 1965 * SCALE_FACTOR;
                cameraRange.right = mapSize.x;

                zoneNumber = 1;
                zoneName = "Zone 1-2";

                setCheckPoint(2090 * SCALE_FACTOR, 682 * SCALE_FACTOR, zoneNumber);
            }
            
            else if (camera.x >= 3370 * SCALE_FACTOR && camera.x < 3860 * SCALE_FACTOR)
            {
                cameraRange.top = 490 * SCALE_FACTOR;
                cameraRange.bottom = mapSize.y;
            
                cameraRange.left = 0 * SCALE_FACTOR;
                cameraRange.right = mapSize.x;

                zoneNumber = 2;
                zoneName = "Zone 2";
            }

            else if (camera.x >= 3860 * SCALE_FACTOR && camera.x < 5008 * SCALE_FACTOR && camera.y >= 490 * SCALE_FACTOR)
            {
                cameraRange.top = 490 * SCALE_FACTOR;
                cameraRange.bottom = 730 * SCALE_FACTOR;
                cameraRange.left = 0 * SCALE_FACTOR;
                cameraRange.right = 5400 * SCALE_FACTOR;

                zoneNumber = 3;
                zoneName = "Zone 3";
            }

            else if (camera.x >= 5008 * SCALE_FACTOR && camera.x < 5216 * SCALE_FACTOR)
            {
                cameraRange.top = 0;

                // 카메라 좌우 범위
                if (camera.y <= (290 - 120) * SCALE_FACTOR)
                {
                    cameraRange.left = 5008 * SCALE_FACTOR;
                    cameraRange.right = mapSize.x;
                    cameraRange.bottom = cameraLerpY((5184 - 160) * SCALE_FACTOR, (5376 - 160) * SCALE_FACTOR,
                        450 * SCALE_FACTOR, 270 * SCALE_FACTOR);

                    zoneNumber = 6;
                    zoneName = "Zone 6";
                }
                
                else if (camera.y <= (490 - 120) * SCALE_FACTOR)
                {
                    cameraRange.left = 5008 * SCALE_FACTOR;
                    cameraRange.right = 5440 * SCALE_FACTOR;
                    cameraRange.bottom = 720 * SCALE_FACTOR;

                    zoneNumber = 5;
                    zoneName = "Zone 5";
                }

                else
                {
                    cameraRange.left = 0 * SCALE_FACTOR;
                    cameraRange.right = 5440 * SCALE_FACTOR;
                    cameraRange.bottom = 730 * SCALE_FACTOR;

                    zoneNumber = 4;
                    zoneName = "Zone 4";

                }
            }

            else if (camera.x >= 5216 * SCALE_FACTOR && camera.x < 5743 * SCALE_FACTOR)
            {
                if (!cameraMove)
                {
                    cameraRange.top = 10 * SCALE_FACTOR;
                    cameraRange.left = 0 * SCALE_FACTOR;
                    cameraRange.right = 5743 * SCALE_FACTOR;
                    cameraRange.bottom = 280 * SCALE_FACTOR;
                }
                
                zoneNumber = 98;
                zoneName = "Zone Last";

            }

            else if (camera.x >= 5743 * SCALE_FACTOR && camera.x < 6063 * SCALE_FACTOR)
            {
                cameraRange.top = 10 * SCALE_FACTOR;
                cameraRange.left = 5743 * SCALE_FACTOR;
                cameraRange.right = 6063 * SCALE_FACTOR;
                cameraRange.bottom = 250 * SCALE_FACTOR;

                zoneNumber = 99;
                zoneName = "Zone Boss Room";

                setCheckPoint(5904 * SCALE_FACTOR, 0 * SCALE_FACTOR, zoneNumber);
            }

            else if (camera.x >= 6063 * SCALE_FACTOR && camera.x < mapSize.x)
            {
                cameraRange.top = 10 * SCALE_FACTOR;
                cameraRange.left = 6063 * SCALE_FACTOR;
                cameraRange.right = mapSize.x;
                cameraRange.bottom = 250 * SCALE_FACTOR;

                zoneNumber = 100;
                zoneName = "Zone Battle Boss";
            }

            else zoneName = "Zone Else";

            // 보스 입장 준비

            // 보스 전
            break;
    }
}

void CameraManager::startAutoMove(float x, float y, bool useY)
{
    targetX = x;
    targetY = y;
    useLerpY = useY;
    isAutoMove = true;
}

void CameraManager::bossRoomOffest(void)
{
    if (isAutoMove)
    {
        camera.x = camera.x + (targetX - camera.x) * lerpSpeed;

        // 거의 다 도착했으면 멈춤
        if (fabs(camera.x - targetX) < 1.0f)
        {
            camera.x = targetX;
            isAutoMove = false;
        }

        return; // 자동 이동 중엔 playerPos 무시
    }
}

float CameraManager::cameraLerpY(float slopeStartX, float slopeEndX, float bottomStart, float bottomEnd)
{
    float t = (camera.x - slopeStartX) / (slopeEndX - slopeStartX);
    t *= 1.5f;
    if (t > 1.0f) t = 1.0f;

    return bottomStart + (bottomEnd - bottomStart) * t;
}

// 포토샵 가서 원하는 좌표 입력하면 Left는 알아서 움직입니다.
// Top은 테스트 아직 안해봄ㅋㅋ

bool CameraManager::forceCameraMove(int targetPointLeft, int targetPointBottom, int targetRight, int targetTop)
{
    if (camera.x < targetPointLeft)
    {
        // cameraMove = true;
        lerpCompleteX = false;
        camera.x += 5;
        cameraRange.left = camera.x;
        cameraRange.right = targetRight;
    }

    else
    {
        // cameraMove = false;
        lerpCompleteX = true;
        cameraRange.left = targetPointLeft;
        cameraRange.right = targetRight;
    }

    if (moveDir == CameraMoveDir::None)
    {
        if (camera.y + WINSIZE_Y > targetPointBottom) moveDir = CameraMoveDir::Up;
        else if (camera.y + WINSIZE_Y > targetPointBottom) moveDir = CameraMoveDir::Down;
        else lerpCompleteY = true;
    }

    // 화면이 밑에서 위로 올라가야 하면
    if (moveDir == CameraMoveDir::Up)
    {
        if (camera.y + WINSIZE_Y > targetPointBottom)
        {
            // cameraMove = true;
            lerpCompleteY = false;
            camera.y -= 5;
            cameraRange.bottom -= camera.y + 15 * SCALE_FACTOR;;
            cameraRange.top -= 5;
        }

        else
        {
            // cameraRange.top = targetTop * SCALE_FACTOR;
            cameraRange.bottom = targetPointBottom;
            lerpCompleteY = true;

            moveDir = CameraMoveDir::None;
        }
    }
   
    // 화면이 위에서 밑으로 내려갈 때
    else if (moveDir == CameraMoveDir::Down)
    {
        if (camera.y + WINSIZE_Y < targetPointBottom)
        {
            // cameraMove = true;
            lerpCompleteY = false;
            camera.y += 5;
            cameraRange.top = camera.y + 15 * SCALE_FACTOR;
            cameraRange.bottom += 5;
        }

        else
        {
            // cameraRange.top = targetTop * SCALE_FACTOR;
            cameraRange.bottom = targetPointBottom;
            lerpCompleteY = true;

            moveDir = CameraMoveDir::None;
        }
    }

    if(lerpCompleteX && lerpCompleteY) return true;
}
