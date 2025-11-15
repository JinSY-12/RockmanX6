#include "Stdafx.h"
#include "ObjectBase.h"
#include "Player.h"

void ObjectBase::render(void)
{
	oStatus.oImage->render(getMemDC(), oStatus.oHitBox.left, oStatus.oHitBox.top);
	
	if (UIMANAGER->getIsDebugMode())
	{
		DrawRectMakeColor(getMemDC(), oStatus.oHitBox, RGB(255, 255, 0), 2);
	}
}

void ObjectBase::setObjectHitbox(void)
{
	oStatus.oHitBox.left = oStatus.worldRect.left - CAMERAMANAGER->getCameraPos().x;
	oStatus.oHitBox.right = oStatus.worldRect.right - CAMERAMANAGER->getCameraPos().x;

	oStatus.oHitBox.top = oStatus.worldRect.top - CAMERAMANAGER->getCameraPos().y;
	oStatus.oHitBox.bottom = oStatus.worldRect.bottom - CAMERAMANAGER->getCameraPos().y;
}

void ObjectBase::checkPlayerAttCollision(void)
{
	// Do Nothing!
}

void ObjectBase::checkWallCollision(void)
{
	
}

void ObjectBase::checkDead(void)
{
	if (oStatus.hp <= 0)
	{
		oStatus.hp = 0;
		oStatus.dead = true;

	}
}
