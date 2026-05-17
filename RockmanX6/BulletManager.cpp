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
}

void BulletManager::render(void)
{
	for (auto bullets = _vBullet.begin(); bullets != _vBullet.end(); ++bullets)
	{
		(*bullets)->render();
	}
}
	
void BulletManager::fire(BulletType type, int x, int y, bool direct, float velocityX, float velocityY)
{
	switch (type)
	{
	// X 버스터
	case BulletType::Burster:
		bullet = new Burster;
		break;
	case BulletType::ChargeBurst1:
		bullet = new Burster;
		break;
	case BulletType::ChargeBurst2:
		bullet = new Burster;
		break;
	case BulletType::DeathBubble:
		bullet = new DeathBubble;
		break;
	case BulletType::DeathRing:
		bullet = new DeathRing;
		break;
	// 적 총알
	case BulletType::JunkBullet:
		bullet = new JunkBullet;
		break;
	case BulletType::SiegeShoot:
		bullet = new SiegeShoot;
		break;
	case BulletType::DeathBall:
		bullet = new DeathBall;
		break;
	
	default:
		break;
	}
	bullet->init(type, x, y, direct, velocityX, velocityY);
	_vBullet.push_back(bullet);
}