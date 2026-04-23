#include "Stdafx.h"
#include "BossGate.h"


HRESULT BossGate::init(int x, int y, int width, int top, int height)
{
    status.type = CombatEntityType::Object;
    oStatus.oType = ObjectType::BossGate;

    status.maxHp = 10;
    status.hp = status.maxHp;

    oStatus.oImage = new GImage;
    oStatus.oImage = IMAGEMANAGER->findImage("Object_BossGate")->cloneImage();

    status.width = oStatus.oImage->getFrameWidth();
    status.height = oStatus.oImage->getFrameHeight();

    oStatus.oHitBox = RectMakeCenter(x, y + status.height / 2, status.width, status.height);
    oStatus.worldRect = oStatus.oHitBox;

    pos.x = x;
    pos.y = oStatus.worldRect.bottom;

	// left는 Gate의 중앙값 = Gate통과 후 화면의 왼쪽값
	// right는 Gate통과 후 화면의 끝 부분
    // top은 Gate통과 후 화면의 천장 부분
    // top은 Gate통과 후 화면의 바닥 부분
    
    camera.left = x;
    camera.right = camera.left + width;
    camera.top = top;
    camera.bottom = top + height;

    state = DoorState::Closed;

    status.dead = false;
    status.overpower = false;

	closeTimer = 0.0f;

    animPlay = false;
    isUsed = false;
    openClose = false;

    return S_OK;
}

void BossGate::release(void)
{
}

void BossGate::update(void)
{
    setObjectHitbox();

    switch (state)
    {
    case DoorState::Opening:
        oStatus.oImage->play(0.05f);

        if (oStatus.oImage->getFrameX() >= oStatus.oImage->getMaxFrameX())
            state = DoorState::Opened;
        break;

    case DoorState::Opened:
        if (CAMERAMANAGER->getIsCamaraMove() == false)
        {
            openClose = true;
            oStatus.oImage->setFrameX(0);
            state = DoorState::Closing;
        }
        break;

    case DoorState::Closing:
        oStatus.oImage->play(0.05f);

        if (oStatus.oImage->getFrameX() >= oStatus.oImage->getMaxFrameX())
            state = DoorState::Closed;
        break;

    case DoorState::Closed:
        break;
    }
}

void BossGate::render(HDC hdc)
{
    oStatus.oImage->frameRender(hdc, oStatus.oHitBox.left, oStatus.oHitBox.top, oStatus.oImage->getFrameX(), openClose);

    if (UIMANAGER->getIsDebugMode())
    {
        DrawRectMakeColor(hdc, oStatus.oHitBox, RGB(255, 255, 0), 2);
    }
}

void BossGate::animOncePlay(bool play)
{
    if (!isUsed) state = DoorState::Opening;
    isUsed = play;

    CAMERAMANAGER->bossCameraMove(camera.left, camera.bottom, camera.right, camera.top);
 }
