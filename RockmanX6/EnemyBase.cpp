#include "Stdafx.h"
#include "EnemyBase.h"
#include "BulletManager.h"
#include "Player.h"

void EnemyBase::release(void)
{
	// Do nothing!
}

void EnemyBase::update(void)
{
	// Do nothing!
}
void EnemyBase::render(HDC hdc)
{
	if (eType == EnemyType::MetaDridler)
	{
		eStatus.ePartsImage->frameRender(hdc, eStatus.subHitBox.left - eStatus.subOffsetX, eStatus.subHitBox.top - eStatus.subOffsetY
			, eStatus.ePartsImage->getFrameX(), status.lookRight);
	}

	eStatus.eImage->frameRender(hdc, eStatus.eHitBox.left - eStatus.animOffsetX, eStatus.eHitBox.top - eStatus.animOffsetY
		, eStatus.eImage->getFrameX(), status.lookRight);

	if (UIMANAGER->getIsDebugMode())
	{
		DrawRectMakeColor(hdc, eStatus.eHitBox, RGB(0, 255, 255), 2);
		DrawRectMakeColor(hdc, eStatus.subHitBox, RGB(0, 255, 255), 2);
		DrawRectMakeColor(hdc, eStatus.attSight, RGB(255, 0, 255), 2);
	}
}

void EnemyBase::attack(void)
{
	// Do nothing!
}

void EnemyBase::setEnemyHitbox(void)
{
	eStatus.eHitBox.left = pos.x - CAMERAMANAGER->getCameraPos().x;
	eStatus.eHitBox.right = eStatus.eHitBox.left + status.width - CAMERAMANAGER->getCameraPos().x;

	eStatus.eHitBox.bottom = pos.y - CAMERAMANAGER->getCameraPos().y;
	eStatus.eHitBox.top = eStatus.eHitBox.bottom - status.height - CAMERAMANAGER->getCameraPos().y;

	/*
	eStatus.eHitBox.left = eStatus.worldRect.left - CAMERAMANAGER->getCameraPos().x;
	eStatus.eHitBox.right = eStatus.worldRect.right - CAMERAMANAGER->getCameraPos().x;

	eStatus.eHitBox.top = eStatus.worldRect.top - CAMERAMANAGER->getCameraPos().y;
	eStatus.eHitBox.bottom = eStatus.worldRect.bottom - CAMERAMANAGER->getCameraPos().y;
	*/

	if (status.lookRight)
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


}

void EnemyBase::pattern(void)
{
	if (!eStatus.isOnAttack)
	{
		eStatus.patternTimer += 0.1f;

		if (eStatus.patternTimer >= eStatus.maxPatternTime)
		{
			eStatus.attackAble = true;
			eStatus.patternTimer = 0.0f;
		}
	}
}

void EnemyBase::chasePlayer(float angle)
{
	// Do nothing!
}

void EnemyBase::isDead(void)
{
	if (status.hp <= 0)
	{
		status.hp = 0;
		status.dead = true;
	}
}

void EnemyBase::changeDirection(void)
{
	float angle = atan2f(getDiffPlayer(this->getCenterX(), this->getCenterY()).y, getDiffPlayer(this->getCenterX(), this->getCenterY()).x) * 180 / PI;

	if (eState == EnemyState::Idle)
	{
		if (angle > -70.0f && angle < 70.0f)
		{
			status.lookRight = true;
		}
		else if (angle > 110.0f || angle < -110.0f)
		{
			status.lookRight = false;
		}
	}
}

void EnemyBase::checkPlayerCollision(void)
{
	// Do nothing!
}

void EnemyBase::checkBulletCollision(void)
{
	vector<Bullet*>& bullets = bManager->getBullet();

	for (auto it = bullets.begin(); it != bullets.end();)
	{
		RECT temp;

		if (IntersectRect(&temp, &(*it)->getBulletRect(), &eStatus.eHitBox) && !status.overpower)
		{
			switch (eType)
			{
			case EnemyType::MetaDridler:
			// case EnemyType::MetaWheel:
				// Æ¨±â´Â ¼Ò¸®
				EFFECTMANAGER->spawnEffect(EffectType::BursterBlock, (*it)->getBulletPosX(), (*it)->getBulletPosY(), (*it)->getBulletWidth(), (*it)->getBulletHeight(), (*it)->getBulletDir());
				SOUNDMANAGER->play("SFX_Block", 0.5f);
				it = bullets.erase(it);
				break;

			default:
				status.hp -= (*it)->getBulletDamage();
				status.overpower = true;
				SOUNDMANAGER->play("SFX_X_Burster1Hit", 0.5f);
				if((*it)->getBulletType() != BulletType::ChargeBurst2) EFFECTMANAGER->spawnEffect(EffectType::BursterHit_1, (*it)->getBulletPosX(), (*it)->getBulletPosY(), (*it)->getBulletWidth(), (*it)->getBulletHeight(), (*it)->getBulletDir());
				else EFFECTMANAGER->spawnEffect(EffectType::BursterHit_2, (*it)->getBulletPosX(), (*it)->getBulletPosY(), (*it)->getBulletWidth(), (*it)->getBulletHeight(), (*it)->getBulletDir());

				if ((*it)->getBulletType() == BulletType::ChargeBurst2 || (*it)->getBulletType() == BulletType::FalconBurst2)
				{
					if (status.hp > 0) it = bullets.erase(it);
				}

				else it = bullets.erase(it);
				break;
			}
		}

		else ++it;
	}
}

void EnemyBase::enemyInvincibleTimerUpdate(void)
{
	if (eStatus.invincibleTimer >= eStatus.invincibleMaxTime)
	{
		eStatus.invincibleTimer = 0.0f;
		status.overpower = false;
	}

	else if (status.overpower)
	{
		eStatus.invincibleTimer += 0.1f;
	}
}

void EnemyBase::makeShootEvent(BulletType bType)
{
	// Do Nothing!!

	ShootEvent shootEvent;
	shootEvent.bType = bType;
	shootEvent.direct = status.lookRight;

	switch (bType)
	{
	case BulletType::JunkBullet:
		shootEvent.x = (eStatus.worldRect.left + eStatus.worldRect.right) / 2;
		shootEvent.y = eStatus.worldRect.top + fPos.y;
		break;
	}
	
	shootEvent.velocityX = normalize(getDiffPlayer(shootEvent.x, shootEvent.y)).x;
	shootEvent.velocityY = normalize(getDiffPlayer(shootEvent.x, shootEvent.y)).y;

	EVENTMANAGER->dispatchEvents({ EventType::ShootBulltet, &shootEvent });

}

Vector2 EnemyBase::getDiffPlayer(int firePointX, int firePointY)
{
	diff = { static_cast<float>(player->getCenterX() - firePointX),
						 static_cast<float>(player->getCenterY() - firePointY) };

	return diff;
}