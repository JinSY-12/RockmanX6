#include "Stdafx.h"
#include "HighMax.h"
#include "BulletManager.h"
#include "Player.h"

HRESULT HighMax::init(int x, int y)
{
	status.maxHp = 80;
	status.hp = 0;
	status.physicalDamage = 4;

	bStatus.bImage = new GImage;
	status.width = 35 * SCALE_FACTOR;
	status.height = 70 * SCALE_FACTOR;

	appearEvent = false;

	spawn(x, y);

	UIMANAGER->setFreeze(true);

	// X는 RECT 왼쪽, Y는 RECT 바닥
	
	return S_OK;
}

void HighMax::update(void)
{
	bool allowInput = !(CAMERAMANAGER->getIsCamaraMove() || !CAMERAMANAGER->getCameraMoveEnd()
		|| UIMANAGER->getIsUiPrint() || deadTest);
	bStatus.movable = allowInput;
	
	if (!status.dead)
	{
		if (bStatus.movable)
		{
			// 전투 시작!
			if (appearanceDone && gameStart)
			{
				switch (bState)
				{
				case BossState::Idle:
					bStatus.bImage->play(0.08f);
					readyPattern();
					break;
				case BossState::Damaged:
					status.overpower = true;
					bStatus.bImage->play(0.08f);
					groggy();					
					break;
				case BossState::Attack:
					if (bStatus.effectOn) bStatus.effectImage->play(0.03f);

					switch (attPattern)
					{
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
					}
					break;
				case BossState::Dodge:
					dodge();
					break;
				case BossState::Dead:
					bStatus.bImage = IMAGEMANAGER->findImage("HighMax_Damaged");
					bStatus.bImage->setFrameX(1);
					status.dead = true;
					break;
				}
			}

			else if (appearanceDone && !gameStart)
			{
				hpBar.setBossVisible(true);
				battleStart();
			}

			else
			{
				bossAppearance();
			}
		
		}
	}
	
	// 죽었다고 판단되면
	else
	{
		deadAnim();
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

	if (KEYMANAGER->isOnceKeyDown('H'))
	{
		patternTimer.reset();
		reduceHp(20);
	}

	hpBar.updateBossInfop(status.hp, status.maxHp, status.mp, 0);
	hpBar.update();

	offsetFix();
	setBossHitbox();
	bossInvincibleTimerUpdate();
	damagestock();
}

void HighMax::bossAppearance(void)
{
	if (pos.y < 110 * SCALE_FACTOR)
	{
		if (!musciStart)
		{
			pos.y += 1 * SCALE_FACTOR;
		}

		else
		{
			if (!phase2) pos.y += 3 * SCALE_FACTOR;
			else pos.y += 4 * SCALE_FACTOR;
		}
	}

	else
	{
		pos.y = 110 * SCALE_FACTOR;
		bState = BossState::Attack;
		appearanceDone = true;

		appearnaceEvent();

		// 첫패턴 무조건 시즈볼
		// 시즈볼
		bState = BossState::Attack;
		attAction = Action::AttReady;
		attPattern = AttPattern::SiegeShoot;
		bStatus.bImage = IMAGEMANAGER->findImage("HighMax_AttReady")->cloneImage();

		if (!musciStart)
		{
			musciStart = true;
			SOUNDMANAGER->play("SFX_HighMaxRun", 0.4f);
		}
	}
}

void HighMax::dodge()
{	
	if (pos.y > -20)
	{
		if (attAction == Action::Dodge) status.overpower = true;
		pos.y -= 3 * SCALE_FACTOR;
	}

	else
	{
		attAction = Action::None;

		pos.y = -20 * SCALE_FACTOR;
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

	switch (attAction)
	{
		case Action::AttReady:
			if (animDir == AnimDirection::Forward)
			{
				bStatus.bImage->play(animSpeed);

				if (bStatus.bImage->getChangeReady())
				{
					bStatus.bImage->setChangeReady(false);
					bStatus.bImage = IMAGEMANAGER->findImage("HighMax_RightReady");
					attAction = Action::RightReady;

					SOUNDMANAGER->play("Voice_SiegeBall", 0.5f);
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
					attAction = Action::None;
					prevFrame = -1;
					SiegeSecondAtt = false;
					patternTime = TIMEMANAGER->getWorldTime();
				}
			}
			break;

		case Action::RightReady:
			if (animDir == AnimDirection::Forward)
			{
				bStatus.bImage->play(animSpeed);

				if (bStatus.bImage->getChangeReady())
				{
					bStatus.bImage->setChangeReady(false);
					bStatus.bImage = IMAGEMANAGER->findImage("HighMax_RightAtt");
					attAction = Action::RightAtt;
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
						attAction = Action::LeftReady;
						animDir = AnimDirection::Forward;
						SiegeSecondAtt = true;
					}

					else
					{
						bStatus.bImage->setChangeReady(false);
						bStatus.bImage = IMAGEMANAGER->findImage("HighMax_AttReady");
						attAction = Action::AttReady;
						animDir = AnimDirection::Backward;
					}
				}
			}
			break;

		case Action::RightAtt:

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
						attAction = Action::RightReady;
						bStatus.bImage = IMAGEMANAGER->findImage("HighMax_RightReady");
					}
				}
				prevFrame = bStatus.bImage->getFrameX();
			}
			break;

		case Action::LeftReady:
			if (animDir == AnimDirection::Forward)
			{
				bStatus.bImage->play(animSpeed);

				if (bStatus.bImage->getChangeReady())
				{
					bStatus.bImage->setChangeReady(false);
					bStatus.bImage = IMAGEMANAGER->findImage("HighMax_LeftAtt");
					attAction = Action::LeftAtt;
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
					attAction = Action::RightReady;
					animDir = AnimDirection::Forward;
				}
			}
			break;

		case Action::LeftAtt:
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
						attAction = Action::LeftReady;
						bStatus.bImage = IMAGEMANAGER->findImage("HighMax_LeftReady");
						
					}
				}
				prevFrame = bStatus.bImage->getFrameX();
			}
			break;
	}
}

void HighMax::deathBall(void)
{
	bStatus.effectOnTop = true;

	switch(attAction)
	{
	case Action::DeathBallShoot:
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
			SOUNDMANAGER->play("SFX_RingSound", 0.5f);
			bStatus.effectOn = false;
			bStatus.effectImage->setChangeReady(false);
			bStatus.effectImage->setFrameX(0);
			bStatus.bImage->setChangeReady(false);
			bStatus.bImage->setFrameX(0);
			makeShootEvent(BulletType::DeathBall);

			attAction = Action::DeathBallShootIdle;
			bStatus.bImage = IMAGEMANAGER->findImage("HighMax_DeathBallShootIdle")->cloneImage();
		}

		break;

	case Action::DeathBallShootIdle:
		animSpeed = 0.05f;

		if (!phase2)
		{
			if(patternTimer.update(2.5f))
			{
				bStatus.bImage->setFrameX(0);
				attAction = Action::Idle;
				bStatus.bImage = IMAGEMANAGER->findImage("HighMax_Idle")->cloneImage();
			}
		}
		else
		{
			if (patternTimer.update(2.0f))
			{
				bStatus.bImage->setFrameX(0);
				attAction = Action::Idle;
				bStatus.bImage = IMAGEMANAGER->findImage("HighMax_Idle")->cloneImage();
				random = rand() % 3;
			}
		}		
		break;

	case Action::Idle:
		animSpeed = 0.08f;
		
		// 패턴 이후 재정비
		if (random > 0)
		{
			if (patternTimer.update(2.0f))
			{
				bStatus.bImage = IMAGEMANAGER->findImage("HighMax_Move")->cloneImage();
				bState = BossState::Dodge;
				attAction = Action::None;

				SOUNDMANAGER->play("SFX_HighMaxRun", 0.5f);
			}
		}
		
		// 패턴 이후 다시 공격
		else
		{
			if (patternTimer.update(2.0f))
			{
				bState = BossState::Idle;
				attAction = Action::None;
				patternTime = TIMEMANAGER->getWorldTime();
			}
		}
		break;
	}

}

void HighMax::deathRush(void)
{
	if (!phase2) animSpeed = 0.06;
	else animSpeed = 0.04;

	switch (attAction)
	{
	case Action::AttReady:
		bStatus.bImage->play(animSpeed);
		if (bStatus.bImage->getChangeReady())
		{
			bStatus.bImage->setChangeReady(false);
			attAction = Action::RightReady;
			bStatus.bImage = IMAGEMANAGER->findImage("HighMax_RightReady")->cloneImage();
		}
		break;
	case Action::RightReady:
		bStatus.bImage->play(animSpeed);
		if (bStatus.bImage->getChangeReady())
		{
			bStatus.bImage->setChangeReady(false);
			attAction = Action::LeftReady;
			bStatus.bImage = IMAGEMANAGER->findImage("HighMax_LeftReady")->cloneImage();
			bStatus.effectImage = IMAGEMANAGER->findImage("SFX_RushCharge")->cloneImage();
		}
		break;
	case Action::LeftReady:
		bStatus.bImage->play(animSpeed);

		bStatus.effectOnTop = false;
		bStatus.effectOn = true;
		
		if (bStatus.effectImage->getChangeReady())
		{
			effPattern = EffectState::RushBall;
			bStatus.effectImage->setChangeReady(false);
			bStatus.effectImage = IMAGEMANAGER->findImage("SFX_RushBall")->cloneImage();
			SOUNDMANAGER->play("Voice_Mudada", 0.3f);
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
						attAction = Action::RightAtt;
						bStatus.bImage = IMAGEMANAGER->findImage("HighMax_RightAtt");
					}

				}
				
				else
				{
					if (pos.x < 6120 * SCALE_FACTOR) pos.x += 4 * SCALE_FACTOR;
					else
					{
						attAction = Action::RightAtt;
						bStatus.bImage = IMAGEMANAGER->findImage("HighMax_RightAtt");
					}
				}
			}
		}
		break;
	case Action::RightAtt:
		bStatus.effectOnTop = true;
		bStatus.effectCollisionOn = true;

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

				if (patternTimer.update(1.5f))
				{
					bStatus.effectOn = false;
					attAction = Action::Idle;
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
				if (patternTimer.update(1.5f))
				{
					bStatus.effectOn = false;
					bStatus.effectCollisionOn = false;

					attAction = Action::Idle;
					bStatus.bImage = IMAGEMANAGER->findImage("HighMax_Idle");
				}
			}
		}
		break;
	
	case Action::Idle:
		animSpeed = 0.08f;
		if (patternTimer.update(1.0f))
		{
			animSpeed = 0.06f;
			bState = BossState::Dodge;
			attAction = Action::None;
			bStatus.bImage = IMAGEMANAGER->findImage("HighMax_Move")->cloneImage();
			SOUNDMANAGER->play("SFX_HighMaxRun", 0.3f);
		}
		break;
	}
}

void HighMax::groggy(void)
{
	// 일정 pos.y까지 떨어뜨리기
	if (pos.y < 190 * SCALE_FACTOR)
	{
		if (!status.lookRight)
		{
			if (pos.x < 6365 * SCALE_FACTOR) pos.x += 1 * SCALE_FACTOR;
			else pos.x = 6365 * SCALE_FACTOR;
		}
		else
		{
			if (pos.x > 6080 * SCALE_FACTOR) pos.x -= 1 * SCALE_FACTOR;
			else pos.x = 6080 * SCALE_FACTOR;
		}

		pos.y += gravity;
		if (gravity <= maxGravity) gravity += gravityAccel;
		else gravity = maxGravity;
	}

	else
	{
		soundSupport.play("SFX_BossDrop", 0.3f);
		pos.y = 190 * SCALE_FACTOR;
		gravity = 0.0f;

		if (patternTimer.update(15.0f))
		{
			soundSupport.playReset();
			bStatus.bImage = IMAGEMANAGER->findImage("HighMax_Move");
			bState = BossState::Dodge;
			attAction = Action::Dodge;

			SOUNDMANAGER->play("SFX_HighMaxRun", 0.3f);
		}
	}
}

void HighMax::readyPattern(void)
{
	if (!phase2) pattenrCoolDown = 2.0f;
	else pattenrCoolDown = 1.5f;

	if (TIMEMANAGER->getWorldTime() - patternTime > pattenrCoolDown)
	{
		bState = BossState::Attack;

		int rnd = rand() % 10;

		// 데스볼 샷
		if (rnd >= 4)
		{
			attPattern = AttPattern::DeathBallShoot;
			attAction = Action::DeathBallShoot;
			bStatus.bImage = IMAGEMANAGER->findImage("HighMax_DeathBallShoot")->cloneImage();
			SOUNDMANAGER->play("Voice_DeathBall", 0.3f);
		}

		// 데스볼 러시
		else
		{
			attPattern = AttPattern::DeathRush;
			attAction = Action::AttReady;
			bStatus.bImage = IMAGEMANAGER->findImage("HighMax_AttReady")->cloneImage();
		}
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
		if (attAction == Action::LeftReady)
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
	switch (attAction)
	{
	case Action::None:
	case Action::Idle:
		bStatus.offsetX = bStatus.originX + 0 * SCALE_FACTOR;
		bStatus.offsetY = bStatus.originY + 0 * SCALE_FACTOR;
		break;
	case Action::AttReady:
	case Action::LeftReady:
	case Action::LeftAtt:
	case Action::RightReady:
	case Action::RightAtt:
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
	case Action::DeathBallShoot:
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
	case Action::DeathBallShootIdle:
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
	if (!phase2) animSpeed = 0.08f;
	else animSpeed = 0.06f;

	bStatus.offsetX = bStatus.originX + 0 * SCALE_FACTOR;
	bStatus.offsetY = bStatus.originY + 0 * SCALE_FACTOR;

	// attAction = Action::None;
	animDir = AnimDirection::Forward;

	// 보스 상태값 관련 리셋
	SiegeSecondAtt = false;
	rushStart = false;
	timer = 0.0f;
	attTimes = 0.0f;

	// 이펙트 관련 리셋
	bStatus.effectOn = false;
	bStatus.effectCollisionOn = false;

	// 프레임 리셋도 시킬것
	// 이미지 클론이 필요한지도 테스트 해볼것
}

void HighMax::damagestock()
{
	if (stockDamage >= 20)
	{
		stockDamage = 0;
		knockOutCount++;
		
		if (status.hp > 0)
		{
			stateReset();
			bStatus.bImage = IMAGEMANAGER->findImage("HighMax_Damaged");
			bState = BossState::Damaged;
			SOUNDMANAGER->play("Voice_HighMaxDamaged", 0.3f);
		}
	}

	if (knockOutCount >= 2) phase2 = true;
}

void HighMax::spawn(int x, int y)
{
	status.type = CombatEntityType::Boss;
	btype = BossType::Intro;
	bState = BossState::Apperance;
	status.lastBoss = true;

	pos.x = x;
	pos.y = y;

	bStatus.bImage = IMAGEMANAGER->findImage("HighMax_Move")->cloneImage();
	bStatus.effectImage = IMAGEMANAGER->findImage("SFX_DeathBallCharge")->cloneImage();

	hpBar.init(BossType::Intro, status.maxHp);
	hpBar.setPlayerInfo(static_cast<int>(status.hp), static_cast<int>(status.maxHp), static_cast<int>(status.mp), 30);

	status.overpower = false;
	status.lookRight = false;
	bStatus.effectCollisionOn = false;

	bStatus.bHitBox = RectMakeCenter(x + status.width / 2, y - status.height / 2, status.width, status.height);

	if (!status.lookRight) bStatus.originX = 30 * SCALE_FACTOR;
	else bStatus.originX = 22 * SCALE_FACTOR;

	bStatus.originY = 20 * SCALE_FACTOR;

	bStatus.offsetX = bStatus.originX + 0 * SCALE_FACTOR;
	bStatus.offsetY = bStatus.originY + 0 * SCALE_FACTOR;

	animSpeed = 0.1f;
	attTimes = 0;

	animDir = AnimDirection::Forward;
	attPattern = AttPattern::Idle;
	effPattern = EffectState::BallCharge;

	bStatus.effectOn = false;
	bStatus.effectOnTop = true;

	bStatus.invincibleMaxTime = 2.0f;

	SiegeSecondAtt = false;
	prevFrame = -1;
	timer = 0.0f;
	phase2 = false;
	BossBGM = "BGM_VS_HighMax";
	appearanceDone = false;
	musciStart = false;
	attCycle = false;
	rushStart = false;
	gameStart = false;

	bossAlpha = 255;
	pattenrCoolDown = 2.0f;
	stockDamage = 0;
	knockOutCount = 0;

	
	// 사망 테스트
	deadTest = false;
}

void HighMax::appearnaceEvent(void)
{
	switch (SCENEMANAGER->getStageBossType())
	{
	case BossType::Intro:
		UIMANAGER->addUi(UiType::EventDialogue, 1);
		break;
	default:
		break;
	}
}
