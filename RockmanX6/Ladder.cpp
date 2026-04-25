#include "Stdafx.h"
#include "Ladder.h"

HRESULT Ladder::init(int x, int y)
{
    status.type = CombatEntityType::Object;
    oStatus.oType = ObjectType::Ladder;

    status.maxHp = 10;
    status.hp = status.maxHp;

    oStatus.oImage = new GImage;
    oStatus.oImage = IMAGEMANAGER->findImage("Object_Ladder")->cloneImage();

    status.width = oStatus.oImage->getWidth();
    status.height = oStatus.oImage->getHeight();

    oStatus.oHitBox = RectMakeCenter(x + status.width / 2, y + status.height / 2, status.width, status.height);
    oStatus.oHitBox.top -= 10 * SCALE_FACTOR; // 사다리의 윗부분이 플레이어의 발에 닿도록 hitbox를 살짝 늘려준다.
    
    oStatus.subRect.left = oStatus.oHitBox.left;
    oStatus.subRect.right = oStatus.oHitBox.right;
    oStatus.subRect.top = oStatus.subRect.top;
    oStatus.subRect.bottom = oStatus.oHitBox.top + 5 * SCALE_FACTOR;

    oStatus.worldRect = oStatus.oHitBox;

    pos.x = x;
    pos.y = oStatus.worldRect.bottom;

    status.dead = false;
    status.overpower = false;

    return S_OK;
}

void Ladder::release(void)
{

}

void Ladder::update(void)
{
    setObjectHitbox();

    checkDead();
}

void Ladder::render(HDC hdc)
{

    if (UIMANAGER->getIsDebugMode())
    {
        DrawRectMakeColor(hdc, oStatus.oHitBox, RGB(255, 255, 0), 2);
        DrawRectMakeColor(hdc, oStatus.subRect, RGB(0, 255, 0), 2);
    }
}
