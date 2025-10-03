#pragma once
#include "GameNode.h"
#include "Bullet.h"

class X;
class Zero;
class EnemeyManager;

class BulletManager : public GameNode
{
private:
	Bullet* bullet;

	typedef vector<Bullet*> vBullet;
	typedef vector<Bullet*>::iterator viBullet;
	/*
	typedef vector<Bullet*> vEnemyBullet;
	typedef vector<Bullet*>::iterator viEnemyBullet;
	*/

private:
	vBullet _vBullet;
	viBullet _viBullet;
	/*
	vEnemyBullet _vEnemyBullet;
	viEnemyBullet _viEnemyBullet;
	*/

public:
	HRESULT init(void);
	void release(void);
	void update(void);
	void render(void);

	void fire(int type, int x, int y, bool direct);

	inline int getMaxBullets() { return _vBullet.size(); }

	BulletManager() {}
	~BulletManager() {}
};