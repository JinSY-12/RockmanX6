#include "Stdafx.h"
#include "BulletManager.h"
#include "Player.h"
#include "EffectManager.h"

HRESULT BulletManager::init(void)
{
	return S_OK;
}

void BulletManager::release(void)
{
}

void BulletManager::update(void)
{
	for (auto bullets = _vBullet.begin(); bullets != _vBullet.end();)
	{
		(*bullets)->update();

		if ((*bullets)->bStatus.isFire == false)
		{
			bullets = _vBullet.erase(bullets);
		}
			
		else ++bullets;
	}

	for (auto enemyBullets = _vEnemyBullet.begin(); enemyBullets != _vEnemyBullet.end();)
	{
		(*enemyBullets)->update();

		if ((*enemyBullets)->bStatus.isFire == false)
		{
			enemyBullets = _vEnemyBullet.erase(enemyBullets);
		}

		else ++enemyBullets;
	}

	if(_vEnemyBullet.size() > 0) checkPlayerCollision();
}

void BulletManager::render(void)
{
	for (auto bullets = _vBullet.begin(); bullets != _vBullet.end();++bullets)
	{
		(*bullets)->render();
	}

	for (auto enemyBullets = _vEnemyBullet.begin(); enemyBullets != _vEnemyBullet.end();++enemyBullets)
	{
		(*enemyBullets)->render();
	}
}
	
void BulletManager::fire(BulletType pType, int x, int y, bool direct)
{
	switch (pType)
	{
		//버스터
		case BulletType::Buster:
			bullet = new Burster;
			bullet->init(pType, x, y, direct);
			_vBullet.push_back(bullet);
			break;
		case BulletType::ChargeBurst1:
			bullet = new Burster;
			bullet->init(pType, x, y, direct);
			_vBullet.push_back(bullet);
			break;
		case BulletType::ChargeBurst2:
			bullet = new Burster;
			bullet->init(pType, x, y, direct);
			_vBullet.push_back(bullet);
			break;
		// 제로 버스터 추가 예정
		defalut:
			break;
	}
}

void BulletManager::fire(EnemyBulletType eType, int x, int y, bool direct)
{
	switch (eType)
	{
		
	case EnemyBulletType::JunkBullet:
		bullet = new JunkBullet;
		bullet->init(eType, x, y, direct);
		_vEnemyBullet.push_back(bullet);
		break;
	
	defalut:
		break;
	}
}

void BulletManager::checkPlayerCollision()
{
	for (auto enemyBullets = _vEnemyBullet.begin(); enemyBullets != _vEnemyBullet.end();)
	{
		RECT temp;
		if (IntersectRect(&temp, &(*enemyBullets)->getBulletRect(), &_player->getPlayerRect()) && _player->getOverPower() == false)
		{
			_player->reduceHp((*enemyBullets)->getBulletDamage(), (*enemyBullets)->getBulletSize());
			playExplodeEffect((*enemyBullets)->getEnemyBulletType(), (*enemyBullets)->getBulletPosX(), (*enemyBullets)->getBulletPosY(), (*enemyBullets)->getBulletWidth(), (*enemyBullets)->getBulletHeight(), 0);
			playExplodeSound((*enemyBullets)->getEnemyBulletType());
			enemyBullets = _vEnemyBullet.erase(enemyBullets);
		}

		else ++enemyBullets;
	}
}
