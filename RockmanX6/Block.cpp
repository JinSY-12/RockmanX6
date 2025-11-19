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
    oStatus.oType = ObjectType::Block;

    oStatus.maxHp = 10;
    oStatus.hp = oStatus.maxHp;

    oStatus.oImage = new GImage;
    oStatus.oImage = IMAGEMANAGER->findImage("Object_Block")->cloneImage();

    oStatus.width = oStatus.oImage->getWidth();
    oStatus.height = oStatus.oImage->getHeight();
    
    oStatus.oHitBox = RectMakeCenter(x + oStatus.width / 2, y + oStatus.height / 2, oStatus.width, oStatus.height);
    oStatus.worldRect = oStatus.oHitBox;
    
    oPos.x = x;
    oPos.y = oStatus.worldRect.bottom;

    oStatus.dead = false;
    
    return S_OK;
}

void Block::release(void)
{

}

void Block::update(void)
{
    setObjectHitbox();
    checkPlayerAttCollision();
    checkDead();
}


void Block::checkPlayerAttCollision(void)
{
    RECT temp;
    if (IntersectRect(&temp, &oPlayer->getSaberRect(), &oStatus.oHitBox) && oPlayer->getCanHit() )
    {
        oStatus.hp -= oPlayer->getSaberDamage();
        oPlayer->setAnimDelay(true);
        SOUNDMANAGER->play("SFX_SaberHit", 0.5f);
    }
    
    vector<Bullet*>& bullets = oBManager->getBullet();

    for (auto it = bullets.begin(); it != bullets.end();)
    {
        if (IntersectRect(&temp, &(*it)->getBulletRect(), &oStatus.oHitBox))
        {
            // ★튕기기 이펙트 이미지 추가★            
            SOUNDMANAGER->play("SFX_SaberHit", 0.5f); // 사운드 대체 할 것
            it = bullets.erase(it);
        }

        else ++it;
    }
    
}
