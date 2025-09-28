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
	/////////////////////////////////
	// ù ���� ���� ��Ʈ
	/////////////////////////////////

	// ���� ���۽� ���������� ��ȯ �Ǵ� ��Ȳ
	if (cState == CharacterState::Warp)
	{
		// �ϴÿ��� �������� ���� �� - ��������Ʈ ���� �ִϸ��̼��� ���� ����
		if (pStatus.hitBox.bottom < WINSIZE_Y)
		//if (pStatus.isOnGround == false)
		{
			pStatus.hitBox.top += 12;
			pStatus.hitBox.bottom += 12;

			inputEnabled = false;
		}

		// ���� ����
		else
		{
			
			/*
			// ���� �ִϸ��̼� ����
			pStatus.player->play(0.5f);
			
			// ���� �ִϸ��̼� �� - ��¥ ���� ����
			if (pStatus.player->getFrameX() >= pStatus.player->getMaxFrameX())
			{
				cState = CharacterState::Idle;
				inputEnabled = true;
			}
			*/
			inputEnabled = true;
		}
	}

	/////////////////////////////////
	// �÷��̾ ĳ���� ��Ʈ�� ��Ʈ 
	/////////////////////////////////

	// �÷��̾��� ��Ʈ���� ��� ���ƾ� �ϴ� ��Ȳ
	else if (cState == CharacterState::OverPower || cState == CharacterState::Dead) inputEnabled = false;
	else if (cState == CharacterState::SpecialAtt) inputEnabled = false;
	else inputEnabled = true;
	
	/////////////////////////////////
	// �÷��̾��� ���� �÷��� �Ĥ�Ʈ
	/////////////////////////////////

	if (inputEnabled == true)
	{
		if (KEYMANAGER->isStayKeyDown(VK_RIGHT))
		{
			cout << "RIGHT" << endl;
			pStatus.lookRight = true;
			move(pStatus.lookRight);
		}

		else if (KEYMANAGER->isStayKeyDown(VK_LEFT))
		{
			cout << "LEFT" << endl;
			pStatus.lookRight = false;
			move(pStatus.lookRight);
		}

		if (KEYMANAGER->isStayKeyDown('X'))
		{
			if (pStatus.isOnGround) Player::jump();
			else if (!pStatus.isOnGround) jump();
		}

		if (KEYMANAGER->isStayKeyDown('C'))
		{
			pStatus.isAtt = true;
			attack();
		}
	}	
}

void X::render(void)
{
	DrawRectMake(getMemDC(), pStatus.hitBox);
}

void X::jump(void)
{
	CAMERAMANAGER->setPos(4, 0);
}

void X::attack(void)
{

}

void X::spawn(int x, int y)
{
	// ĳ���� ��ȯ
	cState = CharacterState::Warp;
	// pStatus.player = IMAGEMANAGER->findImage("Warp");
	pStatus.hitBox = RectMakeCenter(x, y, hixWidth, hixHeight);

	// ĳ���� ����
	pStatus.hp = pStatus.maxHp;
	pStatus.mp = pStatus.maxMp;

	// 	
}
