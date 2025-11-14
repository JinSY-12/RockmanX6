#include "Stdafx.h"
#include "EnemyBase.h"
#include "BulletManager.h"
#include "Player.h"

/*
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
*/

void EnemyBase::render(void)
{
	eStatus.eImage->frameRender(getMemDC(), eStatus.eHitBox.left, eStatus.eHitBox.top
		, eStatus.eImage->getFrameX(), eStatus.lookRight);

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
	if (eStatus.lookRight)
	{
		eStatus.attSight.left = eStatus.eHitBox.right;
		eStatus.attSight.right = eStatus.attSight.left + eStatus.sightWidth;
	}
	
	else
	{
		eStatus.attSight.left = eStatus.eHitBox.left - eStatus.sightWidth;
		eStatus.attSight.right = eStatus.attSight.left + eStatus.sightWidth;	
	}

	eStatus.attSight.top = (eStatus.eHitBox.top + eStatus.eHitBox.bottom - eStatus.sightHeight) / 2;
	eStatus.attSight.bottom = eStatus.attSight.top + eStatus.sightHeight;


	eStatus.eHitBox.left = eStatus.worldRect.left - CAMERAMANAGER->getCameraPos().x;
	eStatus.eHitBox.right = eStatus.worldRect.right - CAMERAMANAGER->getCameraPos().x;

	eStatus.eHitBox.top = eStatus.worldRect.top - CAMERAMANAGER->getCameraPos().y;
	eStatus.eHitBox.bottom = eStatus.worldRect.bottom - CAMERAMANAGER->getCameraPos().y;
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

void EnemyBase::checkPlayerCollision(void)
{
	// Do nothing!
	// 각자 타입에 맞는 공격을 하기 때문 각자 체크
}

void EnemyBase::checkPlayerAttCollision(void)
{
	// Do nothing!
	// 공격 타입에 따라 안맞는 공격도 있어서 각자 체크
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
