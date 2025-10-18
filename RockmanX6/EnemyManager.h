#pragma once
#include "GameNode.h"
#include "BossType.h"
#include "EnemyBase.h"
#include "Junkroid.h"

class Player;
class Bullet;
class BulletManager;

class EnemyManager : public GameNode
{
private:
	
	typedef vector<EnemyBase*> vEnemyBase;
	typedef vector<EnemyBase*>::iterator viEnemyBase;

	// typedef vector<BossBase*> vBossBase;
	// typedef vector<BossBase*>::iterator viBossBase;

private:
	
	vEnemyBase _vEnemy;
	viEnemyBase _viEnemy;

	// vBossBase _vBoss;
	// viBossBase _viBoss;
	
	Player* _player;
	EnemyBase* enemy;
	BulletManager* _bManager;

	string Map;

	bool worldStop;
	bool soundOnce;

public:
	HRESULT init(void);
	void release(void);
	void update(void);
	void render(void);
	
	void checkAttackCollision(void);
	void checkHitBoxCollision(void);
	void checkSightCollision(void);

	void spawnEnemy(EnemyType eType, int x, int y);
	void spawnBoss(BossType bType, int x, int y);

	void setttingPlayer(Player* player) { _player = player; }
	void setttingBulletManager(BulletManager* bManager) { _bManager = bManager; }
	
	EnemyManager() {}
	~EnemyManager() {}
};
