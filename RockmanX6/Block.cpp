#include "Stdafx.h"
#include "Block.h"
#include "Player.h"
#include "BulletManager.h"

HRESULT Block::init(void)
{
	return S_OK;
}

HRESULT Block::init(int x, int y)
{
    status.type = CombatEntityType::Object;
    oStatus.oType = ObjectType::Block;

    status.maxHp = 10;
    status.hp = status.maxHp;

    oStatus.oImage = new GImage;
    oStatus.oImage = IMAGEMANAGER->findImage("Object_Block")->cloneImage();

    status.width = oStatus.oImage->getWidth();
    status.height = oStatus.oImage->getHeight();
    
    oStatus.oHitBox = RectMakeCenter(x + status.width / 2, y + status.height / 2, status.width, status.height);

    oStatus.worldRect = oStatus.oHitBox;
    
    pos.x = x;
    pos.y = oStatus.worldRect.bottom;

    status.dead = false;
    status.overpower = false;
    
    return S_OK;
}

void Block::release(void)
{

}

void Block::update(void)
{
    setObjectHitbox();

    checkDead();
}

void Block::render(HDC hdc)
{
    oStatus.oImage->render(hdc, oStatus.oHitBox.left, oStatus.oHitBox.top);

    if (UIMANAGER->getIsDebugMode())
    {
        DrawRectMakeColor(hdc, oStatus.oHitBox, RGB(255, 255, 0), 2);
    }
}

