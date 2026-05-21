#include "Stdafx.h"
#include "X.h"
#include "BulletManager.h"


HRESULT X::init(int x, int y)
{
	// 캐릭터 기본값
	status.width = 25 * SCALE_FACTOR;
	status.height = 41 * SCALE_FACTOR;

	status.maxHp = 30.0;
	status.maxMp = 10.0;
	
	maxDashTime = 3.5f;
	
	// 캐릭터 소환 - 게임 시작
	spawn(x, y);

	return S_OK;
}

void X::release(void)
{

}

void X::update(void)
{
	CAMERAMANAGER->setPlayerPos(pos.x, pos.y - status.hitBoxHeight / 2);

	bool allowInput = !(CAMERAMANAGER->getIsCamaraMove() || !CAMERAMANAGER->getCameraMoveEnd()
		|| UIMANAGER->getIsUiPrint() || UIMANAGER->getFreeze() || !ladderDone
		|| currentState == CharacterState::Warp);
	inputEnabled = allowInput;
	
#pragma region WarpIn
	/////////////////////////////////
	// 첫 등장 연출 파트
	/////////////////////////////////
		
	// 게임 시작시 스테이지에 소환 되는 상황
	if (currentState == CharacterState::Warp)
	{
		// 하늘에서 내려오고 있을 때 - 스프라이트 보면 애니메이션이 동작 안함
		// if (pStatus.hitBox.bottom <= WINSIZE_Y)
		if (pStatus.isOnGround == false)
		{
			pStatus.player->pause();
			inputEnabled = false;
		}

		// 땅에 도착
		else
		{
			pStatus.isWarp = false;

			pStatus.player->resume();
			// 등장 애니메이션
			if (pStatus.player->getFrameX() >= pStatus.player->getMaxFrameX())
			{
				pStatus.player->pause();

				if (TIMEMANAGER->getWorldTime() - delayTimer > 1.0f)
				{
					pStatus.player->resume();
					currentState = CharacterState::Idle;
					inputEnabled = true;
				}
			}
			else delayTimer = TIMEMANAGER->getWorldTime();
		}
	}

#pragma endregion

#pragma region Character Control

	/////////////////////////////////
	// 플레이어가 캐릭터 상태 컨트롤 파트 
	/////////////////////////////////

	// 플레이어의 컨트롤을 잠시 막아야 하는 상황
	// if (currentState == CharacterState::OverPower || currentState == CharacterState::Dead) inputEnabled = false;
	// else if (currentState == CharacterState::SpecialAtt) inputEnabled = false;
	// else inputEnabled = true;
	

	/////////////////////////////////
	// 플레이어의 게임 플레이 파트
	/////////////////////////////////

	if (inputEnabled == true && animDelay == false)
	{
		/////////////////////////////////
		// 이동
		/////////////////////////////////

		if (pStatus.movable)
		{
			if (KEYMANAGER->isOnceKeyDown(VK_RIGHT)) lastKeyIsRight = true;
			if (KEYMANAGER->isOnceKeyDown(VK_LEFT)) lastKeyIsRight = false;

			if (KEYMANAGER->isStayKeyDown(VK_RIGHT) && KEYMANAGER->isStayKeyDown(VK_LEFT))
			{
				multiInput = true;

				if (lastKeyIsRight == true)
				{
					// 왼쪽 유지
					status.lookRight = false;

					if (pStatus.touchLeft)
					{
						if (pStatus.isOnGround)	currentState = CharacterState::Idle;
						else if (!pStatus.isOnGround && !status.lookRight && !pStatus.isJumpUp) wallSlide(); // 벽타기
						else if (!pStatus.isOnGround && !status.lookRight && pStatus.isJumpUp);
					}

					else move(status.lookRight);
				}

				else if (lastKeyIsRight == false)
				{
					// 오른쪽 유지
					status.lookRight = true;

					if (pStatus.touchRight)
					{
						if (pStatus.isOnGround) currentState = CharacterState::Idle;
						else if (!pStatus.isOnGround && status.lookRight && !pStatus.isJumpUp) wallSlide();
						else if (!pStatus.isOnGround && status.lookRight && pStatus.isJumpUp);
					}

					else move(status.lookRight);
				}
			}

			else if (KEYMANAGER->isStayKeyDown(VK_RIGHT) && !(pStatus.isWallKick && pStatus.wallKickRight)) // !(pStatus.velocityX < 0.0f) 
			{
				multiInput = false;
				status.lookRight = true;
				pressRight = true;

				if (pStatus.touchRight)
				{
					if (pStatus.isOnGround)	currentState = CharacterState::Idle;
					else  if (!pStatus.isOnGround && status.lookRight && !pStatus.isJumpUp) wallSlide(); // 벽타기
					else if (!pStatus.isOnGround && status.lookRight && pStatus.isJumpUp);
				}

				else move(status.lookRight);
			}

			else if (KEYMANAGER->isStayKeyDown(VK_LEFT) && !(pStatus.isWallKick && !pStatus.wallKickRight))  // !(pStatus.velocityX > 0.0f) 
			{
				multiInput = false;
				status.lookRight = false;
				pressLeft = true;
				if (pStatus.touchLeft)
				{
					if (pStatus.isOnGround) currentState = CharacterState::Idle;
					else if (!pStatus.isOnGround && !status.lookRight && !pStatus.isJumpUp) wallSlide(); // 벽타기
					else if (!pStatus.isOnGround && !status.lookRight && pStatus.isJumpUp);
				}

				else move(status.lookRight);
			}

			// 대기 상태 변환 + 대기 상태 변환 애니메이션
			else if (!(KEYMANAGER->isStayKeyDown(VK_RIGHT) && !KEYMANAGER->isStayKeyDown(VK_LEFT)) && 1)
			{
				if (!pStatus.isWallKick || !pStatus.movable)
				{
					pStatus.velocityX = 0.0f;
				}
				if (pStatus.isOnGround) returnToIdle();

				attChange = false;
				multiInput = false;
			}

			// 벽타기에서 벽을 놓기
			if (KEYMANAGER->isOnceKeyUp(VK_RIGHT) && !KEYMANAGER->isStayKeyDown(VK_LEFT))
			{
				wallDrop();
				pressRight = false;
			}

			if (KEYMANAGER->isOnceKeyUp(VK_LEFT) && !KEYMANAGER->isStayKeyDown(VK_RIGHT))
			{
				wallDrop();
				pressLeft = false;
			}

#pragma region Ladder
			if (KEYMANAGER->isStayKeyDown(VK_DOWN) && !pStatus.isWallSlide && inputEnabled && ladderAble && !pStatus.isDash && !pStatus.isJumpDash)
			{
				pStatus.velocityX = 0.0f;
				pos.x = ladderPosX;

				pStatus.isDash = false;
				pStatus.isJumpDash = false;

				if (pStatus.isOnLadder)
				{
					if(!pStatus.isBurst) pStatus.velocityY = 4.0f;
					else pStatus.velocityY = 0.0f;
					pStatus.player->resume();
					pStatus.player->play(animSpeed);
					
					currentState = CharacterState::LadderLoop;
				}

				else
				{
					if (ladderEnd)
					{
						currentState = CharacterState::LadderStart;
						pos.y += 100.0;
						pStatus.isOnLadder = true;
					}
				}
			}

			if (KEYMANAGER->isStayKeyDown(VK_UP) && !pStatus.isWallSlide && inputEnabled && ladderAble)
			{
				pStatus.velocityX = 0.0f;
				pos.x = ladderPosX;
				pStatus.isDash = false;
				pStatus.isJumpDash = false;

				// 매달려 있을 때
				if (pStatus.isOnLadder)
				{
					if (ladderEnd)
					{
						currentState = CharacterState::LadderEnd;
						pos.y -= 80.0f;
						pStatus.velocityY = 0.0f;
						pStatus.isOnLadder = false;
					}

					else
					{
						currentState = CharacterState::LadderLoop;
						if (!pStatus.isBurst) pStatus.velocityY = -4.0f;
						else pStatus.velocityY = 0.0f;
						pStatus.player->resume();
						pStatus.player->play(animSpeed);
					}
				}

				// 매달리지 않을 때
				else
				{
					if (attState != SholderState::Special)
					{
						if (!ladderEnd)
						{
							pStatus.velocityX = 0.0f;

							if (pStatus.isOnGround)
							{
								pos.y -= 4.0f;
								pStatus.hitBox.top -= 4.0f;
								pStatus.hitBox.bottom -= 4.0f;
							}

							currentState = CharacterState::LadderLoop;
							pStatus.isOnLadder = true;
						}
					}
				}
			}
		}

		if (!actionLock)
		{

#pragma region Dash
			if (KEYMANAGER->isOnceKeyDown('Z'))						// 'ㅋ' 문자 찾기 용
			{
				dash(status.lookRight);
			}

			if (KEYMANAGER->isStayKeyDown('Z') && pStatus.movable && !pStatus.isOnLadder)
			{
				dashTimer += 0.1f;

				pressDash = true;

				if (dashTimer <= maxDashTime)
				{
					if (pStatus.isDash && !pStatus.isJumpDash && dashTimer >= 0.3f)
					{
						currentState = CharacterState::Dash;
						move(status.lookRight);
					}
				}

				else
				{
					if (!pStatus.isJumpDash && pStatus.isDash)
					{
						pStatus.isDash = false;
						aniDash = false;
						pStatus.player->setFrameX(0);

						currentState = CharacterState::DashEnd;
					}
				}
			}

			if (KEYMANAGER->isOnceKeyUp('Z') && pStatus.movable)
			{
				SOUNDMANAGER->stop("SFX_DashStart");

				pressDash = false;

				if (!pStatus.isJumpDash)
				{
					if (pStatus.isDash)
					{
						pStatus.isDash = false;

						if (currentState == CharacterState::Dash)
							currentState = CharacterState::DashEnd;
					}
				}

				aniDash = false;
				dashTimer = 0.0f;
			}

#pragma endregion

#pragma region Jump
			/////////////////////////////////
			// 점프 파트
			/////////////////////////////////

			// 점프 누르면 점프 파워 만큼 계속 올라가고
			// 점프를 떼면 바로 낙하를 하게 만들어서 소점프, 대점프 구현

			if (KEYMANAGER->isOnceKeyDown('X')) // 'ㅌ' 검색을 위한 주석ㅋㅋㅋ
			{
				jump();
			}

			if (KEYMANAGER->isOnceKeyUp('X') && !pStatus.isWallKick)
			{
				if (pStatus.velocityY < 0.0f) pStatus.velocityY = 0.0f;
				pStatus.isJumpUp = false;
			}

#pragma endregion

#pragma region Attack
			/////////////////////////////////
			// 공격 파트
			/////////////////////////////////

			if (now - lastShootTime >= shotCoolDown) normalBurstAble = true;
			else normalBurstAble = false;

			now = TIMEMANAGER->getWorldTime();

			if (TIMEMANAGER->getWorldTime() - chargeBurstCount >= 0.3f) chargeBurstDelay = false;

			if (KEYMANAGER->isOnceKeyDown('C'))		// 'ㅊ' 검색을 위한 주석ㅋㅋㅋ
			{
				attackTimer = TIMEMANAGER->getWorldTime();
				attCheckOnce = true;

				if (normalBurstAble == true && chargeBurstDelay == false && bManager->getMaxBullets() < 3) attack();
			}

			if (KEYMANAGER->isStayKeyDown('C') && attCheckOnce == true)
			{
				// 시작하기전에 미리 누르고 있으면 차지 안되게
				if (attCheckOnce)
				{
					// 차지
					chargeCount += chargeSpeed;

					if (chargeCount >= 0.3f && chargeCount < 1.0f)
					{
						chargeEffectAlpha = 255;
						chargeEffect->play(0.02f);
					}

					else if (chargeCount >= 1.0f)
					{
						chargeEffectAlpha = 255;
						chargeEffect->play(0.02f);

						chargeAuraAlpha = 255;
						chargeAura->play(0.03f);
					}

				}

				if (!isCharging && chargeCount >= 0.3f)
				{
					isCharging = true;
					SOUNDMANAGER->play("SFX_X_BurstCharge", 0.5f);
				}
			}

			if (KEYMANAGER->isOnceKeyUp('C') && attCheckOnce == true)
			{
				// 차지 시간에 따른 버스터 발사
				chargeBurst();
				chargeEffectAlpha = 0;
				chargeAuraAlpha = 0;

				chargeEffect->setFrameX(0);
				chargeAura->setFrameX(0);
				SOUNDMANAGER->stop("SFX_X_BurstCharge");
				chargeCount = 0.0f;
				isCharging = false;
				attCheckOnce = false;
				attCheckOnce = true;
			}

			if (KEYMANAGER->isOnceKeyDown('V'))					// 'ㅍ' 문자 찾기 용
			{
				specialAttack();
			}
		}
#pragma endregion
	}
#pragma endregion
#pragma endregion

	if (KEYMANAGER->isOnceKeyUp(VK_DOWN) && pStatus.isOnLadder && ladderDone)
	{
		currentState = CharacterState::LadderClimb;
		pStatus.velocityY = 0.0f;
	}

	if (KEYMANAGER->isOnceKeyUp(VK_UP) && pStatus.isOnLadder)
	{
		currentState = CharacterState::LadderClimb;
		pStatus.velocityY = 0.0f;
	}

#pragma region Animation Change + SFX Sound Play
	
	if (!animDelay)
	{
		applyForce();

		if (CAMERAMANAGER->getIsCamaraMove())
		{
			cameraMoveDone = true;
			pStatus.velocityX = 1.0f;

			if (currentState == CharacterState::JumpUp || currentState == CharacterState::Climb
				|| currentState == CharacterState::FallingDown || currentState == CharacterState::DashEnd
				|| currentState == CharacterState::Idle);

			else pStatus.player->play(animSpeed);
		}

		else
		{
			if (cameraMoveDone)
			{
				pStatus.velocityX = 0.0f;

				pStatus.isDash = false;
				pStatus.isJumpDash = false;
				currentState = CharacterState::Idle;

				if (CAMERAMANAGER->getCameraMoveEnd())
				{
					cameraMoveDone = false;
				}
			}

			else
			{
				if (UIMANAGER->getIsUiPrint())
				{
					pStatus.velocityX = 0.0f;
					if (currentState != CharacterState::Warp
						&& currentState != CharacterState::Victory && currentState != CharacterState::WarpOut)
						currentState = CharacterState::Idle;
				}
			}

			if (!status.dead)
			{
				pStatus.player->play(animSpeed);
			}
		}
		attackHandEffect->play(effectAnimSpeed);
		currentAnimChange();
		bursterEffectAlphaDown();
		frameCheck();
	}

	multiHitControl();
	setHitBox();
	invincibleTimerUpdate();
	deathAnim();

	hpBar.updatePlayerInfop(status.hp, status.maxHp, status.mp, progress.life);
	hpBar.update();

#pragma endregion
}

void X::jump(void)
{
	if (pStatus.movable)
	{
		if (pStatus.isOnGround || pStatus.touchLeft || pStatus.touchRight) Player::jump();
		else if (!pStatus.isOnGround && pStatus.isOnLadder) pStatus.isOnLadder = false;
		else;// 호버링
	}
}

void X::dash(bool direction)
{
	if (pStatus.movable)
	{
		if (pStatus.isOnGround) Player::dash(direction);
		else if (!pStatus.isOnGround); // 아머가 있을때 에어대시
	}
}

void X::attack(void)
{
	if (pStatus.movable && !pStatus.isAttack)
	{
		if (pStatus.isOnLadder)	pStatus.velocityY = 0.0f;

		attState = SholderState::Burst;
		busterPos.x = 0 * SCALE_FACTOR;
		shootEvent = makeShootEvent(BulletType::Burster);
		SOUNDMANAGER->play("SFX_X_Burster1", 0.5f);

		bursterEffectName = "SFX_BursterEffect1";
		bursterEffectAlpha = 255;
		attackHandEffect->setFrameX(0);
		effectAnimSpeed = 0.05f;
		
		burstloop = true;
		lastShootTime = now;
		isCharging = false;
		pStatus.isBurst = true;

		EVENTMANAGER->dispatchEvents({EventType::ShootBulltet, &shootEvent});
	}
}

void X::chargeBurst(void)
{
	if (pStatus.movable && !pStatus.isAttack)
	{
		if (chargeCount >= 0.3f && chargeCount < 1.0f)
		{
			attState = SholderState::Burst;
			busterPos.x = 0 * SCALE_FACTOR;
			shootEvent = makeShootEvent(BulletType::ChargeBurst1);
			SOUNDMANAGER->play("SFX_X_Burster1", 0.5f);

			bursterEffectName = "SFX_BursterEffect2";
			bursterEffectAlpha = 255;
			attackHandEffect->setFrameX(0);
			effectAnimSpeed = 0.05f;

			// 공격 회수 장면을 위한 타이머 저장 -> 안하면 바로 내림ㅋㅋ
			attackTimer = TIMEMANAGER->getWorldTime();

			burstloop = true;
			chargeBurstCount = TIMEMANAGER->getWorldTime();
			chargeBurstDelay = true;
			pStatus.isAttack = true;

			EVENTMANAGER->dispatchEvents({ EventType::ShootBulltet, &shootEvent });
		}

		else if (chargeCount >= 1.0f)
		{
			attState = SholderState::LargeBurst;
			busterPos.x = 30 * SCALE_FACTOR;

			shootEvent = makeShootEvent(BulletType::ChargeBurst2);
			SOUNDMANAGER->play("SFX_X_Burster3", 0.5f);
			if (RND->getInt(10) < 5) SOUNDMANAGER->play("Voice_X_Burster", 0.5f);

			bursterEffectName = "SFX_BursterEffect3";
			bursterEffectAlpha = 255;
			attackHandEffect->setFrameX(0);
			effectAnimSpeed = 0.07f;

			attackTimer = TIMEMANAGER->getWorldTime();

			burstloop = true;
			chargeBurstCount = TIMEMANAGER->getWorldTime();
			chargeBurstDelay = true;
			pStatus.isAttack = true;

			EVENTMANAGER->dispatchEvents({ EventType::ShootBulltet, &shootEvent });
		}
	}
	else;
}

void X::frameCheck(void)
{
	// Burst 끝났으면 Hold로
	if (currentState == CharacterState::Idle)
	{
		if (attState == SholderState::Burst)
		{
			if (burstloop == true)
			{
				pStatus.player->setFrameX(0);
				burstloop = false;
			}

			if (pStatus.player->getFrameX() >= pStatus.player->getMaxFrameX())
			{
				attState = SholderState::Hold;
			}
		}

		// Hold 끝났으면 None으로
		else if (attState == SholderState::Hold)
		{ 
			if(pStatus.player->getFrameX() >= pStatus.player->getMaxFrameX())
			{
				attState = SholderState::None;
				pStatus.player->setFrameX(0);
			}
		}

		else if (attState == SholderState::LargeBurst)
		{
			if (burstloop == true)
			{
				pStatus.player->setFrameX(0);
				burstloop = false;
			}

			if (pStatus.player->getFrameX() >= pStatus.player->getMaxFrameX())
			{
				attState = SholderState::None;
				pStatus.player->setFrameX(0);
			}
		}
	}
}

void X::setHitBox(void)
{
	pStatus.hitBox.left = pos.x - status.hitBoxWidth / 2 - CAMERAMANAGER->getCameraPos().x;
	pStatus.hitBox.right = pos.x + status.hitBoxWidth / 2 - CAMERAMANAGER->getCameraPos().x;
	
	pStatus.hitBox.bottom = pos.y - CAMERAMANAGER->getCameraPos().y;
	pStatus.hitBox.top = pStatus.hitBox.bottom - status.hitBoxHeight;
	
	switch (currentState)
	{
	case CharacterState::Warp:
		status.hitBoxWidth = 25 * SCALE_FACTOR;
		status.hitBoxHeight = 41 * SCALE_FACTOR;
		break;
	case CharacterState::Idle:
		status.hitBoxWidth = 25 * SCALE_FACTOR;
		status.hitBoxHeight = 41 * SCALE_FACTOR;
		break;
	case CharacterState::Walk:
		status.hitBoxWidth = 25 * SCALE_FACTOR;
		status.hitBoxHeight = 41 * SCALE_FACTOR;
		break;
	case CharacterState::JumpUp:
		status.hitBoxWidth = 25 * SCALE_FACTOR;
		status.hitBoxHeight = 45 * SCALE_FACTOR;
		break;
	case CharacterState::FallingDown:
		status.hitBoxWidth = 25 * SCALE_FACTOR;
		status.hitBoxHeight = 45 * SCALE_FACTOR;
		break;
	case CharacterState::Dash:
		status.hitBoxWidth = 25 * SCALE_FACTOR;
		status.hitBoxHeight = 28 * SCALE_FACTOR;
		break;
	case CharacterState::WallSlide:
		status.hitBoxWidth = 25 * SCALE_FACTOR;
		status.hitBoxHeight = 45 * SCALE_FACTOR;
		break;
	}

	pStatus.floorCheck = RectMake((pStatus.hitBox.left + pStatus.hitBox.right) / 2 - status.hitBoxWidth / 2 + 5, pStatus.hitBox.bottom, status.hitBoxWidth - 10, 5);
	pStatus.subRect = RectMakeCenter((pStatus.hitBox.left + pStatus.hitBox.right) / 2, (pStatus.hitBox.top + pStatus.hitBox.bottom) / 2, 5 * SCALE_FACTOR, status.hitBoxHeight);
	
	if (status.lookRight)
	{
		if(currentState == CharacterState::WallSlide) pStatus.saberHitBox = RectMake(pStatus.hitBox.left - saberWidth + saberOffsetX, pStatus.hitBox.bottom - saberHeight + saberOffsetY, saberWidth, saberHeight);
		else
		{
			pStatus.saberHitBox = RectMake(pStatus.hitBox.right + saberOffsetX, pStatus.hitBox.bottom - saberHeight + saberOffsetY, saberWidth, saberHeight);
		}
	}

	else
	{
		if (currentState == CharacterState::WallSlide) pStatus.saberHitBox = RectMake(pStatus.hitBox.right + saberOffsetX, pStatus.hitBox.bottom - saberHeight + saberOffsetY, saberWidth, saberHeight);
		else pStatus.saberHitBox = RectMake(pStatus.hitBox.left - saberWidth + saberOffsetX, pStatus.hitBox.bottom - saberHeight + saberOffsetY, saberWidth, saberHeight);
	}
}

void X::colorSetting(void)
{
	playerColor[0] =
	{
		RGB(232, 48, 40), // Base
		RGB(0, 16, 96), // After Shadow1
		RGB(0, 40, 120), // After Shadow2
		RGB(24, 64, 160), // After Shadow3
		RGB(40, 232, 48), // Charged1
		RGB(40, 232, 48), // Charged2
		RGB(0, 0, 0), // Burst
		RGB(112, 160, 248), // Low Damaged
		RGB(0, 0, 0), // High Damaged
	};

	playerColor[1] =
	{
		RGB(96, 40, 24), // Base
		RGB(0, 16, 96), // After Shadow1
		RGB(0, 40, 120), // After Shadow2
		RGB(24, 64, 160), // After Shadow3
		RGB(24, 112, 64), // Charged1
		RGB(24, 112, 64), // Charged2
		RGB(0, 0, 0), // Burst
		RGB(176, 200, 232), // Low Damaged
		RGB(0, 0, 0), // High Damaged
	};

	playerColor[2] =
	{
		RGB(208, 120, 88), // Base
		RGB(0, 16, 96), // After Shadow1
		RGB(0, 40, 120), // After Shadow2
		RGB(24, 64, 160), // After Shadow3
		RGB(88, 208, 120), // Charged1
		RGB(88, 208, 120), // Charged2
		RGB(0, 0, 0), // Burst
		RGB(128, 160, 240), // Low Damaged
		RGB(0, 0, 0), // High Damaged
	};

	playerColor[3] =
	{
		RGB(232, 184, 152), // Base
		RGB(0, 16, 96), // After Shadow1
		RGB(0, 40, 120), // After Shadow2
		RGB(24, 64, 160), // After Shadow3
		RGB(152, 232, 184), // Charged1
		RGB(152, 232, 184), // Charged2
		RGB(0, 0, 0), // Burst
		RGB(128, 160, 240), // Low Damaged
		RGB(0, 0, 0), // High Damaged
	};

	playerColor[4] =
	{
		RGB(72, 248, 216), // Base
		RGB(0, 16, 96), // After Shadow1
		RGB(0, 40, 120), // After Shadow2
		RGB(24, 64, 160), // After Shadow3
		RGB(64, 240, 208), // Charged1
		RGB(64, 240, 208), // Charged2
		RGB(0, 0, 0), // Burst
		RGB(128, 160, 240), // Low Damaged
		RGB(0, 0, 0), // High Damaged
	};

	playerColor[5] =
	{
		RGB(24, 160, 128), // Base
		RGB(0, 16, 96), // After Shadow1
		RGB(0, 40, 120), // After Shadow2
		RGB(24, 64, 160), // After Shadow3
		RGB(16, 152, 120), // Charged1
		RGB(16, 152, 120), // Charged2
		RGB(0, 0, 0), // Burst
		RGB(128, 160, 240), // Low Damaged
		RGB(0, 0, 0), // High Damaged
	};

	playerColor[6] =
	{
		RGB(24, 64, 88), // Base
		RGB(0, 16, 96), // After Shadow1
		RGB(0, 40, 120), // After Shadow2
		RGB(24, 64, 160), // After Shadow3
		RGB(24, 128, 80), // Charged1
		RGB(24, 128, 80), // Charged2
		RGB(0, 0, 0), // Burst
		RGB(176, 200, 232), // Low Damaged
		RGB(0, 0, 0), // High Damaged
	};

	playerColor[7] =
	{
		RGB(216, 224, 240), // Base
		RGB(0, 16, 96), // After Shadow1
		RGB(0, 40, 120), // After Shadow2
		RGB(24, 64, 160), // After Shadow3
		RGB(184, 248, 232), // Charged1
		RGB(184, 248, 232), // Charged2
		RGB(0, 0, 0), // Burst
		RGB(128, 160, 240), // Low Damaged
		RGB(0, 0, 0), // High Damaged
	};

	playerColor[8] =
	{
		RGB(168, 176, 192), // Base
		RGB(0, 16, 96), // After Shadow1
		RGB(0, 40, 120), // After Shadow2
		RGB(24, 64, 160), // After Shadow3
		RGB(128, 184, 168), // Charged1
		RGB(128, 184, 168), // Charged2
		RGB(0, 0, 0), // Burst
		RGB(128, 160, 240), // Low Damaged
		RGB(0, 0, 0), // High Damaged
	};

	playerColor[9] =
	{
		RGB(72, 88, 112), // Base
		RGB(0, 16, 96), // After Shadow1
		RGB(0, 40, 120), // After Shadow2
		RGB(24, 64, 160), // After Shadow3
		RGB(40, 112, 88), // Charged1
		RGB(40, 112, 88), // Charged2
		RGB(0, 0, 0), // Burst
		RGB(104, 144, 208), // Low Damaged
		RGB(0, 0, 0), // High Damaged
	};

	playerColor[10] =
	{
		RGB(24, 32, 40), // Base
		RGB(0, 16, 96), // After Shadow1
		RGB(0, 40, 120), // After Shadow2
		RGB(24, 64, 160), // After Shadow3
		RGB(32, 144, 64), // Charged1
		RGB(32, 144, 64), // Charged2
		RGB(0, 0, 0), // Burst
		RGB(176, 200, 232), // Low Damaged
		RGB(0, 0, 0), // High Damaged
	};

	playerColor[11] =
	{
		RGB(72, 152, 232), // Base
		RGB(0, 16, 96), // After Shadow1
		RGB(0, 40, 120), // After Shadow2
		RGB(24, 64, 160), // After Shadow3
		RGB(72, 248, 168), // Charged1
		RGB(72, 248, 168), // Charged2
		RGB(0, 0, 0), // Burst
		RGB(128, 160, 240), // Low Damaged
		RGB(0, 0, 0), // High Damaged
	};

	playerColor[12] =
	{
		RGB(56, 120, 216), // Base
		RGB(0, 16, 96), // After Shadow1
		RGB(0, 40, 120), // After Shadow2
		RGB(24, 64, 160), // After Shadow3
		RGB(56, 232, 136), // Charged1
		RGB(56, 232, 136), // Charged2
		RGB(0, 0, 0), // Burst
		RGB(128, 160, 240), // Low Damaged
		RGB(0, 0, 0), // High Damaged
	};

	playerColor[13] =
	{
		RGB(32, 72, 176), // Base
		RGB(0, 16, 96), // After Shadow1
		RGB(0, 40, 120), // After Shadow2
		RGB(24, 64, 160), // After Shadow3
		RGB(24, 160, 64), // Charged1
		RGB(32, 200, 96), // Charged2
		RGB(0, 0, 0), // Burst
		RGB(128, 160, 240), // Low Damaged
		RGB(0, 0, 0), // High Damaged
	};

	playerColor[14] =
	{
		RGB(24, 40, 136), // Base
		RGB(0, 16, 96), // After Shadow1
		RGB(0, 40, 120), // After Shadow2
		RGB(24, 64, 160), // After Shadow3
		RGB(24, 160, 64), // Charged1
		RGB(24, 160, 64), // Charged2
		RGB(0, 0, 0), // Burst
		RGB(176, 200, 232), // Low Damaged
		RGB(0, 0, 0), // High Damaged
	};
}

void X::colorChange(void)
{
	switch (colorType)
	{

	}
	colorTimer++;

	for (int j = 0; j < pStatus.player->getFrameHeight(); j++)
	{
		for (int i = 0; i < pStatus.player->getFrameWidth(); i++)
		{
			COLORREF color = GetPixel(pStatus.player->getMemDC() , i, j);

			if (color == playerColor[i + j].base);
		}
	}
}



void X::spawn(int x, int y)
{
	////////////////////
	// 캐릭터 소환
	////////////////////
	
	// 캐릭터 생성
	// x는 센터, y는 바닥 기준 좌표
	pos.x = x;
	pos.y = y;
	
	status.type = CombatEntityType::Player;

	pStatus.hitBox = RectMakeCenter(pos.x, 0 - status.hitBoxHeight / 2, status.hitBoxWidth, status.hitBoxHeight);
	pStatus.floorCheck = RectMakeCenter(pos.x, pos.y, status.hitBoxWidth, 4);
	pStatus.saberHitBox = RectMake(pStatus.hitBox.right + saberOffsetX, pStatus.hitBox.bottom - saberHeight + saberOffsetY, saberWidth, saberHeight);
	pStatus.subRect = RectMakeCenter(pos.x, 0 - status.hitBoxHeight / 2, 5 * SCALE_FACTOR, status.hitBoxHeight);
	
	hitBoxCenter.x = (pStatus.hitBox.left + pStatus.hitBox.right) / 2;
	hitBoxCenter.y = pStatus.hitBox.bottom - status.hitBoxHeight;

	saberWidth = 1;
	saberHeight = 1;

	// 캐릭터 세팅
	status.hp = status.maxHp;
	status.mp = status.maxMp;
	status.defense = 0.0f;
	pStatus.moveSpeed = 4.5f;
	pStatus.dashSpeed = 12.0f;
	
	pStatus.charName = "X_";
	pStatus.touchLeft = false;
	pStatus.touchRight = false;
	pStatus.wallKickRight = true;
	progress.life = 2;
	hpBar.init(PlayerType::X);
	hpBar.setPlayerInfo(static_cast<int>(status.hp), static_cast<int>(status.maxHp), static_cast<int>(status.mp), 2);
	status.physicalDamage = 1;
	pStatus.isWarp = true;
	pStatus.isBurst = false;

	////////////////////
	// 상태 초기화
	////////////////////

	// 캐릭터 초기화
	previousState = CharacterState::Idle;
	currentState = CharacterState::Warp;
	attState == SholderState::None;
	status.lookRight = true;
	isMoving = false;
	pStatus.isDash = false;
	pStatus.isWallKick = false;
	pStatus.isWallSlide = false;
	pStatus.isOnLadder = false;
	colorType = 0;
	status.dead = false;
	status.deadDone = false;
	pStatus.movable = true;
	pStatus.isOnTop = false;
	ladderAble = false;
	ladderEnd = false;
	cameraMoveDone = false;
	warpSoundOnce = false;

	// 입력 초기화
	inputEnabled = false;
	multiInput = false;
	actionLock = false;

	// 점프 초기화
	pStatus.jumpPower = -15.0;
	pStatus.maxFallSpeed = 15.0f;
	pStatus.velocityY = 0.0f;
	wallkickMaxTime = 1.0f;
	wallkickTimer = 0.0f;

	// 벽타기 초기화
	pStatus.isJumpUp = false;
	pressRight = false;
	pressLeft = false;
	
	// 대시 초기화
	dashTimer = 0.0f;
	pStatus.isDash = false;
	pStatus.isJumpDash = false;
	pressDash = false;
	afterImageTimer = 0.0f;
	afterImageInterval = 0.4f; // 이거 늘리면 잔상이 나오는 빈도수가 적어집니다.
	hideAfterimage = false;

	// 무적 초기화
	pStatus.invincibleMaxTime = 5.0f;
	pStatus.overpowerMaxTime = 10.0f;
	pStatus.invincibleTimer = 0.0f;

	// 상태 초기화 - 공격 관련
	normalBurstAble = true;
	chargeBurstDelay = false;
	attCheckOnce = false;
	attackTimer = 0.0f;
	lastShootTime = 0.0f;
	shotCoolDown = 0.06f;
	burstSound = "SFX_X_Burster1";
	bursterEffectName = "SFX_BursterEffect1";
	chargeCount = 0.0f;
	chargeSpeed = 0.01f;
	isCharging = false;
	pStatus.firePointY = 12 * SCALE_FACTOR;
	chargeEffectAlpha = 0;
	chargeAuraAlpha = 0;
	bursterEffectAlpha = 0;
	pStatus.isAttack = false;
	canHit = false;

	pStatus.attackDelayTimer = 0.0f;
	pStatus.attackDelayMaxTime = 0.0f;

	multiHitTimer = 0.0f;
	multiHitMaxTime = 0.5f;

	// 애니메이션 초기화
	previousAnim = "X_Idle";
	currentAnim = "X_Spawn";
	pStatus.player = IMAGEMANAGER->findImage(currentAnim);
	chargeEffect = IMAGEMANAGER->findImage("SFX_Charge");
	chargeAura = IMAGEMANAGER->findImage("SFX_ChargeAura"); 
	attackHandEffect = IMAGEMANAGER->findImage(bursterEffectName);
	charAlpha = 255;

	prevFrame = -1;
	currentFrame = pStatus.player->getFrameX();

	animSpeed = 0.1f;
	effectAnimSpeed = 0.05f;
	attChange = false;
	burstloop = false;
	aniDash = false;
	timer = 0.0f;

	CAMERAMANAGER->setPlayerPos(pos.x, pos.y - status.hitBoxHeight / 2);
}

void X::specialAttack(void)
{
	if (!pStatus.isAttack) // && pStatus.movable)
	{
		attState = SholderState::Special;
		// pStatus.movable = false;

		if (pStatus.isOnGround)
		{
			pStatus.movable = false;
			if(!status.overpower) currentState = CharacterState::Idle;
			pStatus.isDash = false;
		}

		if (pStatus.isOnLadder)
		{
			pStatus.isOnLadder = false;
		}

		pStatus.isAttack = true;
		pStatus.velocityX = 0.0f;
		hideAfterimage = true;
	}
}

void X::coolDownControl(void)
{
}

void X::multiHitControl(void)
{
	int frame = pStatus.player->getFrameX();
	
	if (!animDelay)
	{
		if (attState == SholderState::Special)
		{
			switch (currentState)
			{
			case CharacterState::Idle:
			case CharacterState::Walk:
			case CharacterState::Dash:
			case CharacterState::DashEnd:
				canHit = prevFrame != frame && (frame == 3 || frame == 4 || frame == 5);
				// Saber 3 4 5 = 43 * 43 / 54 * 70 / 55 * 55
				switch(frame)
				{
					case 3:
						saberWidth = 60 * SCALE_FACTOR;
						saberHeight = 43 * SCALE_FACTOR;
						saberOffsetX = 0 * SCALE_FACTOR;
						saberOffsetY = -8 * SCALE_FACTOR;
						status.physicalDamage = 1;
						break;
					case 4:
						saberWidth = 65 * SCALE_FACTOR;
						saberHeight = 70 * SCALE_FACTOR;
						saberOffsetX = 0 * SCALE_FACTOR;
						saberOffsetY = 8 * SCALE_FACTOR;
						status.physicalDamage = 1;
						break;
					case 5:
						saberWidth = 66 * SCALE_FACTOR;
						saberHeight = 55 * SCALE_FACTOR;
						saberOffsetX = 0 * SCALE_FACTOR;
						saberOffsetY = 9 * SCALE_FACTOR;
						status.physicalDamage = 2;
						break;
				}
				break;
			case CharacterState::JumpUp:
			case CharacterState::FallingDown:
				canHit = prevFrame != frame && (frame == 3 || frame == 4);
				// Jump 3 4 = 50 * 46 / 60 * 52
				switch (frame)
				{
				case 3:
					status.lookRight;
					saberWidth = 45 * SCALE_FACTOR;
					saberHeight = 46 * SCALE_FACTOR;
					saberOffsetX = status.lookRight ? -10 * SCALE_FACTOR : 10 * SCALE_FACTOR;
					saberOffsetY = 0 * SCALE_FACTOR;
					status.physicalDamage = 1;
					break;
				case 4:
					saberWidth = 55 * SCALE_FACTOR;
					saberHeight = 52 * SCALE_FACTOR;
					saberOffsetX = status.lookRight ? -10 * SCALE_FACTOR : 10 * SCALE_FACTOR;
					saberOffsetY = 0 * SCALE_FACTOR;
					status.physicalDamage = 1;
					break;
				}
				break;
			case CharacterState::WallSlide:
				canHit = prevFrame != frame && (frame == 2 || frame == 3);
				// Wall 2 3 = 58 * 44 / 58 * 40
				switch (frame)
				{
				case 2:
					saberWidth = 58 * SCALE_FACTOR;
					saberHeight = 44 * SCALE_FACTOR;
					saberOffsetX = 0 * SCALE_FACTOR;
					saberOffsetY = -12 * SCALE_FACTOR;
					status.physicalDamage = 1;
					break;
				case 3:
					saberWidth = 58 * SCALE_FACTOR;
					saberHeight = 40 * SCALE_FACTOR;
					saberOffsetX = 0 * SCALE_FACTOR;
					saberOffsetY = -12 * SCALE_FACTOR;
					status.physicalDamage = 1;
					break;
				}
				break;
			default:
				canHit = false;
				break;
			}
		}
		else canHit = false;
	}

	else
	{
		multiHitTimer += 0.1f;

		if (multiHitTimer > multiHitMaxTime)
		{
			multiHitTimer = 0.0f;
			animDelay = false;
		}
		canHit = false;
	}

	prevFrame = frame;
}

void X::returnToIdle(void)
{
	switch (previousState)
	{
	case CharacterState::Dash:
		break;

	case CharacterState::JumpUp:
		currentState = CharacterState::Idle;
		break;

	case CharacterState::FallingDown:
		currentState = CharacterState::Idle;
		break;

	case CharacterState::DashEnd:
		if (pStatus.player->getFrameX() >= pStatus.player->getMaxFrameX())
			currentState = CharacterState::Idle;
		break;

	case CharacterState::OverPower:
		if (pStatus.player->getFrameX() >= pStatus.player->getMaxFrameX())
		{
			currentState = CharacterState::Idle;
		}
		break;
	default:
		currentState = CharacterState::Idle;
		isMoving = false;
		break;
	}
}

ShootEvent X::makeShootEvent(BulletType bType)
{
	ShootEvent shootEvent;

	shootEvent.bType = bType;
	shootEvent.x = status.lookRight != (currentState == CharacterState::WallSlide)
		? pos.x + status.hitBoxWidth / 2 + (busterPos.x + pStatus.firePointX) : pos.x - status.hitBoxWidth / 2 - (busterPos.x + pStatus.firePointX);
	shootEvent.y = pos.y - status.hitBoxHeight + pStatus.firePointY;
	shootEvent.direct = status.lookRight != (currentState == CharacterState::WallSlide);
	
	return shootEvent;
}
