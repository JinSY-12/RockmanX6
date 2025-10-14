#include "Stdafx.h"
#include "X.h"

HRESULT X::init(void)
{
	return S_OK;
}

HRESULT X::init(int x, int y)
{
	// 캐릭터 기본값
	hitBoxWidth = 25 * SCALE_FACTOR;
	hitBoxHeight = 41 * SCALE_FACTOR;

	pStatus.maxHp = 10.0;
	pStatus.maxMp = 10.0;
	
	maxDashTime = 3.0f;
	
	// 캐릭터 소환 - 게임 시작
	spawn(x, y);

	return S_OK;
}

void X::release(void)
{

}

void X::update(void)
{
	CAMERAMANAGER->setPlayerPos(charPos.x, charPos.y - hitBoxHeight / 2);

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
	//else if (currentState == CharacterState::OverPower || currentState == CharacterState::Dead) inputEnabled = false;
	//else if (currentState == CharacterState::SpecialAtt) inputEnabled = false;
	// else inputEnabled = true;
	

	/////////////////////////////////
	// 플레이어의 게임 플레이 파트
	/////////////////////////////////

	if (inputEnabled == true)
	{
		/////////////////////////////////
		// 이동
		/////////////////////////////////

		if (KEYMANAGER->isOnceKeyDown(VK_RIGHT)) lastKeyIsRight = true;
		if (KEYMANAGER->isOnceKeyDown(VK_LEFT)) lastKeyIsRight = false;
		
		if (KEYMANAGER->isStayKeyDown(VK_RIGHT) && KEYMANAGER->isStayKeyDown(VK_LEFT))
		{
			multiInput = true;

			if (lastKeyIsRight == true)
			{
				// 왼쪽 유지
				pStatus.lookRight = false;

				if (pStatus.touchLeft)
				{
					if(pStatus.isOnGround) currentState = CharacterState::Idle;
					else if (!pStatus.isOnGround && !pStatus.lookRight && !isJumpUp) wallSlide();// 벽타기
					else if (!pStatus.isOnGround && !pStatus.lookRight && isJumpUp);
				}

				else move(pStatus.lookRight);
			}

			else if (lastKeyIsRight == false)
			{
				// 오른쪽 유지
				pStatus.lookRight = true;

				if (pStatus.touchRight)
				{
					if(pStatus.isOnGround) currentState = CharacterState::Idle;
					else if (!pStatus.isOnGround && pStatus.lookRight && !isJumpUp) wallSlide();
					else if (!pStatus.isOnGround && pStatus.lookRight && isJumpUp);
				}
				
				else move(pStatus.lookRight);
			}
		}

		else if (KEYMANAGER->isStayKeyDown(VK_RIGHT) && !(pStatus.isWallKick && pStatus.wallKickRight)) // !(pStatus.velocityX < 0.0f) 
		{
			multiInput = false;
			pStatus.lookRight = true;
			pressRight = true;

			if (pStatus.touchRight)
			{
				if (pStatus.isOnGround)	currentState = CharacterState::Idle;
				else  if (!pStatus.isOnGround && pStatus.lookRight && !isJumpUp) wallSlide(); // 벽타기
				else if (!pStatus.isOnGround && pStatus.lookRight && isJumpUp);
			}
			
			else move(pStatus.lookRight);
		}

		else if (KEYMANAGER->isStayKeyDown(VK_LEFT) && !(pStatus.isWallKick && !pStatus.wallKickRight))  // !(pStatus.velocityX > 0.0f) 
		{
			multiInput = false;
			pStatus.lookRight = false;
			pressLeft = true;
			if (pStatus.touchLeft)
			{
				if(pStatus.isOnGround) currentState = CharacterState::Idle;
				else if (!pStatus.isOnGround && !pStatus.lookRight && !isJumpUp) wallSlide(); // 벽타기
				else if (!pStatus.isOnGround && !pStatus.lookRight && isJumpUp);
			}

			else move(pStatus.lookRight);
		}

		// 대기 상태 변환 + 대기 상태 변환 애니메이션
		else if (!(KEYMANAGER->isStayKeyDown(VK_RIGHT) || KEYMANAGER->isStayKeyDown(VK_LEFT)))
		{
			pStatus.velocityX = 0.0f;

			if (pStatus.isOnGround)
			{
				returnToIdle();
			}

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

		

		/////////////////////////////////
		// 점프 파트
		/////////////////////////////////

		// 점프 누르면 점프 파워 만큼 계속 올라가고
		// 점프를 떼면 바로 낙하를 하게 만들어서 소점프, 대점프 구현
		
		if (KEYMANAGER->isOnceKeyDown('X') && !multiInput) // 'ㅌ' 검색을 위한 주석ㅋㅋㅋ
		{
			jump();
		}

		if (KEYMANAGER->isOnceKeyUp('X'))
		{
			if (pStatus.velocityY < 0.0f) pStatus.velocityY = 0.0f;
			isJumpUp = false;
		}

		/////////////////////////////////
		// 공격 파트
		/////////////////////////////////

		if (now - lastShootTime >= shotCoolDown) normalBurstAble = true;
		else normalBurstAble = false;

		now = TIMEMANAGER->getWorldTime();

		if (TIMEMANAGER->getWorldTime() - chargeBurstCount >= 0.5f) chargeBurstDelay = false;

		
		if (KEYMANAGER->isOnceKeyDown('C')) // 'ㅊ' 검색을 위한 주석ㅋㅋㅋ
		{
			prevAniFrame = pStatus.player->getFrameX();
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
			chargeCount = 0.0f;
			attCheckOnce = false;
			attCheckOnce = true;
		}

		if (KEYMANAGER->isOnceKeyDown('Z'))						// 'ㅋ' 문자 찾기 용
		{
			dash(pStatus.lookRight);
		}

		if (KEYMANAGER->isStayKeyDown('Z'))
		{
			dashTimer += 0.1f;

			pressDash = true;

			if (dashTimer <= maxDashTime)
			{
				if (pStatus.isDash && !pStatus.isJumpDash)
				{
					pStatus.isDash = true;
					currentState = CharacterState::Dash;

					move(pStatus.lookRight);
				}
			}

			else
			{
				if (!pStatus.isJumpDash && pStatus.isDash)
				{
					pStatus.isDash = false;
					aniDash = false;
					pStatus.player->setFrameX(0);
					currentState = CharacterState::Idle;
				}
			}
		}

		if (KEYMANAGER->isOnceKeyUp('Z'))
		{
			SOUNDMANAGER->stop("SFX_DashStart");

			pressDash = false;
			
			if (!pStatus.isJumpDash)
			{
				pStatus.isDash = false;
				pStatus.player->setFrameX(0);
			}
			aniDash = false;
			dashTimer = 0.0f;
		}
	}

#pragma endregion


#pragma region Animation Change + SFX Sound Play


	applyForce();
	sfxPlay();
	frameCheck();
	currentAnimChange();
	pStatus.player->play(animSpeed);

#pragma endregion

}

void X::render(void)
{
	pStatus.player->frameAlphaRender(getMemDC(), hitBoxCenter.x - pStatus.player->getFrameWidth() / 2 - animOffset.x,
		hitBoxCenter.y - pStatus.player->getFrameHeight() - animOffset.y,
		pStatus.player->getFrameX(), pStatus.lookRight, 255);
	
	if (UIMANAGER->getIsDebugMode() == true)
	{
		// 상태값 출력

		// 캐릭터 좌표
		TEXTMANAGER->drawTextColor(getMemDC(), WINSIZE_X / 50, WINSIZE_Y / 100, "현재 상태", "DNF_M_18", RGB(0, 255, 255));
		TEXTMANAGER->drawTextColor(getMemDC(), WINSIZE_X / 50, WINSIZE_Y / 100 + 20, printBodyState(), "DNF_M_18", RGB(0, 255, 255));

		TEXTMANAGER->drawTextColor(getMemDC(), WINSIZE_X / 50, WINSIZE_Y / 100 + 45, "스피드", "DNF_M_18", RGB(0, 255, 255));
		TEXTMANAGER->drawTextColor(getMemDC(), WINSIZE_X / 50, WINSIZE_Y / 100 + 65, to_string(pStatus.velocityX), "DNF_M_18", RGB(0, 255, 255));

		TEXTMANAGER->drawTextColor(getMemDC(), WINSIZE_X / 50 + 80, WINSIZE_Y / 100, "공격 상태", "DNF_M_18", RGB(0, 255, 255));
		TEXTMANAGER->drawTextColor(getMemDC(), WINSIZE_X / 50 + 80, WINSIZE_Y / 100 + 20, printAttState(), "DNF_M_18", RGB(0, 255, 255));

		TEXTMANAGER->drawTextColor(getMemDC(), WINSIZE_X / 50 + 80, WINSIZE_Y / 100 + 45, "점프 파워", "DNF_M_18", RGB(0, 255, 255));
		TEXTMANAGER->drawTextColor(getMemDC(), WINSIZE_X / 50 + 80, WINSIZE_Y / 100 + 65, to_string(pStatus.velocityY), "DNF_M_18", RGB(0, 255, 255));

		TEXTMANAGER->drawTextColor(getMemDC(), WINSIZE_X / 50 + 160, WINSIZE_Y / 100, "캐릭터 X", "DNF_M_18", RGB(0, 255, 255));
		TEXTMANAGER->drawTextColor(getMemDC(), WINSIZE_X / 50 + 160, WINSIZE_Y / 100 + 20, to_string(charPos.x), "DNF_M_18", RGB(0, 255, 255));

		TEXTMANAGER->drawTextColor(getMemDC(), WINSIZE_X / 50 + 240, WINSIZE_Y / 100, "캐릭터 Y", "DNF_M_18", RGB(0, 255, 255));
		TEXTMANAGER->drawTextColor(getMemDC(), WINSIZE_X / 50 + 240, WINSIZE_Y / 100 + 20, to_string(charPos.y), "DNF_M_18", RGB(0, 255, 255));
		
		TEXTMANAGER->drawTextColor(getMemDC(), WINSIZE_X / 50 + 160, WINSIZE_Y / 100 + 45, "카메라 X", "DNF_M_18", RGB(0, 255, 255));
		TEXTMANAGER->drawTextColor(getMemDC(), WINSIZE_X / 50 + 160, WINSIZE_Y / 100 + 65, to_string(CAMERAMANAGER->getCameraPos().x), "DNF_M_18", RGB(0, 255, 255));

		TEXTMANAGER->drawTextColor(getMemDC(), WINSIZE_X / 50 + 240, WINSIZE_Y / 100 + 45, "카메라 Y", "DNF_M_18", RGB(0, 255, 255));
		TEXTMANAGER->drawTextColor(getMemDC(), WINSIZE_X / 50 + 240, WINSIZE_Y / 100 + 65, to_string(CAMERAMANAGER->getCameraPos().y), "DNF_M_18", RGB(0, 255, 255));
		
		string temp;
		if (CAMERAMANAGER->getLockX()) temp = "O";
		else temp = "X";

		TEXTMANAGER->drawTextColor(getMemDC(), WINSIZE_X / 50 + 320, WINSIZE_Y / 100, "카메라 Lock X", "DNF_M_18", RGB(0, 255, 255));
		TEXTMANAGER->drawTextColor(getMemDC(), WINSIZE_X / 50 + 320, WINSIZE_Y / 100 + 20, temp, "DNF_M_18", RGB(0, 255, 255));

		if (CAMERAMANAGER->getLockY()) temp = "O";
		else temp = "X";

		TEXTMANAGER->drawTextColor(getMemDC(), WINSIZE_X / 50 + 320, WINSIZE_Y / 100 + 45, "카메라 Lock Y", "DNF_M_18", RGB(0, 255, 255));
		TEXTMANAGER->drawTextColor(getMemDC(), WINSIZE_X / 50 + 320, WINSIZE_Y / 100 + 65, temp, "DNF_M_18", RGB(0, 255, 255));
		
		if (pStatus.lookRight) temp = "우측";
		else temp = "좌축";
		
		TEXTMANAGER->drawTextColor(getMemDC(), WINSIZE_X / 50 + 420, WINSIZE_Y / 100, "시야 방향", "DNF_M_18", RGB(0, 255, 255));
		TEXTMANAGER->drawTextColor(getMemDC(), WINSIZE_X / 50 + 420, WINSIZE_Y / 100 + 20, temp, "DNF_M_18", RGB(0, 255, 255));

		if (pStatus.isDash) temp = "O";
		else temp = "X";

		TEXTMANAGER->drawTextColor(getMemDC(), WINSIZE_X / 50 + 500, WINSIZE_Y / 100, "대시 중", "DNF_M_18", RGB(0, 255, 255));
		TEXTMANAGER->drawTextColor(getMemDC(), WINSIZE_X / 50 + 500, WINSIZE_Y / 100 + 20, temp, "DNF_M_18", RGB(0, 255, 255));

		if (pStatus.isJumpDash) temp = "O";
		else temp = "X";

		TEXTMANAGER->drawTextColor(getMemDC(), WINSIZE_X / 50 + 500, WINSIZE_Y / 100 + 45, "대시 점프 중", "DNF_M_18", RGB(0, 255, 255));
		TEXTMANAGER->drawTextColor(getMemDC(), WINSIZE_X / 50 + 500, WINSIZE_Y / 100 + 65, temp, "DNF_M_18", RGB(0, 255, 255));

		// 히트박스 출력
		DrawRectMakeColor(getMemDC(), pStatus.hitBox, RGB(255, 0, 0), 2);
		DrawRectMakeColor(getMemDC(), pStatus.floorCheck, RGB(0, 0, 255), 2);
	}
}

void X::jump(void)
{
	if (pStatus.isOnGround || pStatus.touchLeft || pStatus.touchRight) Player::jump();
	else; // 호버링
}

void X::dash(bool direction)
{
	if (pStatus.isOnGround) Player::dash(direction);
	else if (!pStatus.isOnGround); // 아머가 있을때 에어대시
}

void X::attack(void)
{
	attState = SholderState::Burst;
	SOUNDMANAGER->play("SFX_X_Burster1", 0.5f);

	burstloop = true;
	lastShootTime = now;
	isCharging = false;

	if (pStatus.lookRight)
	{
		if (currentState == CharacterState::WallSlide) bManager->fire(0, pStatus.hitBox.left + pStatus.firePointX, pStatus.hitBox.top + pStatus.firePointY, !pStatus.lookRight);
		else bManager->fire(0, pStatus.hitBox.right + pStatus.firePointX, pStatus.hitBox.top + pStatus.firePointY, pStatus.lookRight);
	}

	else
	{
		if (currentState == CharacterState::WallSlide) bManager->fire(0, pStatus.hitBox.right + pStatus.firePointX, pStatus.hitBox.top + pStatus.firePointY, !pStatus.lookRight);
		else bManager->fire(0, pStatus.hitBox.left + pStatus.firePointX, pStatus.hitBox.top + pStatus.firePointY, pStatus.lookRight);
	}
}

void X::chargeBurst(void)
{
	if (chargeCount >= 0.4f && chargeCount < 1.5f)
	{
		attState = SholderState::Burst;
		SOUNDMANAGER->play("SFX_X_Burster2", 0.5f);
		
		// 이동 공격을 위한 프레임 저장
		prevAniFrame = pStatus.player->getFrameX();
		// 공격 회수 장면을 위한 타이머 저장 -> 안하면 바로 내림ㅋㅋ
		attackTimer = TIMEMANAGER->getWorldTime();

		burstloop = true;
		chargeBurstCount = TIMEMANAGER->getWorldTime();
		chargeBurstDelay = true;
		isCharging = false;

		if (pStatus.lookRight)
		{
			if (currentState == CharacterState::WallSlide) bManager->fire(1, pStatus.hitBox.left + pStatus.firePointX, pStatus.hitBox.top + pStatus.firePointY, !pStatus.lookRight);
			else bManager->fire(1, pStatus.hitBox.right + pStatus.firePointX, pStatus.hitBox.top + pStatus.firePointY, pStatus.lookRight);
		}
		else
		{
			if (currentState == CharacterState::WallSlide) bManager->fire(1, pStatus.hitBox.right + pStatus.firePointX, pStatus.hitBox.top + pStatus.firePointY, !pStatus.lookRight);
			else bManager->fire(1, pStatus.hitBox.left + pStatus.firePointX, pStatus.hitBox.top + pStatus.firePointY, pStatus.lookRight);
		}
	}

	else if (chargeCount >= 1.5f)
	{
		attState = SholderState::LargeBurst;

		SOUNDMANAGER->play("SFX_X_Burster3", 0.5f);
		if (RND->getInt(10) < 5) SOUNDMANAGER->play("Voice_X_Burster", 0.5f);
					
		attackTimer = TIMEMANAGER->getWorldTime();

		burstloop = true;
		chargeBurstCount = TIMEMANAGER->getWorldTime();
		chargeBurstDelay = true;
		isCharging = false;

		if (pStatus.lookRight)
		{
			if (currentState == CharacterState::WallSlide) bManager->fire(2, pStatus.hitBox.left + pStatus.firePointX, pStatus.hitBox.top + pStatus.firePointY, !pStatus.lookRight);
			else bManager->fire(2, pStatus.hitBox.right + pStatus.firePointX, pStatus.hitBox.top + pStatus.firePointY, pStatus.lookRight);
		}
		else
		{
			if (currentState == CharacterState::WallSlide) bManager->fire(2, pStatus.hitBox.right + pStatus.firePointX, pStatus.hitBox.top + pStatus.firePointY, !pStatus.lookRight);
			else bManager->fire(2, pStatus.hitBox.left + pStatus.firePointX, pStatus.hitBox.top + pStatus.firePointY, pStatus.lookRight);
		}
	}

	SOUNDMANAGER->stop("SFX_X_BurstCharge");
}


void X::currentAnimChange(void)
{
	// 상태값에 다른 애니메이션 변경

	////////////////////////
	// 워프
	////////////////////////

	if (currentState == CharacterState::Warp)
	{
		currentAnim = "X_Spawn";
		animSpeed = 0.07f;
		animOffset.x = 0;
		animOffset.y = - 17 * SCALE_FACTOR;
		pStatus.firePointX = 0 * SCALE_FACTOR;
		pStatus.firePointY = 10 * SCALE_FACTOR;
	}

	////////////////////////
	// 대기
	////////////////////////

	else if (currentState == CharacterState::Idle)
	{
		if (pStatus.lookRight) pStatus.firePointX = 0 * SCALE_FACTOR;
		else pStatus.firePointX = 0 * SCALE_FACTOR;

		pStatus.firePointY = 11 * SCALE_FACTOR;

		if (attState == SholderState::LargeBurst)
		{
			if (previousState == CharacterState::Walk) attState = SholderState::Burst;
			else
			{
				currentAnim = "X_StandChargeBurst";

				animSpeed = 0.07f;
				animOffset.x = -1;
				animOffset.y = -1 * SCALE_FACTOR;
			}
		}

		else if (attState == SholderState::Burst)
		{
			currentAnim = "X_StandBurstLoop";
			animSpeed = 0.07f;
			animOffset.x = -1;
			animOffset.y = -1 * SCALE_FACTOR;
			
		}

		else if (attState == SholderState::Hold)
		{
			if (previousAnim != "X_StandBurstEnd") pStatus.player->setFrameX(0);

			currentAnim = "X_StandBurstEnd";
			animSpeed = 0.1f;
			animOffset.x = 0;
			animOffset.y = -1 * SCALE_FACTOR;

		}

		else if (attState == SholderState::None)
		{
			currentAnim = "X_Idle";
			animSpeed = 0.17f;
			animOffset.x = 0;
			animOffset.y = -1 * SCALE_FACTOR;
		}
	}

	////////////////////////
	// 걷기
	////////////////////////

	else if (currentState == CharacterState::Walk)
	{
		if (pStatus.lookRight) pStatus.firePointX = +14 * SCALE_FACTOR;
		else pStatus.firePointX = -14 * SCALE_FACTOR;

		pStatus.firePointY = 8 * SCALE_FACTOR;

		if (!isMoving)
		{
			if (attState == SholderState::Burst || attState == SholderState::LargeBurst)
			{
				currentAnim = "X_WalkBurstStart";
				animSpeed = 0.1f;
				animOffset.x = 0;
				animOffset.y = -1 * SCALE_FACTOR;

				if (pStatus.lookRight) animOffset.x = -7;
				else if (!pStatus.lookRight) animOffset.x = 14;
			}

			else if (attState == SholderState::None || attState == SholderState::Hold)
			{
				currentAnim = "X_WalkStart";
				animSpeed = 0.1f;
				if (pStatus.lookRight) animOffset.x = 0;
				else if (!pStatus.lookRight) animOffset.x = 0;
			}
		}

		animSpeed = 0.04f;
		animOffset.y = -1 * SCALE_FACTOR;

		if ((currentAnim == "X_WalkStart" || currentAnim == "X_WalkBurstStart")
			&& pStatus.player->getFrameX() >= pStatus.player->getMaxFrameX()) isMoving = true;

		if (isMoving)
		{
			if (attState == SholderState::Burst || attState == SholderState::LargeBurst)
			{
				if (pStatus.lookRight) animOffset.x = -7;
				else if (!pStatus.lookRight) animOffset.x = 14;

				currentAnim = "X_WalkBurstLoop";
				isMoving = true;
			}

			else if (attState == SholderState::None || attState == SholderState::Hold)
			{
				if (pStatus.lookRight) animOffset.x = 0;
				else if (!pStatus.lookRight) animOffset.x = 0;
				currentAnim = "X_WalkLoop";
				attState = SholderState::None;
				isMoving = true;
			}
		}
	}


	////////////////////////
	// 대시
	////////////////////////

	else if (currentState == CharacterState::Dash && pStatus.isDash)
	{
		if (pStatus.player->getFrameX() == 0) pStatus.firePointY = 15 * SCALE_FACTOR;
		else if (pStatus.player->getFrameX() == 1) pStatus.firePointY = 21 * SCALE_FACTOR;
		else pStatus.firePointY = 30 * SCALE_FACTOR;

		if (pStatus.lookRight)
		{
			pStatus.firePointX = +14 * SCALE_FACTOR;
			animOffset.x = -14 * SCALE_FACTOR;
		}

		else
		{
			pStatus.firePointX = -14 * SCALE_FACTOR;
			animOffset.x = 14 * SCALE_FACTOR;
		}

		pStatus.firePointY = 10 * SCALE_FACTOR;
		
		currentAnim = "X_Dash";
		animSpeed = 0.04f;

		if (attState == SholderState::Burst || attState == SholderState::LargeBurst)
		{	
			currentAnim = "X_DashBurst";
		}

		if (pStatus.player->getFrameX() >= 2) aniDash = true;

		if (aniDash)
		{
			if (pStatus.player->getFrameX() >= pStatus.player->getMaxFrameX()) pStatus.player->setFrameX(2);
		}
		else
		{
			if(pStatus.player->getFrameX() >= pStatus.player->getMaxFrameX()) pStatus.player->setFrameX(0);
		}
	}

	////////////////////////
	// 점프
	////////////////////////

	else if (currentState == CharacterState::JumpUp)
	{
		if (pStatus.lookRight)
		{
			animOffset.x = 1 * SCALE_FACTOR;
			pStatus.firePointX = 0 * SCALE_FACTOR;
		}
		else
		{
			animOffset.x = -4 * SCALE_FACTOR;
			pStatus.firePointX = 0 * SCALE_FACTOR;
		}

		pStatus.firePointY = 8 * SCALE_FACTOR;

		animSpeed = 0.06f;
		animOffset.y = -6 * SCALE_FACTOR;

		if (attState == SholderState::Burst || attState == SholderState::LargeBurst)
		{
			currentAnim = "X_JumpBurst";
			if (pStatus.lookRight) animOffset.x = -6 * SCALE_FACTOR;
			else animOffset.x = 3 * SCALE_FACTOR;
		}
		else currentAnim = "X_Jump";

		if (pStatus.player->getFrameX() >= 4) pStatus.player->setFrameX(4);
	}

	////////////////////////
	// 낙하
	////////////////////////

	else if (currentState == CharacterState::FallingDown)
	{
		if (pStatus.lookRight)
		{
			animOffset.x = 1 * SCALE_FACTOR;
			pStatus.firePointX = 0 * SCALE_FACTOR;
		}
		else
		{
			animOffset.x = -4 * SCALE_FACTOR;
			pStatus.firePointX = 8 * SCALE_FACTOR;
		}
		
		pStatus.firePointY = 8 * SCALE_FACTOR;

		animSpeed = 0.06f;
		animOffset.y = -6 * SCALE_FACTOR;

		if (attState == SholderState::Burst || attState == SholderState::LargeBurst)
		{
			currentAnim = "X_JumpBurst";
			if (pStatus.lookRight) animOffset.x = -6 * SCALE_FACTOR; 
			else animOffset.x = 3 * SCALE_FACTOR;
		}
		else currentAnim = "X_Jump";

		if (pStatus.player->getFrameX() >= pStatus.player->getMaxFrameX()) pStatus.player->setFrameX(pStatus.player->getMaxFrameX());
	}

	////////////////////////
	// 벽 타기
	////////////////////////

	else if (currentState == CharacterState::WallSlide)
	{
		animSpeed = 0.06f;
		if (pStatus.lookRight) animOffset.x = 2 * SCALE_FACTOR;
		else animOffset.x = -2 * SCALE_FACTOR;
		animOffset.y = -12 * SCALE_FACTOR;

		pStatus.firePointY = 9 * SCALE_FACTOR;

		if (attState == SholderState::Burst || attState == SholderState::LargeBurst) currentAnim = "X_WallSlideBurst";
		else currentAnim = "X_WallSlide";

		if (pStatus.player->getFrameX() >= pStatus.player->getMaxFrameX()) pStatus.player->setFrameX(pStatus.player->getMaxFrameX());
	}

	////////////////////////
	// 벽 차기
	////////////////////////

	else if (currentState == CharacterState::WallKick)
	{
		animSpeed = 0.06f;
		if (pStatus.lookRight) animOffset.x = -4 * SCALE_FACTOR;
		else animOffset.x = 4 * SCALE_FACTOR;
		animOffset.y = -8 * SCALE_FACTOR;
		
		if (attState == SholderState::Burst || attState == SholderState::LargeBurst) currentAnim = "X_WallKickBurst";
		else currentAnim = "X_WallKick";
	}

	////////////////////////
	// 공격 애니메이션
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
			else if (currentState == CharacterState::Idle) attState = SholderState::Hold;
			prevAniFrame = pStatus.player->getFrameX();
		}
	}	 

	hitBoxCenter.x = (pStatus.hitBox.left + pStatus.hitBox.right) / 2;
	hitBoxCenter.y = pStatus.hitBox.bottom;	

	// pStatus.hitBox.right = pStatus.hitBox.left + hitBoxWidth;
	// pStatus.hitBox.top = pStatus.hitBox.bottom - hitBoxHeight;
	setHitBox();
	pStatus.player = IMAGEMANAGER->findImage(currentAnim);

	// 애니메이션 사이의 프레임 연결
	if (previousAnim != currentAnim)
	{
		// 이동 + 공격 애니메이션은 전부 추가
		if (currentAnim.find("Burst") != string::npos)
		{
			// 이전 동작과 현재 동작이 일치해야할 때 -> 대기동작 안빼면 이상해짐
			if (currentState == previousState && currentState != CharacterState::Idle) pStatus.player->setFrameX(prevAniFrame);
			// 건들 필요 없음 -> 언제나 공격이 0으로 시작해야 하는 상황 -> + 세이버...?
			else if (currentState == CharacterState::Idle || currentState == CharacterState::WallSlide) pStatus.player->setFrameX(0);
			// 특수 동작들 -> 동작이 추가되면 조건 추가해줘야함
			else if (currentState == CharacterState::FallingDown && previousState == CharacterState::Walk) pStatus.player->setFrameX(5);
			else if (currentState == CharacterState::FallingDown && previousState == CharacterState::JumpUp) pStatus.player->setFrameX(prevAniFrame);
		}

		else if (previousAnim.find("Burst") != string::npos)
		{
			if ((currentState != CharacterState::Idle)) pStatus.player->setFrameX(prevAniFrame);
			else pStatus.player->setFrameX(0);
		}

		else if (previousAnim.find("Kick") && currentState == CharacterState::FallingDown) pStatus.player->setFrameX(5);
				
		else
		{
			pStatus.player->setFrameX(0);
		}
	}

	// 애니메이션 비교용 변수 초기화
	previousAnim = currentAnim;
	previousState = currentState;
	
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
				pStatus.player->setFrameX(0);
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
	if (CAMERAMANAGER->getLockX() == true)
	{
		pStatus.hitBox.left = charPos.x - hitBoxWidth / 2 - CAMERAMANAGER->getCameraPos().x;
		pStatus.hitBox.right = charPos.x + hitBoxWidth / 2 - CAMERAMANAGER->getCameraPos().x;
	}

	else
	{
		pStatus.hitBox.left = hitBoxCenter.x - (hitBoxWidth / 2);
		pStatus.hitBox.right = hitBoxCenter.x + (hitBoxWidth / 2);
	}

	if (CAMERAMANAGER->getLockY() == true)
	{
		pStatus.hitBox.bottom = charPos.y - CAMERAMANAGER->getCameraPos().y;
		pStatus.hitBox.top = pStatus.hitBox.bottom - hitBoxHeight;
	}
	
	else
	{
		pStatus.hitBox.bottom = hitBoxCenter.y;
		pStatus.hitBox.top = hitBoxCenter.y - hitBoxHeight;
	}

	switch (currentState)
	{
	case CharacterState::Warp:
		hitBoxWidth = 25 * SCALE_FACTOR;
		hitBoxHeight = 41 * SCALE_FACTOR;
		break;
	case CharacterState::Idle:
		hitBoxWidth = 25 * SCALE_FACTOR;
		hitBoxHeight = 41 * SCALE_FACTOR;
		break;
	case CharacterState::Walk:
		hitBoxWidth = 25 * SCALE_FACTOR;
		hitBoxHeight = 41 * SCALE_FACTOR;
		break;
	case CharacterState::JumpUp:
		hitBoxWidth = 25 * SCALE_FACTOR;
		hitBoxHeight = 45 * SCALE_FACTOR;
		break;
	case CharacterState::FallingDown:
		hitBoxWidth = 25 * SCALE_FACTOR;
		hitBoxHeight = 45 * SCALE_FACTOR;
		break;
	case CharacterState::Dash:
		hitBoxWidth = 25 * SCALE_FACTOR;
		hitBoxHeight = 28 * SCALE_FACTOR;
		break;
	case CharacterState::WallSlide:
		hitBoxWidth = 25 * SCALE_FACTOR;
		hitBoxHeight = 45 * SCALE_FACTOR;
		break;
	}

	pStatus.floorCheck = RectMake((pStatus.hitBox.left + pStatus.hitBox.right) / 2 - 5 * SCALE_FACTOR, pStatus.hitBox.bottom, 10 * SCALE_FACTOR, 5);
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
	charPos.x = x;
	charPos.y = y;

	pStatus.hitBox = RectMakeCenter(charPos.x, 0 - hitBoxHeight / 2, hitBoxWidth, hitBoxHeight);
	pStatus.floorCheck = RectMakeCenter(charPos.x, charPos.y, 10, 4);

	hitBoxCenter.x = (pStatus.hitBox.left + pStatus.hitBox.right) / 2;
	hitBoxCenter.y = pStatus.hitBox.bottom - hitBoxHeight;

	// 캐릭터 세팅
	pStatus.hp = pStatus.maxHp;
	pStatus.mp = pStatus.maxMp;
	pStatus.moveSpeed = 4.5f;
	pStatus.dashSpeed = 12.0f;
	
	pStatus.charName = "X_";
	pStatus.touchLeft = false;
	pStatus.touchRight = false;
	pStatus.wallKickRight = true;

	////////////////////
	// 상태 초기화
	////////////////////

	// 캐릭터 초기화
	previousState = CharacterState::Idle;
	currentState = CharacterState::Warp;
	attState == SholderState::None;
	pStatus.lookRight = true;
	isMoving = false;
	pStatus.isDash = false;
	pStatus.isWallKick = false;
	colorType = 0;

	// 입력 초기화
	inputEnabled = false;
	multiInput = false;

	// 점프 초기화
	pStatus.jumpPower = -15.0;
	pStatus.maxFallSpeed = 12.0f;
	pStatus.velocityY = 0.0f;

	// 벽타기 초기화
	isJumpUp = false;
	pressRight = false;
	pressLeft = false;
	
	// 대시 초기화
	dashTimer = 0.0f;
	pStatus.isDash = false;
	pStatus.isJumpDash = false;
	pressDash = false;

	// 상태 초기화 - 공격 관련
	normalBurstAble = true;
	chargeBurstDelay = false;
	attCheckOnce = false;
	attackTimer = 0.0f;
	lastShootTime = 0.0f;
	shotCoolDown = 0.06f;
	burstSound = "SFX_X_Burster1";
	chargeCount = 0.0f;
	chargeSpeed = 0.01f;
	isCharging = false;
	pStatus.firePointY = 12 * SCALE_FACTOR;
	
	// 애니메이션 초기화
	currentAnim = "X_Spawn";
	previousAnim = currentAnim;
	pStatus.player = IMAGEMANAGER->findImage(currentAnim);
	afterImage = IMAGEMANAGER->findImage(currentAnim);
	animSpeed = 0.1f;
	attChange = false;
	burstloop = false;
	aniDash = false;
	chargeEffect = IMAGEMANAGER->findImage("SFX_X_Charge");

	CAMERAMANAGER->setPlayerPos(charPos.x, charPos.y - hitBoxHeight / 2);
}

void X::returnToIdle(void)
{
	switch (previousState)
	{
	case CharacterState::Dash:
		// currentAnim = "X_DashEnd";
		if (pStatus.player->getFrameX() >= pStatus.player->getMaxFrameX())
			currentState = CharacterState::Idle;
		break;

	case CharacterState::JumpUp:
		currentState = CharacterState::Idle;
		break;

	case CharacterState::FallingDown:
		currentState = CharacterState::Idle;
		break;

	/*
	case CharacterState::JumpUp:
		// currentAnim = "X_Land";
		if (pStatus.player->getFrameX() >= pStatus.player->getMaxFrameX())
			currentState = CharacterState::Idle;
		break;
	*/
	default:
		currentState = CharacterState::Idle;
		break;
	}

	isMoving = false;
	// aniDash = false;
}


