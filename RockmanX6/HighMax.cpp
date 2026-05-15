#include "Stdafx.h"
#include "HighMax.h"
#include "BulletManager.h"
#include "Player.h"

HRESULT HighMax::init(int x, int y)
{
	status.type = CombatEntityType::Enemy;
	btype = BossType::Intro;
	bState = BossState::Apperance;

	status.maxHp = 4;
	status.hp = status.maxHp;
	status.physicalDamage = 2;

	bStatus.bImage = new GImage;
	bStatus.bImage = IMAGEMANAGER->findImage("HighMax_Move")->cloneImage();
	bStatus.effectImage = IMAGEMANAGER->findImage("SFX_DeathBallCharge")->cloneImage();

	status.width = 35 * SCALE_FACTOR;
	status.height = 70 * SCALE_FACTOR;

	bStatus.overPower = false;
	status.lookRight = false;

	bStatus.bHitBox = RectMakeCenter(x + status.width /2, y - status.height / 2, status.width, status.height);
	
	// X는 RECT 왼쪽, Y는 RECT 바닥
	pos.x = x;
	pos.y = y;

	if(!status.lookRight) bStatus.originX = 30 * SCALE_FACTOR;
	else bStatus.originX = 22 * SCALE_FACTOR;
	
	bStatus.originY = 20 * SCALE_FACTOR;

	bStatus.offsetX = bStatus.originX + 0 * SCALE_FACTOR;
	bStatus.offsetY = bStatus.originY + 0 * SCALE_FACTOR;

	animSpeed = 0.1f;
	attTimes = 0;

	animDir = AnimDirection::Forward;
	attPattern = AttPattern::Idle;
	effPattern = EffectState::BallCharge;

	SiegeSecondAtt = false;
	prevFrame = -1;
	timer = 0.0f;
	bStatus.effectOn = false;
	phase2 = false;
	BossBGM = "BGM_VS_HighMax";
	isPattern = false;
	appearanceDone = false;
	musciStart = false;
	attCycle = false;
	bStatus.effectOnTop = true;
	rushStart = false;

	pattenrCoolDown = 2.0f;

	// 패턴을 바로 발동하게 테스트용 Bool값이라서 패턴 발동이 완성되면 지울것
	patternTest = false;
	patternTest2 = false;

	/*
	appearanceDone = true;
	bState = BossState::AttReady;
	attPattern = AttPattern::SiegeShoot;
	isPattern = true;
	*/
	// 등장 모션을 위한 캐릭터 움직임 잠금
	UIMANAGER->setFreeze(true);

	return S_OK;
}

void HighMax::update(void)
{
	bool allowInput = !(CAMERAMANAGER->getIsCamaraMove() || !CAMERAMANAGER->getCameraMoveEnd()
		|| UIMANAGER->getIsUiPrint());
	bStatus.movable = allowInput;
	
	if (allowInput)
	{
		if (appearanceDone)
		{
			if (!isPattern)
			{
				bStatus.bImage->play(animSpeed);
				readyPattern();
			}

			else
			{
				if (bStatus.effectOn) bStatus.effectImage->play(0.03f);

				switch (attPattern)
				{
				case AttPattern::Idle:
					bStatus.bImage->play(0.08f);
					break;
				case AttPattern::SiegeShoot:
					siegeShoot();
					break;
				case AttPattern::DeathBallShoot:
					bStatus.bImage->play(animSpeed);
					deathBall();
					break;
				case AttPattern::DeathRush:
					deathRush();
					break;
				case AttPattern::Dodge:
					dodge();
					break;
				}
			}
		}

		else bossAppearance();
	}

	if (KEYMANAGER->isOnceKeyDown('K'))
	{
		status.lookRight = true;
		pos.x = 6110 * SCALE_FACTOR;
	}

	if (KEYMANAGER->isOnceKeyDown('L'))
	{
		status.lookRight = false;
		pos.x = 6340 * SCALE_FACTOR;
	}

	if (KEYMANAGER->isOnceKeyDown('J'))
	{
		phase2 = !phase2;
	}

	offsetFix();
	setBossHitbox();
}

void HighMax::bossAppearance(void)
{
	// 주석
	// if (pos.y < 830 * SCALE_FACTOR)
	if (pos.y < 110 * SCALE_FACTOR)
	{
		if (!musciStart) pos.y += 1 * SCALE_FACTOR;
		else
		{
			if (!phase2) pos.y += 3 * SCALE_FACTOR;
			else pos.y += 4 * SCALE_FACTOR;
		}
	}
	else
	{
		// pos.y = 830 * SCALE_FACTOR;
		pos.y = 110 * SCALE_FACTOR;
		isPattern = true;
		appearanceDone = true;
		// 첫패턴 무조건 시즈볼

	
		// 시즈볼
		bState = BossState::AttReady;
		attPattern = AttPattern::SiegeShoot;
		bStatus.bImage = IMAGEMANAGER->findImage("HighMax_AttReady")->cloneImage();
		
		/*
		// 데스볼 테스트
		bState = BossState::DeathBallShoot;
		attPattern = AttPattern::DeathBallShoot;
		bStatus.bImage = IMAGEMANAGER->findImage("HighMax_DeathBallShoot")->cloneImage();
		SOUNDMANAGER->play("Voice_DeathBall", 0.3f);
		*/

		/*
		// 데스 러시 테스트
		bState = BossState::AttReady;
		attPattern = AttPattern::DeathRush;
		bStatus.bImage = IMAGEMANAGER->findImage("HighMax_AttReady")->cloneImage();
		*/

		if (!musciStart)
		{
			musciStart = true;
			UIMANAGER->setFreeze(false);
			SOUNDMANAGER->play(BossBGM, 0.3f);
		}
	}
}

void HighMax::dodge()
{	
	if (pos.y > -20) pos.y -= 3 * SCALE_FACTOR;
	else
	{
		pos.y = -20 * SCALE_FACTOR;
		isPattern = true;
		appearanceDone = false;
		// 무적 추가

		int rnd = rand() % 2;

		if (rnd > 0)
		{
			pos.x = 6110 * SCALE_FACTOR;
			status.lookRight = true;
		}

		else
		{
			pos.x = 6340 * SCALE_FACTOR;
			status.lookRight = false;
		}
	}
}

void HighMax::siegeShoot(void)
{
	if (!phase2) animSpeed = 0.08f;
	else animSpeed = 0.07f;

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
					animSpeed = 0.08f;
					bStatus.bImage->setChangeReady(false);
					bStatus.bImage = IMAGEMANAGER->findImage("HighMax_Idle");
					animDir = AnimDirection::Forward;
					bState = BossState::Idle;
					attPattern = AttPattern::Idle;
					prevFrame = -1;
					isPattern = false;
					SiegeSecondAtt = false;
					patternTimer = TIMEMANAGER->getWorldTime();
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
			animSpeed = 0.08f;
			patternTest = false;
			SiegeSecondAtt = false;
			break;
	}
}

void HighMax::deathBall(void)
{
	bStatus.effectOnTop = true;

	switch(bState)
	{
	case BossState::DeathBallShoot:
		if (!phase2) animSpeed = 0.05f;
		else animSpeed = 0.03f;

		if (prevFrame != bStatus.bImage->getFrameX())
		{
			if (bStatus.bImage->getFrameX() == 3)
			{
				bStatus.effectOn = true;
				bStatus.effectImage = IMAGEMANAGER->findImage("SFX_DeathBallCharge")->cloneImage();
				effPattern = EffectState::BallCharge;
			}
		}
		prevFrame = bStatus.bImage->getFrameX();

		if (bStatus.effectImage->getChangeReady() && effPattern == EffectState::BallCharge)
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

		break;

	case BossState::DeathBallShootIdle:
		animSpeed = 0.05f;

		if (!phase2)
		{
			if(timerClock(2.5f))
			{
				bStatus.bImage->setFrameX(0);
				bState = BossState::Idle;
				bStatus.bImage = IMAGEMANAGER->findImage("HighMax_Idle")->cloneImage();
			}
		}
		else
		{
			if (timerClock(2.0f))
			{
				bStatus.bImage->setFrameX(0);
				bState = BossState::Idle;
				bStatus.bImage = IMAGEMANAGER->findImage("HighMax_Idle")->cloneImage();
			}
		}		
		break;

	case BossState::Idle:
		animSpeed = 0.08f;
		
		int rnd = rand() % 3;

		// 패턴 이후 재정비
		if (rnd > 0)
		{
			if (timerClock(1.0f))
			{
				isPattern = true;
				attPattern = AttPattern::Dodge;
				bStatus.bImage = IMAGEMANAGER->findImage("HighMax_Move")->cloneImage();
			}
		}
		
		// 패턴 이후 다시 공격
		else
		{
			if (timerClock(1.0f))
			{
				isPattern = false;
				patternTimer = TIMEMANAGER->getWorldTime();
			}
		}
		break;
	}

}

void HighMax::deathRush(void)
{
	if (!phase2) animSpeed = 0.06;
	else animSpeed = 0.04;

	switch (bState)
	{
	case BossState::AttReady:
		bStatus.bImage->play(animSpeed);
		if (bStatus.bImage->getChangeReady())
		{
			bStatus.bImage->setChangeReady(false);
			bState = BossState::RightReady;
			bStatus.bImage = IMAGEMANAGER->findImage("HighMax_RightReady")->cloneImage();
		}
		break;
	case BossState::RightReady:
		bStatus.bImage->play(animSpeed);
		if (bStatus.bImage->getChangeReady())
		{
			bStatus.bImage->setChangeReady(false);
			bState = BossState::LeftReady;
			bStatus.bImage = IMAGEMANAGER->findImage("HighMax_LeftReady")->cloneImage();
			bStatus.effectImage = IMAGEMANAGER->findImage("SFX_RushCharge")->cloneImage();
		}
		break;
	case BossState::LeftReady:
		bStatus.bImage->play(animSpeed);

		bStatus.effectOnTop = false;
		bStatus.effectOn = true;
		
		if (bStatus.effectImage->getChangeReady())
		{
			effPattern = EffectState::RushBall;
			bStatus.effectImage->setChangeReady(false);
			bStatus.effectImage = IMAGEMANAGER->findImage("SFX_RushBall")->cloneImage();
		}

		if (effPattern == EffectState::RushBall)
		{
			if (bStatus.effectImage->getFrameX() >= bStatus.effectImage->getMaxFrameX())
			{
				rushStart = true;
			}
		}

		if (rushStart == true)
		{
			if (pos.y < 160 * SCALE_FACTOR)
			{
				if (!phase2) pos.y += 5 * SCALE_FACTOR;
				else pos.y += 7 * SCALE_FACTOR;
			}
			else
			{
				pos.y = 160 * SCALE_FACTOR;

				if (!status.lookRight)
				{
					if (pos.x > 6310 * SCALE_FACTOR) pos.x -= 4 * SCALE_FACTOR;
					else
					{
						bState = BossState::RightAtt;
						bStatus.bImage = IMAGEMANAGER->findImage("HighMax_RightAtt");
					}

				}
				
				else
				{
					if (pos.x < 6120 * SCALE_FACTOR) pos.x += 4 * SCALE_FACTOR;
					else
					{
						bState = BossState::RightAtt;
						bStatus.bImage = IMAGEMANAGER->findImage("HighMax_RightAtt");
					}
				}
			}
		}
		break;
	case BossState::RightAtt:
		bStatus.effectOnTop = true;

		if (!status.lookRight)
		{
			if (pos.x > 6110 * SCALE_FACTOR)
			{
				if (!phase2) pos.x -= 6 * SCALE_FACTOR;
				else pos.x -= 8 * SCALE_FACTOR;
			}

			else
			{
				if (rushStart)
				{
					rushStart = false;
					SOUNDMANAGER->play("SFX_CrushWall", 0.3f);
				}

				if (timerClock(1.5f))
				{
					bStatus.effectOn = false;
					bState = BossState::Idle;
					bStatus.bImage = IMAGEMANAGER->findImage("HighMax_Idle");
				}
			}
		}

		else
		{
			if (pos.x < 6330 * SCALE_FACTOR)
			{
				if (!phase2) pos.x += 6 * SCALE_FACTOR;
				else pos.x += 8 * SCALE_FACTOR;
				
			}
			else
			{
				if (rushStart)
				{
					rushStart = false;
					SOUNDMANAGER->play("SFX_CrushWall", 0.3f);
				}

				if (timerClock(1.5f))
				{
					bStatus.effectOn = false;
					bState = BossState::Idle;
					bStatus.bImage = IMAGEMANAGER->findImage("HighMax_Idle");
				}
			}
		}
		break;
	case BossState::Idle:
		animSpeed = 0.08f;
		if (timerClock(1.0f))
		{
			animSpeed = 0.06f;
			isPattern = true;
			attPattern = AttPattern::Dodge;
			bStatus.bImage = IMAGEMANAGER->findImage("HighMax_Move")->cloneImage();
		}
		break;
	}
}


void HighMax::readyPattern(void)
{
	if (!phase2) pattenrCoolDown = 2.0f;
	else pattenrCoolDown = 1.5f;

	if (TIMEMANAGER->getWorldTime() - patternTimer > pattenrCoolDown)
	{
		int rnd = rand() % 10;

		// 데스볼 샷
		if (rnd >= 4)
		{
			bState = BossState::DeathBallShoot;
			attPattern = AttPattern::DeathBallShoot;
			isPattern = true;
			bStatus.bImage = IMAGEMANAGER->findImage("HighMax_DeathBallShoot")->cloneImage();
			SOUNDMANAGER->play("Voice_DeathBall", 0.3f);
		}

		// 데스볼 러시
		else
		{
			bState = BossState::AttReady;
			attPattern = AttPattern::DeathRush;
			isPattern = true;
			bStatus.bImage = IMAGEMANAGER->findImage("HighMax_AttReady")->cloneImage();
		}
	}
}

void HighMax::movetoPoint()
{

}

void HighMax::changeAnim(BossState bossState)
{
	switch (bossState)
	{
	case BossState::Apperance:
		bStatus.bImage = IMAGEMANAGER->findImage("HighMax_Move");
		bStatus.offsetX = bStatus.originX + 0 * SCALE_FACTOR;
		bStatus.offsetY = bStatus.originY + 0 * SCALE_FACTOR;
		animSpeed = 0.06f;
		animDir = AnimDirection::Forward;
		break;
	case BossState::Idle:
		bStatus.bImage = IMAGEMANAGER->findImage("HighMax_Idle");
		bStatus.offsetX = bStatus.originX + 0 * SCALE_FACTOR;
		bStatus.offsetY = bStatus.originY + 0 * SCALE_FACTOR;
		if (!phase2) animSpeed = 0.06f;
		else animSpeed = 0.04f;
		animDir = AnimDirection::Forward;
		break;
	}
}

void HighMax::offsetFix(void)
{
	if (!status.lookRight) bStatus.originX = 30 * SCALE_FACTOR;
	else bStatus.originX = 22 * SCALE_FACTOR;

	// FirePoint
	switch (attPattern)
	{
	case AttPattern::Idle:
		bStatus.effectOffsetX = 0;
		bStatus.effectOffsetY = 0;
		break;
	case AttPattern::DeathBallShoot:
		if (!status.lookRight) bStatus.effectOffsetX = 110;
		else bStatus.effectOffsetX = 102;
		bStatus.effectOffsetY = -10;
		break;
	case AttPattern::DeathRush:
		// 러시 준비
		if (bState == BossState::LeftReady)
		{
			if (!status.lookRight) bStatus.effectOffsetX = bStatus.effectImage->getFrameWidth()/2 - 15 * SCALE_FACTOR;
			else bStatus.effectOffsetX = -(status.width - 15 * SCALE_FACTOR - bStatus.effectImage->getFrameWidth() / 2);
			bStatus.effectOffsetY = bStatus.effectImage->getFrameHeight()/2 - 6 * SCALE_FACTOR;
		}

		// 러시 시작
		else
		{
			if (!status.lookRight) bStatus.effectOffsetX = bStatus.effectImage->getFrameWidth() / 2 + 30 * SCALE_FACTOR;
			else bStatus.effectOffsetX = -(status.width + 30 * SCALE_FACTOR - bStatus.effectImage->getFrameWidth() / 2);
			bStatus.effectOffsetY = bStatus.effectImage->getFrameHeight()/2 - 12 * SCALE_FACTOR;
		}

		break;
	case AttPattern::SiegeShoot:
		bStatus.effectOffsetX = 30 * SCALE_FACTOR;
		bStatus.effectOffsetY = -15 * SCALE_FACTOR;
		break;
	}

	// AnimOffset
	switch (bState)
	{
	case BossState::Move:
	case BossState::Apperance:
	case BossState::Idle:
		bStatus.offsetX = bStatus.originX + 0 * SCALE_FACTOR;
		bStatus.offsetY = bStatus.originY + 0 * SCALE_FACTOR;
		break;
	case BossState::AttReady:
	case BossState::LeftReady:
	case BossState::LeftAtt:
	case BossState::RightReady:
	case BossState::RightAtt:
		if (!status.lookRight)
		{
			bStatus.offsetX = bStatus.originX + 12 * SCALE_FACTOR;
			bStatus.offsetY = bStatus.originY + -4 * SCALE_FACTOR;
		}

		else
		{
			bStatus.offsetX = bStatus.originX + 6 * SCALE_FACTOR;
			bStatus.offsetY = bStatus.originY + -4 * SCALE_FACTOR;
		}
		break;
	case BossState::DeathBallShoot:
		if (!status.lookRight)
		{
			bStatus.offsetX = bStatus.originX + 2 * SCALE_FACTOR;
			bStatus.offsetY = bStatus.originY + -4 * SCALE_FACTOR;
		}

		else
		{
			bStatus.offsetX = bStatus.originX + 10 * SCALE_FACTOR;
			bStatus.offsetY = bStatus.originY + -4 * SCALE_FACTOR;
		}
		break;
	case BossState::DeathBallShootIdle:
		if (!status.lookRight)
		{
			bStatus.offsetX = bStatus.originX + -10 * SCALE_FACTOR;
			bStatus.offsetY = bStatus.originY + -4 * SCALE_FACTOR;
		}

		else
		{
			bStatus.offsetX = bStatus.originX + 18 * SCALE_FACTOR;
			bStatus.offsetY = bStatus.originY + -4 * SCALE_FACTOR;
		}
		break;
	}
}

void HighMax::stateReset(void)
{
	// 이미지 관련 리셋
	IMAGEMANAGER->findImage("HighMax_AttReady")->setChangeReady(false);
	IMAGEMANAGER->findImage("HighMax_RightReady")->setChangeReady(false);
	IMAGEMANAGER->findImage("HighMax_LeftReady")->setChangeReady(false);

	prevFrame = -1;
	if (!phase2) animSpeed = 0.06f;
	else animSpeed = 0.04f;

	bStatus.offsetX = bStatus.originX + 0 * SCALE_FACTOR;
	bStatus.offsetY = bStatus.originY + 0 * SCALE_FACTOR;

	animDir = AnimDirection::Forward;

	// 보스 상태값 관련 리셋
	SiegeSecondAtt = false;
	rushStart = false;
	timer = 0.0f;
	attTimes = 0.0f;

	// 이펙트 관련 리셋
	bStatus.effectOn = false;


	// 프레임 리셋도 시킬것
	// 이미지 클론이 필요한지도 테스트 해볼것
}