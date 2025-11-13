#include "Stdafx.h"
#include "ObjectBase.h"
#include "Player.h"

void ObjectBase::render(void)
{
	oStatus.oImage->render(getMemDC(), oPos.x - oStatus.oImage->getWidth() / 2 - CAMERAMANAGER->getCameraPos().x,
		oPos.y - oStatus.oImage->getHeight() - CAMERAMANAGER->getCameraPos().y);
	
	if (UIMANAGER->getIsDebugMode())
	{
		DrawRectMakeColor(getMemDC(), oStatus.oHitBox, RGB(0, 255, 255), 2);
	}
}

void ObjectBase::setObjectHitbox(void)
{
	oStatus.oHitBox.left = oPos.x - oStatus.width / 2 - CAMERAMANAGER->getCameraPos().x;
	oStatus.oHitBox.right = oPos.x + oStatus.width / 2 - CAMERAMANAGER->getCameraPos().x;

	oStatus.oHitBox.top = oPos.y - oStatus.height - CAMERAMANAGER->getCameraPos().y;
	oStatus.oHitBox.bottom = oPos.y - CAMERAMANAGER->getCameraPos().y;
}

void ObjectBase::checkPlayerAttCollision(void)
{
	// Do Nothing!
}

void ObjectBase::checkDead(void)
{
	if (oStatus.hp <= 0)
	{
		oStatus.hp = 0;
		oStatus.dead = true;
	}
}
