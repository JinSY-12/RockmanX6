#pragma once
#include "GameNode.h"
//#include "EnemyBase.h"
//#include "BossBase.h"

//class Link;
//class Knight;

class EnemyManager : public GameNode
{
private:
	/*
	typedef vector<EnemyBase*> vEnemyBase;
	typedef vector<EnemyBase*>::iterator viEnemyBase;

	typedef vector<BossBase*> vBossBase;
	typedef vector<BossBase*>::iterator viBossBase;
	*/
private:
	/*
	vEnemyBase _vEnemy;
	viEnemyBase _viEnemy;

	vBossBase _vBoss;
	viBossBase _viBoss;
	*/
	//Link* _eLink;

	string Map;

	bool worldStop;
	bool soundOnce;

public:
	HRESULT init(void);
	void release(void);
	void update(void);
	void render(void);

	void damageCollision();
	void attackCollision();
	void chaseCollision();
	void randomItemDrop();

	void bossAttCollision();
	void bossaDamageCollision();

	void efindPixel(void);
	void getMapname(string mapname) { Map = mapname; }

	//void setLinkMemoryAddress(Link* link) { _eLink = link; }

	void spawnBoss(int type, int x, int y);
	void spawnEnemy(int type, int x, int y);
	void setWorldStop(bool stop) { worldStop = stop; }

	EnemyManager() {}
	~EnemyManager() {}
};


/*

*/