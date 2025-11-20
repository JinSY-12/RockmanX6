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
	for (auto enemy = _vEnemy.begin(); enemy != _vEnemy.end(); ++enemy)
	{
		(*enemy)->update();
	}

	if (_vEnemy.size() > 0)
	{
		checkDead();
	}
}

void EnemyManager::render(void)
{
	for (auto enemy = _vEnemy.begin(); enemy != _vEnemy.end(); ++enemy)
	{
		(*enemy)->render();
	}
}

void EnemyManager::checkDead(void)
{
	for (auto enemy = _vEnemy.begin(); enemy != _vEnemy.end();)
	{
		if ((*enemy)->getIsDead())
		{
			playExplodeEffect((*enemy)->getEnemyType(), (*enemy)->getEnemyPos().x, (*enemy)->getEnemyPos().y, (*enemy)->getEnemyWidth(), (*enemy)->getEnemyHeight(), (*enemy)->getEnemyLook());
			playExplodeSound((*enemy)->getEnemyType());

			enemy = _vEnemy.erase(enemy);
		}

		else ++enemy;
	}
}
void EnemyManager::spawnEnemy(EnemyType eType, int x, int y)
{
	switch (eType)
	{
	case EnemyType::Junkroid:
		_enemy = new Junkroid;
		_enemy->init(x, y);
		_enemy->settingBulletManager(_bManager);
		_enemy->settingPlayer(_player);
		_vEnemy.push_back(_enemy);
		break;
	}
}

void EnemyManager::spawnBoss(BossType bType, int x, int y)
{

}

void EnemyManager::playExplodeEffect(EnemyType eType, int x, int y, int width, int height, int look)
{
	switch (eType)
	{
	case EnemyType::Junkroid:
		EFFECTMANAGER->spawnEffect(EffectType::SmallEnemyBomb, x, y, width, height, look);
		EFFECTMANAGER->SpawnFragments(eType, x, y);
		break;
	}
}

void EnemyManager::playExplodeSound(EnemyType eType)
{
	switch(eType)
	{
	case EnemyType::Junkroid:
		SOUNDMANAGER->play("SFX_SmallExplosion", 0.5f);
		break;
	}
}

void EnemyManager::playHitEffect(BulletType bType, int x, int y, int look)
{
	switch (bType)
	{
	case BulletType::Buster:
		break;
	}
}

void EnemyManager::playHitSound(BulletType bType)
{
	switch (bType)
	{
	case BulletType::Buster:
		SOUNDMANAGER->play("SFX_X_Burster1Hit", 0.5f);
		break;
	}
}

