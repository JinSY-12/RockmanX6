#include "Stdafx.h"
#include "ObjectBase.h"
#include "BulletManager.h"
#include "Player.h"


void ObjectBase::release(void)
{
	// Do Nothing!
}

void ObjectBase::update(void)
{
	// Do Nothing!
}

void ObjectBase::render(HDC hdc)
{
	// Do Nothing!
}

void ObjectBase::setObjectHitbox(void)
{
	oStatus.oHitBox.left = oStatus.worldRect.left - CAMERAMANAGER->getCameraPos().x;
	oStatus.oHitBox.right = oStatus.worldRect.right - CAMERAMANAGER->getCameraPos().x;

	oStatus.oHitBox.top = oStatus.worldRect.top - CAMERAMANAGER->getCameraPos().y;
	oStatus.oHitBox.bottom = oStatus.worldRect.bottom - CAMERAMANAGER->getCameraPos().y;

	oStatus.subRect.left = oStatus.worldRect.left - CAMERAMANAGER->getCameraPos().x;
	oStatus.subRect.right = oStatus.worldRect.right - CAMERAMANAGER->getCameraPos().x;

	oStatus.subRect.top = oStatus.oHitBox.top;
	oStatus.subRect.bottom = oStatus.subRect.top + 3 * SCALE_FACTOR;
}

void ObjectBase::checkDead(void)
{
	if (status.hp <= 0)
	{
		status.hp = 0;
		status.dead = true;
	}
}

void ObjectBase::animOncePlay(bool play)
{
	if (!isUsed) animPlay = play;
	isUsed = play;
}
