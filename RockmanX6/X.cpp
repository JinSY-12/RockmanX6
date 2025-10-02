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
	// ù ���� ���� ��Ʈ
	/////////////////////////////////

	
	// ���� ���۽� ���������� ��ȯ �Ǵ� ��Ȳ
	if (currentState == CharacterState::Warp)
	{
		// �ϴÿ��� �������� ���� �� - ��������Ʈ ���� �ִϸ��̼��� ���� ����
		if (pStatus.hitBox.bottom <= WINSIZE_Y)
		//if (pStatus.isOnGround == false)
		{
			pStatus.player->pause();
			
			pStatus.isOnGround = false;
			inputEnabled = false;
		}

		// ���� ����
		else
		{
			pStatus.isOnGround = true;

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

		// ��� ���� ��ȯ + ��� ���� ��ȯ �ִϸ��̼�
		else if (!(KEYMANAGER->isStayKeyDown(VK_RIGHT) || KEYMANAGER->isStayKeyDown(VK_LEFT)))
		{
			returnToIdle();
			pStatus.isAtt = false;
			attChange = false;
		}

		/////////////////////////////////
		// ���� ��Ʈ
		/////////////////////////////////

		if (KEYMANAGER->isStayKeyDown('X')) // '��' �˻��� ���� �ּ�������
		{
			cout << CAMERAMANAGER->getCameraPos().y << endl;

			if (pStatus.isOnGround) Player::jump();
			else if (!pStatus.isOnGround) jump();
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
			pStatus.isAtt = true;
			
			if (normalBurstAble == true && chargeBurstDelay == false) attack();
		}

		if (KEYMANAGER->isStayKeyDown('C'))
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
		// ���°� ���
		TEXTMANAGER->drawTextColor(getMemDC(), WINSIZE_X / 50, WINSIZE_Y / 100, "���� ����", "DNF_M_18", RGB(0,255,255));
		TEXTMANAGER->drawTextColor(getMemDC(), WINSIZE_X / 50 + 80, WINSIZE_Y / 100, "���� ����", "DNF_M_18", RGB(0, 255, 255));
		TEXTMANAGER->drawTextColor(getMemDC(), WINSIZE_X / 50, WINSIZE_Y / 100 + 20, printBodyState(), "DNF_M_18", RGB(0, 255, 255));
		TEXTMANAGER->drawTextColor(getMemDC(), WINSIZE_X / 50 + 80, WINSIZE_Y / 100 + 20, printAttState(), "DNF_M_18", RGB(0, 255, 255));

		// ĳ���� ��ǥ
		TEXTMANAGER->drawTextColor(getMemDC(), WINSIZE_X / 50 + 160, WINSIZE_Y / 100, "ĳ���� X", "DNF_M_18", RGB(0, 255, 255));
		TEXTMANAGER->drawTextColor(getMemDC(), WINSIZE_X / 50 + 160, WINSIZE_Y / 100 + 20, to_string(charPos.x), "DNF_M_18", RGB(255, 0, 0));

		TEXTMANAGER->drawTextColor(getMemDC(), WINSIZE_X / 50 + 240, WINSIZE_Y / 100, "ĳ���� Y", "DNF_M_18", RGB(0, 255, 255));
		TEXTMANAGER->drawTextColor(getMemDC(), WINSIZE_X / 50 + 240, WINSIZE_Y / 100 + 20, to_string(charPos.y), "DNF_M_18", RGB(0, 255, 255));

		TEXTMANAGER->drawTextColor(getMemDC(), WINSIZE_X / 50 + 160, WINSIZE_Y / 100 + 40, "ī�޶� X", "DNF_M_18", RGB(0, 255, 255));
		TEXTMANAGER->drawTextColor(getMemDC(), WINSIZE_X / 50 + 160, WINSIZE_Y / 100 + 60, to_string(CAMERAMANAGER->getCameraPos().x), "DNF_M_18", RGB(255, 0, 0));

		TEXTMANAGER->drawTextColor(getMemDC(), WINSIZE_X / 50 + 240, WINSIZE_Y / 100 + 40, "ī�޶� Y", "DNF_M_18", RGB(0, 255, 255));
		TEXTMANAGER->drawTextColor(getMemDC(), WINSIZE_X / 50 + 240, WINSIZE_Y / 100 + 60, to_string(CAMERAMANAGER->getCameraPos().y), "DNF_M_18", RGB(0, 255, 255));
		
		string Check;
		if (CAMERAMANAGER->getLockX()) Check = "O";
		else Check = "X";

		TEXTMANAGER->drawTextColor(getMemDC(), WINSIZE_X / 50 + 300, WINSIZE_Y / 100 + 40, "ī�޶� LOCK X", "DNF_M_18", RGB(0, 255, 255));
		TEXTMANAGER->drawTextColor(getMemDC(), WINSIZE_X / 50 + 300, WINSIZE_Y / 100 + 60, Check, "DNF_M_18", RGB(0, 255, 255));
		
		
		TEXTMANAGER->drawTextColor(getMemDC(), WINSIZE_X / 50 + 300, WINSIZE_Y / 100, "��¡ ������", "DNF_M_18", RGB(0, 255, 255));
		TEXTMANAGER->drawTextColor(getMemDC(), WINSIZE_X / 50 + 300, WINSIZE_Y / 100 + 20, to_string(chargeCount), "DNF_M_18", RGB(0, 255, 255));
		
		string shoot;
		if (normalBurstAble) shoot = "����";
		else shoot = "�Ұ���";

		TEXTMANAGER->drawTextColor(getMemDC(), WINSIZE_X / 50 + 420, WINSIZE_Y / 100, "���� ����", "DNF_M_18", RGB(0, 255, 255));
		TEXTMANAGER->drawTextColor(getMemDC(), WINSIZE_X / 50 + 420, WINSIZE_Y / 100 + 20, shoot, "DNF_M_18", RGB(0, 255, 255));
		
		if (chargeBurstDelay) shoot = "����";
		else shoot = "����";

		TEXTMANAGER->drawTextColor(getMemDC(), WINSIZE_X / 50 + 500, WINSIZE_Y / 100, "������ �ĵ�����", "DNF_M_18", RGB(0, 255, 255));
		TEXTMANAGER->drawTextColor(getMemDC(), WINSIZE_X / 50 + 500, WINSIZE_Y / 100 + 20, shoot, "DNF_M_18", RGB(0, 255, 255));
		
		// ��Ʈ�ڽ� ���
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
		
		// �̵� ������ ���� ������ ����
		prevAniFrame = pStatus.player->getFrameX();
		// ���� ȸ�� ����� ���� Ÿ�̸� ���� -> ���ϸ� �ٷ� ��������
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
	// �ִϸ��̼� ���� ����
	if (pStatus.lookRight == true) animDir = 1;
	else if (pStatus.lookRight == false) animDir = 0;
	
	// ���°��� �ٸ� �ִϸ��̼� ����
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
	
	// �ִϸ��̼� �񱳿� ���� �ʱ�ȭ
	previousAnim = currentAnim;
	previousState = currentState;

	// �ִϸ��̼� ������ �ʱ� ���·� ��ȯ
	// ���� �� �� ���� ���� -> ���̹��� ���� �˴ϴ� �װ� ���� ���°Ŵϱ�
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

void X::spawn(int x, int y)
{
	// ĳ���� ��ȯ
	// ĳ���� ����
	charPos.x = x;
	charPos.y = y;

	pStatus.hitBox = RectMakeCenter(WINSIZE_X / 2, 0, hitBoxWidth, hitBoxHeight);

	hitBoxCenter.x = (pStatus.hitBox.left + pStatus.hitBox.right) / 2;
	hitBoxCenter.y = pStatus.hitBox.top;

	// ĳ���� ����
	pStatus.hp = pStatus.maxHp;
	pStatus.mp = pStatus.maxMp;
	pStatus.speed = 4.0f;
	pStatus.jumpSpeed = 4.0f;
	pStatus.charName = "X_";

	// ���� �ʱ�ȭ
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
	
	// ���� �ʱ�ȭ - ���� ����
	attackTimer = 0.0f;
	lastShootTime = 0.0f;
	shotCoolDown = 0.05f;
	burstSound = "SFX_X_Burster1";
	chargeCount = 0.0f;
	chargeSpeed = 0.01f;
	isCharging = false;
	
	// �ִϸ��̼� �ʱ�ȭ
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

