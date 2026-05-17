#include "Stdafx.h"
#include "DamageManager.h"

void DamageManager::onEvent(const Event& event)
{
	DamageEvent* damage = static_cast<DamageEvent*>(event.data);

	switch (event.eType)
	{
		// 물리 근접 공격
		case EventType::TouchDamage:
			// Player To Enemy
			if (damage->target->getEntityType() == CombatEntityType::Enemy)
			{
				EnemyBase* enemy = static_cast<EnemyBase*>(damage->target);
				enemy->reduceHp(damage->damage);
	
				if (damage->dType == DamageType::Saber)
				{
					int rnd = RND->getInt(4);
					SOUNDMANAGER->play("SFX_SaberHit", 0.5f);
					static const EffectType saberEffects[4] = {
						EffectType::SaberHit_1, EffectType::SaberHit_2, EffectType::SaberHit_3,	EffectType::SaberHit_4 };
					
					EFFECTMANAGER->spawnEffect(saberEffects[rnd], damage->target->getPos().x, damage->target->getPos().y, damage->target->getWidth(), damage->target->getHeight(), damage->target->getLookRight());					
				}
			}

			// player To Boss
			else if (damage->target->getEntityType() == CombatEntityType::Boss)
			{
				BossBase* boss = static_cast<BossBase*>(damage->target);
				boss->reduceHp(damage->damage);

				if (damage->dType == DamageType::Saber)
				{
					int rnd = RND->getInt(4);
					SOUNDMANAGER->play("SFX_SaberHit", 0.5f);
					static const EffectType saberEffects[4] = {
						EffectType::SaberHit_1, EffectType::SaberHit_2, EffectType::SaberHit_3,	EffectType::SaberHit_4 };

					EFFECTMANAGER->spawnEffect(saberEffects[rnd], damage->target->getPos().x, damage->target->getPos().y, damage->target->getWidth(), damage->target->getHeight(), damage->target->getLookRight());
				}
			}

			// player To Object
			else if (damage->target->getEntityType() == CombatEntityType::Object)
			{
				ObjectBase* object = static_cast<ObjectBase*>(damage->target);
				object->reduceHp(damage->damage);

				if (damage->dType == DamageType::Saber)
				{
					int rnd = RND->getInt(4);
					SOUNDMANAGER->play("SFX_SaberHit", 0.5f);

					static const EffectType saberEffects[4] = {
						EffectType::SaberHit_1, EffectType::SaberHit_2, EffectType::SaberHit_3,	EffectType::SaberHit_4 };

					EFFECTMANAGER->spawnEffect(saberEffects[rnd], damage->target->getPos().x, damage->target->getPos().y, damage->target->getWidth(), damage->target->getHeight(), damage->target->getLookRight());
				}
			}

			// Enemy To Player
			else if (damage->target->getEntityType() == CombatEntityType::Player)
			{
				Player* player = static_cast<Player*>(damage->target);
				player->reduceHp(damage->damage);
			}
			break;

		// 총알 공격 - Player Bullet vs Enemy / Enemy Bullet vs Player
		case EventType::BulletDamage:
			// Player의 공격
			Bullet* bullet = static_cast<Bullet*>(damage->bullet);
			int offset;

			// 플레이어가 적을 공격
			if (damage->target->getEntityType() == CombatEntityType::Enemy)
			{
				EnemyBase* enemy = static_cast<EnemyBase*>(damage->target);
				switch (enemy->getEnemyType())
				{
				case EnemyType::MetaDridler:
					bullet->setBulletFire(false);
					offset = bullet->getBulletDir() ? damage->target->getWidth() / 2 : -(damage->target->getWidth() / 2);
					EFFECTMANAGER->spawnEffect(EffectType::BursterBlock, damage->target->getPos().x - offset, bullet->getBulletPosY(), damage->target->getWidth(), bullet->getBulletHeight(), bullet->getBulletDir());
					break;

				default:
					enemy->reduceHp(damage->damage);
					SOUNDMANAGER->play("SFX_X_Burster1Hit", 0.5f);

					offset = bullet->getBulletDir() ? damage->target->getWidth() / 2 : -(damage->target->getWidth() / 2);

					if (bullet->getBulletType() == BulletType::ChargeBurst2 || bullet->getBulletType() == BulletType::FalconBurst2)
					{
						EFFECTMANAGER->spawnEffect(EffectType::BursterHit_2, damage->target->getPos().x - offset, bullet->getBulletPosY(), damage->target->getWidth(), bullet->getBulletHeight(), bullet->getBulletDir());
						if (enemy->getCurrentHp() > 0) bullet->setBulletFire(false);
					}
					else
					{
						EFFECTMANAGER->spawnEffect(EffectType::BursterHit_1, damage->target->getPos().x - offset, bullet->getBulletPosY(), damage->target->getWidth(), bullet->getBulletHeight(), bullet->getBulletDir());
						bullet->setBulletFire(false);
					}
					break;
				}
			}

			else if (damage->target->getEntityType() == CombatEntityType::Boss)
			{
				BossBase* boss = static_cast<BossBase*>(damage->target);

				boss->reduceHp(damage->damage);
				SOUNDMANAGER->play("SFX_X_Burster1Hit", 0.5f);

				offset = bullet->getBulletDir() ? damage->target->getWidth() / 2 : -(damage->target->getWidth() / 2);

				if (bullet->getBulletType() == BulletType::ChargeBurst2 || bullet->getBulletType() == BulletType::FalconBurst2)
				{
					EFFECTMANAGER->spawnEffect(EffectType::BursterHit_2, damage->target->getPos().x - offset, bullet->getBulletPosY(), damage->target->getWidth(), bullet->getBulletHeight(), bullet->getBulletDir());
					if (boss->getCurrentHp() > 0) bullet->setBulletFire(false);
				}

				else
				{
					EFFECTMANAGER->spawnEffect(EffectType::BursterHit_1, damage->target->getPos().x - offset, bullet->getBulletPosY(), damage->target->getWidth(), bullet->getBulletHeight(), bullet->getBulletDir());
					bullet->setBulletFire(false);
				}
			}

			// 적이 플레이어를 공격
			else if (damage->target->getEntityType() == CombatEntityType::Player)
			{
				
				Player* player = static_cast<Player*>(damage->target);
				player->reduceHp(damage->damage);
				
				int offset;
				switch (bullet->getBulletType())
				{
					
					case BulletType::JunkBullet:
						offset = bullet->getBulletDir() ? damage->target->getWidth() / 2 : -(damage->target->getWidth() / 2);
						EFFECTMANAGER->spawnEffect(EffectType::SmallEnemyBomb, damage->target->getPos().x - offset, bullet->getBulletPosY(), damage->target->getWidth(), bullet->getBulletHeight(), bullet->getBulletDir());
						SOUNDMANAGER->play("SFX_SmallExplosion", 0.5f);
						bullet->setBulletFire(false);
						break;
						
					default:
						break;
				}
			
			}

			else if (damage->target->getEntityType() == CombatEntityType::Object)
			{
				ObjectBase* obejct = static_cast<ObjectBase*>(damage->target);
				int offset;
				switch (obejct->getObjectType())
				{
				case ObjectType::Block:
					offset = bullet->getBulletDir() ? damage->target->getWidth() / 2 : -(damage->target->getWidth() / 2);
					EFFECTMANAGER->spawnEffect(EffectType::BursterBlock, damage->target->getPos().x - offset, bullet->getBulletPosY(), damage->target->getWidth(), bullet->getBulletHeight(), bullet->getBulletDir());

					SOUNDMANAGER->play("SFX_Block", 0.3f);
					bullet->setBulletFire(false);
					break;

				case ObjectType::BossGate:
					offset = bullet->getBulletDir() ? damage->target->getWidth() / 2 : -(damage->target->getWidth() / 2);
					if (bullet->getBulletType() != BulletType::DeathBall) {

						EFFECTMANAGER->spawnEffect(EffectType::BursterHit_1, damage->target->getPos().x - offset, bullet->getBulletPosY(), damage->target->getWidth(), bullet->getBulletHeight(), bullet->getBulletDir());
						SOUNDMANAGER->play("SFX_X_Burster1Hit", 0.3f);
						bullet->setBulletFire(false);
					}					
					break;

				default:
					break;
				}
			}
			break;

			// Enemy의 공격

	}
}

void DamageManager::damageToEnemy(GameNode* target)
{
	
}
