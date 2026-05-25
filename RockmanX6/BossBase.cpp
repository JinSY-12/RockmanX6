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
	if (bStatus.effectOnTop)
	{
		bStatus.bImage->frameAlphaRender(hdc, bStatus.bHitBox.left - bStatus.offsetX, bStatus.bHitBox.top - bStatus.offsetY
			, bStatus.bImage->getFrameX(), status.lookRight, bossAlpha);

		if (bStatus.effectImage != nullptr && bStatus.effectOn == true)
		{
			bStatus.effectImage->frameRender(hdc, bStatus.effectRect.left, bStatus.effectRect.top,
				bStatus.effectImage->getFrameX(), status.lookRight);
		}
	}

	else
	{
		if (bStatus.effectImage != nullptr && bStatus.effectOn == true)
		{
			bStatus.effectImage->frameAlphaRender(hdc, bStatus.effectRect.left, bStatus.effectRect.top,
				bStatus.effectImage->getFrameX(), status.lookRight, bossAlpha);
		}

		bStatus.bImage->frameRender(hdc, bStatus.bHitBox.left - bStatus.offsetX, bStatus.bHitBox.top - bStatus.offsetY
			, bStatus.bImage->getFrameX(), status.lookRight);
	}

	hpBar.render(hdc);

	if (UIMANAGER->getIsDebugMode())
	{
		DrawRectMakeColor(hdc, bStatus.bHitBox, RGB(0, 255, 255), 2);
		DrawRectMakeColor(hdc, bStatus.effectRect, RGB(0, 255, 0), 2);

		TEXTMANAGER->drawTextColor(hdc, WINSIZE_X / 50, 0.85 * WINSIZE_Y + 50, "보스 X", "DNF_M_18", RGB(0, 255, 255));
		TEXTMANAGER->drawTextColor(hdc, WINSIZE_X / 50, 0.85 * WINSIZE_Y + 70, to_string(pos.x / SCALE_FACTOR), "DNF_M_18", RGB(0, 255, 255));

		TEXTMANAGER->drawTextColor(hdc, WINSIZE_X / 50 + 80, 0.85 * WINSIZE_Y + 50, "보스 Y", "DNF_M_18", RGB(0, 255, 255));
		TEXTMANAGER->drawTextColor(hdc, WINSIZE_X / 50 + 80, 0.85 * WINSIZE_Y + 70, to_string(pos.y / SCALE_FACTOR), "DNF_M_18", RGB(0, 255, 255));
	}
}

void BossBase::setBossHitbox(void)
{
	bStatus.bHitBox.left = pos.x - CAMERAMANAGER->getCameraPos().x;
	bStatus.bHitBox.right = bStatus.bHitBox.left + status.width;

	bStatus.bHitBox.bottom = pos.y - CAMERAMANAGER->getCameraPos().y;
	bStatus.bHitBox.top = bStatus.bHitBox.bottom - status.height;

	bStatus.effectRect.left = bStatus.bHitBox.left - bStatus.effectOffsetX;
	bStatus.effectRect.right = bStatus.effectRect.left + bStatus.effectImage->getFrameWidth();

	bStatus.effectRect.top = bStatus.bHitBox.top - bStatus.effectOffsetY;
	bStatus.effectRect.bottom = bStatus.effectRect.top + bStatus.effectImage->getFrameHeight();
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
	case BulletType::DeathBall:
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

void BossBase::deadAnim(void)
{
	if (!bossClear)
	{
		UIMANAGER->setIsUiPrint(true);
				
		if (effectTimer.count <= 5)
			effectTimer.effectSpeed -= 0.02;
		else if (effectTimer.count > 5 && effectTimer.count <= 10)
			effectTimer.effectSpeed -= 0.02;
		else if (effectTimer.count > 10 && effectTimer.count <= 15)
			effectTimer.effectSpeed -= 0.01;
		else if (effectTimer.count > 25)
		{
			CAMERAMANAGER->whiteOut(1.0f);
			SOUNDMANAGER->play("SFX_BossExplode", 0.3f);

			bossClear = true;
		}
		
		if (effectTimer.update(effectTimer.effectSpeed) && effectTimer.count <= 25)
		{
			int rndX = RND->getFromIntTo(-status.width, status.width);
			int rndY = RND->getFromIntTo(-status.height, status.height);

			EFFECTMANAGER->spawnEffect(EffectType::SmallEnemyBomb,
				pos.x + status.width / 2 + rndX, pos.y - status.height / 2 + rndY, 0, 0, 0);
				
			effectTimer.count++;
		}
	}

	else
	{
		if(bossAlpha != 0)
		{
			if (CAMERAMANAGER->isPadeOutComplete())
			{
				if (effectTimer.update(5.0f))
				{
					bossAlpha = 0;
					CAMERAMANAGER->whiteIn(1.5f);
				}
			}
		}
		
		else
		{
			if (CAMERAMANAGER->isPadeInComplete())
			{
				if (effectTimer.update(5.0f))
				{
					status.deadDone = true;
				}
			};
		}
	}
	

	
}

void BossBase::bossInvincibleTimerUpdate(void)
{
	if (status.overpower)
	{
		bStatus.invincibleTimer += 0.1f;

		if (static_cast<int>(bStatus.invincibleTimer * 10) % 5 == 0)
		{
			if(!status.dead) bossAlpha = (bossAlpha >= 200) ? 50 : 200;
		}

		if (bStatus.invincibleTimer >= bStatus.invincibleMaxTime)
		{
			bossAlpha = 255;
			bStatus.invincibleTimer = 0.0f;
			status.overpower = false;
		}
	}
}

void BossBase::battleStart(void)
{
	if (status.hp <= status.maxHp)
	{
		// UIMANAGER->setIsUiPrint(true);
		status.hp += 1;
		SOUNDMANAGER->play("Menu_Dialogue", 0.3f);
	}

	else
	{
		cout << "Battle Start!" << endl;
		gameStart = true;
		UIMANAGER->setIsUiPrint(false);
		UIMANAGER->setFreeze(false);
		SOUNDMANAGER->play(BossBGM, 0.3f);
		SOUNDMANAGER->setCurrentBGM(BossBGM);
	}

}

Vector2 BossBase::getDiffPlayer(int firePointX, int firePointY)
{
	diff = { static_cast<float>(player->getCenterX() - firePointX),
						 static_cast<float>(player->getCenterY() - firePointY) };

	return diff;
}