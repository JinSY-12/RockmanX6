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

	isAutoMove = false;
	targetX = 0.0f;
    targetY = 0.0f;
	lerpSpeed = 0.05f;

    cameraMove = false;
    lerpCompleteX = false;
    lerpCompleteY = false;
    test = false;
    locationTestX = 0;
    locationTestRight = 0;

    locationTestY = 0;
    locationTestBottom = 0;

    moveDir = None;

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

    if (KEYMANAGER->isOnceKeyDown('1'))
    {
        locationTestX = 5743 * SCALE_FACTOR;
        locationTestRight = locationTestX + (320 * SCALE_FACTOR);
        locationTestY = 0;
        locationTestBottom = 240 * SCALE_FACTOR;
        test = true;
    }

    if (KEYMANAGER->isOnceKeyDown('2'))
    {
        locationTestX = 6063 * SCALE_FACTOR;
        locationTestRight = locationTestX + (400 * SCALE_FACTOR);
        locationTestY = 0;
        locationTestBottom = 240 * SCALE_FACTOR;
        test = true;
    }

    if (test)
    {
        if (forceCameraMove(locationTestX, locationTestBottom, locationTestRight, locationTestY, 5, 5))
        {
            cameraMove = false;
            test = false;
        }
    }

    setMaxCameraRange();
    cameraOffset();

    //////////////////////////
    // 카메라 테스트
    //////////////////////////

    if (UIMANAGER->getIsDebugMode() == true)
    {
        cout << "캐릭터 X : " << playerPos.x << endl;
        cout << "캐릭터 Y : " << playerPos.y << endl;
        cout << "카메라 X : " << camera.x << endl;
        cout << "카메라 Y : " << camera.y << endl;
        cout << "카메라 Top : " << cameraRange.top << endl;
        cout << "카메라 Bottom : " << cameraRange.bottom << endl;
        cout << "카메라 Left : " << cameraRange.left << endl;
        cout << "카메라 Right : " << cameraRange.right << endl;
        cout << endl;
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
        // 그래서 Zone을 정하고 싶은 위치는 (캐릭터 좌표 - 160)을 해주시면 Zone 설정을 할 수 있습니다.
        // 카메라 고정 값 top은 이미지 위쪽에서 밑으로 내려서 정하면 됩니다.
        // 카메라 고정 값 bottom은 이미지 위쪽에서 밑으로 내려서 정하면 됩니다.
        // 카메라 고정 값 left는 왼쪽 끝으로 정하고 싶은 좌표에서 camera.x의 값과 동일하게 하거나 (캐릭터 좌표 - 160)을 하면 고정이 됩니다.
        // 카메라 고정 값 right는 맵 이미지에서 우측 끝으로 맞추고 싶은 픽셀 값 -320을 하지면 됩니다. 혹은 (캐릭터 좌표 + 160)을 해도 됩니다.

        case BossType::Intro:            
            if (camera.x >= 0 * SCALE_FACTOR && camera.x < (1760 - 160) * SCALE_FACTOR)
            {
                cameraRange.top = 672 * SCALE_FACTOR;
                cameraRange.bottom = mapSize.y;
                cameraRange.left = 0 * SCALE_FACTOR;
                cameraRange.right = mapSize.x;
                
                zoneNum = "Zone 1-1";
            }

            if (camera.x >= (1760 - 160) * SCALE_FACTOR && camera.x < (3530 - 160) * SCALE_FACTOR)
            {
                cameraRange.top = 672 * SCALE_FACTOR;
                cameraRange.bottom = mapSize.y;
                cameraRange.left = (1760 - 160) * SCALE_FACTOR;
                cameraRange.right = mapSize.x;

                zoneNum = "Zone 1-2";
            }
            
            else if (camera.x >= (3530 - 160) * SCALE_FACTOR && camera.x < (4020 - 160) * SCALE_FACTOR)
            {
                cameraRange.top = 480 * SCALE_FACTOR;
                cameraRange.bottom = mapSize.y;
            
                cameraRange.left = 0 * SCALE_FACTOR;
                cameraRange.right = mapSize.x;

                zoneNum = "Zone 2";
            }

            else if (camera.x >= (4020 - 160) * SCALE_FACTOR && camera.x < (5168 - 160) * SCALE_FACTOR && camera.y >= 480 * SCALE_FACTOR)
            {
                cameraRange.top = 480 * SCALE_FACTOR;
                cameraRange.bottom = 720 * SCALE_FACTOR;
                cameraRange.left = 0 * SCALE_FACTOR;
                cameraRange.right = 5400 * SCALE_FACTOR;

                zoneNum = "Zone 3";
            }

            // 여기 위까지 확인 완료
            else if (camera.x >= (5168 - 160) * SCALE_FACTOR && camera.x < (5376 - 160) * SCALE_FACTOR)
            {
                cameraRange.top = 0;

                // Zone6 오르막 구간 정의
                /*
                float slopeStartX = (5184 - 160) * SCALE_FACTOR;
                float slopeEndX = (5376 - 160) * SCALE_FACTOR;
                float bottomStart = WINSIZE_Y + 533 * SCALE_FACTOR;; // 오르막 시작 bottom
                float bottomEnd = WINSIZE_Y + 700 * SCALE_FACTOR;  // 오르막 끝 bottom

                float t = (camera.x - slopeStartX) / (slopeEndX - slopeStartX);
                t *= 1.5f;
                if (t > 1.0f) t = 1.0f;
                */

                // 카메라 좌우 범위
                if (camera.y <= (280 - 120) * SCALE_FACTOR)
                {
                    cameraRange.left = (5168 - 160) * SCALE_FACTOR;
                    cameraRange.right = mapSize.x;
                    cameraRange.bottom = cameraLerpY((5184 - 160) * SCALE_FACTOR, (5376 - 160) * SCALE_FACTOR,
                        450 * SCALE_FACTOR, 270 * SCALE_FACTOR);

                    zoneNum = "Zone 6";
                }
                
                else if (camera.y <= (480 - 120) * SCALE_FACTOR)
                {
                    cameraRange.left = 5008 * SCALE_FACTOR;
                    cameraRange.right = 5440 * SCALE_FACTOR;
                    cameraRange.bottom = 720 * SCALE_FACTOR;

                    zoneNum = "Zone 5";
                }

                else
                {
                    cameraRange.left = 5008 * SCALE_FACTOR;
                    cameraRange.right = 5440 * SCALE_FACTOR;
                    cameraRange.bottom = 720 * SCALE_FACTOR;

                    zoneNum = "Zone 4";
                }
            }

            else if (camera.x >= (5376 - 160) * SCALE_FACTOR && camera.x < (5600 - 160) * SCALE_FACTOR)
            {
                cameraRange.left = 0 * SCALE_FACTOR;
                cameraRange.right = 5760 * SCALE_FACTOR;
                cameraRange.bottom = 270 * SCALE_FACTOR;
                
                zoneNum = "Zone Last";
            }

            else zoneNum = "Zone Else";

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

bool CameraManager::forceCameraMove(int targetPointX, int targetPointY, int targetRight, int targetTop, int cameraMoveSpeedX, int cameraMoveSpeedY)
{
    int offsetY;

    if (camera.x < targetPointX )
    {
        cameraMove = true;
        lerpCompleteX = false;
        camera.x += cameraMoveSpeedX;
        cameraRange.left = camera.x + 24 * SCALE_FACTOR;
        cameraRange.right += cameraMoveSpeedX;
    }

    else
    {
        cameraRange.left = targetPointX;
        cameraRange.right = targetRight;
        lerpCompleteX = true;
    }

    if (moveDir == CameraMoveDir::None)
    {
        if (camera.y + WINSIZE_Y > targetPointY) moveDir = CameraMoveDir::Up;
        else if (camera.y + WINSIZE_Y > targetPointY) moveDir = CameraMoveDir::Down;
        else lerpCompleteY = true;
    }

    // 화면이 밑에서 위로 올라가야 하면
    if (moveDir == CameraMoveDir::Up)
    {
        if (camera.y + WINSIZE_Y > targetPointY)
        {
            cout << "밑에서 위로" << endl;

            cameraMove = true;
            lerpCompleteY = false;
            camera.y -= cameraMoveSpeedY;
            cameraRange.bottom -= camera.y + 15 * SCALE_FACTOR;;
            cameraRange.top -= cameraMoveSpeedY;
        }

        else
        {
            cameraRange.top = targetTop * SCALE_FACTOR;
            cameraRange.bottom = targetPointY;
            lerpCompleteY = true;

            moveDir = CameraMoveDir::None;
        }
    }
   
    // 화면이 위에서 밑으로 내려갈 때
    else if (moveDir == CameraMoveDir::Down)
    {
        if (camera.y + WINSIZE_Y < targetPointY)
        {
            cout << "위에서 밑으로" << endl;

            cameraMove = true;
            lerpCompleteY = false;
            camera.y += cameraMoveSpeedY;
            cameraRange.top = camera.y + 15 * SCALE_FACTOR;
            cameraRange.bottom += cameraMoveSpeedY;
        }

        else
        {
            cameraRange.top = targetTop * SCALE_FACTOR;
            cameraRange.bottom = targetPointY;
            lerpCompleteY = true;

            moveDir = CameraMoveDir::None;
        }
    }

    if(lerpCompleteX && lerpCompleteY) return true;
}
