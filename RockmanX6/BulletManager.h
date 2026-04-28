#pragma once
#include "GameNode.h"
#include "Bullet.h"
#include "BulletType.h"
#include "IEventListener.h"

class Player;
class EnemeyManager;

class BulletManager : public IEventListener
{
private:
	Bullet* bullet;

	typedef vector<Bullet*> vBullet;
	typedef vector<Bullet*>::iterator viBullet;
	
	typedef vector<Bullet*> vEnemyBullet;
	typedef vector<Bullet*>::iterator viEnemyBullet;
	

private:
	vBullet _vBullet;
	viBullet _viBullet;
	
	vEnemyBullet _vEnemyBullet;
	viEnemyBullet _viEnemyBullet;
	
	Player* _player;

public:
	void onEvent(const Event& event) override;

	void update(void);
	void render(void);

	// 직선 공격
	void fire(BulletType type, int x, int y, bool direct, float velocityX, float velocityY);

	vector<Bullet*>& getBullet() { return _vBullet; }
	vector<Bullet*>& getEnemyBullet() { return _vEnemyBullet; }

	void checkPlayerCollision(void);

	inline int getMaxBullets() { return _vBullet.size(); }
	inline void settingPlayer(Player* player) { _player = player; }
	
	void playExplodeEffect(BulletType eBType, int x, int y, int width, int height, int look)
	{
		switch (eBType)
		{
		case BulletType::JunkBullet:
			EFFECTMANAGER->spawnEffect(EffectType::SmallEnemyBomb, x, y, width, height, look);

			break;
		}
	}

	void playExplodeSound(BulletType eBType)
	{
		switch (eBType)
		{
		case BulletType::JunkBullet:
			
			// SOUNDMANAGER.play("SFX_SmallExplosion", 0.5f);
			break;
		}
	}

	BulletManager() {}
	~BulletManager() {}
};