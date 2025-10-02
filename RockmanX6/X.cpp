#include "Stdafx.h"
#include "X.h"

HRESULT X::init(void)
{

	return S_OK;
}

HRESULT X::init(int x, int y)
{
	hitBoxWidth = 60;
	hitBoxHeight = 80;

	cout << "Init" << y << endl;

	pStatus.maxHp = 10.0;
	pStatus.maxMp = 10.0;

	spawn(x, y);

	inputEnabled = false;

	return S_OK;
}

void X::release(void)
{

}

void X::update(void)
{
	CAMERAMANAGER->setPlayerPos(charPos.x, charPos.y);
	applyGravity();
	
#pragma region WarpIn

	/////////////////////////////////
	// 첫 등장 연출 파트
	/////////////////////////////////

	
	// 게임 시작시 스테이지에 소환 되는 상황
	if (currentState == CharacterState::Warp)
	{
		// 하늘에서 내려오고 있을 때 - 스프라이트 보면 애니메이션이 동작 안함
		if (pStatus.hitBox.bottom <= WINSIZE_Y)
		//if (pStatus.isOnGround == false)
		{
			pStatus.player->pause();
			
			pStatus.isOnGround = false;
			inputEnabled = false;
		}

		// 땅에 도착
		else
		{
			pStatus.isOnGround = true;

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

		if (KEYMANAGER->isStayKeyDown(VK_RIGHT) && KEYMANAGER->isStayKeyDown(VK_LEFT))
		{
			if (KEYMANAGER->isStayKeyDown(VK_RIGHT))
			{
				pStatus.lookRight = true;
				move(pStatus.lookRight);
			}

			else if (KEYMANAGER->isStayKeyDown(VK_LEFT))
			{
				pStatus.lookRight = false;
				move(pStatus.lookRight);
			}
		}

		else if (KEYMANAGER->isStayKeyDown(VK_RIGHT))
		{
			pStatus.lookRight = true;
			move(pStatus.lookRight);
		}

		else if (KEYMANAGER->isStayKeyDown(VK_LEFT))
		{
			pStatus.lookRight = false;
			move(pStatus.lookRight);
		}

		// 대기 상태 변환 + 대기 상태 변환 애니메이션
		else if (!(KEYMANAGER->isStayKeyDown(VK_RIGHT) || KEYMANAGER->isStayKeyDown(VK_LEFT)))
		{
			returnToIdle();
			pStatus.isAtt = false;
			attChange = false;
		}

		/////////////////////////////////
		// 점프 파트
		/////////////////////////////////

		if (KEYMANAGER->isStayKeyDown('X')) // 'ㅌ' 검색을 위한 주석ㅋㅋㅋ
		{
			cout << CAMERAMANAGER->getCameraPos().y << endl;

			if (pStatus.isOnGround) Player::jump();
			else if (!pStatus.isOnGround) jump();
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
			pStatus.isAtt = true;
			
			if (normalBurstAble == true && chargeBurstDelay == false) attack();
		}

		if (KEYMANAGER->isStayKeyDown('C'))
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
	}

#pragma endregion


#pragma region Animation Change + SFX Sound Play

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
		pStatus.player->getFrameX(), animDir, 255);
	
	if (UIMANAGER->getIsDebugMode() == true)
	{
		// 상태값 출력
		TEXTMANAGER->drawTextColor(getMemDC(), WINSIZE_X / 50, WINSIZE_Y / 100, "현재 상태", "DNF_M_18", RGB(0,255,255));
		TEXTMANAGER->drawTextColor(getMemDC(), WINSIZE_X / 50 + 80, WINSIZE_Y / 100, "공격 상태", "DNF_M_18", RGB(0, 255, 255));
		TEXTMANAGER->drawTextColor(getMemDC(), WINSIZE_X / 50, WINSIZE_Y / 100 + 20, printBodyState(), "DNF_M_18", RGB(0, 255, 255));
		TEXTMANAGER->drawTextColor(getMemDC(), WINSIZE_X / 50 + 80, WINSIZE_Y / 100 + 20, printAttState(), "DNF_M_18", RGB(0, 255, 255));

		// 캐릭터 좌표
		TEXTMANAGER->drawTextColor(getMemDC(), WINSIZE_X / 50 + 160, WINSIZE_Y / 100, "캐릭터 X", "DNF_M_18", RGB(0, 255, 255));
		TEXTMANAGER->drawTextColor(getMemDC(), WINSIZE_X / 50 + 160, WINSIZE_Y / 100 + 20, to_string(charPos.x), "DNF_M_18", RGB(255, 0, 0));

		TEXTMANAGER->drawTextColor(getMemDC(), WINSIZE_X / 50 + 240, WINSIZE_Y / 100, "캐릭터 Y", "DNF_M_18", RGB(0, 255, 255));
		TEXTMANAGER->drawTextColor(getMemDC(), WINSIZE_X / 50 + 240, WINSIZE_Y / 100 + 20, to_string(charPos.y), "DNF_M_18", RGB(0, 255, 255));

		TEXTMANAGER->drawTextColor(getMemDC(), WINSIZE_X / 50 + 160, WINSIZE_Y / 100 + 40, "카메라 X", "DNF_M_18", RGB(0, 255, 255));
		TEXTMANAGER->drawTextColor(getMemDC(), WINSIZE_X / 50 + 160, WINSIZE_Y / 100 + 60, to_string(CAMERAMANAGER->getCameraPos().x), "DNF_M_18", RGB(255, 0, 0));

		TEXTMANAGER->drawTextColor(getMemDC(), WINSIZE_X / 50 + 240, WINSIZE_Y / 100 + 40, "카메라 Y", "DNF_M_18", RGB(0, 255, 255));
		TEXTMANAGER->drawTextColor(getMemDC(), WINSIZE_X / 50 + 240, WINSIZE_Y / 100 + 60, to_string(CAMERAMANAGER->getCameraPos().y), "DNF_M_18", RGB(0, 255, 255));
		
		string Check;
		if (CAMERAMANAGER->getLockX()) Check = "O";
		else Check = "X";

		TEXTMANAGER->drawTextColor(getMemDC(), WINSIZE_X / 50 + 300, WINSIZE_Y / 100 + 40, "카메라 LOCK X", "DNF_M_18", RGB(0, 255, 255));
		TEXTMANAGER->drawTextColor(getMemDC(), WINSIZE_X / 50 + 300, WINSIZE_Y / 100 + 60, Check, "DNF_M_18", RGB(0, 255, 255));
		
		
		TEXTMANAGER->drawTextColor(getMemDC(), WINSIZE_X / 50 + 300, WINSIZE_Y / 100, "차징 게이지", "DNF_M_18", RGB(0, 255, 255));
		TEXTMANAGER->drawTextColor(getMemDC(), WINSIZE_X / 50 + 300, WINSIZE_Y / 100 + 20, to_string(chargeCount), "DNF_M_18", RGB(0, 255, 255));
		
		string shoot;
		if (normalBurstAble) shoot = "가능";
		else shoot = "불가능";

		TEXTMANAGER->drawTextColor(getMemDC(), WINSIZE_X / 50 + 420, WINSIZE_Y / 100, "샤격 가능", "DNF_M_18", RGB(0, 255, 255));
		TEXTMANAGER->drawTextColor(getMemDC(), WINSIZE_X / 50 + 420, WINSIZE_Y / 100 + 20, shoot, "DNF_M_18", RGB(0, 255, 255));
		
		if (chargeBurstDelay) shoot = "있음";
		else shoot = "없음";

		TEXTMANAGER->drawTextColor(getMemDC(), WINSIZE_X / 50 + 500, WINSIZE_Y / 100, "차지샷 후딜레이", "DNF_M_18", RGB(0, 255, 255));
		TEXTMANAGER->drawTextColor(getMemDC(), WINSIZE_X / 50 + 500, WINSIZE_Y / 100 + 20, shoot, "DNF_M_18", RGB(0, 255, 255));
		
		// 히트박스 출력
		DrawRectMakeColor(getMemDC(), pStatus.hitBox, RGB(255, 0, 0), 2);
	}
}

void X::jump(void)
{
}

void X::attack(void)
{
	attState = SholderState::Burst;
	SOUNDMANAGER->play("SFX_X_Burster1", 0.5f);

	burstloop = true;
	lastShootTime = now;
}

void X::chargeBurst(void)
{
	if (chargeCount >= 0.2f && chargeCount < 1.0f)
	{
		attState = SholderState::Burst;
		SOUNDMANAGER->play("SFX_X_Burster2", 0.5f);
		
		// 이동 공격을 위한 프레임 저장
		prevAniFrame = pStatus.player->getFrameX();
		// 공격 회수 장면을 위한 타이머 저장 -> 안하면 바로 내림ㅋㅋ
		attackTimer = TIMEMANAGER->getWorldTime();
		pStatus.isAtt = true;

		attackTimer = TIMEMANAGER->getWorldTime();
		burstloop = true;
		chargeBurstCount = TIMEMANAGER->getWorldTime();
		chargeBurstDelay = true;
		isCharging = false;
		
	}

	else if (chargeCount >= 1.0f)
	{
		attState = SholderState::LargeBurst;
		SOUNDMANAGER->play("SFX_X_Burster3", 0.5f);
		
		attackTimer = TIMEMANAGER->getWorldTime();
		pStatus.isAtt = true;

		burstloop = true;
		chargeBurstCount = TIMEMANAGER->getWorldTime();
		chargeBurstDelay = true;
		isCharging = false;
		
	}
	SOUNDMANAGER->stop("SFX_X_BurstCharge");
}


void X::currentAnimChange(void)
{
	// 애니메이션 방향 설정
	if (pStatus.lookRight == true) animDir = 1;
	else if (pStatus.lookRight == false) animDir = 0;
	
	// 상태값에 다른 애니메이션 변경
	if (currentState == CharacterState::Warp)
	{
		currentAnim = "X_Spawn";
		animSpeed = 0.07f;
		animOffset.x = 0;
		animOffset.y = - 17 * SCALE_FACTOR;
	}

	else if (currentState == CharacterState::Idle)
	{
		if (attState == SholderState::LargeBurst)
		{
			if (previousState == CharacterState::Walk) attState = SholderState::Burst;
			else
			{
				//if (previousAnim != "X_StandChargeBurst")
					//pStatus.player->setFrameX(0);

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
			if (previousAnim != "X_StandBurstEnd")
				pStatus.player->setFrameX(0);

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

	else if (currentState == CharacterState::Walk)
	{
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

	if (attState == SholderState::Burst || attState == SholderState::LargeBurst)
	{
		if (TIMEMANAGER->getWorldTime() - attackTimer >= attackDuration)
		{
			if (currentState == CharacterState::Walk) attState = SholderState::None;
			else if (currentState == CharacterState::Idle) attState = SholderState::Hold;
			prevAniFrame = pStatus.player->getFrameX();
			pStatus.isAtt = false;
		}
	}
	
	// 애니메이션 비교용 변수 초기화
	previousAnim = currentAnim;
	previousState = currentState;

	// 애니메이션 동작을 초기 상태로 변환
	// 공격 중 일 때는 예외 -> 세이버는 빼도 됩니다 그건 서서 쓰는거니까
	if (previousAnim != currentAnim)
	{
		if (currentAnim.find("WalkBurst") != string::npos) pStatus.player->setFrameX(prevAniFrame);
		else if (previousAnim.find("WalkBurst") != string::npos) pStatus.player->setFrameX(prevAniFrame);
		else pStatus.player->setFrameX(0);
	}

	hitBoxCenter.x = (pStatus.hitBox.left + pStatus.hitBox.right) / 2;
	hitBoxCenter.y = pStatus.hitBox.bottom;	

	pStatus.player = IMAGEMANAGER->findImage(currentAnim);
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

void X::spawn(int x, int y)
{
	// 캐릭터 소환
	// 캐릭터 생성
	charPos.x = x;
	charPos.y = y;

	pStatus.hitBox = RectMakeCenter(WINSIZE_X / 2, 0, hitBoxWidth, hitBoxHeight);

	hitBoxCenter.x = (pStatus.hitBox.left + pStatus.hitBox.right) / 2;
	hitBoxCenter.y = pStatus.hitBox.top;

	// 캐릭터 세팅
	pStatus.hp = pStatus.maxHp;
	pStatus.mp = pStatus.maxMp;
	pStatus.speed = 4.0f;
	pStatus.jumpSpeed = 4.0f;
	pStatus.charName = "X_";

	// 상태 초기화
	previousState = CharacterState::Idle;
	currentState = CharacterState::Warp;
	attState == SholderState::None;
	pStatus.lookRight = true;
	animDir = 1;
	pStatus.isAtt = false;
	isMoving = false;
	normalBurstAble = true;
	chargeBurstDelay = false;
	attCheckOnce = false;
	
	// 상태 초기화 - 공격 관련
	attackTimer = 0.0f;
	lastShootTime = 0.0f;
	shotCoolDown = 0.05f;
	burstSound = "SFX_X_Burster1";
	chargeCount = 0.0f;
	chargeSpeed = 0.01f;
	isCharging = false;
	
	// 애니메이션 초기화
	currentAnim = "X_Spawn";
	previousAnim = currentAnim;
	pStatus.player = IMAGEMANAGER->findImage(currentAnim);
	animSpeed = 0.1f;
	attChange = false;
	burstloop = false;
	chargeEffect = IMAGEMANAGER->findImage("SFX_X_Charge");

	animBaseline.x = x;
	animBaseline.y = pStatus.hitBox.bottom - animOffset.y;

	CAMERAMANAGER->setPlayerPos(charPos.x, charPos.y);
}

void X::returnToIdle(void)
{
	switch (currentState)
	{
	case CharacterState::Dash:
		// currentAnim = "X_DashEnd";
		if (pStatus.player->getFrameX() >= pStatus.player->getMaxFrameX())
			currentState = CharacterState::Idle;
		break;

	case CharacterState::JumpUp:
		// currentAnim = "X_Land";
		if (pStatus.player->getFrameX() >= pStatus.player->getMaxFrameX())
			currentState = CharacterState::Idle;
		break;

	case CharacterState::FallingDown:
		// currentAnim = "X_Land";
		if (pStatus.player->getFrameX() >= pStatus.player->getMaxFrameX())
			currentState = CharacterState::Idle;
		break;

	default:
		currentState = CharacterState::Idle;
		break;
	}

	isMoving = false;
}

