#include "Stdafx.h"
#include "X.h"

HRESULT X::init(void)
{

	return S_OK;
}

HRESULT X::init(int x, int y)
{
	hixWidth = 60;
	hixHeight = 80;

	pStatus.maxHp = 10;
	pStatus.maxMp = 10;

	spawn(x, y);

	inputEnabled = false;

	return S_OK;
}

void X::release(void)
{

}

void X::update(void)
{
	currentAnimChange();
	pStatus.player->play(animSpeed);
	
	/////////////////////////////////
	// 첫 등장 연출 파트
	/////////////////////////////////

	// 게임 시작시 스테이지에 소환 되는 상황
	if (currentState == CharacterState::Warp)
	{
		// 하늘에서 내려오고 있을 때 - 스프라이트 보면 애니메이션이 동작 안함
		if (pStatus.hitBox.bottom < WINSIZE_Y)
		//if (pStatus.isOnGround == false)
		{
			pStatus.player->pause();

			pStatus.hitBox.top += 12;
			pStatus.hitBox.bottom += 12;
			animBaseline.y += 12;

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

	/////////////////////////////////
	// 플레이어가 캐릭터 상태 컨트롤 파트 
	/////////////////////////////////

	// 플레이어의 컨트롤을 잠시 막아야 하는 상황
	else if (currentState == CharacterState::OverPower || currentState == CharacterState::Dead) inputEnabled = false;
	else if (currentState == CharacterState::SpecialAtt) inputEnabled = false;
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

		if (KEYMANAGER->isStayKeyDown('X'))
		{
			if (pStatus.isOnGround) Player::jump();
			else if (!pStatus.isOnGround) jump();
		}


		/////////////////////////////////
		// 공격 파트
		/////////////////////////////////

		now = TIMEMANAGER->getWorldTime();;
		
		if (KEYMANAGER->isOnceKeyDown('C'))
		{
			/*
			if (now - lastShootTime >= shotCoolDown)
			{
				prevAniFrame = pStatus.player->getFrameX();
				attackTimer = TIMEMANAGER->getWorldTime();
				attCheckOnce = true;
				attack();
			}
			*/

			if (!attCheckOnce)
			{
				prevAniFrame = pStatus.player->getFrameX();
				attackTimer = TIMEMANAGER->getWorldTime();
				attCheckOnce = true;
				pStatus.isAtt = true;

				attack();
			}
		}

		if (KEYMANAGER->isStayKeyDown('C'))
		{
			// 차지
		}

		if (KEYMANAGER->isOnceKeyUp('C'))
		{
			// 차지 시간에 따른 버스터 발사
			attCheckOnce = false;
		}
	}

	if (UIMANAGER->getIsDebugMode())
	{
		// 나중에 왼쪽 위에 이쁘게 텍스트박스로 그릴 것 + 네모나고 이쁘게 히트 박스도 구현
		cout << "히트박스 X : " << pStatus.hitBox.left << endl;
		cout << "히트박스 Y : " << pStatus.hitBox.bottom << endl;

		cout << "애니메이션 X : " << animBaseline.x << endl;
		cout << "애니메이션 Y : " << animBaseline.y << endl;
	}
}

void X::render(void)
{
	pStatus.player->frameAlphaRender(getMemDC(), animBaseline.x - pStatus.player->getFrameWidth() / 2 - animOffset.x,
		animBaseline.y - pStatus.player->getFrameHeight() - animOffset.y,
		pStatus.player->getFrameX(), animDir, 255);

	if (UIMANAGER->getIsDebugMode() == true)
	{
		DrawRectMake(getMemDC(), pStatus.hitBox);
	}
}

void X::jump(void)
{
	CAMERAMANAGER->setPos(4, 0);
}

void X::attack(void)
{
	attState = SholderState::Burst;

	if (now - lastShootTime >= shotCoolDown)
	{
		cout << "버스터 발사" << endl;
		burstloop = true;
		lastShootTime = now;
	}
}

void X::spawn(int x, int y)
{
	// 캐릭터 소환

	// 캐릭터 생성
	hitBoxCenter.x = x;
	hitBoxCenter.y = y;

	pStatus.hitBox = RectMakeCenter(x, y, hixWidth, hixHeight);

	// 캐릭터 세팅
	pStatus.hp = pStatus.maxHp;
	pStatus.mp = pStatus.maxMp;

	// 상태 초기화
	currentState = CharacterState::Warp;
	attState == SholderState::None;
	pStatus.lookRight = true;
	animDir = 1;
	pStatus.isAtt = false;
	isMoving = false;

	// 상태 초기화 - 공격 관련
	attackTimer = 0.0f;
	lastShootTime = 0.0f;
	shotCoolDown = 0.02f;

	// 애니메이션 초기화
	currentAnim = "X_Spawn";
	previousAnim = currentAnim;
	pStatus.player = IMAGEMANAGER->findImage(currentAnim);
	animSpeed = 0.1f;
	attChange = false;
	burstloop = false;

	animBaseline.x = x;
	animBaseline.y = pStatus.hitBox.bottom - animOffset.y;
}

void X::currentAnimChange(void)
{
	// 애니메이션 방향 설정
	if (pStatus.lookRight == true) animDir = 1;
	else if (pStatus.lookRight == false) animDir = 0;
	
	// 애니메이션 비교용 변수 초기화
	previousAnim = currentAnim;

	cout << "이전 애니메이션 : " << previousAnim << endl;
	cout << "현재 애니메이션 : "<< currentAnim << endl;
	cout << "프레임 : " << pStatus.player->getFrameX() << endl;

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
		if (attState == SholderState::Burst)
		{
			currentAnim = "X_StandBurstLoop";
			animSpeed = 0.07f;
			animOffset.x = -1;
			animOffset.y = -1 * SCALE_FACTOR;
			
			if (pStatus.player->getFrameX() >= 1)
			{
				pStatus.player->setFrameX(1);
				pStatus.player->pause();
			}

			if (burstloop == true)
			{
				pStatus.player->setFrameX(0);
				pStatus.player->resume();
				burstloop = false;
			}

			// else pStatus.player->setFrameX(0);

			// if (pStatus.player->getFrameX() >= 1)
			// {
			//	pStatus.player->setFrameX(1);
			// }
					
			// if(!burstloop) pStatus.player->setFrameX(1);
			//else
			//{
			//	pStatus.player->setFrameX(0);
			//	burstloop = false;
			//}
			//}
		}

		else if (attState == SholderState::Hold) // && pStatus.isAtt == false)
		{
			if (previousAnim != "X_StandBurstEnd")
				pStatus.player->setFrameX(0);

			currentAnim = "X_StandBurstEnd";
			animSpeed = 0.1f;
			animOffset.x = 0;
			animOffset.y = -1 * SCALE_FACTOR;
			
			if (pStatus.player->getFrameX() >= pStatus.player->getMaxFrameX())
				attState = SholderState::None;
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
			if (attState == SholderState::Burst)
			{
				currentAnim = "X_WalkBurstStart";
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

		if ((currentAnim == "X_WalkStart" || currentAnim == "X_WalkBurstStart") && pStatus.player->getFrameX() >= pStatus.player->getMaxFrameX()) isMoving = true;

		if (isMoving)
		{
			if (attState == SholderState::Burst)
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

	if (attState == SholderState::Burst)
	{
		// cout <<"어디로 갈까? : " << static_cast<int>(currentState) << endl;

		if (TIMEMANAGER->getWorldTime() - attackTimer >= attackDuration)
		{
			if (currentState == CharacterState::Walk) attState = SholderState::None;
			else if (currentState == CharacterState::Idle) attState = SholderState::Hold;
			prevAniFrame = pStatus.player->getFrameX();
			pStatus.isAtt = false;
		}
	}

	// 애니메이션 동작을 초기 상태로 변환
	// 공격 중 일 떄는 예외 -> 세이버는 빼도 됩니다 그건 서서 쓰는거니까
	if (previousAnim != currentAnim)
	{
		if (currentAnim.find("WalkBurst") != string::npos) pStatus.player->setFrameX(prevAniFrame);
		else if (previousAnim.find("WalkBurst") != string::npos) pStatus.player->setFrameX(prevAniFrame);
		else pStatus.player->setFrameX(0);
	}

	pStatus.player = IMAGEMANAGER->findImage(currentAnim);

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

