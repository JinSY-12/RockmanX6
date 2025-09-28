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
	// 첫 등장 연출 파트
	/////////////////////////////////

	// 게임 시작시 스테이지에 소환 되는 상황
	if (cState == CharacterState::Warp)
	{
		// 하늘에서 내려오고 있을 때 - 스프라이트 보면 애니메이션이 동작 안함
		if (pStatus.hitBox.bottom < WINSIZE_Y)
		//if (pStatus.isOnGround == false)
		{
			pStatus.hitBox.top += 12;
			pStatus.hitBox.bottom += 12;

			inputEnabled = false;
		}

		// 땅에 도착
		else
		{
			
			/*
			// 등장 애니메이션 시작
			pStatus.player->play(0.5f);
			
			// 등장 애니메이션 끝 - 진짜 게임 시작
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
	// 플레이어가 캐릭터 컨트롤 파트 
	/////////////////////////////////

	// 플레이어의 컨트롤을 잠시 막아야 하는 상황
	else if (cState == CharacterState::OverPower || cState == CharacterState::Dead) inputEnabled = false;
	else if (cState == CharacterState::SpecialAtt) inputEnabled = false;
	else inputEnabled = true;
	
	/////////////////////////////////
	// 플레이어의 게임 플레이 파ㅏ트
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
	// 캐릭터 소환
	cState = CharacterState::Warp;
	// pStatus.player = IMAGEMANAGER->findImage("Warp");
	pStatus.hitBox = RectMakeCenter(x, y, hixWidth, hixHeight);

	// 캐릭터 세팅
	pStatus.hp = pStatus.maxHp;
	pStatus.mp = pStatus.maxMp;

	// 	
}
