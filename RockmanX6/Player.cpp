#include "Stdafx.h"
#include "Player.h"
#include "BulletManager.h"

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
	if (pStatus.movable)
	{
		if (pStatus.isJumpDash)
		{
			if (pStatus.lookRight) pStatus.velocityX = dashSpeed;
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

			SOUNDMANAGER->play("SFX_Jump", 0.5f);
		}

		
		else if (currentState == CharacterState::WallKick)
		{

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

	pStatus.velocityY = -8.0f;
	pStatus.isWallKick = true;
	
	SOUNDMANAGER->play("Voice_" + pStatus.charName + "Jump1", 0.5f);

	if (pStatus.lookRight)
	{
		pStatus.wallKickRight = true;
		
		if (pressDash == true)
		{
			pStatus.isJumpDash = true;
			pStatus.velocityX = -pStatus.dashSpeed;
		}

		else pStatus.velocityX = -8.0f;
	}

	else
	{
		pStatus.wallKickRight = false;

		if (pressDash == true)
		{
			pStatus.isJumpDash = true;
			pStatus.velocityX = pStatus.dashSpeed;
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

#pragma region Ư�� ��Ȳ

	// ������
	if (pStatus.isWallKick)
	{
		wallkickTimer += 0.1f;

		if (wallkickTimer >= wallkickMaxTime)
		{
			pStatus.isWallKick = false;
			wallkickTimer = 0.0f;
			pStatus.velocityX = 0.0f;
		}
	}

#pragma endregion

#pragma region X��, Y�� �̵�

	// X�� �̵� - �⺻ ���̽�
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

	// ���� - ���� ������
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

	// Y�� �̵� - �⺻ ���̽�
	else if (!pStatus.isOnGround)
	{
		pStatus.isDash = false;

		// �ִ� ���� �ӵ� ����
		if (pStatus.velocityY > pStatus.maxFallSpeed)
			pStatus.velocityY = pStatus.maxFallSpeed;

		// �⺻ �߷�
		if (CAMERAMANAGER->getLockY() == true)
		{
			charPos.y += pStatus.velocityY;
			pStatus.hitBox.top += pStatus.velocityY;
			pStatus.hitBox.bottom += pStatus.velocityY;
		}

		else charPos.y += pStatus.velocityY;
		
		// ������ �� �߷� ���� ����
		if(!pStatus.isWallKick)	pStatus.velocityY += progress.gravityAccel;

		// ���߿��� ���� ����
		if (currentState == CharacterState::JumpUp)	if (pStatus.velocityY > -7.0f) isJumpUp = false;
		if (pStatus.velocityY > 0.0f && currentState != CharacterState::WallSlide && currentState != CharacterState::OverPower) currentState = CharacterState::FallingDown;
	}

#pragma endregion
	
}

void Player::spawn(int x, int y)
{
	// Do Nothing!!
}

void Player::currentAnimChange(void)
{
	// Do Nothing!!
}

void Player::setOverPower(bool op, BulletSize bullet)
{
	pStatus.invincible = op;

	if (pStatus.invincible) currentState = CharacterState::OverPower;

	switch (bullet)
	{
		case BulletSize::Small:
			currentAnim = pStatus.charName + "SmallDamaged";
			pStatus.movable = false;
			break;
		case BulletSize::Large:
			currentAnim = pStatus.charName + "LargeDamaged";
			pStatus.movable = false;
			break;
	}
}

void Player::colorSetting(void)
{
	// Do Nothing!!
}

void Player::colorChange(void)
{
	// Do Nothing!!
}

void Player::changeAnimation(const string& animName, int frame)
{
	if (currentAnim != animName)
	{
		currentAnim = animName;
		pStatus.player = IMAGEMANAGER->findImage(currentAnim);
		pStatus.player->setFrameX(frame);
	}
}

string Player::printBodyState(void)
{
	string result;

	if (currentState == CharacterState::Idle) result = "���";
	else if (currentState == CharacterState::Walk) result = "�ȱ�";
	else if (currentState == CharacterState::JumpUp) result = "����";
	else if (currentState == CharacterState::FallingDown) result = "����";
	else if (currentState == CharacterState::Dash) result = "���";
	else if (currentState == CharacterState::DashEnd) result = "��� ����";
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
