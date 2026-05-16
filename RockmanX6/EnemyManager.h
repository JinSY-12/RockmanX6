#pragma once
#include "GameNode.h"
#include "CombatEntity.h"
#include "BossType.h"
#include "EnemyBase.h"
#include "BossBase.h"
#include "HighMax.h"
#include "Junkroid.h"
#include "MetaWheel.h"
#include "MetaDridler.h"
#include "EffectType.h"

class Player;
class Bullet;
class BulletManager;

class EnemyManager : public GameNode
{
private:
	
	typedef vector<EnemyBase*> vEnemyBase;
	typedef vector<EnemyBase*>::iterator viEnemyBase;

	typedef vector<BossBase*> vBossBase;
	typedef vector<BossBase*>::iterator viBossBase;

private:
	vEnemyBase _vEnemy;
	viEnemyBase _viEnemy;

	vBossBase _vBoss;
	viBossBase _viBoss;
	
	Player* _player;
	EnemyBase* _enemy;
	BossBase* _boss;
	BulletManager* _bManager;

	string Map;

	bool worldStop;
	bool soundOnce;

public:
	HRESULT init(void);
	void release(void);
	void update(void);
	void render(void);
	
	void checkDead(void);

	void spawnEnemy(EnemyType eType, int x, int y, int sub = 0);
	void spawnBoss(BossType bType, int x, int y);
	void enemyReset();

	void setttingPlayer(Player* player) { _player = player; }
	void setttingBulletManager(BulletManager* bManager) { _bManager = bManager; }
	
	void playExplodeEffect(EnemyType eType, int x, int y, int width, int height, int look);
	void playExplodeSound(EnemyType eType);

	void playHitEffect(BulletType bType, int x, int y, int look);
	void playHitSound(BulletType bType);

	vector<EnemyBase*>& getEnemy() { return _vEnemy; }
	vector<BossBase*>& getBoss() { return _vBoss; }

	EnemyManager() {}
	~EnemyManager() {}
};
