#include "Stdafx.h"
#include "EffectManager.h"

HRESULT EffectManager::init(void)
{
	return S_OK;
}

void EffectManager::release(void)
{
}

void EffectManager::update(void)
{
}

void EffectManager::render(HDC hdc)
{
}

void EffectManager::spawnEffect()
{
	/*
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
	*/
}
