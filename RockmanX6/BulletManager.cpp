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
	for (_viBullet = _vBullet.begin(); _viBullet != _vBullet.end();)
	{
		(*_viBullet)->update();

		if ((*_viBullet)->bStatus.isFire == false)
		{
			_viBullet = _vBullet.erase(_viBullet);
		}
			
		else ++_viBullet;
	}

	for (_viEnemyBullet = _vEnemyBullet.begin(); _viEnemyBullet != _vEnemyBullet.end();)
	{
		(*_viEnemyBullet)->update();

		if ((*_viEnemyBullet)->bStatus.isFire == false)
		{
			_viEnemyBullet = _vEnemyBullet.erase(_viEnemyBullet);
		}

		else ++_viEnemyBullet;
	}

	if(_vEnemyBullet.size() > 0) checkPlayerCollision();
}

void BulletManager::render(void)
{
	for (_viBullet = _vBullet.begin(); _viBullet != _vBullet.end(); ++_viBullet)
	{
		(*_viBullet)->render();
	}
	
	for (_viEnemyBullet = _vEnemyBullet.begin(); _viEnemyBullet != _vEnemyBullet.end(); ++_viEnemyBullet)
	{
		(*_viEnemyBullet)->render();
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
	for (_viEnemyBullet = _vEnemyBullet.begin(); _viEnemyBullet != _vEnemyBullet.end();)
	{
		RECT temp;
		if (IntersectRect(&temp, &(*_viEnemyBullet)->getBulletRect(), &_player->getPlayerRect()) && _player->getOverPower() == false)
		{
			_player->reduceHp((*_viEnemyBullet)->getBulletDamage(), (*_viEnemyBullet)->getBulletSize());
			playExplodeEffect((*_viEnemyBullet)->getEnemyBulletType(), (*_viEnemyBullet)->getBulletPosX(), (*_viEnemyBullet)->getBulletPosY(), 0);
			playExplodeSound((*_viEnemyBullet)->getEnemyBulletType());
			_viEnemyBullet = _vEnemyBullet.erase(_viEnemyBullet);
		}

		else ++_viEnemyBullet;
	}
}
