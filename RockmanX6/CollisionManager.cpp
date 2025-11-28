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
			int rnd = RND->getInt(4);
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
				damageEvent.bType = BulletType::None;
				damageEvent.damage = damageEvent.target->getPhyscialDamage();
				EVENTMANAGER->dispatchEvents({EventType::Damage, &damageEvent});

				/*
				// enemy->;
				enemy->reduceHp(player->getSaberDamage());
				player->setAnimDelay(true);
				SOUNDMANAGER->play("SFX_SaberHit", 0.5f);

				switch (rnd)
				{
				case 0:
					//EFFECTMANAGER->spawnEffect(EffectType::SaberHit_1, ePos.x, ePos.y, eStatus.width, eStatus.height, eStatus.lookRight);
					break;
				case 1:
					//EFFECTMANAGER->spawnEffect(EffectType::SaberHit_2, ePos.x, ePos.y, eStatus.width, eStatus.height, eStatus.lookRight);
					break;
				case 2:
					//EFFECTMANAGER->spawnEffect(EffectType::SaberHit_3, ePos.x, ePos.y, eStatus.width, eStatus.height, eStatus.lookRight);
					break;
				case 3:
					//EFFECTMANAGER->spawnEffect(EffectType::SaberHit_4, ePos.x, ePos.y, eStatus.width, eStatus.height, eStatus.lookRight);
					break;
				}
				*/
				break;
			}
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