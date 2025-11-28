#include "Stdafx.h"
#include "DamageManager.h"

void DamageManager::onEvent(const Event& event)
{
	DamageEvent* damage = static_cast<DamageEvent*>(event.data);

	switch (event.eType)
	{
	case EventType::Damage:
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

		else if (damage->target->getEntityType() == CombatEntityType::Player)
		{
			Player* player = static_cast<Player*>(damage->target);
			player->reduceHp(damage->damage);
		}
		break;
	}
}

void DamageManager::damageToEnemy(GameNode* target)
{
	
}
