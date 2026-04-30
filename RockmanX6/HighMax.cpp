#include "Stdafx.h"
#include "HighMax.h"
#include "BulletManager.h"
#include "Player.h"

HRESULT HighMax::init(int x, int y)
{
	status.type = CombatEntityType::Enemy;
	btype = BossType::Intro;
	bState = BossState::Idle;

	status.maxHp = 4;
	status.hp = status.maxHp;
	status.physicalDamage = 2;

	bStatus.bImage = new GImage;
	bStatus.bImage = IMAGEMANAGER->findImage("HighMax_Idle")->cloneImage();
	bStatus.effectImage = IMAGEMANAGER->findImage("SFX_DeathBallCharge")->cloneImage();

	status.width = bStatus.bImage->getFrameWidth();
	status.height = bStatus.bImage->getFrameHeight();

	bStatus.overPower = false;
	status.lookRight = false;

	bStatus.bHitBox = bStatus.bWorldRect = RectMakeCenter(x + status.width /2, y - status.height / 2, status.width, status.height);
	
	pos.x = x;
	pos.y = y;

	bStatus.offsetX = 0 * SCALE_FACTOR;
	bStatus.offsetY = 0 * SCALE_FACTOR;

	animSpeed = 0.1f;
	attTimes = 0;

	animDir = AnimDirection::Forward;

	patternTest = false;
	SiegeSecondAtt = false;
	prevFrame = -1;
	timer = 0.0f;

	bStatus.effectOn = false;

	return S_OK;
}

void HighMax::update(void)
{
	switch (bState)
	{
	case BossState::Idle:
		animSpeed = 0.06f;
		// bStatus.bImage->play(animSpeed);
		break;
	}

	if (KEYMANAGER->isOnceKeyDown('B'))
	{
		cout << "등장" << endl;
	}

	if (KEYMANAGER->isOnceKeyDown('N'))
	{
		cout << "시즈샷" << endl;

		bState = BossState::AttReady;
		bStatus.bImage = IMAGEMANAGER->findImage("HighMax_AttReady")->cloneImage();
		patternTest = true;
		// siegeShoot();
	}

	if (KEYMANAGER->isOnceKeyDown('M'))
	{
		cout << "데스볼" << endl;

		bState = BossState::DeathBallShoot;
		bStatus.bImage = IMAGEMANAGER->findImage("HighMax_DeathBallShoot")->cloneImage();
		SOUNDMANAGER->play("Voice_DeathBall", 0.3f);
		patternTest = true;
		// deathBall();
	}

	if (patternTest)
	{
		deathBall();
	}
	
	setBossHitbox();

}

void HighMax::bossAppearance(void)
{

}

void HighMax::siegeShoot(void)
{
	animSpeed = 0.07f;

	switch (bState)
	{
		case BossState::AttReady:
			if (animDir == AnimDirection::Forward)
			{
				bStatus.bImage->play(animSpeed);

				if (bStatus.bImage->getChangeReady())
				{
					bStatus.bImage->setChangeReady(false);
					bStatus.bImage = IMAGEMANAGER->findImage("HighMax_RightReady");
					bState = BossState::RightReady;
				}
			}

			else
			{
				bStatus.bImage->reversePlay(animSpeed);

				if (bStatus.bImage->getChangeReady())
				{
					bStatus.bImage->setChangeReady(false);
					bStatus.bImage = IMAGEMANAGER->findImage("HighMax_Idle");
					animDir = AnimDirection::Forward;
					bState = BossState::Idle;
					prevFrame = -1;
				}
			}
			break;

		case BossState::RightReady:
			if (animDir == AnimDirection::Forward)
			{
				bStatus.bImage->play(animSpeed);

				if (bStatus.bImage->getChangeReady())
				{
					bStatus.bImage->setChangeReady(false);
					bStatus.bImage = IMAGEMANAGER->findImage("HighMax_RightAtt");
					bState = BossState::RightAtt;
				}
			}
			else
			{
				bStatus.bImage->reversePlay(animSpeed);

				if (bStatus.bImage->getChangeReady())
				{
					bStatus.bImage->setChangeReady(false);

					if (!SiegeSecondAtt)
					{
						bStatus.bImage->setChangeReady(false);
						bStatus.bImage = IMAGEMANAGER->findImage("HighMax_LeftReady");
						bState = BossState::LeftReady;
						animDir = AnimDirection::Forward;
						SiegeSecondAtt = true;
					}

					else
					{
						bStatus.bImage->setChangeReady(false);
						bStatus.bImage = IMAGEMANAGER->findImage("HighMax_AttReady");
						bState = BossState::AttReady;
						animDir = AnimDirection::Backward;
					}
				}
			}
			break;

		case BossState::RightAtt:
			animSpeed = 0.08f;
			if (animDir == AnimDirection::Forward)
			{
				bStatus.bImage->play(animSpeed);
				///////////////////
				if (prevFrame != bStatus.bImage->getFrameX())
				{
					if (bStatus.bImage->getFrameX() == 1)
					{
						makeShootEvent(BulletType::SiegeShoot);
						attTimes++;
					}
					
					// 정자세로 리셋되면
					else if (bStatus.bImage->getFrameX() == 0 && attTimes > 3)
					{
						attTimes = 0;
						animDir = AnimDirection::Backward;
						bState = BossState::RightReady;
						bStatus.bImage = IMAGEMANAGER->findImage("HighMax_RightReady");
					}
				}
				prevFrame = bStatus.bImage->getFrameX();
			}
			break;

		case BossState::LeftReady:
			if (animDir == AnimDirection::Forward)
			{
				bStatus.bImage->play(animSpeed);

				if (bStatus.bImage->getChangeReady())
				{
					bStatus.bImage->setChangeReady(false);
					bStatus.bImage = IMAGEMANAGER->findImage("HighMax_LeftAtt");
					bState = BossState::LeftAtt;
				}
			}
			else
			{
				bStatus.bImage->reversePlay(animSpeed);

				if (bStatus.bImage->getChangeReady())
				{
					bStatus.bImage->setChangeReady(false);
					bStatus.bImage = IMAGEMANAGER->findImage("HighMax_RightReady");
					bStatus.bImage->setFrameX(0);
					bState = BossState::RightReady;
					animDir = AnimDirection::Forward;
				}
			}
			break;

		case BossState::LeftAtt:
			animSpeed = 0.08f;
			if (animDir == AnimDirection::Forward)
			{
				bStatus.bImage->play(animSpeed);


				if (prevFrame != bStatus.bImage->getFrameX())
				{
					if (bStatus.bImage->getFrameX() == 1)
					{
						makeShootEvent(BulletType::SiegeShoot);
						attTimes++;
					}

					// 정자세로 리셋되면
					else if (bStatus.bImage->getFrameX() == 0 && attTimes > 3)
					{
						attTimes = 0;
						animDir = AnimDirection::Backward;
						bState = BossState::LeftReady;
						bStatus.bImage = IMAGEMANAGER->findImage("HighMax_LeftReady");
					}
				}
				prevFrame = bStatus.bImage->getFrameX();
			}
			break;

		case BossState::Idle:
			patternTest = false;
			SiegeSecondAtt = false;
			break;
	}

	// 애니메이션 위치 보정
	if (bState != BossState::Idle)
	{
		bStatus.offsetX = 12 * SCALE_FACTOR;
		bStatus.offsetY = -4 * SCALE_FACTOR;
	}

	else
	{
		bStatus.offsetX = 0 * SCALE_FACTOR;
		bStatus.offsetY = 0 * SCALE_FACTOR;
	}
}

void HighMax::deathBall(void)
{
	animSpeed = 0.05f;
	bStatus.bImage->play(animSpeed);
	if(bStatus.effectOn) bStatus.effectImage->play(0.03f);

	switch(bState)
	{
	case BossState::DeathBallShoot:
		if (prevFrame != bStatus.bImage->getFrameX())
		{
			if (bStatus.bImage->getFrameX() == 3)
			{
				bStatus.effectOn = true;
				bStatus.effectImage = IMAGEMANAGER->findImage("SFX_DeathBallCharge")->cloneImage();
				//SOUNDMANAGER->play("SFX_DeathBallCharge", 0.3f);
			}
		}
		prevFrame = bStatus.bImage->getFrameX();

		if (bStatus.effectImage->getChangeReady() && bStatus.effectOn)
		{
			SOUNDMANAGER->play("SFX_RingSound", 0.3f);
			bStatus.effectOn = false;
			bStatus.effectImage->setChangeReady(false);
			bStatus.effectImage->setFrameX(0);
			bStatus.bImage->setChangeReady(false);
			bStatus.bImage->setFrameX(0);
			makeShootEvent(BulletType::DeathBall1);
			bState = BossState::DeathBallShootIdle;
			bStatus.bImage = IMAGEMANAGER->findImage("HighMax_DeathBallShootIdle")->cloneImage();
		}
		else
		{
			cout << bStatus.effectImage->getChangeReady() <<endl;
		}

		break;

	case BossState::DeathBallShootIdle:

		if (timer <= 5.0f)
		{
			timer += 0.01f;
		}

		else
		{
			bStatus.bImage->setFrameX(0);
			bState = BossState::Idle;
			bStatus.bImage = IMAGEMANAGER->findImage("HighMax_Idle");
		}
		break;

	case BossState::Idle:
		timer = 0.0f;
		patternTest = false;
		break;
	}

	// 애니메이션 위치 보정
	if (bState != BossState::Idle)
	{
		bStatus.offsetX = 2 * SCALE_FACTOR;
		bStatus.offsetY = -4 * SCALE_FACTOR;
	}

	else
	{
		bStatus.offsetX = 0 * SCALE_FACTOR;
		bStatus.offsetY = 0 * SCALE_FACTOR;
	}
}

void HighMax::patternShoot(void)
{

}

void HighMax::stateReset(void)
{
	// 이미지 관련 리셋
	IMAGEMANAGER->findImage("HighMax_AttReady")->setChangeReady(false);
	IMAGEMANAGER->findImage("HighMax_RightReady")->setChangeReady(false);
	IMAGEMANAGER->findImage("HighMax_LeftReady")->setChangeReady(false);
	prevFrame = -1;

	// 보스 상태값 관련 리셋
	SiegeSecondAtt = false;
}
