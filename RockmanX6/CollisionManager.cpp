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
		// cout << "적 위치 : " << enemy->getEnemyHitBox().left << endl;;

		if (IntersectRect(&temp, &player->getSaberRect(), &enemy->getEnemyHitBox()) && !enemy->getOverPower() && player->getCanHit())
		{
			player->setAnimDelay(true);

			switch (enemy->getEnemyType())
			{
			case EnemyType::MetaDridler:
				// 튕기는 이펙트
				SOUNDMANAGER->play("SFX_Block", 0.5f);
				break;

			default:
				damageEvent.attacker = player;
				damageEvent.target = enemy;
				damageEvent.dType = DamageType::Saber;
				damageEvent.damage = damageEvent.attacker->getPhyscialDamage();
				EVENTMANAGER->dispatchEvents({EventType::TouchDamage, &damageEvent});
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
			damageEvent.damage = damageEvent.attacker->getPhyscialDamage();
			EVENTMANAGER->dispatchEvents({ EventType::TouchDamage, &damageEvent });
			break;
		}
	}
}

void CollisionManager::checkPlayerVsObject(void)
{
	// 공격 가능 오브젝트와 플레이어 공격범위 판정
	
	for (auto& object : objects->getObject())
	{
		RECT temp;
		
		// cout << "오브젝트 위치 : " << object->getObjectHitbox().left << endl;;

		if (IntersectRect(&temp, &player->getSaberRect(), &object->getObjectHitbox()) && !object->getOverPower() && player->getCanHit())
		{
			switch (object->getObjectType())
			{
			case ObjectType::Block:
				// 튕기는 이펙트
				player->setAnimDelay(true);
				damageEvent.attacker = player;
				damageEvent.target = object;
				damageEvent.dType = DamageType::Saber;
				damageEvent.damage = damageEvent.attacker->getPhyscialDamage();
				EVENTMANAGER->dispatchEvents({ EventType::TouchDamage, &damageEvent });
				break;

			default:
				
				break;
			}
		}
	}
}

void CollisionManager::checkPlayerVsBullet(void)
{
	RECT temp;

	for (auto& bullet : bullets->getEnemyBullet())
	{
		if (IntersectRect(&temp, &bullet->getBulletRect(), &player->getPlayerHitBox()) && !player->getOverPower() && !player->getIsDead())
		{
			damageEvent.attacker = player;
			damageEvent.target = player;
			damageEvent.bType = bullet->getBulletType();
			damageEvent.damage = bullet->getBulletDamage();
			damageEvent.bullet = bullet;
			EVENTMANAGER->dispatchEvents({ EventType::BulletDamage, &damageEvent });
			break;
		}
	}
}

void CollisionManager::checkBulletVsEnemy(void)
{
	for (auto& enemy : ememies->getEnemy())
	{
		RECT temp;

		for (auto& bullet : bullets->getBullet())
		{
			if (IntersectRect(&temp, &bullet->getBulletRect(), &enemy->getEnemyHitBox()) && !enemy->getOverPower() && !enemy->getIsDead())
			{
				damageEvent.attacker = player;
				damageEvent.target = enemy;
				damageEvent.bType = bullet->getBulletType();
				damageEvent.damage = bullet->getBulletDamage();
				damageEvent.bullet = bullet;
				EVENTMANAGER->dispatchEvents({ EventType::BulletDamage, &damageEvent });
				break;
			}
		}
	}

	for (auto& object : objects->getObject())
	{
		RECT temp;
		
		for (auto& bullet : bullets->getBullet())
		{
			if (IntersectRect(&temp, &bullet->getBulletRect(), &object->getObjectHitbox()) && !object->getIsDead())
			{
				damageEvent.attacker = player;
				damageEvent.target = object;
				damageEvent.bType = bullet->getBulletType();
				damageEvent.damage = bullet->getBulletDamage();
				damageEvent.bullet = bullet;
				EVENTMANAGER->dispatchEvents({ EventType::BulletDamage, &damageEvent });
				break;
			}
		}
	}

}

void CollisionManager::checkBulletVsObject(void)
{

}

void CollisionManager::checkEnemyVsBullet(void)
{

}