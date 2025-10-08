#include "Stdafx.h"
#include "X.h"

HRESULT X::init(void)
{
	return S_OK;
}

HRESULT X::init(int x, int y)
{
	hitBoxWidth = 25 * SCALE_FACTOR;
	hitBoxHeight = 41 * SCALE_FACTOR;

	pStatus.maxHp = 10.0;
	pStatus.maxMp = 10.0;

	pStatus.jumpStack = false;
	multiInput = false;

	// ���� �׽�Ʈ -> ���߿� �ű�ÿ�
	pStatus.jumpPower = -15.0;
	pStatus.jumpAccel = 0.3f;
	pStatus.maxJumpHoldTime = 0.1f;
	pStatus.maxFallSpeed = 12.0f;

	pStatus.velocityY = 0.0f;

	pressRight = false;
	pressLeft = false;

	spawn(x, y);

	inputEnabled = false;

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
	// ù ���� ���� ��Ʈ
	/////////////////////////////////

	
	// ���� ���۽� ���������� ��ȯ �Ǵ� ��Ȳ
	if (currentState == CharacterState::Warp)
	{
		// �ϴÿ��� �������� ���� �� - ��������Ʈ ���� �ִϸ��̼��� ���� ����
		// if (pStatus.hitBox.bottom <= WINSIZE_Y)
		if (pStatus.isOnGround == false)
		{
			pStatus.player->pause();
			
			inputEnabled = false;
		}

		// ���� ����
		else
		{
			pStatus.player->resume();
			// ���� �ִϸ��̼�
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
	// �÷��̾ ĳ���� ���� ��Ʈ�� ��Ʈ 
	/////////////////////////////////

	// �÷��̾��� ��Ʈ���� ��� ���ƾ� �ϴ� ��Ȳ
	//else if (currentState == CharacterState::OverPower || currentState == CharacterState::Dead) inputEnabled = false;
	//else if (currentState == CharacterState::SpecialAtt) inputEnabled = false;
	// else inputEnabled = true;
	

	/////////////////////////////////
	// �÷��̾��� ���� �÷��� ��Ʈ
	/////////////////////////////////

	if (inputEnabled == true)
	{
		/////////////////////////////////
		// �̵�
		/////////////////////////////////
		if (KEYMANAGER->isOnceKeyDown(VK_LEFT)) lastKeyIsRight = false;
		if (KEYMANAGER->isOnceKeyDown(VK_RIGHT)) lastKeyIsRight = true;
		
		if (KEYMANAGER->isStayKeyDown(VK_RIGHT) && KEYMANAGER->isStayKeyDown(VK_LEFT))
		{
			multiInput = true;

			if (lastKeyIsRight == true)
			{
				// ���� ����
				pStatus.lookRight = false;
				if (pStatus.touchLeft && pStatus.isOnGround) currentState = CharacterState::Idle;
				else if (pStatus.touchLeft && !pStatus.isOnGround && !pStatus.lookRight) wallSlide();// ��Ÿ��
				else move(pStatus.lookRight);
			}

			else if (lastKeyIsRight == false)
			{
				// ������ ����
				pStatus.lookRight = true;
				if (pStatus.touchRight && pStatus.isOnGround) currentState = CharacterState::Idle;
				else if (pStatus.touchRight && !pStatus.isOnGround && pStatus.lookRight) wallSlide();
				else move(pStatus.lookRight);
			}
		}

		else if (KEYMANAGER->isStayKeyDown(VK_RIGHT))
		{
			multiInput = false;
			pStatus.lookRight = true;
			pressRight = true;
			if (pStatus.touchRight && pStatus.isOnGround) currentState = CharacterState::Idle;
			else if (pStatus.touchRight && !pStatus.isOnGround && pStatus.lookRight) wallSlide();
			else move(pStatus.lookRight);
		}

		else if (KEYMANAGER->isStayKeyDown(VK_LEFT) && !KEYMANAGER->isStayKeyDown(VK_RIGHT))
		{
			multiInput = false;
			pStatus.lookRight = false;
			pressLeft = true;
			if (pStatus.touchLeft && pStatus.isOnGround) currentState = CharacterState::Idle;
			else if (pStatus.touchLeft && !pStatus.isOnGround && !pStatus.lookRight) wallSlide();// ��Ÿ��
			else move(pStatus.lookRight);

			if (KEYMANAGER->isStayKeyDown(VK_RIGHT))
			{
				cout << "����" << endl;
			}
		}

		// ��� ���� ��ȯ + ��� ���� ��ȯ �ִϸ��̼�
		else if (!(KEYMANAGER->isStayKeyDown(VK_RIGHT) || KEYMANAGER->isStayKeyDown(VK_LEFT)) && pStatus.isOnGround)
		{
			returnToIdle();
			attChange = false;
			multiInput = false;
		}

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
		// ���� ��Ʈ
		/////////////////////////////////

		// ���� ������ ���� �Ŀ� ��ŭ ��� �ö󰡰�
		// ������ ���� �ٷ� ���ϸ� �ϰ� ���� ������, ������ ����
		
		if (KEYMANAGER->isOnceKeyDown('X') && !multiInput) // '��' �˻��� ���� �ּ�������
		{
			jump();
		}

		if (KEYMANAGER->isOnceKeyUp('X'))
		{
			if (pStatus.velocityY < 0.0f) pStatus.velocityY = 0.0f;
		}

		/////////////////////////////////
		// ���� ��Ʈ
		/////////////////////////////////

		if (now - lastShootTime >= shotCoolDown) normalBurstAble = true;
		else normalBurstAble = false;

		now = TIMEMANAGER->getWorldTime();

		if (TIMEMANAGER->getWorldTime() - chargeBurstCount >= 0.5f) chargeBurstDelay = false;

		
		if (KEYMANAGER->isOnceKeyDown('C')) // '��' �˻��� ���� �ּ�������
		{
			prevAniFrame = pStatus.player->getFrameX();
			attackTimer = TIMEMANAGER->getWorldTime();
			attCheckOnce = true;
			
			if (normalBurstAble == true && chargeBurstDelay == false && bManager->getMaxBullets() < 3) attack();
		}

		if (KEYMANAGER->isStayKeyDown('C') && attCheckOnce == true)
		{
			// �����ϱ����� �̸� ������ ������ ���� �ȵǰ�
			if (attCheckOnce)
			{
				// ����
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
			// ���� �ð��� ���� ������ �߻�
			chargeBurst();
			chargeCount = 0.0f;
			attCheckOnce = false;
			attCheckOnce = true;
		}
	}

#pragma endregion


#pragma region Animation Change + SFX Sound Play

	applyGravity();

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
		// ���°� ���
		
		// ĳ���� ��ǥ
		TEXTMANAGER->drawTextColor(getMemDC(), WINSIZE_X / 50, WINSIZE_Y / 100, "���� ����", "DNF_M_18", RGB(0, 255, 255));
		TEXTMANAGER->drawTextColor(getMemDC(), WINSIZE_X / 50, WINSIZE_Y / 100 + 20, printBodyState(), "DNF_M_18", RGB(0, 255, 255));

		string temp;
		if (pStatus.jumpStack == true) temp = "O";
		else temp = "X";

		TEXTMANAGER->drawTextColor(getMemDC(), WINSIZE_X / 50, WINSIZE_Y / 100 + 45, "���� ����", "DNF_M_18", RGB(0, 255, 255));
		TEXTMANAGER->drawTextColor(getMemDC(), WINSIZE_X / 50, WINSIZE_Y / 100 + 65, temp, "DNF_M_18", RGB(0, 255, 255));

		TEXTMANAGER->drawTextColor(getMemDC(), WINSIZE_X / 50 + 80, WINSIZE_Y / 100, "���� ����", "DNF_M_18", RGB(0, 255, 255));
		TEXTMANAGER->drawTextColor(getMemDC(), WINSIZE_X / 50 + 80, WINSIZE_Y / 100 + 20, printAttState(), "DNF_M_18", RGB(0, 255, 255));

		TEXTMANAGER->drawTextColor(getMemDC(), WINSIZE_X / 50 + 80, WINSIZE_Y / 100 + 45, "���� �Ŀ�", "DNF_M_18", RGB(0, 255, 255));
		TEXTMANAGER->drawTextColor(getMemDC(), WINSIZE_X / 50 + 80, WINSIZE_Y / 100 + 65, to_string(pStatus.velocityY), "DNF_M_18", RGB(0, 255, 255));

		TEXTMANAGER->drawTextColor(getMemDC(), WINSIZE_X / 50 + 160, WINSIZE_Y / 100, "ĳ���� X", "DNF_M_18", RGB(0, 255, 255));
		TEXTMANAGER->drawTextColor(getMemDC(), WINSIZE_X / 50 + 160, WINSIZE_Y / 100 + 20, to_string(charPos.x), "DNF_M_18", RGB(0, 255, 255));

		TEXTMANAGER->drawTextColor(getMemDC(), WINSIZE_X / 50 + 240, WINSIZE_Y / 100, "ĳ���� Y", "DNF_M_18", RGB(0, 255, 255));
		TEXTMANAGER->drawTextColor(getMemDC(), WINSIZE_X / 50 + 240, WINSIZE_Y / 100 + 20, to_string(charPos.y), "DNF_M_18", RGB(0, 255, 255));
		
		TEXTMANAGER->drawTextColor(getMemDC(), WINSIZE_X / 50 + 160, WINSIZE_Y / 100 + 45, "ī�޶� X", "DNF_M_18", RGB(0, 255, 255));
		TEXTMANAGER->drawTextColor(getMemDC(), WINSIZE_X / 50 + 160, WINSIZE_Y / 100 + 65, to_string(CAMERAMANAGER->getCameraPos().x), "DNF_M_18", RGB(0, 255, 255));

		TEXTMANAGER->drawTextColor(getMemDC(), WINSIZE_X / 50 + 240, WINSIZE_Y / 100 + 45, "ī�޶� Y", "DNF_M_18", RGB(0, 255, 255));
		TEXTMANAGER->drawTextColor(getMemDC(), WINSIZE_X / 50 + 240, WINSIZE_Y / 100 + 65, to_string(CAMERAMANAGER->getCameraPos().y), "DNF_M_18", RGB(0, 255, 255));

		if (CAMERAMANAGER->getLockX()) temp = "O";
		else temp = "X";

		TEXTMANAGER->drawTextColor(getMemDC(), WINSIZE_X / 50 + 320, WINSIZE_Y / 100, "ī�޶� Lock X", "DNF_M_18", RGB(0, 255, 255));
		TEXTMANAGER->drawTextColor(getMemDC(), WINSIZE_X / 50 + 320, WINSIZE_Y / 100 + 20, temp, "DNF_M_18", RGB(0, 255, 255));

		if (CAMERAMANAGER->getLockY()) temp = "O";
		else temp = "X";

		TEXTMANAGER->drawTextColor(getMemDC(), WINSIZE_X / 50 + 320, WINSIZE_Y / 100 + 45, "ī�޶� Lock Y", "DNF_M_18", RGB(0, 255, 255));
		TEXTMANAGER->drawTextColor(getMemDC(), WINSIZE_X / 50 + 320, WINSIZE_Y / 100 + 65, temp, "DNF_M_18", RGB(0, 255, 255));
		
		if (pStatus.lookRight) temp = "����";
		else temp = "����";
		
		TEXTMANAGER->drawTextColor(getMemDC(), WINSIZE_X / 50 + 420, WINSIZE_Y / 100, "�þ� ����", "DNF_M_18", RGB(0, 255, 255));
		TEXTMANAGER->drawTextColor(getMemDC(), WINSIZE_X / 50 + 420, WINSIZE_Y / 100 + 20, temp, "DNF_M_18", RGB(0, 255, 255));
		
		// ��Ʈ�ڽ� ���
		DrawRectMakeColor(getMemDC(), pStatus.hitBox, RGB(255, 0, 0), 2);
	}
}

void X::jump(void)
{
	if (pStatus.jumpStack == false && pStatus.isOnGround) Player::jump();
	else cout << "ȣ����" << endl;
}

void X::attack(void)
{
	attState = SholderState::Burst;
	SOUNDMANAGER->play("SFX_X_Burster1", 0.5f);

	burstloop = true;
	lastShootTime = now;
	isCharging = false;

	if (pStatus.lookRight) bManager->fire(0, pStatus.hitBox.right, pStatus.hitBox.top + pStatus.firePoint, pStatus.lookRight);
	else bManager->fire(0, pStatus.hitBox.left, pStatus.hitBox.top + pStatus.firePoint, pStatus.lookRight);
}

void X::chargeBurst(void)
{
	if (chargeCount >= 0.4f && chargeCount < 1.5f)
	{
		attState = SholderState::Burst;
		SOUNDMANAGER->play("SFX_X_Burster2", 0.5f);
		
		// �̵� ������ ���� ������ ����
		prevAniFrame = pStatus.player->getFrameX();
		// ���� ȸ�� ����� ���� Ÿ�̸� ���� -> ���ϸ� �ٷ� ��������
		attackTimer = TIMEMANAGER->getWorldTime();

		attackTimer = TIMEMANAGER->getWorldTime();
		burstloop = true;
		chargeBurstCount = TIMEMANAGER->getWorldTime();
		chargeBurstDelay = true;
		isCharging = false;

		if (pStatus.lookRight) bManager->fire(1, pStatus.hitBox.right, pStatus.hitBox.top + pStatus.firePoint, pStatus.lookRight);
		else bManager->fire(1, pStatus.hitBox.left, pStatus.hitBox.top + pStatus.firePoint, pStatus.lookRight);
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

		if (pStatus.lookRight) bManager->fire(2, pStatus.hitBox.right, pStatus.hitBox.top + pStatus.firePoint, pStatus.lookRight);
		else bManager->fire(2, pStatus.hitBox.left, pStatus.hitBox.top + pStatus.firePoint, pStatus.lookRight);
	}

	SOUNDMANAGER->stop("SFX_X_BurstCharge");
}


void X::currentAnimChange(void)
{
	// ���°��� �ٸ� �ִϸ��̼� ����
	if (currentState == CharacterState::Warp)
	{
		currentAnim = "X_Spawn";
		animSpeed = 0.07f;
		animOffset.x = 0;
		animOffset.y = - 17 * SCALE_FACTOR;
		pStatus.firePoint = 10 * SCALE_FACTOR;
	}

	else if (currentState == CharacterState::Idle)
	{
		pStatus.firePoint = 11 * SCALE_FACTOR;

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
		pStatus.firePoint = 8 * SCALE_FACTOR;

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
	else if (currentState == CharacterState::JumpUp)
	{
		currentAnim = "X_JumpUp";
		animSpeed = 0.08f;
		animOffset.y = -6 * SCALE_FACTOR;

		if (pStatus.player->getFrameX() >= pStatus.player->getMaxFrameX()) pStatus.player->setFrameX(pStatus.player->getMaxFrameX());
	}


	else if (currentState == CharacterState::FallingDown)
	{
		currentAnim = "X_FallingDown";
		animSpeed = 0.08f;
		animOffset.y = -6 * SCALE_FACTOR;

		if (pStatus.player->getFrameX() >= pStatus.player->getMaxFrameX()) pStatus.player->setFrameX(pStatus.player->getMaxFrameX());
	}

	else if (currentState == CharacterState::WallSlide)
	{
		currentAnim = "X_WallSlide";
		animSpeed = 0.08f;
		animOffset.y = -9 * SCALE_FACTOR;
		animOffset.y = -10 * SCALE_FACTOR;

		if (pStatus.player->getFrameX() >= pStatus.player->getMaxFrameX()) pStatus.player->setFrameX(pStatus.player->getMaxFrameX());
	}


	if (attState == SholderState::Burst || attState == SholderState::LargeBurst)
	{
		if (TIMEMANAGER->getWorldTime() - attackTimer >= attackDuration)
		{
			if (currentState == CharacterState::Walk) attState = SholderState::None;
			else if (currentState == CharacterState::Idle) attState = SholderState::Hold;
			prevAniFrame = pStatus.player->getFrameX();
		}
	}	 

	hitBoxCenter.x = (pStatus.hitBox.left + pStatus.hitBox.right) / 2;
	hitBoxCenter.y = pStatus.hitBox.bottom;	

	// pStatus.hitBox.right = pStatus.hitBox.left + hitBoxWidth;
	// pStatus.hitBox.top = pStatus.hitBox.bottom - hitBoxHeight;
	
	pStatus.player = IMAGEMANAGER->findImage(currentAnim);

	// �ִϸ��̼� ������ �ʱ� ���·� ��ȯ
		// ���� �� �� ���� ���� -> ���̹��� ���� �˴ϴ� �װ� ���� ���°Ŵϱ�
	if (previousAnim != currentAnim)
	{
		if (currentAnim.find("WalkBurst") != string::npos) pStatus.player->setFrameX(prevAniFrame);
		else if (previousAnim.find("WalkBurst") != string::npos) pStatus.player->setFrameX(prevAniFrame);
		else pStatus.player->setFrameX(0);
	}

	// �ִϸ��̼� �񱳿� ���� �ʱ�ȭ
	previousAnim = currentAnim;
	previousState = currentState;
	setHitBox();
}

void X::frameCheck(void)
{
	// Burst �������� Hold��
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

		// Hold �������� None����
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
		pStatus.hitBox.left = charPos.x - hitBoxWidth / 2;
		pStatus.hitBox.right = charPos.x + hitBoxWidth / 2;
	}
	
	// pStatus.hitBox.bottom = WINSIZE_Y - (CAMERAMANAGER->getMapSize().y - charPos.y);
	// pStatus.hitBox.top = pStatus.hitBox.bottom - hitBoxHeight;

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
	}
}

void X::spawn(int x, int y)
{
	// ĳ���� ��ȯ
	// ĳ���� ����
	charPos.x = x;
	charPos.y = y;

	pStatus.hitBox = RectMakeCenter(charPos.x, 0 - hitBoxHeight / 2, hitBoxWidth, hitBoxHeight);

	hitBoxCenter.x = (pStatus.hitBox.left + pStatus.hitBox.right) / 2;
	hitBoxCenter.y = pStatus.hitBox.bottom - hitBoxHeight;

	// ĳ���� ����
	pStatus.hp = pStatus.maxHp;
	pStatus.mp = pStatus.maxMp;
	pStatus.speed = 4.0f;
	
	pStatus.charName = "X_";
	pStatus.touchLeft = false;
	pStatus.touchRight = false;

	// ���� �ʱ�ȭ
	previousState = CharacterState::Idle;
	currentState = CharacterState::Warp;
	attState == SholderState::None;
	pStatus.lookRight = true;
	isMoving = false;
	normalBurstAble = true;
	chargeBurstDelay = false;
	attCheckOnce = false;
	
	// ���� �ʱ�ȭ - ���� ����
	attackTimer = 0.0f;
	lastShootTime = 0.0f;
	shotCoolDown = 0.06f;
	burstSound = "SFX_X_Burster1";
	chargeCount = 0.0f;
	chargeSpeed = 0.01f;
	isCharging = false;
	pStatus.firePoint = 12 * SCALE_FACTOR;
	
	// �ִϸ��̼� �ʱ�ȭ
	currentAnim = "X_Spawn";
	previousAnim = currentAnim;
	pStatus.player = IMAGEMANAGER->findImage(currentAnim);
	animSpeed = 0.1f;
	attChange = false;
	burstloop = false;
	chargeEffect = IMAGEMANAGER->findImage("SFX_X_Charge");

	CAMERAMANAGER->setPlayerPos(charPos.x, charPos.y - hitBoxHeight / 2);
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
		/*
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
		*/
	default:
		currentState = CharacterState::Idle;
		break;
	}

	isMoving = false;
}

