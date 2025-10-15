#include "Stdafx.h"
#include "BulletManager.h"

HRESULT BulletManager::init(void)
{
	return S_OK;
}

void BulletManager::release(void)
{
}

void BulletManager::update(void)
{
	for (_viBullet = _vBullet.begin(); _viBullet != _vBullet.end();)// ++_viBullet)
	{
		(*_viBullet)->update();

		if ((*_viBullet)->bStatus.isFire == false)
		{
			_viBullet = _vBullet.erase(_viBullet);
		}
			
		else ++_viBullet;
	}
}

void BulletManager::render(void)
{
	for (_viBullet = _vBullet.begin(); _viBullet != _vBullet.end(); ++_viBullet)
	{
		(*_viBullet)->render();
	}
}
	
void BulletManager::fire(BulletType type, int x, int y, bool direct)
{
	switch (type)
	{
		//버스터
		case BulletType::Buster:
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
		defalut:
			break;
	}
}