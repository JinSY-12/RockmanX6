#include "Stdafx.h"
#include "Player.h"
#include "BulletManager.h"

void Player::render(HDC memDC)
{
	pStatus.player->frameAlphaRender(memDC, hitBoxCenter.x - pStatus.player->getFrameWidth() / 2 + animOffset.x,
		pStatus.hitBox.bottom - pStatus.player->getFrameHeight() + animOffset.y,
		pStatus.player->getFrameX(), status.lookRight, charAlpha);

	afterImageControl();

	if (chargeEffect != nullptr) chargeEffect->frameAlphaRender(memDC, hitBoxCenter.x - chargeEffect->getFrameWidth() / 2 + 3 * SCALE_FACTOR,
		hitBoxCenter.y - chargeEffect->getFrameHeight() + 10 * SCALE_FACTOR,
		chargeEffect->getFrameX(), status.lookRight, chargeEffectAlpha);

	if (chargeAura != nullptr) chargeAura->frameAlphaRender(memDC, hitBoxCenter.x - chargeEffect->getFrameWidth() / 2 + 3 * SCALE_FACTOR,
		hitBoxCenter.y - chargeAura->getFrameHeight() + 10 * SCALE_FACTOR,
		chargeAura->getFrameX(), status.lookRight, chargeAuraAlpha);

	int aimX;
	aimX = status.lookRight != (currentState == CharacterState::WallSlide) ? pStatus.hitBox.right - (busterPos.x + pStatus.firePointX) : pStatus.hitBox.left - attackHandEffect->getFrameWidth() + (busterPos.x + pStatus.firePointX);
	bool dir = status.lookRight != (currentState == CharacterState::WallSlide) ? true : false;

	if (attackHandEffect != nullptr) attackHandEffect->frameAlphaRender(memDC, aimX,
		pStatus.hitBox.top - attackHandEffect->getFrameHeight() / 2 + pStatus.firePointY + busterPos.y,
		attackHandEffect->getFrameX(), dir, bursterEffectAlpha);

	
	if (UIMANAGER->getIsDebugMode() == true)
	{
		// 캐릭터 좌표
		string temp1;
		if (pStatus.isDash) temp1 = "O";
		else temp1 = "X";

		TEXTMANAGER->drawTextColor(memDC, WINSIZE_X / 50, WINSIZE_Y / 100, "현재 상태", "DNF_M_18", RGB(0, 255, 255));
		TEXTMANAGER->drawTextColor(memDC, WINSIZE_X / 50, WINSIZE_Y / 100 + 20, printBodyState(), "DNF_M_18", RGB(0, 255, 255));
								   
		TEXTMANAGER->drawTextColor(memDC, WINSIZE_X / 50, WINSIZE_Y / 100 + 45, "스피드", "DNF_M_18", RGB(0, 255, 255));
		TEXTMANAGER->drawTextColor(memDC, WINSIZE_X / 50, WINSIZE_Y / 100 + 65, to_string(pStatus.velocityX), "DNF_M_18", RGB(0, 255, 255));

		TEXTMANAGER->drawTextColor(memDC, WINSIZE_X / 50 + 80, WINSIZE_Y / 100, "애니메이션", "DNF_M_18", RGB(0, 255, 255));
		TEXTMANAGER->drawTextColor(memDC, WINSIZE_X / 50 + 80, WINSIZE_Y / 100 + 20, currentAnim, "DNF_M_18", RGB(0, 255, 255));
								   
		TEXTMANAGER->drawTextColor(memDC, WINSIZE_X / 50 + 80, WINSIZE_Y / 100 + 45, "점프 파워", "DNF_M_18", RGB(0, 255, 255));
		TEXTMANAGER->drawTextColor(memDC, WINSIZE_X / 50 + 80, WINSIZE_Y / 100 + 65, to_string(pStatus.velocityY), "DNF_M_18", RGB(0, 255, 255));
								  
		TEXTMANAGER->drawTextColor(memDC, WINSIZE_X / 50 + 160, WINSIZE_Y / 100, "캐릭터 X", "DNF_M_18", RGB(0, 255, 255));
		TEXTMANAGER->drawTextColor(memDC, WINSIZE_X / 50 + 160, WINSIZE_Y / 100 + 20, to_string(pos.x), "DNF_M_18", RGB(0, 255, 255));
								 
		TEXTMANAGER->drawTextColor(memDC, WINSIZE_X / 50 + 240, WINSIZE_Y / 100, "캐릭터 Y", "DNF_M_18", RGB(0, 255, 255));
		TEXTMANAGER->drawTextColor(memDC, WINSIZE_X / 50 + 240, WINSIZE_Y / 100 + 20, to_string(pos.y), "DNF_M_18", RGB(0, 255, 255));
								   
		TEXTMANAGER->drawTextColor(memDC, WINSIZE_X / 50 + 160, WINSIZE_Y / 100 + 45, "카메라 X", "DNF_M_18", RGB(0, 255, 255));
		TEXTMANAGER->drawTextColor(memDC, WINSIZE_X / 50 + 160, WINSIZE_Y / 100 + 65, to_string(CAMERAMANAGER->getCameraPos().x / 3), "DNF_M_18", RGB(0, 255, 255));
								   
		TEXTMANAGER->drawTextColor(memDC, WINSIZE_X / 50 + 240, WINSIZE_Y / 100 + 45, "카메라 Y", "DNF_M_18", RGB(0, 255, 255));
		TEXTMANAGER->drawTextColor(memDC, WINSIZE_X / 50 + 240, WINSIZE_Y / 100 + 65, to_string(CAMERAMANAGER->getCameraPos().y / 3), "DNF_M_18", RGB(0, 255, 255));

		string temp;
		if (CAMERAMANAGER->getLockX()) temp = "O";
		else temp = "X";

		TEXTMANAGER->drawTextColor(memDC, WINSIZE_X / 50 + 320, WINSIZE_Y / 100, "카메라 Lock X", "DNF_M_18", RGB(0, 255, 255));
		TEXTMANAGER->drawTextColor(memDC, WINSIZE_X / 50 + 320, WINSIZE_Y / 100 + 20, temp, "DNF_M_18", RGB(0, 255, 255));

		if (CAMERAMANAGER->getLockY()) temp = "O";
		else temp = "X";

		TEXTMANAGER->drawTextColor(memDC, WINSIZE_X / 50 + 320, WINSIZE_Y / 100 + 45, "카메라 Lock Y", "DNF_M_18", RGB(0, 255, 255));
		TEXTMANAGER->drawTextColor(memDC, WINSIZE_X / 50 + 320, WINSIZE_Y / 100 + 65, temp, "DNF_M_18", RGB(0, 255, 255));

		if (status.lookRight) temp = "우측";
		else temp = "좌축";

		TEXTMANAGER->drawTextColor(memDC, WINSIZE_X / 50 + 420, WINSIZE_Y / 100, "시야 방향", "DNF_M_18", RGB(0, 255, 255));
		TEXTMANAGER->drawTextColor(memDC, WINSIZE_X / 50 + 420, WINSIZE_Y / 100 + 20, temp, "DNF_M_18", RGB(0, 255, 255));

		TEXTMANAGER->drawTextColor(memDC, WINSIZE_X / 50 + 420, WINSIZE_Y / 100 + 45, "캐릭터 위치", "DNF_M_18", RGB(0, 255, 255));
		TEXTMANAGER->drawTextColor(memDC, WINSIZE_X / 50 + 420, WINSIZE_Y / 100 + 65, CAMERAMANAGER->getZoneNum(), "DNF_M_18", RGB(0, 255, 255));

		if (pStatus.isDash) temp = "O";
		else temp = "X";

		TEXTMANAGER->drawTextColor(memDC, WINSIZE_X / 50 + 500, WINSIZE_Y / 100, "대시 중", "DNF_M_18", RGB(0, 255, 255));
		TEXTMANAGER->drawTextColor(memDC, WINSIZE_X / 50 + 500, WINSIZE_Y / 100 + 20, temp, "DNF_M_18", RGB(0, 255, 255));

		if (pStatus.isJumpDash) temp = "O";
		else temp = "X";

		TEXTMANAGER->drawTextColor(memDC, WINSIZE_X / 50 + 500, WINSIZE_Y / 100 + 45, "대시 점프 중", "DNF_M_18", RGB(0, 255, 255));
		TEXTMANAGER->drawTextColor(memDC, WINSIZE_X / 50 + 500, WINSIZE_Y / 100 + 65, temp, "DNF_M_18", RGB(0, 255, 255));

		if (ladderAble) temp = "O";
		else temp = "X";

		TEXTMANAGER->drawTextColor(memDC, WINSIZE_X / 50 + 580, WINSIZE_Y / 100, "사다리 가능", "DNF_M_18", RGB(0, 255, 255));
		TEXTMANAGER->drawTextColor(memDC, WINSIZE_X / 50 + 580, WINSIZE_Y / 100 + 20, temp, "DNF_M_18", RGB(0, 255, 255));

		if (pStatus.isOnLadder) temp = "O";
		else temp = "X";

		TEXTMANAGER->drawTextColor(memDC, WINSIZE_X / 50 + 580, WINSIZE_Y / 100 + 45, "사다리 매달림", "DNF_M_18", RGB(0, 255, 255));
		TEXTMANAGER->drawTextColor(memDC, WINSIZE_X / 50 + 580, WINSIZE_Y / 100 + 65, temp, "DNF_M_18", RGB(0, 255, 255));

		// 히트박스 출력
		DrawRectMakeColor(memDC, pStatus.hitBox, RGB(255, 0, 0), 2);
		DrawRectMakeColor(memDC, pStatus.floorCheck, RGB(0, 0, 255), 2);
		DrawRectMakeColor(memDC, pStatus.subRect, RGB(255, 0, 255), 2);
		// if(canHit) DrawRectMakeColor(memDC(), pStatus.saberHitBox, RGB(255, 0, 255), 4);
	}
	
}

void Player::move(bool direction)
{
	if (pStatus.movable && !pStatus.isOnLadder)
	{
		if (pStatus.isJumpDash)
		{
			if (status.lookRight && !pStatus.touchRight) pStatus.velocityX = dashSpeed;
			else if (!status.lookRight && !pStatus.touchLeft) pStatus.velocityX = -dashSpeed;
			// 이동 막기 - 테스트용
			// pStatus.velocityX = 0;
		}

		else if (pStatus.isDash)
		{
			if (status.lookRight && !pStatus.touchRight) pStatus.velocityX = lerp(pStatus.velocityX, dashSpeed, 1.0f);
			else if (!status.lookRight && !pStatus.touchLeft) pStatus.velocityX = -lerp(pStatus.velocityX, dashSpeed, 1.0f);
			else
			{
				currentState = CharacterState::Idle;
				pStatus.velocityX = 0.0f;
			}
			// 이동 막기 - 테스트용
			// pStatus.velocityX = 0;
		}

		else if (!pStatus.isDash)
		{
			if (pStatus.isOnGround) currentState = CharacterState::Walk;

			float moveSpeed;
			moveSpeed = direction ? pStatus.moveSpeed : -pStatus.moveSpeed;
			// 이동 막기 - 테스트용
			// moveSpeed = 0;
			pStatus.velocityX = moveSpeed;
		}

		pStatus.isWallSlide = false;
	}
}

void Player::jump(void)
{
	// 일반 점프
	if (pStatus.isOnGround)  // 땅에 있을 때만 점프
	{
		pStatus.isJumpUp = true;
		currentState = CharacterState::JumpUp;

		pStatus.velocityY = pStatus.jumpPower;
		
		pStatus.hitBox.bottom -= 8;
		pStatus.hitBox.top -= 8;
		pos.y -= 8;
		// charPos.y -= 8;

		if (pStatus.isDash) pStatus.isJumpDash = true;
	}

	// 벽차기
	else
	{
		if ((pStatus.touchLeft && !status.lookRight) || (pStatus.touchRight && status.lookRight)) wallKick();
	}
}

void Player::dash(bool direction)
{
	pStatus.velocityX = 0.0f;

	isMoving = false;

	if (direction == true && !pStatus.touchRight)
	{
		currentState = CharacterState::Dash;
		pStatus.isDash = true;
		dashSpeed = pStatus.dashSpeed;
	}

	else if (direction == false && !pStatus.touchLeft)
	{
		currentState = CharacterState::Dash;
		pStatus.isDash = true;
		dashSpeed = pStatus.dashSpeed;
	}

	else dashSpeed = 0.0f;
}

void Player::sfxPlay(void)
{
	// 추가 해야 할 것

	// 아머 획득 사운드
	// 약한, 강한 피격음
	// 점프 사운드

	if (previousState != currentState)
	{
		if (currentState == CharacterState::Warp)
		{
			soundResult = "SFX_" + pStatus.charName + "WarpIn";
			SOUNDMANAGER->play(soundResult, 0.5f);
		}

		else if (currentState == CharacterState::JumpUp)
		{
			int random = RND->getInt(4);
			if (random == 0);
			else SOUNDMANAGER->play("Voice_" + pStatus.charName + "Jump" + to_string(random), 0.5f);
			SOUNDMANAGER->play("SFX_Jump", 0.5f);
		}

		else if (currentState == CharacterState::WallKick)
		{
			SOUNDMANAGER->play("Voice_" + pStatus.charName + "Jump1", 0.5f);
		}

		else if (currentState == CharacterState::Dash)
		{
			soundResult = "SFX_DashStart";
			SOUNDMANAGER->play(soundResult, 0.5f);
		}

		else if (currentState == CharacterState::Idle && previousState == CharacterState::Dash)
		{
			// soundResult = "SFX_DashEnd";
			// SOUNDMANAGER->play(soundResult, 0.5f);
		}

		else if (currentState == CharacterState::WallSlide)
		{
			soundResult = "SFX_WallTouch";
			SOUNDMANAGER->play(soundResult, 0.5f);
		}

		// 기능 미구현
		/*
		// 대시 시작

		// 대시 끝

		// 착지
		else if (currentState == CharacterState::Land)
		{
			soundResult = "SFX_" + pStatus.charName + "DashEnd";
			SOUNDMANAGER->play(soundResult, 0.5f);
		}

		// 벽차기
		else if (previousState == CharacterState::Wall && currentState == CharacterState::JumpUp)
		{
			soundResult = "SFX_" + pStatus.charName + "WallKick";
			SOUNDMANAGER->play(soundResult, 0.5f);

			soundResult = "Voice_" + pStatus.charName + "WallKick";
			SOUNDMANAGER->play(soundResult, 0.5f);
		}
		*/
		}
	
		// X 전용 사운드
		if (pStatus.charName == "X_")
		{
			if (previousAnim != currentAnim)
			{
				if (currentAnim.find("Saber") != string::npos)
				{
					soundResult = "SFX_" + pStatus.charName + "Saber";
					SOUNDMANAGER->play(soundResult, 0.5f);
				}
			}
		}

		// Zero 전용 사운드
		else if (pStatus.charName == "Zero_")
		{

		}
}

void Player::soundPlay(string soundName)
{
	
}

void Player::wallSlide(void)
{
	currentState = CharacterState::WallSlide;
	pStatus.isWallSlide = true;
	pStatus.velocityY = 1.9f;
}

void Player::wallDrop(void)
{
	if (currentState == CharacterState::WallSlide)
	{
		currentState = CharacterState::FallingDown;

		int fall;
		if (status.lookRight) fall = -4;
		else fall = 4;

		if (CAMERAMANAGER->getLockX() == true)
		{
			pStatus.hitBox.left += fall;
			pStatus.hitBox.right += fall;
			pos.x += fall;
			// charPos.x += fall;
		}
		else pos.x += fall;
			//charPos.x += fall;

		pStatus.isWallSlide = false;
	}
}

void Player::wallKick(void)
{
	currentState = CharacterState::WallKick;
	
	pStatus.velocityY = -9.0f;
	pStatus.isWallKick = true;

	int kickOffset;
	kickOffset = status.lookRight ? 0 : pStatus.player->getFrameWidth() / 2;
	EFFECTMANAGER->spawnEffect(EffectType::WallKick, pos.x - kickOffset, pos.y, pStatus.player->getFrameWidth(), pStatus.player->getFrameHeight(), status.lookRight);
	// EFFECTMANAGER->spawnEffect(EffectType::WallKick, charPos.x - kickOffset, charPos.y, pStatus.player->getFrameWidth(), pStatus.player->getFrameHeight(), pStatus.lookRight);

	pStatus.wallKickRight = status.lookRight;

	if (pressDash == true)
	{
		pStatus.isJumpDash = true;
		pStatus.velocityX = status.lookRight ? -pStatus.dashSpeed : pStatus.dashSpeed;
	}

	else pStatus.velocityX = status.lookRight ? -5.0f : 5.0f;

	pStatus.isWallSlide = false;
}

void Player::ladderClimb(void)
{
	if (ladderAble)
	{
		// 사다리 타기 실행
	}

	else ;
}

void Player::attack(void)
{
	// Do Nothing!!	
}

void Player::specialAttack(void)
{
	// Do Nothing!!
}

void Player::applyForce(void)
{
#pragma region 특수 상황

	// 벽차기 타이머
	if (pStatus.isWallKick)
	{	
		wallkickTimer += 0.1f;
		
		if (wallkickTimer >= wallkickMaxTime)
		{
			pStatus.isWallKick = false;
			wallkickTimer = 0.0f;
			pStatus.velocityX = 0.0f;
		}
	}
	
#pragma endregion

#pragma region X축, Y축 이동

	// X축 이동 - 기본 베이스
	if (CAMERAMANAGER->getLockX() == true)
	{
		// 카메라 왼쪽으로 이동 막기
		if (pos.x - 12 * SCALE_FACTOR + pStatus.velocityX <= CAMERAMANAGER->getCameraRange().left)
		{
			pos.x = CAMERAMANAGER->getCameraRange().left + 12 * SCALE_FACTOR;
			currentState = CharacterState::Idle;
		}

		// 카메라 오른쪽으로 이동 막기
		else if (pos.x + 12 * SCALE_FACTOR + pStatus.velocityX >= CAMERAMANAGER->getCameraRange().right && !CAMERAMANAGER->getIsCamaraMove())
		{
			pos.x = CAMERAMANAGER->getCameraRange().right - 12 * SCALE_FACTOR;
			currentState = CharacterState::Idle;
		}

		else
		{
			pStatus.hitBox.left += pStatus.velocityX;
			pStatus.hitBox.right += pStatus.velocityX;
			pos.x += pStatus.velocityX;
		}
	}

	else pos.x += pStatus.velocityX;
		
	// 워프 - 존나 빠르게
	if (!pStatus.isOnGround && currentState == CharacterState::Warp)
	{
		if (CAMERAMANAGER->getLockY() == true)
		{
			pos.y += 16;
			pStatus.hitBox.top += 16;
			pStatus.hitBox.bottom += 16;
		}

		else pos.y += 16;
	}

	// Y축 이동 - 기본 베이스
	else if (!pStatus.isOnGround && !CAMERAMANAGER->getIsCamaraMove() )
	{
		pStatus.isDash = false;

		if (!pStatus.isOnLadder)
		{
			// 중력 가속도 추가
			if (!pStatus.isWallKick) pStatus.velocityY += progress.gravityAccel;

			// 최대 낙하 속도 제한
			if (pStatus.velocityY > pStatus.maxFallSpeed)
				pStatus.velocityY = pStatus.maxFallSpeed;
		}

		// 기본 중력
		if (CAMERAMANAGER->getLockY() == true)
		{
			pos.y += pStatus.velocityY;
			pStatus.hitBox.top += pStatus.velocityY;
			pStatus.hitBox.bottom += pStatus.velocityY;
		}

		else pos.y += pStatus.velocityY;
		
		// 공중에서 상태 변경
		if (!pStatus.isOnLadder)
		{
			if (currentState == CharacterState::JumpUp)	if (pStatus.velocityY > -7.0f) pStatus.isJumpUp = false;
			if (pStatus.velocityY > 0.0f && !pStatus.isWallSlide && currentState != CharacterState::OverPower) currentState = CharacterState::FallingDown;
		}
	}

#pragma endregion
	
}

void Player::spawn(int x, int y)
{
	// Do Nothing!!
}

void Player::currentAnimChange(void)
{
	////////////////////////
	// 워프
	////////////////////////
	
	if (currentState == CharacterState::Warp)
	{
		if (pStatus.charName == "X_")
		{
			animSpeed = 0.07f;
			animOffset.x = 0 * SCALE_FACTOR;
			animOffset.y = 16 * SCALE_FACTOR; // 16픽셀 아래로 = 워프 이펙트와 발 위치가 다름
		}

		changeAnimation(pStatus.charName + "Spawn", 0);
	}

	////////////////////////
	// 대기
	////////////////////////

	else if (currentState == CharacterState::Idle)
	{
		pStatus.firePointX = 0 * SCALE_FACTOR;
		pStatus.firePointY = 10 * SCALE_FACTOR;

		switch (attState)
		{
		case SholderState::LargeBurst:
			animSpeed = 0.07f;
			animOffset.x = 0 * SCALE_FACTOR;
			animOffset.y = 0 * SCALE_FACTOR;
			changeAnimation(pStatus.charName + "StandChargeBurst", 0);
			break;

		case SholderState::Burst:
			animSpeed = 0.07f;
			animOffset.x = 0 * SCALE_FACTOR;
			animOffset.y = 0 * SCALE_FACTOR;
			changeAnimation(pStatus.charName + "StandBurstLoop", 0);
			break;

		case SholderState::Hold:
			animSpeed = 0.1f;
			animOffset.x = 0 * SCALE_FACTOR;
			animOffset.y = 0 * SCALE_FACTOR;
			changeAnimation(pStatus.charName + "StandBurstEnd", 0);
			break;

		case SholderState::None:
			animSpeed = 0.17f;
			animOffset.x = 0 * SCALE_FACTOR;
			animOffset.y = 0 * SCALE_FACTOR;
			changeAnimation(pStatus.charName + "Idle", 0);
			break;

		case SholderState::Special:
			if (pStatus.charName == "X_")
			{
				if (previousState == CharacterState::JumpUp || previousState == CharacterState::FallingDown)
				{
					attState = SholderState::None;
					pStatus.movable = true;
					pStatus.isAttack = false;
				}

				else
				{
					animSpeed = 0.045f;
					animOffset.x = status.lookRight ? 27 * SCALE_FACTOR : -27 * SCALE_FACTOR;
					animOffset.y = 9 * SCALE_FACTOR;
					changeAnimation("X_Saber", 0);
				}
			}
			break;
		}
	}

	////////////////////////
	// 걷기
	////////////////////////

	else if (currentState == CharacterState::Walk)
	{
		pStatus.firePointX = -6 * SCALE_FACTOR;
		pStatus.firePointY = 8 * SCALE_FACTOR;

		if (!isMoving)
		{
			switch (attState)
			{
			case SholderState::Burst:
			case SholderState::LargeBurst:
				animSpeed = 0.1f;
				animOffset.x = 0 * SCALE_FACTOR;
				animOffset.y = 0 * SCALE_FACTOR;
				changeAnimation(pStatus.charName + "WalkBurstStart", 0);
				break;
			case SholderState::Hold:
			case SholderState::None:
				animSpeed = 0.1f;
				animOffset.x = 0 * SCALE_FACTOR;
				animOffset.y = 0 * SCALE_FACTOR;
				changeAnimation(pStatus.charName + "WalkStart", 0);
				break;
			case SholderState::Special:
				if (pStatus.charName == "X_")
				{
					if (previousState == CharacterState::JumpUp || previousState == CharacterState::FallingDown)
					{
						attState = SholderState::None;
						pStatus.movable = true;
						pStatus.isAttack = false;
					}

					else
					{
						animSpeed = 0.045f;
						animOffset.x = status.lookRight ? 27 * SCALE_FACTOR : -27 * SCALE_FACTOR;
						animOffset.y = 9 * SCALE_FACTOR;
						changeAnimation("X_Saber", 0);
					}
				}
				break;
			}

			if (pStatus.player->getFrameX() >= pStatus.player->getMaxFrameX()) isMoving = true;
		}

		else if (isMoving)
		{
			switch (attState)
			{
			case SholderState::Burst:
			case SholderState::LargeBurst:
				animSpeed = 0.04f;

				animOffset.x = 0 * SCALE_FACTOR;
				animOffset.y = 0 * SCALE_FACTOR;
				if (previousAnim == pStatus.charName + "WalkBurstStart") changeAnimation(pStatus.charName + "WalkBurstLoop", 0);
				else changeAnimation(pStatus.charName + "WalkBurstLoop", pStatus.player->getFrameX());
				break;
			case SholderState::Hold:
			case SholderState::None:
				animSpeed = 0.04f;
				animOffset.x = 0 * SCALE_FACTOR;
				animOffset.y = 0 * SCALE_FACTOR;
				if (previousAnim == pStatus.charName + "WalkStart") changeAnimation(pStatus.charName + "WalkLoop", 0);
				else changeAnimation(pStatus.charName + "WalkLoop", pStatus.player->getFrameX());
				break;
			case SholderState::Special:
				if (pStatus.charName == "X_")
				{
					if (previousState == CharacterState::JumpUp || previousState == CharacterState::FallingDown)
					{
						attState = SholderState::None;
						pStatus.movable = true;
						pStatus.isAttack = false;
					}
					else
					{
						animSpeed = 0.045f;
						animOffset.x = status.lookRight ? 27 * SCALE_FACTOR : -27 * SCALE_FACTOR;
						animOffset.y = 9 * SCALE_FACTOR;
						changeAnimation("X_Saber", 0);
					}
				}
				break;
			}
		}
	}


	////////////////////////
	// 점프
	////////////////////////

	else if (currentState == CharacterState::JumpUp)
	{
		pStatus.firePointX = 1 * SCALE_FACTOR;
		pStatus.firePointY = 5 * SCALE_FACTOR;

		switch (attState)
		{
		case SholderState::Burst:
		case SholderState::LargeBurst:
			animSpeed = 0.06f;
			
			animOffset.x = 0 * SCALE_FACTOR;
			animOffset.y = 0 * SCALE_FACTOR;
			if (previousAnim == pStatus.charName + "Jump") changeAnimation(pStatus.charName + "JumpBurst", pStatus.player->getFrameX());
			else changeAnimation(pStatus.charName + "JumpBurst", 0);
			break;
		case SholderState::Hold:
		case SholderState::None:
			animSpeed = 0.06f;
			
			animOffset.x = 0 * SCALE_FACTOR;
			animOffset.y = 0 * SCALE_FACTOR;
			if (previousAnim == pStatus.charName + "JumpBurst") changeAnimation(pStatus.charName + "Jump", pStatus.player->getFrameX());
			else if (previousAnim == pStatus.charName + "JumpSaber") changeAnimation(pStatus.charName + "Jump", 4);
			else changeAnimation(pStatus.charName + "Jump", 0);
			break;
		case SholderState::Special:
			if (pStatus.charName == "X_")
			{
				if (previousState == CharacterState::WallKick)
				{
					changeAnimation(pStatus.charName + "Jump", 0);
					pStatus.movable = true;
					pStatus.isAttack = false;
				}
				else
				{
					animSpeed = 0.045f;

					animOffset.x = status.lookRight ? 11 * SCALE_FACTOR : -11 * SCALE_FACTOR;
					animOffset.y = -1 * SCALE_FACTOR;
					changeAnimation("X_JumpSaber", 0);
				}
			}
			break;

		default:
			break;
		}

		// 점프 도중 모션 고정
		if (attState != SholderState::Special && pStatus.player->getFrameX() >= 4) pStatus.player->setFrameX(4);
	}

	////////////////////////
	// 낙하
	////////////////////////

	else if (currentState == CharacterState::FallingDown)
	{
		pStatus.firePointX = 1 * SCALE_FACTOR;
		pStatus.firePointY = 5 * SCALE_FACTOR;

		switch (attState)
		{
		case SholderState::Burst:
		case SholderState::LargeBurst:
			animSpeed = 0.06f;

			animOffset.x = 0 * SCALE_FACTOR;
			animOffset.y = 0 * SCALE_FACTOR;
			if (previousAnim == pStatus.charName + "Jump") changeAnimation(pStatus.charName + "JumpBurst", pStatus.player->getFrameX());
			else changeAnimation(pStatus.charName + "JumpBurst", 5);
			break;
		case SholderState::Hold:
		case SholderState::None:
			animSpeed = 0.06f;

			animOffset.x = 0 * SCALE_FACTOR;
			animOffset.y = 0 * SCALE_FACTOR;
			if (previousAnim == pStatus.charName + "JumpBurst") changeAnimation(pStatus.charName + "Jump", pStatus.player->getFrameX());
			else changeAnimation(pStatus.charName + "Jump", 5);
			break;
		case SholderState::Special:
			if (pStatus.charName == "X_")
			{
				if (previousState == CharacterState::WallKick || previousState == CharacterState::WallSlide)
				{
					attState = SholderState::None;
					pStatus.movable = true;
					pStatus.isAttack = false;
				}
				else
				{
					animSpeed = 0.045f;
					
					animOffset.x = status.lookRight ? 11 * SCALE_FACTOR : -11 * SCALE_FACTOR;
					animOffset.y = -1 * SCALE_FACTOR;
					changeAnimation("X_JumpSaber", 0);
				}
			}
			break;
		}

		if (pStatus.player->getFrameX() >= pStatus.player->getMaxFrameX()) pStatus.player->setFrameX(pStatus.player->getMaxFrameX());
	}

	////////////////////////
	// 대시
	////////////////////////

	else if (currentState == CharacterState::Dash && pStatus.isDash)
	{
		if (!isMoving)
		{
			switch (attState)
			{
			case SholderState::Burst:
			case SholderState::LargeBurst:
				animSpeed = 0.08f;
				animOffset.x = 0 * SCALE_FACTOR;
				animOffset.y = 0 * SCALE_FACTOR;
				changeAnimation(pStatus.charName + "DashBurstStart", 0);
				break;
			case SholderState::Hold:
			case SholderState::None:
				animSpeed = 0.08f;
				animOffset.x = 0 * SCALE_FACTOR;
				animOffset.y = 0 * SCALE_FACTOR;
				changeAnimation(pStatus.charName + "DashStart", 0);
				break;
			case SholderState::Special:
				if (pStatus.charName == "X_")
				{
					if (previousState == CharacterState::JumpUp || previousState == CharacterState::FallingDown)
					{
						attState = SholderState::None;
						pStatus.movable = true;
						pStatus.isAttack = false;
					}
					else
					{
						animSpeed = 0.045f;
						animOffset.x = status.lookRight ? 27 * SCALE_FACTOR : -27 * SCALE_FACTOR;
						animOffset.y = 9 * SCALE_FACTOR;
						changeAnimation("X_Saber", 0);
					}
				}
				break;
			}

			// 프레임 관련 설정
			switch (pStatus.player->getFrameX())
			{
			case 0:
				pStatus.firePointX = -3 * SCALE_FACTOR;
				pStatus.firePointY = 5;
				break;
			case 1:
				pStatus.firePointX = -5 * SCALE_FACTOR;
				pStatus.firePointY = 6 * SCALE_FACTOR;
				break;
			}

			if (pStatus.player->getFrameX() >= pStatus.player->getMaxFrameX())
			{
				if (pStatus.isOnGround)
				{
					int dashOffset;
					dashOffset = status.lookRight ? status.hitBoxWidth * 3.8 : -status.hitBoxWidth;
					EFFECTMANAGER->spawnEffect(EffectType::DashStartDust, pos.x - dashOffset, pos.y, pStatus.player->getFrameWidth(), pStatus.player->getFrameHeight(), status.lookRight);
				}
				isMoving = true;
			}
		}

		else if (isMoving)
		{
			pStatus.firePointX = -12 * SCALE_FACTOR;
			pStatus.firePointY = 10 * SCALE_FACTOR;

			switch (attState)
			{
			case SholderState::Burst:
			case SholderState::LargeBurst:
				animSpeed = 0.05f;
				animOffset.x = 0 * SCALE_FACTOR;
				animOffset.y = 0 * SCALE_FACTOR;
				if (previousAnim == pStatus.charName + "DashBurstStart") changeAnimation(pStatus.charName + "DashBurstLoop", 0);
				else changeAnimation(pStatus.charName + "DashBurstLoop", pStatus.player->getFrameX());
				break;
			case SholderState::Hold:
			case SholderState::None:
				animSpeed = 0.05f;
				animOffset.x = 0 * SCALE_FACTOR;
				animOffset.y = 0 * SCALE_FACTOR;
				if (previousAnim == pStatus.charName + "DashStart") changeAnimation(pStatus.charName + "DashLoop", 0);
				else changeAnimation(pStatus.charName + "DashLoop", pStatus.player->getFrameX());
				break;
			case SholderState::Special:
				if (pStatus.charName == "X_")
				{
					if (previousState == CharacterState::JumpUp || previousState == CharacterState::FallingDown)
					{
						attState = SholderState::None;
						pStatus.movable = true;
						pStatus.isAttack = false;
					}
					else
					{
						animSpeed = 0.045f;
						animOffset.x = status.lookRight ? 27 * SCALE_FACTOR : -27 * SCALE_FACTOR;
						animOffset.y = 9 * SCALE_FACTOR;
						changeAnimation("X_Saber", 0);
					}
				}
				break;
			}
		}
	}

	////////////////////////
	// 대시 종료
	////////////////////////

	else if (currentState == CharacterState::DashEnd)
	{
		switch (attState)
		{
		case SholderState::Burst:
		case SholderState::LargeBurst:
			animSpeed = 0.1f;
			
			animOffset.x = 0 * SCALE_FACTOR;
			animOffset.y = 0 * SCALE_FACTOR;
			if (previousAnim == pStatus.charName + "DashEnd") changeAnimation(pStatus.charName + "DashEndBurst", pStatus.player->getFrameX());
			else changeAnimation(pStatus.charName + "DashEndBurst", 0);
			break;
		case SholderState::Hold:
		case SholderState::None:
			animSpeed = 0.1f;
			
			animOffset.x = 0 * SCALE_FACTOR;
			animOffset.y = 0 * SCALE_FACTOR;
			if (previousAnim == pStatus.charName + "DashEndBurst") changeAnimation(pStatus.charName + "DashEnd", pStatus.player->getFrameX());
			else changeAnimation(pStatus.charName + "DashEnd", 0);
			break;
		case SholderState::Special:
			if (pStatus.charName == "X_")
			{
				if (previousState == CharacterState::JumpUp || previousState == CharacterState::FallingDown)
				{
					attState = SholderState::None;
					pStatus.movable = true;
					pStatus.isAttack = false;
				}
				else
				{
					animSpeed = 0.045f;
					
					animOffset.x = status.lookRight ? 27 * SCALE_FACTOR : -27 * SCALE_FACTOR;
					animOffset.y = 9 * SCALE_FACTOR;
					changeAnimation("X_Saber", 0);
				}
			}
			break;
		}

		// 프레임 별 버스터 발사 위치 설정
		switch (pStatus.player->getFrameX())
		{
		case 0:
			pStatus.firePointX = -10 * SCALE_FACTOR;
			pStatus.firePointY = 0;
			break;
		case 1:
			pStatus.firePointX = -11 * SCALE_FACTOR;
			pStatus.firePointY = 0;
			break;
		case 2:
			pStatus.firePointX = -10 * SCALE_FACTOR;
			pStatus.firePointY = 1;
			break;
		case 4:
			pStatus.firePointX = -10 * SCALE_FACTOR;
			pStatus.firePointY = 1;
			break;
		}
	} 

	////////////////////////
	// 벽 타기
	////////////////////////

	else if (currentState == CharacterState::WallSlide)
	{
		pStatus.firePointX = 0 * SCALE_FACTOR;
		pStatus.firePointY = 10 * SCALE_FACTOR;

		switch (attState)
		{
		case SholderState::Burst:
		case SholderState::LargeBurst:
			animSpeed = 0.06f;
						
			animOffset.x = 0 * SCALE_FACTOR;
			animOffset.y = 0 * SCALE_FACTOR;
			if (previousAnim == pStatus.charName + "WallSlide") changeAnimation(pStatus.charName + "WallSlideBurst", pStatus.player->getFrameX());
			else changeAnimation(pStatus.charName + "WallSlideBurst", 0);
			break;
		case SholderState::Hold:
		case SholderState::None:
			animSpeed = 0.06f;

			animOffset.x = 0 * SCALE_FACTOR;
			animOffset.y = 0 * SCALE_FACTOR;
			if (previousAnim == pStatus.charName + "WallSlideBurst") changeAnimation(pStatus.charName + "WallSlide", pStatus.player->getFrameX());
			else changeAnimation(pStatus.charName + "WallSlide", 0);
			break;
		case SholderState::Special:
			if (pStatus.charName == "X_")
			{
				if (previousState == CharacterState::JumpUp || previousState == CharacterState::FallingDown)
				{
					attState = SholderState::None;
					pStatus.movable = true;
					pStatus.isAttack = false;
				}
				else
				{
					animSpeed = 0.045f;
					
					animOffset.x = status.lookRight ? -28 * SCALE_FACTOR : 28 * SCALE_FACTOR;
					animOffset.y = 0 * SCALE_FACTOR;
					changeAnimation("X_WallSlideSaber", 0);
				}
			}
			break;
		}

		if (pStatus.player->getFrameX() >= pStatus.player->getMaxFrameX()) pStatus.player->setFrameX(pStatus.player->getMaxFrameX());
	}

	////////////////////////
	// 벽 차기
	////////////////////////

	else if (currentState == CharacterState::WallKick)
	{
		switch (attState)
		{
		case SholderState::Burst:
		case SholderState::LargeBurst:
			animSpeed = 0.06f;
			
			animOffset.x = 0 * SCALE_FACTOR;
			animOffset.y = 0 * SCALE_FACTOR;
			if (previousAnim == pStatus.charName + "WallKick") changeAnimation(pStatus.charName + "WallKickBurst", pStatus.player->getFrameX());
			else changeAnimation(pStatus.charName + "WallKickBurst", 0);
			break;
		case SholderState::Hold:
		case SholderState::None:
			animSpeed = 0.06f;

			animOffset.x = 0 * SCALE_FACTOR;
			animOffset.y = 0 * SCALE_FACTOR;
			if (previousAnim == pStatus.charName + "WallKickBurst") changeAnimation(pStatus.charName + "WallKick", pStatus.player->getFrameX());
			else changeAnimation(pStatus.charName + "WallKick", 0);
			break;
		case SholderState::Special:
			if (pStatus.charName == "X_")
			{
				if (previousState == CharacterState::WallSlide || previousState == CharacterState::JumpUp || previousState == CharacterState::FallingDown)
				{
					attState = SholderState::None;
					pStatus.movable = true;
					pStatus.isAttack = false;
				}
			}
			break;
		}

		if (pStatus.player->getFrameX() >= pStatus.player->getMaxFrameX()) currentState = CharacterState::FallingDown;
	}

	////////////////////////
	// 사다리 타기
	////////////////////////

	else if (currentState == CharacterState::LadderStart)
	{
		animSpeed = 0.1f;
		animOffset.x = 0 * SCALE_FACTOR;
		animOffset.y = 0 * SCALE_FACTOR;
		
		changeAnimation(pStatus.charName + "LadderStart", 0);

		if (pStatus.player->getFrameX() >= pStatus.player->getMaxFrameX())
		{
			currentState = CharacterState::LadderLoop;
		}
	}

	else if (currentState == CharacterState::LadderLoop)
	{
		inputEnabled = true;
		animSpeed = 0.1f;
		
		animOffset.y = 12 * SCALE_FACTOR;

		switch (attState)
		{
		case SholderState::Burst:
		case SholderState::LargeBurst:
			animSpeed = 0.06f;

			pStatus.firePointY = 13 * SCALE_FACTOR;

			if(status.lookRight) animOffset.x = 5 * SCALE_FACTOR;
			else animOffset.x = -5 * SCALE_FACTOR;

			if (previousAnim == pStatus.charName + "LadderLoop") changeAnimation(pStatus.charName + "LadderBurst", 0);
			else changeAnimation(pStatus.charName + "LadderBurst", 0);

			if (pStatus.player->getFrameX() >= pStatus.player->getMaxFrameX())
			{
				pStatus.player->setFrameX(pStatus.player->getMaxFrameX());
			}
			break;

		case SholderState::Hold:
		case SholderState::None:

			animSpeed = 0.06f;
			animOffset.x = 0 * SCALE_FACTOR;
			
			if (previousAnim == pStatus.charName + "LadderBurst") changeAnimation(pStatus.charName + "LadderLoop", 0);
			else changeAnimation(pStatus.charName + "LadderLoop", 0);

			pStatus.player->pause();

			break;
		}
	}

	else if (currentState == CharacterState::LadderEnd)
	{
		animSpeed = 0.1f;
		animOffset.x = 0 * SCALE_FACTOR;
		animOffset.y = 0 * SCALE_FACTOR;
		
		changeAnimation(pStatus.charName + "LadderEnd", 0);

		if (pStatus.player->getFrameX() >= pStatus.player->getMaxFrameX())
		{
			currentState = CharacterState::Idle;
			inputEnabled = true;
			ladderEnd = false;
			pStatus.isOnLadder = false;

			// 사다리 상단에 도착하면 바닥에 닿게 보정 해줘야하는데
			// 애니메이션이 바뀌기 전에 RECT부터 움직여서 꼼수로
			// 애니메이션이 바뀌기 전에 이미지를 내려버림
			// ㄴ> 어처피 상단 도착 애니메이션이 다시 나오면 보정 또 들어가서 상관없음
			animOffset.y = 30 * SCALE_FACTOR;

			pos.y -= 90.0f;
			pStatus.hitBox.top -= 90.0f;
			pStatus.hitBox.bottom -= 90.0f;
		}
	}


	////////////////////////
	// 데미지 애니메이션 종료
	////////////////////////

	if (currentState == CharacterState::OverPower)
	{
		animOffset.x = 0 * SCALE_FACTOR;
		animOffset.y = 0 * SCALE_FACTOR;

		if (pStatus.player->getFrameX() >= pStatus.player->getMaxFrameX())
		{
			pStatus.invincible = true;
			pStatus.movable = true;
			currentState = CharacterState::Idle;
		}
	}

	////////////////////////
	// 세이버 애니메이션 종료
	////////////////////////

	if (attState == SholderState::Special)
	{
		if (pStatus.player->getFrameX() >= 3 && pStatus.isOnGround) pStatus.movable = false;

		if (pStatus.player->getFrameX() >= pStatus.player->getMaxFrameX())
		{
			attState = SholderState::None;
			pStatus.movable = true;
			pStatus.isAttack = false;
		}
	}

	////////////////////////
	// 공격 애니메이션 타이밍 체크
	////////////////////////

	if (attState == SholderState::Burst || attState == SholderState::LargeBurst)
	{
		if (TIMEMANAGER->getWorldTime() - attackTimer >= attackDuration)
		{
			if (currentState == CharacterState::Walk) attState = SholderState::None;
			else if (currentState == CharacterState::JumpUp) attState = SholderState::None;
			else if (currentState == CharacterState::FallingDown) attState = SholderState::None;
			else if (currentState == CharacterState::WallSlide) attState = SholderState::None;
			else if (currentState == CharacterState::WallKick) attState = SholderState::None;
			else if (currentState == CharacterState::Dash) attState = SholderState::None;
			else if (currentState == CharacterState::DashEnd) attState = SholderState::None;
			else if (currentState == CharacterState::LadderLoop) attState = SholderState::None;
			else if (currentState == CharacterState::Idle) attState = SholderState::Hold;
		}
	}

	else if(attState == SholderState::None) pStatus.isAttack = false;

	hitBoxCenter.x = (pStatus.hitBox.left + pStatus.hitBox.right) / 2;
	hitBoxCenter.y = pStatus.hitBox.bottom;

	previousAnim = currentAnim;
	previousState = currentState;	
}

void Player::setOverPower(bool op, BulletSize bullet)
{
	pStatus.invincible = op;

	// if (pStatus.invincible) currentState = CharacterState::OverPower;

	switch (bullet)
	{
		case BulletSize::Small:
			currentAnim = pStatus.charName + "SmallDamaged";
			pStatus.movable = false;
			break;
		case BulletSize::Large:
			currentAnim = pStatus.charName + "LargeDamaged";
			pStatus.movable = false;
			break;
	}
}

void Player::reduceHp(int damage)
{
	currentState = CharacterState::OverPower;
	
	status.hp -= damage;
	status.overpower = true;
	pStatus.isAttack = false;
	pStatus.isDash = false;
	pStatus.isJumpDash = false;
	pStatus.movable = false;


	dashTimer = 0.0f;

	if (status.hp > 0)
	{
		int random = RND->getInt(2);
		if (random == 0) SOUNDMANAGER->play("Voice_"+ pStatus.charName + "Damaged1");
		else SOUNDMANAGER->play("Voice_" + pStatus.charName + "Damaged2");
				
		animSpeed = 0.06f;

		if (damage > 5)
		{
			changeAnimation(pStatus.charName + "LargeDamaged", 0);

			pStatus.velocityX = status.lookRight ? -2.0f : 2.0f;
			pStatus.velocityY = 0.0f;
		}

		else if (damage > 0 && damage <= 5)
		{
			changeAnimation(pStatus.charName + "SmallDamaged", 0);

			pStatus.velocityX = status.lookRight ? -3.0f : 3.0f;
			pStatus.velocityY = 0.0f;
		}
	}

	else
	{
		currentState = CharacterState::Dead;
		SOUNDMANAGER->play("Voice_" + pStatus.charName + "Dead");

		status.dead = true;
		pStatus.invincible = true;
		pStatus.movable = false;
	}
}

void Player::colorSetting(void)
{
	// Do Nothing!!
}

void Player::colorChange(void)
{
	// Do Nothing!!
}

void Player::changeAnimation(const string& animName, int frame)
{
	if (previousAnim != animName)
	{
		currentAnim = animName;
		pStatus.player = IMAGEMANAGER->findImage(currentAnim);
		pStatus.player->setFrameX(frame);

		sfxPlay();
	}
}

void Player::coolDownControl()
{
	// Do Nothing!!
}

string Player::printBodyState(void)
{
	string result;

	if (currentState == CharacterState::Idle) result = "대기";
	else if (currentState == CharacterState::Walk) result = "걷기";
	else if (currentState == CharacterState::JumpUp) result = "점프";
	else if (currentState == CharacterState::FallingDown) result = "낙하";
	else if (currentState == CharacterState::Dash) result = "대시";
	else if (currentState == CharacterState::DashEnd) result = "대시 종료";
	else if (currentState == CharacterState::Land) result = "착지";
	else if (currentState == CharacterState::Crouch) result = "앉기";
	else if (currentState == CharacterState::Warp) result = "워프";
	else if (currentState == CharacterState::WallSlide) result = "벽타기";
	else if (currentState == CharacterState::WallKick) result = "벽차기";
	else if (currentState == CharacterState::Dead) result = "사망";
	else if (currentState == CharacterState::LadderStart) result = "사다리 타기";
	else if (currentState == CharacterState::LadderLoop) result = "사다리 타는중";
		
	return result;
}

string Player::printAttState(void)
{
	string result;

	if (attState == SholderState::None) result = "대기";
	else if (attState == SholderState::Burst) result = "버스터";
	else if (attState == SholderState::LargeBurst) result = "버스터";
	else if (attState == SholderState::Hold) result = "버스터";
	
	return result;
}

void Player::setHitBox(void)
{
	// Do Nothing!
}

void Player::multiHitControl(void)
{
	// Do Nothing!
}

void Player::ladderUpper()
{
	currentState = CharacterState::LadderStart;
	ladderEnd = true;
}

ShootEvent Player::makeShootEvent(BulletType bType)
{
	// Do Nothing!
	return ShootEvent();
}

void Player::afterImageControl(void)
{
	if (pStatus.isDash || pStatus.isJumpDash)
	{
		if(hideAfterimage || attState == SholderState::Special)
			EFFECTMANAGER->deleteDashAfterImage();

		else
		{
			afterImageTimer += 0.1f;

			if (afterImageTimer >= afterImageInterval)
			{
				int x = pos.x;
				int y = pos.y;

				// int x = charPos.x;
				// int y = charPos.y;
				int frameX = pStatus.player->getFrameX();
				int frameY = pStatus.player->getFrameY();
				string key = currentAnim;
				bool dir = status.lookRight;

				EFFECTMANAGER->addDashAfterImage(x, y, frameX, frameY, dir, key);

				afterImageTimer = 0.0f;
			}
		}
	}
}
