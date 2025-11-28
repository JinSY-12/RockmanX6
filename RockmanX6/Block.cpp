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

    oStatus.width = oStatus.oImage->getWidth();
    oStatus.height = oStatus.oImage->getHeight();
    
    oStatus.oHitBox = RectMakeCenter(x + oStatus.width / 2, y + oStatus.height / 2, oStatus.width, oStatus.height);
    oStatus.worldRect = oStatus.oHitBox;
    
    oPos.x = x;
    oPos.y = oStatus.worldRect.bottom;

    status.dead = false;
    
    return S_OK;
}

void Block::release(void)
{

}

void Block::update(void)
{
    setObjectHitbox();
    checkBulletCollision();
    checkPlayerAttCollision();
    checkDead();
}

/*
void Block::checkPlayerAttCollision(void)
{
    RECT temp;
    if (IntersectRect(&temp, &oPlayer->getSaberRect(), &oStatus.oHitBox) && oPlayer->getCanHit() )
    {
        oStatus.hp -= oPlayer->getSaberDamage();
        oPlayer->setAnimDelay(true);
        
        ("SFX_SaberHit", 0.5f);
    }
    
    vector<Bullet*>& bullets = oBManager->getBullet();

    for (auto it = bullets.begin(); it != bullets.end();)
    {
        if (IntersectRect(&temp, &(*it)->getBulletRect(), &oStatus.oHitBox))
        {
			EFFECTMANAGER->spawnEffect(EffectType::BursterBlock, (*it)->getBulletPosX(), (*it)->getBulletPosY(), (*it)->getBulletWidth(), (*it)->getBulletHeight(), (*it)->getBulletDir());
            SOUNDMANAGER->play("SFX_Block", 0.5f);
            it = bullets.erase(it);
        }

        else ++it;
    }
    
}
*/