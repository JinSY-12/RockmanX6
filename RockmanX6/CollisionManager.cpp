#include "Stdafx.h"
#include "CollisionManager.h"
#include "Player.h"
#include "EnemyManager.h"
#include "BulletManager.h"
#include "ObjectManager.h"


void CollisionManager::release(void)
{

}

void CollisionManager::update(void)
{
	checkCollisions();
}

void CollisionManager::checkCollisions(void)
{
	checkContact();
	checkSaberCollision();
	checkBulletCollision();
}

void CollisionManager::checkContact(void)
{
	// 플레이어와 적이 서로 닿아있는지 판정
	for (auto& enemy : enemies->getEnemy())
	{
		RECT temp;

		if (IntersectRect(&temp, &player->getPlayerHitBox(), &enemy->getEnemyHitBox()) && !player->getOverPower()
			|| IntersectRect(&temp, &player->getPlayerHitBox(), &enemy->getEnemysubHitBox()) && !player->getOverPower())
		{
			damageEvent.attacker = enemy;
			damageEvent.target = player;
			damageEvent.dType = DamageType::Touch;
			damageEvent.damage = damageEvent.attacker->getPhyscialDamage();
			EVENTMANAGER->dispatchEvents({ EventType::TouchDamage, &damageEvent });
			break;
		}
	}

	for (auto& boss : enemies->getBoss())
	{
		RECT temp;

		if ((IntersectRect(&temp, &player->getPlayerHitBox(), &boss->getBossHitBox()) && !player->getOverPower())
			|| (IntersectRect(&temp, &player->getPlayerHitBox(), &boss->getBossSubHitBox()) && !player->getOverPower() && boss->getSubRectAttOn()))
		{
			damageEvent.attacker = boss;
			damageEvent.target = player;
			damageEvent.dType = DamageType::Touch;
			damageEvent.damage = damageEvent.attacker->getPhyscialDamage();
			EVENTMANAGER->dispatchEvents({ EventType::TouchDamage, &damageEvent });
			break;
		}
	}
}

void CollisionManager::checkSaberCollision(void)
{
	// 플레이어의 세이버 공격이 대상에게 닿았는지 판정

	// 세이버 vs 적
	for (auto& enemy : enemies->getEnemy())
	{
		RECT temp;

		if (IntersectRect(&temp, &player->getSaberRect(), &enemy->getEnemyHitBox()) && !enemy->getOverPower() && player->getCanHit()
			|| IntersectRect(&temp, &player->getSaberRect(), &enemy->getEnemysubHitBox()) && !enemy->getOverPower() && player->getCanHit())
		{
			player->setAnimDelay(true);

			switch (enemy->getEnemyType())
			{
			case EnemyType::MetaDridler:
				EFFECTMANAGER->spawnEffect(EffectType::BursterBlock, enemy->getPos().x, player->getPos().y, enemy->getWidth(), enemy->getHeight(), player->getLookRight());
				SOUNDMANAGER->play("SFX_Block", 0.5f);
				break;

			default:
				damageEvent.attacker = player;
				damageEvent.target = enemy;
				damageEvent.dType = DamageType::Saber;
				damageEvent.damage = damageEvent.attacker->getPhyscialDamage();
				EVENTMANAGER->dispatchEvents({ EventType::TouchDamage, &damageEvent });
				break;
			}
		}
	}

	for (auto& boss : enemies->getBoss())
	{
		RECT temp;

		if(IntersectRect(&temp, &player->getSaberRect(), &boss->getBossHitBox()) && player->getCanHit())
		{
			player->setAnimDelay(true);

			switch (boss->getBossType())
			{

			default:
				if (!boss->getOverPower())
				{
					damageEvent.attacker = player;
					damageEvent.target = boss;
					damageEvent.dType = DamageType::Saber;
					damageEvent.damage = damageEvent.attacker->getPhyscialDamage();
					EVENTMANAGER->dispatchEvents({ EventType::TouchDamage, &damageEvent });
				}

				else
				{
					EFFECTMANAGER->spawnEffect(EffectType::BursterBlock, boss->getPos().x,
						player->getPos().y, boss->getWidth(), boss->getHeight(), player->getLookRight());
					SOUNDMANAGER->play("SFX_Block", 0.3f);
				}
				break;
			}
		}
	}

	// 세이버 vs 오브젝트
	for (auto& object : objects->getObject())
	{
		RECT temp;

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

void CollisionManager::checkBulletCollision(void)
{
	// 모든 총알의 충돌을 체크
	// 총알을 발사한 주체가 누구인지에 따라서 따로 판정 체크

	RECT temp;

	for (auto& bullet : bullets->getBullet())
	{
		// 적이 플레이어 공격
		if (bullet->getBulletFaction() == BulletFaction::Enemy)
		{
			if (IntersectRect(&temp, &bullet->getBulletRect(), &player->getPlayerHitBox()) && !player->getOverPower() && !player->getIsDead())
			{
				damageEvent.target = player;
				damageEvent.bType = bullet->getBulletType();
				damageEvent.damage = bullet->getBulletDamage();
				damageEvent.bullet = bullet;
				EVENTMANAGER->dispatchEvents({ EventType::BulletDamage, &damageEvent });
				break;
			}
		}

		// 플레이어가 적을 공격
		else if (bullet->getBulletFaction() == BulletFaction::Player)
		{
			for (auto& enemy : enemies->getEnemy())
			{
				if (IntersectRect(&temp, &bullet->getBulletRect(), &enemy->getEnemyHitBox()) && !enemy->getOverPower() && !enemy->getIsDead()
					|| IntersectRect(&temp, &bullet->getBulletRect(), &enemy->getEnemysubHitBox()) && !enemy->getOverPower() && !enemy->getIsDead())
				{
					switch (enemy->getEnemyType())
					{
					case EnemyType::MetaDridler:
						EFFECTMANAGER->spawnEffect(EffectType::BursterBlock, bullet->getBulletPosX(), bullet->getBulletPosY(), bullet->getBulletWidth(), bullet->getBulletHeight(), bullet->getBulletDir());
						SOUNDMANAGER->play("SFX_Block", 0.5f);
						bullet->setBulletFire(false);
						break;
					default:
						damageEvent.target = enemy;
						damageEvent.bType = bullet->getBulletType();
						damageEvent.damage = bullet->getBulletDamage();
						damageEvent.bullet = bullet;
						EVENTMANAGER->dispatchEvents({ EventType::BulletDamage, &damageEvent });
						break;
					}
				}
			}

			for (auto& boss : enemies->getBoss())
			{
				if (IntersectRect(&temp, &bullet->getBulletRect(), &boss->getBossHitBox()) && !boss->getIsDead())
				{
					switch (boss->getBossType())
					{
					default:
						if (!boss->getOverPower())
						{
							damageEvent.target = boss;
							damageEvent.bType = bullet->getBulletType();
							damageEvent.damage = bullet->getBulletDamage();
							damageEvent.bullet = bullet;
							EVENTMANAGER->dispatchEvents({ EventType::BulletDamage, &damageEvent });
						}
						
						else
						{
							EFFECTMANAGER->spawnEffect(EffectType::BursterBlock, bullet->getBulletPosX(),
								bullet->getBulletPosY(), bullet->getBulletWidth(), bullet->getBulletHeight(), bullet->getBulletDir());
							SOUNDMANAGER->play("SFX_Block", 0.3f);
							bullet->setBulletFire(false);
						}
						break;
					}
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
}