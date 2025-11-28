#include "Stdafx.h"
#include "CollisionManager.h"
#include "Player.h"
#include "EnemyManager.h"
#include "BulletManager.h"
#include "ObjectManager.h"


HRESULT CollisionManager::init(void)
{
    return S_OK;
}

void CollisionManager::release(void)
{

}

void CollisionManager::update(void)
{
	checkCollisions();
}

void CollisionManager::render(void)
{

}



void CollisionManager::checkCollisions(void)
{
	checkPlayerVsEnemy();
	checkPlayerVsObject();
	checkPlayerVsBullet();

	checkBulletVsEnemy();
	checkBulletVsObject();

	checkEnemyVsBullet();
}

void CollisionManager::checkPlayerVsEnemy(void)
{
	for (auto& enemy : ememies->getEnemy())
	{
		RECT temp;

		if (IntersectRect(&temp, &player->getSaberRect(), &enemy->getEnemyHitBox()) && !enemy->getOverPower() && player->getCanHit())
		{
			player->setAnimDelay(true);

			switch (enemy->getEnemyType())
			{
			case EnemyType::MetaDridler:
				// Æ¨±â´Â ÀÌÆåÆ®
				SOUNDMANAGER->play("SFX_Block", 0.5f);
				break;

			default:
				damageEvent.attacker = player;
				damageEvent.target = enemy;
				damageEvent.dType = DamageType::Saber;
				damageEvent.damage = damageEvent.target->getPhyscialDamage();
				EVENTMANAGER->dispatchEvents({EventType::Damage, &damageEvent});
				break;
			}
		}
	}

	for (auto& enemy : ememies->getEnemy())
	{
		RECT temp;

		if (IntersectRect(&temp, &player->getPlayerHitBox(), &enemy->getEnemyHitBox()) && !player->getOverPower())
		{
			damageEvent.attacker = enemy;
			damageEvent.target = player;
			damageEvent.dType = DamageType::Touch;
			damageEvent.damage = damageEvent.target->getPhyscialDamage();
			EVENTMANAGER->dispatchEvents({ EventType::Damage, &damageEvent });
			break;
		}
	}


}

void CollisionManager::checkPlayerVsObject(void)
{

}

void CollisionManager::checkPlayerVsBullet(void)
{

}

void CollisionManager::checkBulletVsEnemy(void)
{

}

void CollisionManager::checkBulletVsObject(void)
{

}

void CollisionManager::checkEnemyVsBullet(void)
{

}