#include "Stdafx.h"
#include "DamageManager.h"

void DamageManager::onEvent(const Event& event)
{
	DamageEvent* damage = static_cast<DamageEvent*>(event.data);

	switch (event.eType)
	{
	case EventType::Damage:
		damage->target->reduceHp(damage->damage);
		if (damage->bType == BulletType::None)
		{
			int rnd = RND->getInt(4);
			SOUNDMANAGER->play("SFX_SaberHit", 0.5f);

			switch (rnd)
			{
			case 0:
				EFFECTMANAGER->spawnEffect(EffectType::SaberHit_1, damage->target->getPos().x, damage->target->getPos().y, damage->target->getWidth(), damage->target->getHeight(), damage->target->getLookRight());
				break;
			case 1:
				EFFECTMANAGER->spawnEffect(EffectType::SaberHit_2, damage->target->getPos().x, damage->target->getPos().y, damage->target->getWidth(), damage->target->getHeight(), damage->target->getLookRight());
				break;
			case 2:
				EFFECTMANAGER->spawnEffect(EffectType::SaberHit_3, damage->target->getPos().x, damage->target->getPos().y, damage->target->getWidth(), damage->target->getHeight(), damage->target->getLookRight());
				break;
			case 3:
				EFFECTMANAGER->spawnEffect(EffectType::SaberHit_4, damage->target->getPos().x, damage->target->getPos().y, damage->target->getWidth(), damage->target->getHeight(), damage->target->getLookRight());
				break;
			}
		}
		break;
	}
}

void DamageManager::damageToEnemy(GameNode* target)
{
	
}
