#pragma once
#include "IEventListener.h"
#include "Player.h"
#include "EnemyManager.h"
#include "ObjectManager.h"
#include "BulletManager.h"

class DamageManager : public IEventListener
{
	private:
		EnemyManager* eManager;
		BulletManager* bManager;
		
	public:
		void onEvent(const Event& event) override;
		void damageToEnemy(GameNode* target);
};


