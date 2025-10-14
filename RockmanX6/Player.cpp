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
	if (pStatus.isJumpDash)
	{
		if(pStatus.lookRight) pStatus.velocityX = dashSpeed;
		else pStatus.velocityX = -dashSpeed;
	}

	else if (pStatus.isDash)
	{
		if (pStatus.lookRight && !pStatus.touchRight) pStatus.velocityX = lerp(pStatus.velocityX, dashSpeed, 1.0f);
		else if (!pStatus.lookRight && !pStatus.touchLeft) pStatus.velocityX = -lerp(pStatus.velocityX, dashSpeed, 1.0f);
		else currentState = CharacterState::Idle;
	}

	else if (!pStatus.isDash)
	{
		if (pStatus.isOnGround) currentState = CharacterState::Walk;

		float moveSpeed;
		if (direction) moveSpeed = pStatus.moveSpeed;
		else moveSpeed = -pStatus.moveSpeed;

		pStatus.velocityX = moveSpeed;
	}
}

void Player::jump(void)
{
	// �Ϲ� ����
	if (pStatus.isOnGround)  // ���� ���� ���� ����
	{
		isJumpUp = true;
		currentState = CharacterState::JumpUp;

		pStatus.velocityY = pStatus.jumpPower;
		
		pStatus.hitBox.bottom -= 8;
		pStatus.hitBox.top -= 8;
		charPos.y -= 8;

		if (pStatus.isDash) pStatus.isJumpDash = true;
	}

	// ������
	else
	{
		if (pStatus.touchLeft || pStatus.touchRight) wallKick();
	}
}

void Player::dash(bool direction)
{
	pStatus.velocityX = 0.0f;

	if (direction == true && !pStatus.touchRight)
	{
		currentState = CharacterState::Dash;
		pStatus.isDash = true;
		dashSpeed = pStatus.dashSpeed;
	}

	else if (direction == false && !pStatus.touchLeft)
	{
		currentState = CharacterState::Dash;
		pStatus.isDash = true;
		dashSpeed = pStatus.dashSpeed;
	}

	else dashSpeed = 0.0f;
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

		else if (currentState == CharacterState::JumpUp)
		{
			int random = RND->getInt(4);
			if (random == 0);
			else SOUNDMANAGER->play("Voice_" + pStatus.charName + "Jump" + to_string(random), 0.5f);

			// ������ 3���� + ������
			// ���ε� 3���� + ������
			// �Ѵ� ������� ���� �Ҹ� �� �ϳ��� ���� ���� + �� ������ �Ҹ���
		}

		
		else if (currentState == CharacterState::WallKick)
		{
			// ������ 3���� + ������
			// ���ε� 3���� + ������
			// �Ѵ� ������� ���� �Ҹ� �� �ϳ��� ���� ���� + �� ������ �Ҹ���
		}
		
		else if (currentState == CharacterState::Dash)
		{
			soundResult = "SFX_DashStart";
			SOUNDMANAGER->play(soundResult, 0.5f);
		}

		else if (currentState == CharacterState::Idle && previousState == CharacterState::Dash)
		{
			soundResult = "SFX_DashEnd";
			SOUNDMANAGER->play(soundResult, 0.5f);
		}

		// ��� �̱���
		/*
		// ��� ����

		// ��� ��

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

void Player::wallSlide(void)
{
	currentState = CharacterState::WallSlide;
	pStatus.velocityY = 1.9f;
}

void Player::wallDrop(void)
{
	if (currentState == CharacterState::WallSlide)
	{
		currentState = CharacterState::FallingDown;

		int fall;
		if (pStatus.lookRight) fall = -4;
		else fall = 4;

		if (CAMERAMANAGER->getLockX() == true)
		{
			pStatus.hitBox.left += fall;
			pStatus.hitBox.right += fall;
			charPos.x += fall;
		}
		else charPos.x += fall;
	}
}

void Player::wallKick(void)
{
	currentState = CharacterState::WallKick;

	pStatus.velocityY = -12.0f;
	pStatus.isWallKick = true;
	
	SOUNDMANAGER->play("Voice_" + pStatus.charName + "Jump1", 0.5f);

	if (pStatus.lookRight)
	{
		pStatus.wallKickRight = true;
		
		if (pressDash == true)
		{
			cout << "�۵�" << endl;
			pStatus.velocityX = -12.0f;
		}

		else pStatus.velocityX = -8.0f;
	}

	else
	{
		pStatus.wallKickRight = false;

		if (pressDash == true)
		{
			pStatus.velocityX = 12.0f;
		}
		else pStatus.velocityX = 8.0f;
	}
}

void Player::attack(void)
{
	// Do Nothing!!	
}

void Player::applyForce(void)
{
	// ����
	if (!pStatus.isOnGround && currentState == CharacterState::Warp)
	{
		if (CAMERAMANAGER->getLockY() == true)
		{
			charPos.y += 16;
			pStatus.hitBox.top += 16;
			pStatus.hitBox.bottom += 16;
		}

		else charPos.y += 12;
	}

	// ���� ���� - ���� + �� �����̵�
	else if (!pStatus.isOnGround)
	{
		pStatus.isDash = false;

		pStatus.velocityY += progress.gravityAccel;

		if (pStatus.velocityY > pStatus.maxFallSpeed)
			pStatus.velocityY = pStatus.maxFallSpeed;

		if (CAMERAMANAGER->getLockY() == true)
		{
			charPos.y += pStatus.velocityY;
			pStatus.hitBox.top += pStatus.velocityY;
			pStatus.hitBox.bottom += pStatus.velocityY;
		}

		else charPos.y += pStatus.velocityY;

		if (currentState == CharacterState::JumpUp)
		{
			if (pStatus.velocityY > -7.0f) isJumpUp = false;
			if (pStatus.velocityY > 0.0f) currentState = CharacterState::FallingDown;
		}

		// ���߿��� �����ϰų� ��Ÿ�� �ϴ°� �ƴϸ� ���� �̹����� ���� -> ������� ������, 2�� ������ ���� ����
		else if (currentState != CharacterState::JumpUp && currentState != CharacterState::WallSlide && currentState != CharacterState::WallKick) currentState = CharacterState::FallingDown;

		// ��Ÿ�� �� ��ư�� ������ ����
		else if (currentState == CharacterState::WallSlide && (!pStatus.touchLeft && !pStatus.touchRight)) currentState = CharacterState::FallingDown;

		else if (currentState == CharacterState::WallKick && pStatus.player->getFrameX() >= pStatus.player->getMaxFrameX()) currentState = CharacterState::FallingDown;

		else;
	}

	// ������
	if (CAMERAMANAGER->getLockX() == true)
	{
		if (pStatus.hitBox.left + pStatus.velocityX >= 0)
		{
			pStatus.hitBox.left += pStatus.velocityX;
			pStatus.hitBox.right += pStatus.velocityX;
			charPos.x += pStatus.velocityX;
		}

		else
		{
			charPos.x = pStatus.hitBox.left + hitBoxWidth / 2;
			currentState = CharacterState::Idle;
		}
	}

	else charPos.x += pStatus.velocityX;

	
	if (!pStatus.lookRight && pStatus.isWallKick)
	{	
		if (pStatus.velocityX > 0.0f) pStatus.velocityX -= 0.8f;
		else
		{
			pStatus.isWallKick = false;
			pStatus.velocityX = 0.0f;
		}
	}

	else if (pStatus.lookRight && pStatus.isWallKick)
	{
		if (pStatus.velocityX < 0.0f) pStatus.velocityX += 0.8f;
		else
		{
			pStatus.isWallKick = false;
			pStatus.velocityX = 0.0f;
		}
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

void Player::colorSetting(void)
{
	// Do Nothing!!
}

void Player::colorChange(void)
{
	// Do Nothing!!
}

string Player::printBodyState(void)
{
	string result;

	if (currentState == CharacterState::Idle) result = "���";
	else if (currentState == CharacterState::Walk) result = "�ȱ�";
	else if (currentState == CharacterState::JumpUp) result = "����";
	else if (currentState == CharacterState::FallingDown) result = "����";
	else if (currentState == CharacterState::Dash) result = "���";
	else if (currentState == CharacterState::Land) result = "����";
	else if (currentState == CharacterState::Climb) result = "��ٸ� Ÿ��";
	else if (currentState == CharacterState::Crouch) result = "�ɱ�";
	else if (currentState == CharacterState::Warp) result = "����";
	else if (currentState == CharacterState::WallSlide) result = "��Ÿ��";
	else if (currentState == CharacterState::WallKick) result = "������";
	else if (currentState == CharacterState::Dead) result = "���";
	
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
