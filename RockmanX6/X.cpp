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
	// ù ���� ���� ��Ʈ
	/////////////////////////////////

	// ���� ���۽� ���������� ��ȯ �Ǵ� ��Ȳ
	if (currentState == CharacterState::Warp)
	{
		// �ϴÿ��� �������� ���� �� - ��������Ʈ ���� �ִϸ��̼��� ���� ����
		if (pStatus.hitBox.bottom < WINSIZE_Y)
		//if (pStatus.isOnGround == false)
		{
			pStatus.player->pause();

			pStatus.hitBox.top += 12;
			pStatus.hitBox.bottom += 12;
			animBaseline.y += 12;

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

	/////////////////////////////////
	// �÷��̾ ĳ���� ���� ��Ʈ�� ��Ʈ 
	/////////////////////////////////

	// �÷��̾��� ��Ʈ���� ��� ���ƾ� �ϴ� ��Ȳ
	else if (currentState == CharacterState::OverPower || currentState == CharacterState::Dead) inputEnabled = false;
	else if (currentState == CharacterState::SpecialAtt) inputEnabled = false;
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

		if (KEYMANAGER->isStayKeyDown('X'))
		{
			if (pStatus.isOnGround) Player::jump();
			else if (!pStatus.isOnGround) jump();
		}


		/////////////////////////////////
		// ���� ��Ʈ
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
			// ����
		}

		if (KEYMANAGER->isOnceKeyUp('C'))
		{
			// ���� �ð��� ���� ������ �߻�
			attCheckOnce = false;
		}
	}

	if (UIMANAGER->getIsDebugMode())
	{
		// ���߿� ���� ���� �̻ڰ� �ؽ�Ʈ�ڽ��� �׸� �� + �׸𳪰� �̻ڰ� ��Ʈ �ڽ��� ����
		cout << "��Ʈ�ڽ� X : " << pStatus.hitBox.left << endl;
		cout << "��Ʈ�ڽ� Y : " << pStatus.hitBox.bottom << endl;

		cout << "�ִϸ��̼� X : " << animBaseline.x << endl;
		cout << "�ִϸ��̼� Y : " << animBaseline.y << endl;
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
		cout << "������ �߻�" << endl;
		burstloop = true;
		lastShootTime = now;
	}
}

void X::spawn(int x, int y)
{
	// ĳ���� ��ȯ

	// ĳ���� ����
	hitBoxCenter.x = x;
	hitBoxCenter.y = y;

	pStatus.hitBox = RectMakeCenter(x, y, hixWidth, hixHeight);

	// ĳ���� ����
	pStatus.hp = pStatus.maxHp;
	pStatus.mp = pStatus.maxMp;

	// ���� �ʱ�ȭ
	currentState = CharacterState::Warp;
	attState == SholderState::None;
	pStatus.lookRight = true;
	animDir = 1;
	pStatus.isAtt = false;
	isMoving = false;

	// ���� �ʱ�ȭ - ���� ����
	attackTimer = 0.0f;
	lastShootTime = 0.0f;
	shotCoolDown = 0.02f;

	// �ִϸ��̼� �ʱ�ȭ
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
	// �ִϸ��̼� ���� ����
	if (pStatus.lookRight == true) animDir = 1;
	else if (pStatus.lookRight == false) animDir = 0;
	
	// �ִϸ��̼� �񱳿� ���� �ʱ�ȭ
	previousAnim = currentAnim;

	cout << "���� �ִϸ��̼� : " << previousAnim << endl;
	cout << "���� �ִϸ��̼� : "<< currentAnim << endl;
	cout << "������ : " << pStatus.player->getFrameX() << endl;

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
		// cout <<"���� ����? : " << static_cast<int>(currentState) << endl;

		if (TIMEMANAGER->getWorldTime() - attackTimer >= attackDuration)
		{
			if (currentState == CharacterState::Walk) attState = SholderState::None;
			else if (currentState == CharacterState::Idle) attState = SholderState::Hold;
			prevAniFrame = pStatus.player->getFrameX();
			pStatus.isAtt = false;
		}
	}

	// �ִϸ��̼� ������ �ʱ� ���·� ��ȯ
	// ���� �� �� ���� ���� -> ���̹��� ���� �˴ϴ� �װ� ���� ���°Ŵϱ�
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

