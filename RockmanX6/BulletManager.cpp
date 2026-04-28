#include "Stdafx.h"
#include "BulletManager.h"
#include "Player.h"
#include "EffectManager.h"

void BulletManager::onEvent(const Event& event)
{
	switch (event.eType)
	{
	case EventType::ShootBulltet:
	{
		ShootEvent* shootData = static_cast<ShootEvent*>(event.data);
		fire(shootData->bType, shootData->x, shootData->y, shootData->direct, shootData->velocityX, shootData->velocityY);
	}
		break;
	case EventType::BulletErase:
	{
		DamageEvent* damageData = static_cast<DamageEvent*>(event.data); 
		// 일단 보류
	}
		break;
	}
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

	// if(_vEnemyBullet.size() > 0) checkPlayerCollision();
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
	
void BulletManager::fire(BulletType type, int x, int y, bool direct, float velocityX, float velocityY)
{
	/*
	switch (type)
	{
		// 버스터
		case BulletType::Burster:
			bullet = new Burster;
			bullet->init(type, x, y, direct);
			_vBullet.push_back(bullet);
			break;
		case BulletType::ChargeBurst1:
			bullet = new Burster;
			bullet->init(type, x, y, direct);
			_vBullet.push_back(bullet);
			break;
		case BulletType::ChargeBurst2:
			bullet = new Burster;
			bullet->init(type, x, y, direct);
			_vBullet.push_back(bullet);
			break;
		// 제로 버스터 추가 예정
		//case BulletType::ZeroBurster:
		//	break;
		
		// 적 총알
		case BulletType::JunkBullet:
			bullet = new JunkBullet;
			bullet->init(type, x, y, direct);
			_vEnemyBullet.push_back(bullet);
			break;
		case BulletType::SiegeShoot:
			bullet = new SiegeShoot;
			bullet->init(type, x, y, direct, velocityX, velocityY);
			_vEnemyBullet.push_back(bullet);
			break;
		defalut:
			break;
	}
	*/

	switch (type)
	{
		// 버스터
	case BulletType::Burster:
		bullet = new Burster;

		break;
	case BulletType::ChargeBurst1:
		bullet = new Burster;
		//bullet->init(type, x, y, direct, velocityX, velocityY);
		//_vBullet.push_back(bullet);
		break;
	case BulletType::ChargeBurst2:
		bullet = new Burster;
		// bullet->init(type, x, y, direct, velocityX, velocityY);
		//_vBullet.push_back(bullet);
		break;
		// 제로 버스터 추가 예정
		//case BulletType::ZeroBurster:
		//	break;

		// 적 총알
	case BulletType::JunkBullet:
		bullet = new JunkBullet;
		//bullet->init(type, x, y, direct);
		//_vEnemyBullet.push_back(bullet);
		break;
	case BulletType::SiegeShoot:
		bullet = new SiegeShoot;
		//bullet->init(type, x, y, direct, velocityX, velocityY);
		//_vEnemyBullet.push_back(bullet);
		break;
	defalut:
		break;
	}
	bullet->init(type, x, y, direct, velocityX, velocityY);
	_vBullet.push_back(bullet);
}

void BulletManager::checkPlayerCollision()
{
	for (auto enemyBullets = _vEnemyBullet.begin(); enemyBullets != _vEnemyBullet.end();)
	{
		RECT temp;
		if (IntersectRect(&temp, &(*enemyBullets)->getBulletRect(), &_player->getPlayerHitBox()) && _player->getOverPower() == false)
		{
			_player->reduceHp((*enemyBullets)->getBulletDamage());
			playExplodeEffect((*enemyBullets)->getBulletType(), (*enemyBullets)->getBulletPosX(), (*enemyBullets)->getBulletPosY(), (*enemyBullets)->getBulletWidth(), (*enemyBullets)->getBulletHeight(), 0);
			playExplodeSound((*enemyBullets)->getBulletType());
			enemyBullets = _vEnemyBullet.erase(enemyBullets);
		}

		else ++enemyBullets;
	}
}
