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
	bStatus.bImage->frameRender(hdc, bStatus.bHitBox.left - bStatus.offsetX, bStatus.bHitBox.top - bStatus.offsetY
		, bStatus.bImage->getFrameX(), status.lookRight);

	if (bStatus.effectImage != nullptr && bStatus.effectOn == true)
	{
		bStatus.effectImage->frameRender(hdc, bStatus.bHitBox.left - bStatus.effectOffsetX, bStatus.bHitBox.top - bStatus.effectOffsetY,
			bStatus.effectImage->getFrameX(), status.lookRight);
	}

	if (UIMANAGER->getIsDebugMode())
	{
		DrawRectMakeColor(hdc, bStatus.bHitBox, RGB(0, 255, 255), 2);

		string temp;
		if (bState == BossState::Idle) temp = "Idle";
		else if (bState == BossState::AttReady) temp = "AttReady";
		else if (bState == BossState::RightReady) temp = "RightReady";
		else if (bState == BossState::RightAtt) temp = "RightAtt";
		else if (bState == BossState::LeftReady) temp = "LeftReady";
		else if (bState == BossState::LeftAtt) temp = "LeftAtt";
		
		TEXTMANAGER->drawTextColor(hdc, WINSIZE_X / 50, 0.85 * WINSIZE_Y, "현재 상태", "DNF_M_18", RGB(0, 255, 255));
		TEXTMANAGER->drawTextColor(hdc, WINSIZE_X / 50, 0.85 * WINSIZE_Y + 20, temp, "DNF_M_18", RGB(0, 255, 255));

		TEXTMANAGER->drawTextColor(hdc, WINSIZE_X / 50, 0.85 * WINSIZE_Y + 50, "보스 X", "DNF_M_18", RGB(0, 255, 255));
		TEXTMANAGER->drawTextColor(hdc, WINSIZE_X / 50, 0.85 * WINSIZE_Y + 70, to_string(pos.x), "DNF_M_18", RGB(0, 255, 255));

		TEXTMANAGER->drawTextColor(hdc, WINSIZE_X / 50 + 80, 0.85 * WINSIZE_Y + 50, "보스 Y", "DNF_M_18", RGB(0, 255, 255));
		TEXTMANAGER->drawTextColor(hdc, WINSIZE_X / 50 + 80, 0.85 * WINSIZE_Y + 70, to_string(pos.y), "DNF_M_18", RGB(0, 255, 255));
	}
}

void BossBase::setBossHitbox(void)
{
	bStatus.bHitBox.left = pos.x - CAMERAMANAGER->getCameraPos().x;
	bStatus.bHitBox.right = bStatus.bHitBox.left + status.width;

	bStatus.bHitBox.bottom = pos.y - CAMERAMANAGER->getCameraPos().y;
	bStatus.bHitBox.top = bStatus.bHitBox.bottom - status.height;

	bStatus.effectRect.left = bStatus.bHitBox.left;
	bStatus.effectRect.right = bStatus.bHitBox.left + bStatus.effectImage->getFrameWidth();

	bStatus.effectRect.top = bStatus.bHitBox.top;
	bStatus.effectRect.bottom = bStatus.bHitBox.top + bStatus.effectImage->getFrameHeight();
}

void BossBase::offsetFix(void)
{
	// Do Nothing!
}

void BossBase::makeShootEvent(BulletType bType)
{
	ShootEvent shootEvent;
	shootEvent.bType = bType;

	switch (bType)
	{
	case BulletType::SiegeShoot:
		if(!status.lookRight) shootEvent.x = pos.x - bStatus.effectOffsetX;
		else shootEvent.x = pos.x + status.width + bStatus.effectOffsetX;
		shootEvent.y = pos.y - status.height - bStatus.effectOffsetY;
		break;
	case BulletType::DeathBall1:
		shootEvent.x = pos.x + status.width / 2;
		shootEvent.y = pos.y - status.height / 2;
		break;
	}
	shootEvent.direct = status.lookRight;
	shootEvent.velocityX = normalize(getDiffPlayer(pos.x, shootEvent.y)).x;
	shootEvent.velocityY = normalize(getDiffPlayer(pos.x, shootEvent.y)).y;

	EVENTMANAGER->dispatchEvents({ EventType::ShootBulltet, &shootEvent });
}

void BossBase::changeAnim(BossState bossState)
{
	// Do Nothing!
}

void BossBase::readyPattern(void)
{
	// Do Nothing!
}

Vector2 BossBase::getDiffPlayer(int firePointX, int firePointY)
{
	diff = { static_cast<float>(player->getCenterX() - firePointX),
						 static_cast<float>(player->getCenterY() - firePointY) };

	return diff;
}