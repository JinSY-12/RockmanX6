#include "Stdafx.h"
#include "BossGate.h"


HRESULT BossGate::init(int x, int y, int width, int top, int height, bool boss)
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

    animPlay = false;
    isUsed = false;
    bossRoom = boss;

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

		if (oStatus.oImage->getChangeReady())
        {
            oStatus.oImage->setChangeReady(false);
            state = DoorState::Opened;
        }
        break;

    case DoorState::Opened:
        if (CAMERAMANAGER->getIsCamaraMove() == false)
        {
            state = DoorState::Closing;
            SOUNDMANAGER->play("SFX_DoorClose", 0.5f);
            
        }
        break;

    case DoorState::Closing:
        oStatus.oImage->reversePlay(0.05f);
        if(oStatus.oImage->getChangeReady())
        {
			// if (BossType::Intro == SCENEMANAGER->getStageBossType()) ENEMYMANAGER->spawnBoss(BossType::Intro, 6110 * SCALE_FACTOR, -20 * SCALE_FACTOR);
            if (bossRoom) UIMANAGER->addUi(UiType::Warning);
            oStatus.oImage->setChangeReady(false);
            state = DoorState::Closed;
        }
        break;

    case DoorState::Closed:
        
        break;
    }
}

void BossGate::render(HDC hdc)
{
    oStatus.oImage->frameRender(hdc, oStatus.oHitBox.left, oStatus.oHitBox.top, oStatus.oImage->getFrameX(), 0);

    if (UIMANAGER->getIsDebugMode())
    {
        DrawRectMakeColor(hdc, oStatus.oHitBox, RGB(255, 255, 0), 2);
    }
}

void BossGate::animOncePlay(bool play)
{
    if (!isUsed)
    {
        state = DoorState::Opening;
        SOUNDMANAGER->play("SFX_DoorOpen", 0.5f);
    }
    isUsed = play;

    CAMERAMANAGER->bossCameraMove(camera.left, camera.bottom, camera.right, camera.top);
 }
