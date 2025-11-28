#pragma once
#include "GameNode.h"
#include "IEventListener.h"
#include "Event.h"

class Player;
class EnemyManager;
class BulletManager;
class ObjectManager;

class CollisionManager : public GameNode
{
private:
	Player* player;
	EnemyManager* ememies;
	BulletManager* bullets;
	ObjectManager* objects;

	DamageEvent damageEvent;

public:
	HRESULT init(void) override;
	void release(void) override;
	void update(void) override;
	void render(void) override;

	void checkCollisions(void);

	void checkPlayerVsEnemy(void);
	void checkPlayerVsObject(void);
	void checkPlayerVsBullet(void);

	void checkBulletVsEnemy(void);
	void checkBulletVsObject(void);
	
	void checkEnemyVsBullet(void);

	void settingManager(Player* pl, EnemyManager* enemy, BulletManager* bullet, ObjectManager* object)
	{
		player = pl;
		ememies = enemy;
		bullets = bullet;
		objects = object;
	};
};

