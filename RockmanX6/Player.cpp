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
	if (pStatus.isOnGround) currentState = CharacterState::Walk;

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
}

void Player::jump(void)
{
	if (pStatus.isOnGround)  // 땅에 있을 때만 점프
	{
		currentState = CharacterState::JumpUp;
		pStatus.jumpStack = true;
		pStatus.velocityY = pStatus.jumpPower;
		pStatus.jumpTimer = 0.0f;
	}
}

void Player::dash(void)
{
	// Later
}

void Player::sfxPlay(void)
{
	// 추가 해야 할 것

	// 아머 획득 사운드
	// 약한, 강한 피격음
	// 점프 사운드
	// 

	if (previousState != currentState)
	{
		// 공통 사운드
		
		// 소환 시작
		if (currentState == CharacterState::Warp)
		{
			soundResult = "SFX_" + pStatus.charName + "WarpIn";
			SOUNDMANAGER->play(soundResult, 0.5f);
		}

		// 기능 미구현

		/*
		// 대시 시작
		else if (currentState == CharacterState::Dash)
		{
			soundResult = "SFX_" + pStatus.charName + "DashStart";
			SOUNDMANAGER->play(soundResult, 0.5f);
		}

		// 대시 끝
		else if (currentState == CharacterState::Idle && previousState == CharacterState::Dash)
		{
			soundResult = "SFX_" + pStatus.charName + "DashEnd";
			SOUNDMANAGER->play(soundResult, 0.5f);
		}

		// 착지
		else if (currentState == CharacterState::Land)
		{
			soundResult = "SFX_" + pStatus.charName + "DashEnd";
			SOUNDMANAGER->play(soundResult, 0.5f);			
		}

		// 벽차기
		else if (previousState == CharacterState::Wall && currentState == CharacterState::JumpUp)
		{
			soundResult = "SFX_" + pStatus.charName + "WallKick";
			SOUNDMANAGER->play(soundResult, 0.5f);
			
			soundResult = "Voice_" + pStatus.charName + "WallKick";
			SOUNDMANAGER->play(soundResult, 0.5f);
		}
		*/


		// X 전용 사운드

		if (pStatus.charName == "X_")
		{

		}

		// Zero 전용 사운드

		else if (pStatus.charName == "Zero_")
		{

		}
	}
}


void Player::attack(void)
{
	// Do Nothing!!	
}

void Player::isJump(void)
{
	if (currentState == CharacterState::JumpUp)
	{
		pStatus.velocityY = pStatus.jumpPower;
		pStatus.jumpTimer = 0.0f;
	}
}

void Player::applyGravity(void)
{
	if (!pStatus.isOnGround && currentState == CharacterState::Warp)
	{
		charPos.y += 12;
		pStatus.hitBox.top += 12;
		pStatus.hitBox.bottom += + 12;
	}

	else if (!pStatus.isOnGround) // && currentState != CharacterState::Warp)
	{
		pStatus.velocityY += progress.gravity;
		if (pStatus.velocityY > pStatus.maxFallSpeed) pStatus.velocityY = pStatus.maxFallSpeed;

		charPos.y += pStatus.velocityY;
		pStatus.hitBox.top += pStatus.velocityY;
		pStatus.hitBox.bottom += pStatus.velocityY;

		if (currentState == CharacterState::JumpUp && pStatus.velocityY > 0.0f)
			currentState = CharacterState::FallingDown;

		if(currentState != CharacterState::JumpUp) currentState = CharacterState::FallingDown;
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

	if (currentState == CharacterState::Idle) result = "대기";
	else if (currentState == CharacterState::Walk) result = "걷기";
	else if (currentState == CharacterState::Dash) result = "대시";
	else if (currentState == CharacterState::JumpUp) result = "점프";
	else if (currentState == CharacterState::FallingDown) result = "낙하";
	else if (currentState == CharacterState::Crouch) result = "앉기";
	else if (currentState == CharacterState::Climb) result = "사다리 타기";
	else if (currentState == CharacterState::Warp) result = "워프";
	else if (currentState == CharacterState::SpecialAtt) result = "기가 어택";
	else if (currentState == CharacterState::Dead) result = "사망";
	else if (currentState == CharacterState::Wall) result = "벽타기";

	return result;
}

string Player::printAttState(void)
{
	string result;

	if (attState == SholderState::None) result = "대기";
	else if (attState == SholderState::Burst) result = "버스터";
	else if (attState == SholderState::LargeBurst) result = "버스터";
	else if (attState == SholderState::Hold) result = "버스터";
	
	return result;
}

void Player::setHitBox(void)
{
	// Do Nothing!
}
