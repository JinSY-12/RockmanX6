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
	for (_viEffect = _vEffect.begin(); _viEffect != _vEffect.end(); )
	{
		(*_viEffect).image->play(0.05f);
		
		if ((*_viEffect).image->getFrameX() >= (*_viEffect).image->getMaxFrameX())
		{
			_viEffect = _vEffect.erase(_viEffect);
		}
		
		else ++_viEffect;
	}

	moveFragments();
}

void EffectManager::render(HDC hdc)
{
	for (_viEffect = _vEffect.begin(); _viEffect != _vEffect.end(); _viEffect++)
	{
		(*_viEffect).image->frameRender(hdc, (*_viEffect).x - (*_viEffect).image->getFrameWidth() / 2 - CAMERAMANAGER->getCameraPos().x,
			(*_viEffect).y - (*_viEffect).image->getFrameHeight() - 10 * SCALE_FACTOR - CAMERAMANAGER->getCameraPos().y, (*_viEffect).image->getFrameX(), (*_viEffect).direct);
	}

	for (_viFragments = _vFragments.begin(); _viFragments != _vFragments.end(); _viFragments++)
	{
		(*_viFragments).image->alphaRender(hdc, (*_viFragments).x - CAMERAMANAGER->getCameraPos().x,
			(*_viFragments).y - CAMERAMANAGER->getCameraPos().y, 120);
	}

}

void EffectManager::spawnEffect(EffectType eType, int x, int y, bool direct)
{
	effect.x = x;
	effect.y = y;
	effect.direct = direct;

	switch (eType)
	{
	case EffectType::SmallEnemyBomb:
		effect.image = IMAGEMANAGER->findImage("SFX_Explosion")->cloneImage();
		_vEffect.push_back(effect);
		break;
	case EffectType::DashStartDust:
		break;
	case EffectType::DashEndDust:
		break;
	case EffectType::WallKick:
		break;
	case EffectType::WallSlide:
		break;
	}
}

void EffectManager::SpawnFragments(EnemyType type, int x, int y)
{
	switch (type)
	{
	case EnemyType::Junkroid:
		for (int i = 0 ; i < 4 ; i++)
		{
			effect.image = IMAGEMANAGER->findImage("Enemy_JunkroidFragment"+to_string(i+1))->cloneImage();
			// 랜덤 위치는 사이즈에 맞게 알아서 조절
			int randomX = RND->getFromIntTo(-20, 20);
			int randomY = RND->getFromIntTo(-20, 0);
			int randomDir = RND->getInt(2);
			effect.x = x + randomX * SCALE_FACTOR;
			effect.startX = effect.x;
			effect.y = y + randomY * SCALE_FACTOR;
			effect.startY = effect.y;
			effect.gravity = 2;
			effect.velocityY = -8 * SCALE_FACTOR;
			if (randomDir == 0) effect.direct = true;
			else effect.direct = false;
			_vFragments.push_back(effect);
		}
		break;
	}
}

void EffectManager::moveFragments()
{
	for (_viFragments = _vFragments.begin(); _viFragments != _vFragments.end(); )
	{
		if((*_viFragments).direct == 0) (*_viFragments).x += 3 * SCALE_FACTOR;
		else (*_viFragments).x -= 3 * SCALE_FACTOR;

		(*_viFragments).y += (*_viFragments).velocityY;
		(*_viFragments).velocityY += (*_viFragments).gravity;

		if ((*_viFragments).y >= (*_viFragments).startY + 80) _viFragments = _vFragments.erase(_viFragments);

		else ++_viFragments;
	}
}

