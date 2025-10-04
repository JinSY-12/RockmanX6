#include "Stdafx.h"
#include "Player.h"


HRESULT Player::init(void)
{
	// Do Nothing!!
	return S_OK;
}

HRESULT Player::init(int x, int y)
{
	return S_OK;
}

void Player::release(void)
{
	// Do Nothing!!
}

void Player::update(void)
{
	// Do Nothing!!
}

void Player::render(void)
{
	// Do Nothing!!
}

void Player::move(bool direction)
{
	currentState = CharacterState::Walk;

	float moveSpeed;

	if (direction) moveSpeed = pStatus.speed;
	else moveSpeed = -pStatus.speed;


	if (CAMERAMANAGER->getLockX() == true)
	{
		if (pStatus.hitBox.left + moveSpeed >= 0)
		{
			charPos.x += moveSpeed;
			pStatus.hitBox.left += moveSpeed;
			pStatus.hitBox.right += moveSpeed;
		}

		else
		{
			charPos.x = pStatus.hitBox.left + hitBoxWidth / 2;
			currentState = CharacterState::Idle;
		}
	}

	else
	{
		charPos.x += moveSpeed;
	}


	/*
	// �¿�� Ȯ��
	float moveSpeed;

	if (pStatus.touchLeft) moveSpeed = direction ? pStatus.speed : 0;
	else if (pStatus.touchRight) moveSpeed = direction ? 0 : -pStatus.speed;
	else if (pStatus.touchRight && pStatus.touchRight) moveSpeed = direction ? 0 : 0;
	else moveSpeed = direction ? pStatus.speed : -pStatus.speed;

	if (CAMERAMANAGER->getLockX() == true)
	{	
		if (pStatus.hitBox.left + moveSpeed >= 0)
		{
			charPos.x += moveSpeed;
			pStatus.hitBox.left += moveSpeed;
			pStatus.hitBox.right += moveSpeed;
		}

		else
		{
			charPos.x = pStatus.hitBox.left + hitBoxWidth / 2;
			currentState = CharacterState::Idle;
		}
	}

	else
	{
		charPos.x += moveSpeed;
	}
	*/
}

void Player::jump(void)
{
	// CAMERAMANAGER->setCameraPos(0, 4);

	charPos.y -= 30;
	pStatus.hitBox.top -= 30;
	pStatus.hitBox.bottom -= 30;

}

void Player::dash(void)
{
	// Later
}

void Player::sfxPlay(void)
{
	// �߰� �ؾ� �� ��

	// �Ƹ� ȹ�� ����
	// ����, ���� �ǰ���
	// ���� ����
	// 

	if (previousState != currentState)
	{
		// ���� ����
		
		// ��ȯ ����
		if (currentState == CharacterState::Warp)
		{
			soundResult = "SFX_" + pStatus.charName + "WarpIn";
			SOUNDMANAGER->play(soundResult, 0.5f);
		}

		// ��� �̱���

		/*
		// ��� ����
		else if (currentState == CharacterState::Dash)
		{
			soundResult = "SFX_" + pStatus.charName + "DashStart";
			SOUNDMANAGER->play(soundResult, 0.5f);
		}

		// ��� ��
		else if (currentState == CharacterState::Idle && previousState == CharacterState::Dash)
		{
			soundResult = "SFX_" + pStatus.charName + "DashEnd";
			SOUNDMANAGER->play(soundResult, 0.5f);
		}

		// ����
		else if (currentState == CharacterState::Land)
		{
			soundResult = "SFX_" + pStatus.charName + "DashEnd";
			SOUNDMANAGER->play(soundResult, 0.5f);			
		}

		// ������
		else if (previousState == CharacterState::Wall && currentState == CharacterState::JumpUp)
		{
			soundResult = "SFX_" + pStatus.charName + "WallKick";
			SOUNDMANAGER->play(soundResult, 0.5f);
			
			soundResult = "Voice_" + pStatus.charName + "WallKick";
			SOUNDMANAGER->play(soundResult, 0.5f);
		}
		*/


		// X ���� ����

		if (pStatus.charName == "X_")
		{

		}

		// Zero ���� ����

		else if (pStatus.charName == "Zero_")
		{

		}
	}
}


void Player::attack(void)
{
	// Do Nothing!!	
}

void Player::applyGravity(void)
{
	if (!pStatus.isOnGround && currentState != CharacterState::Warp)
	{
		charPos.y += progress.gravity;
		pStatus.hitBox.top += progress.gravity;
		pStatus.hitBox.bottom += progress.gravity;
	}

	else if (!pStatus.isOnGround && currentState == CharacterState::Warp)
	{
		charPos.y += progress.gravity + 2;
		pStatus.hitBox.top += progress.gravity + 2;
		pStatus.hitBox.bottom += progress.gravity + 2;
	}
}

void Player::spawn(int x, int y)
{
	// Do Nothing!!
}

void Player::currentAnimChange(void)
{
	// Do Nothing!!
}

string Player::printBodyState(void)
{
	string result;

	if (currentState == CharacterState::Idle) result = "���";
	else if (currentState == CharacterState::Walk) result = "�ȱ�";
	else if (currentState == CharacterState::Dash) result = "���";
	else if (currentState == CharacterState::JumpUp) result = "����";
	else if (currentState == CharacterState::FallingDown) result = "����";
	else if (currentState == CharacterState::Crouch) result = "�ɱ�";
	else if (currentState == CharacterState::Climb) result = "��ٸ� Ÿ��";
	else if (currentState == CharacterState::Warp) result = "����";
	else if (currentState == CharacterState::SpecialAtt) result = "�Ⱑ ����";
	else if (currentState == CharacterState::Dead) result = "���";
	else if (currentState == CharacterState::Wall) result = "��Ÿ��";

	return result;
}

string Player::printAttState(void)
{
	string result;

	if (attState == SholderState::None) result = "���";
	else if (attState == SholderState::Burst) result = "������";
	else if (attState == SholderState::LargeBurst) result = "������";
	else if (attState == SholderState::Hold) result = "������";
	
	return result;
}

void Player::setHitBox(void)
{
	// Do Nothing!
}
