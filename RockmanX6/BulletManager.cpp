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
	
void BulletManager::fire(int type, int x, int y, bool direct)
{
	switch (type)
	{
		//버스터
		case 0:
			bullet = new Burster;
			bullet->init(0, x, y, direct);
			_vBullet.push_back(bullet);
			break;
		case 1:
			bullet = new Burster;
			bullet->init(1, x, y, direct);
			_vBullet.push_back(bullet);
			break;
		case 2:
			bullet = new Burster;
			bullet->init(2, x, y, direct);
			_vBullet.push_back(bullet);
			break;
		// 제로 버스터
		case 3:
			// bullet = new ZeroBurster;
			break;
		case 4:
			break;
	}
}