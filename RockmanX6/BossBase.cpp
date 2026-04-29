#include "Stdafx.h"
#include "BossBase.h"
#include "BulletManager.h"
#include "Player.h"

void BossBase::bossAppearance()
{
	// Do Nothing!!
}

void BossBase::render(HDC hdc)
{
	bStatus.bImage->frameRender(hdc, bStatus.bHitBox.left, bStatus.bHitBox.top 
		, bStatus.bImage->getFrameX(), status.lookRight);

	if (UIMANAGER->getIsDebugMode())
	{
		DrawRectMakeColor(hdc, bStatus.bHitBox, RGB(0, 255, 255), 2);
	}
}

void BossBase::setBossHitbox(void)
{
	bStatus.bHitBox.left = bStatus.bWorldRect.left - CAMERAMANAGER->getCameraPos().x;
	bStatus.bHitBox.right = bStatus.bWorldRect.right - CAMERAMANAGER->getCameraPos().x;

	bStatus.bHitBox.top = bStatus.bWorldRect.top - CAMERAMANAGER->getCameraPos().y;
	bStatus.bHitBox.bottom = bStatus.bWorldRect.bottom - CAMERAMANAGER->getCameraPos().y;
}

ShootEvent BossBase::makeShootEvent(BulletType bType)
{
	// Do Nothing!!
	return ShootEvent();
}

Vector2 BossBase::getDiffPlayer(int firePointX, int firePointY)
{
	diff = { static_cast<float>(player->getCenterX() - firePointX),
						 static_cast<float>(player->getCenterY() - firePointY) };

	return diff;
}