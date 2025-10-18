#include "Stdafx.h"
#include "EnemyManager.h"
#include "Player.h"
#include "Bullet.h"
#include "BulletManager.h"

HRESULT EnemyManager::init(void)
{

	return S_OK;
}

void EnemyManager::release(void)
{
}

void EnemyManager::update(void)
{
	for (_viEnemy = _vEnemy.begin(); _viEnemy != _vEnemy.end(); _viEnemy++)
	{
		(*_viEnemy)->update();
	}

	if (_vEnemy.size() > 0)
	{
		checkSightCollision();
		checkHitBoxCollision();
	}
}

void EnemyManager::render(void)
{
	for (_viEnemy = _vEnemy.begin(); _viEnemy != _vEnemy.end(); _viEnemy++)
	{
		(*_viEnemy)->render();
	}
}

void EnemyManager::checkAttackCollision(void)
{

}

void EnemyManager::checkHitBoxCollision(void)
{
	for (_viEnemy = _vEnemy.begin(); _viEnemy != _vEnemy.end(); )
	{
		vector<Bullet*>& bullets = _bManager->getBullet();

		for (auto it = bullets.begin(); it != bullets.end();)
		{
			RECT temp;
			if (IntersectRect(&temp, &(*it)->getBulletRect(), &(*_viEnemy)->getEnemyHitBox()) && !(*_viEnemy)->getOverPower())
			{
				(*_viEnemy)->reduceHp((*it)->getBulletDamage());
				it = bullets.erase(it);
			}
			else ++it;
		}
		
		if ((*_viEnemy)->getIsDead())
		{
			_viEnemy = _vEnemy.erase(_viEnemy);
		}

		else ++_viEnemy;
	}
}

void EnemyManager::checkSightCollision(void)
{
	for (_viEnemy = _vEnemy.begin(); _viEnemy != _vEnemy.end(); _viEnemy++)
	{
		RECT temp;
		if (IntersectRect(&temp , &(*_viEnemy)->getEnemySight(), &_player->getPlayerRect()) && (*_viEnemy)->getAttAble())
		{
			(*_viEnemy)->attack();
		}
	}
}

void EnemyManager::spawnEnemy(EnemyType eType, int x, int y)
{
	switch (eType)
	{
	case EnemyType::Junkroid:
		enemy = new Junkroid;
		enemy->init(x, y);
		enemy->settingBulletManager(_bManager);
		_vEnemy.push_back(enemy);
		break;
	}
}

void EnemyManager::spawnBoss(BossType bType, int x, int y)
{

}

