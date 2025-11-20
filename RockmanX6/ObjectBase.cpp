#include "Stdafx.h"
#include "ObjectBase.h"
#include "BulletManager.h"
#include "Player.h"

void ObjectBase::render(void)
{
	oStatus.oImage->render(getMemDC(), oStatus.oHitBox.left, oStatus.oHitBox.top);
	
	if (UIMANAGER->getIsDebugMode())
	{
		DrawRectMakeColor(getMemDC(), oStatus.oHitBox, RGB(255, 255, 0), 2);
	}
}

void ObjectBase::setObjectHitbox(void)
{
	oStatus.oHitBox.left = oStatus.worldRect.left - CAMERAMANAGER->getCameraPos().x;
	oStatus.oHitBox.right = oStatus.worldRect.right - CAMERAMANAGER->getCameraPos().x;

	oStatus.oHitBox.top = oStatus.worldRect.top - CAMERAMANAGER->getCameraPos().y;
	oStatus.oHitBox.bottom = oStatus.worldRect.bottom - CAMERAMANAGER->getCameraPos().y;
}

void ObjectBase::checkPlayerAttCollision(void)
{
	RECT temp;
	if (IntersectRect(&temp, &oPlayer->getSaberRect(), &oStatus.oHitBox) && oPlayer->getCanHit())
	{
		int rnd = RND->getInt(4);

		switch (oStatus.oType)
		{
		case ObjectType::Block:
			oStatus.hp -= oPlayer->getSaberDamage();
			oPlayer->setAnimDelay(true);
			SOUNDMANAGER->play("SFX_SaberHit", 0.5f);
			
			switch (rnd)
			{
			case 0:
				EFFECTMANAGER->spawnEffect(EffectType::SaberHit_1, oPos.x, oPos.y, oStatus.width, oStatus.height, 0);
				break;
			case 1:
				EFFECTMANAGER->spawnEffect(EffectType::SaberHit_2, oPos.x, oPos.y, oStatus.width, oStatus.height, 0);
				break;
			case 2:
				EFFECTMANAGER->spawnEffect(EffectType::SaberHit_3, oPos.x, oPos.y, oStatus.width, oStatus.height, 0);
				break;
			case 3:
				EFFECTMANAGER->spawnEffect(EffectType::SaberHit_4, oPos.x, oPos.y, oStatus.width, oStatus.height, 0);
				break;
			}
		break;

		default:
			break;
		}
	}
}

void ObjectBase::checkBulletCollision(void)
{
	RECT temp;

	vector<Bullet*>& bullets = oBManager->getBullet();
	
	for (auto it = bullets.begin(); it != bullets.end();)
	{
		if (IntersectRect(&temp, &(*it)->getBulletRect(), &oStatus.oHitBox))
		{
			switch (oStatus.oType)
			{
			case ObjectType::Block:
				EFFECTMANAGER->spawnEffect(EffectType::BursterBlock, (*it)->getBulletPosX(), (*it)->getBulletPosY(), (*it)->getBulletWidth(), (*it)->getBulletHeight(), (*it)->getBulletDir());
				SOUNDMANAGER->play("SFX_Block", 0.5f);
				it = bullets.erase(it);
				break;
			default:
				break;
			}
		}

		else ++it;
	}
}

void ObjectBase::checkWallCollision(void)
{
	
}

void ObjectBase::checkDead(void)
{
	if (oStatus.hp <= 0)
	{
		oStatus.hp = 0;
		oStatus.dead = true;

	}
}
