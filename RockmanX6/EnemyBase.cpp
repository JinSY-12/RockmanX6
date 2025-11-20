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
	eStatus.eHitBox.left = eStatus.worldRect.left - CAMERAMANAGER->getCameraPos().x;
	eStatus.eHitBox.right = eStatus.worldRect.right - CAMERAMANAGER->getCameraPos().x;

	eStatus.eHitBox.top = eStatus.worldRect.top - CAMERAMANAGER->getCameraPos().y;
	eStatus.eHitBox.bottom = eStatus.worldRect.bottom - CAMERAMANAGER->getCameraPos().y;

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
	RECT temp;

	if (IntersectRect(&temp, &player->getSaberRect(), &eStatus.eHitBox) && !eStatus.overpower && player->getCanHit())
	{
		int rnd = RND->getInt(4);

		switch (eType)
		{
		case EnemyType::MetaDridler:
			player->setAnimDelay(true);
			// 튕기는 소리
			// SOUNDMANAGER->play("SFX_SaberHit", 0.5f);
			break;
			
		default:
			eStatus.hp -= player->getSaberDamage();
			player->setAnimDelay(true);
			SOUNDMANAGER->play("SFX_SaberHit", 0.5f);

			switch (rnd)
			{
			case 0:
				EFFECTMANAGER->spawnEffect(EffectType::SaberHit_1, ePos.x, ePos.y, eStatus.width, eStatus.height, eStatus.lookRight);
				break;
			case 1:
				EFFECTMANAGER->spawnEffect(EffectType::SaberHit_2, ePos.x, ePos.y, eStatus.width, eStatus.height, eStatus.lookRight);
				break;
			case 2:
				EFFECTMANAGER->spawnEffect(EffectType::SaberHit_3, ePos.x, ePos.y, eStatus.width, eStatus.height, eStatus.lookRight);
				break;
			case 3:
				EFFECTMANAGER->spawnEffect(EffectType::SaberHit_4, ePos.x, ePos.y, eStatus.width, eStatus.height, eStatus.lookRight);
				break;
			}			
			break;
		}
	}
}

void EnemyBase::checkBulletCollision(void)
{
	vector<Bullet*>& bullets = bManager->getBullet();

	for (auto it = bullets.begin(); it != bullets.end();)
	{
		RECT temp;

		if (IntersectRect(&temp, &(*it)->getBulletRect(), &eStatus.eHitBox) && !eStatus.overpower)
		{
			switch (eType)
			{
			case EnemyType::MetaDridler:
				// 튕기는 소리
				SOUNDMANAGER->play("SFX_Block", 0.5f);
				EFFECTMANAGER->spawnEffect(EffectType::BursterBlock, (*it)->getBulletPosX(), (*it)->getBulletPosY(), (*it)->getBulletWidth(), (*it)->getBulletHeight(), (*it)->getBulletDir());
				break;

			default:
				eStatus.hp -= (*it)->getBulletDamage();
				eStatus.overpower = true;
				SOUNDMANAGER->play("SFX_X_Burster1Hit", 0.5f);
				if((*it)->getBulletType() != BulletType::ChargeBurst2) EFFECTMANAGER->spawnEffect(EffectType::BursterHit_1, (*it)->getBulletPosX(), (*it)->getBulletPosY(), (*it)->getBulletWidth(), (*it)->getBulletHeight(), (*it)->getBulletDir());
				else EFFECTMANAGER->spawnEffect(EffectType::BursterHit_2, (*it)->getBulletPosX(), (*it)->getBulletPosY(), (*it)->getBulletWidth(), (*it)->getBulletHeight(), (*it)->getBulletDir());

				if ((*it)->getBulletType() == BulletType::ChargeBurst2 || (*it)->getBulletType() == BulletType::FalconBurst2)
				{
					if (eStatus.hp > 0) it = bullets.erase(it);
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
		eStatus.overpower = false;
	}

	else if (eStatus.overpower)
	{
		eStatus.invincibleTimer += 0.1f;
	}
}
