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
	void release(void) override;
	void update(void) override;

	void checkCollisions(void);
	void checkContact(void);
	void checkSaberCollision(void);
	void checkBulletCollision(void);

	void settingManager(Player* pl, EnemyManager* enemy, BulletManager* bullet, ObjectManager* object)
	{
		player = pl;
		ememies = enemy;
		bullets = bullet;
		objects = object;
	};
};

