#include "Stdafx.h"
#include "EnemyBase.h"
#include "BulletManager.h"
#include "Player.h"

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
	// Do nothing!
}

void EnemyBase::update(void)
{
	// Do nothing!
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
	// Do nothing!
}

void EnemyBase::setEnemyHitbox(void)
{
	int dir = (eStatus.lookRight ? 1 : -1);

	eStatus.attSight.left = ePos.x - CAMERAMANAGER->getCameraPos().x;
	eStatus.attSight.right = ePos.x + dir * eStatus.sightWidth - CAMERAMANAGER->getCameraPos().x;;

	if (eStatus.attSight.left > eStatus.attSight.right)
		std::swap(eStatus.attSight.left, eStatus.attSight.right);

	eStatus.attSight.top = ePos.y - (eStatus.height + eStatus.sightHeight) / 2 - CAMERAMANAGER->getCameraPos().y;
	eStatus.attSight.bottom = ePos.y - (eStatus.height - eStatus.sightHeight) / 2 - CAMERAMANAGER->getCameraPos().y;

	eStatus.eHitBox.left = ePos.x - eStatus.width / 2 - CAMERAMANAGER->getCameraPos().x;
	eStatus.eHitBox.right = ePos.x + eStatus.width / 2 - CAMERAMANAGER->getCameraPos().x;

	eStatus.eHitBox.top = ePos.y - eStatus.height - CAMERAMANAGER->getCameraPos().y;
	eStatus.eHitBox.bottom = ePos.y - CAMERAMANAGER->getCameraPos().y;
}

void EnemyBase::pattern(void)
{
	if (eStatus.patternTimer >= eStatus.maxPatternTime)	eStatus.attackAble = true;
		
	else
	{
		eStatus.attackAble = false;
		eStatus.patternTimer += 0.1f;
	}
}

void EnemyBase::chasePlayer(float angle)
{
	// Do nothing!
}

void EnemyBase::isDead(void)
{
	if (eStatus.hp <= 0)
	{
		eStatus.hp = 0;
		eStatus.dead = true;
	}
}

void EnemyBase::changeDirection(void)
{
	float angle = atan2f((float)(player->getCharPos().y - ePos.y), (float)(player->getCharPos().x - ePos.x)) * 180 / PI;

	if (eState == EnemyState::Idle)
	{
		if (angle > -70.0f && angle < 70.0f) eStatus.lookRight = true;
		else if (angle > 110.0f || angle < -110.0f) eStatus.lookRight = false;
	}
}

void EnemyBase::chekcPlayerCollision(void)
{
	// Do nothing!
}

void EnemyBase::enemyInvincibleTimerUpdate(void)
{
	if (eStatus.invincibleTimer >= eStatus.invincibleMaxTime)
	{
		eStatus.invincibleTimer = 0.0f;
		eStatus.overpower = false;
	}

	else if (eStatus.overpower)
	{
		eStatus.invincibleTimer += 0.1f;
	}
}
