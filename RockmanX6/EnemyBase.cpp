#include "Stdafx.h"
#include "EnemyBase.h"
#include "BulletManager.h"

HRESULT EnemyBase::init(void)
{
	return S_OK;
}

HRESULT EnemyBase::init(int x, int y)
{
	return S_OK;
}

void EnemyBase::release(void)
{

}

void EnemyBase::update(void)
{

}

void EnemyBase::render(void)
{
	eStatus.eImage->frameRender(getMemDC(), ePos.x - eStatus.eImage->getFrameWidth() / 2 - CAMERAMANAGER->getCameraPos().x, ePos.y - eStatus.eImage->getFrameHeight() - CAMERAMANAGER->getCameraPos().y, eStatus.eImage->getFrameX(), eStatus.lookRight);

	if (UIMANAGER->getIsDebugMode())
	{
		DrawRectMakeColor(getMemDC(), eStatus.eHitBox, RGB(0, 255, 255), 2);
		DrawRectMakeColor(getMemDC(), eStatus.attSight, RGB(255, 0, 255), 2);
	}
}

void EnemyBase::attack(void)
{

}

void EnemyBase::setEnemyHitbox(void)
{
	eStatus.eHitBox.left = ePos.x - eStatus.width / 2 - CAMERAMANAGER->getCameraPos().x;
	eStatus.eHitBox.right = ePos.x + eStatus.width / 2 - CAMERAMANAGER->getCameraPos().x;
	
	eStatus.eHitBox.top = ePos.y - eStatus.height - CAMERAMANAGER->getCameraPos().y;
	eStatus.eHitBox.bottom = ePos.y - CAMERAMANAGER->getCameraPos().y;

	int dir;
	if (eStatus.lookRight) dir = -1;
	else dir = 1;
	
	eStatus.attSight.left = ePos.x - dir * eStatus.sightWidth - CAMERAMANAGER->getCameraPos().x;
	eStatus.attSight.right = ePos.x - CAMERAMANAGER->getCameraPos().x;
	
	eStatus.attSight.top = ePos.y - (eStatus.height + eStatus.sightHeight)/2 - CAMERAMANAGER->getCameraPos().y;
	eStatus.attSight.bottom = ePos.y - (eStatus.height - eStatus.sightHeight)/2 - CAMERAMANAGER->getCameraPos().y;
}

void EnemyBase::pattern(void)
{
	if (!eStatus.isAtt)
	{
		patternTimer += 0.1f;

		if (patternTimer >= maxPatternTime)	attackAble = true;
	}
}

void EnemyBase::isDead(void)
{
	if (eStatus.hp <= 0)
	{
		eStatus.hp = 0;
		eStatus.dead = true;
	}
}
