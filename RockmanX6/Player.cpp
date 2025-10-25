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

		pStatus.isWallSlide = false;
	}

	
}

void Player::jump(void)
{
	// 일반 점프
	if (pStatus.isOnGround)  // 땅에 있을 때만 점프
	{
		pStatus.isJumpUp = true;
		currentState = CharacterState::JumpUp;

		pStatus.velocityY = pStatus.jumpPower;
		
		pStatus.hitBox.bottom -= 8;
		pStatus.hitBox.top -= 8;
		charPos.y -= 8;

		if (pStatus.isDash) pStatus.isJumpDash = true;
	}

	// 벽차기
	else
	{
		if ((pStatus.touchLeft && !pStatus.lookRight) || (pStatus.touchRight && pStatus.lookRight)) wallKick();
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
	// 추가 해야 할 것

	// 아머 획득 사운드
	// 약한, 강한 피격음
	// 점프 사운드

	if (previousState != currentState)
	{
		// 공통 사운드
		
		// 소환 시작
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

		// 기능 미구현
		/*
		// 대시 시작

		// 대시 끝

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

void Player::wallSlide(void)
{
	currentState = CharacterState::WallSlide;
	pStatus.isWallSlide = true;
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
	
	pStatus.velocityY = -9.0f;
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

		else pStatus.velocityX = -6.5f;
	}

	else
	{
		pStatus.wallKickRight = false;

		if (pressDash == true)
		{
			pStatus.isJumpDash = true;
			pStatus.velocityX = pStatus.dashSpeed;
		}
		else pStatus.velocityX = 6.5f;
	}
}

void Player::attack(void)
{
	// Do Nothing!!	
}

void Player::applyForce(void)
{
#pragma region 특수 상황

	// 벽차기 타이머
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

#pragma region X축, Y축 이동

	// X축 이동 - 기본 베이스
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

	// 워프 - 존나 빠르게
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

	// Y축 이동 - 기본 베이스
	else if (!pStatus.isOnGround)
	{
		pStatus.isDash = false;

		// 중력 가속도 추가
		if (!pStatus.isWallKick) pStatus.velocityY += progress.gravityAccel;

		// 최대 낙하 속도 제한
		if (pStatus.velocityY > pStatus.maxFallSpeed)
			pStatus.velocityY = pStatus.maxFallSpeed;

		// 기본 중력
		if (CAMERAMANAGER->getLockY() == true)
		{
			charPos.y += pStatus.velocityY;
			pStatus.hitBox.top += pStatus.velocityY;
			pStatus.hitBox.bottom += pStatus.velocityY;
		}

		else charPos.y += pStatus.velocityY;
		
		// 공중에서 상태 변경
		if (currentState == CharacterState::JumpUp)	if (pStatus.velocityY > -7.0f) pStatus.isJumpUp = false;
		if (pStatus.velocityY > 0.0f && !pStatus.isWallSlide && currentState != CharacterState::OverPower) currentState = CharacterState::FallingDown;
	}

#pragma endregion
	
}

void Player::spawn(int x, int y)
{
	// Do Nothing!!
}

void Player::currentAnimChange(void)
{
	////////////////////////
// 워프
////////////////////////

	if (currentState == CharacterState::Warp)
	{
		animSpeed = 0.07f;
		changeAnimation(pStatus.charName + "Spawn", 0);
	}

	////////////////////////
	// 대기
	////////////////////////

	else if (currentState == CharacterState::Idle)
	{
		switch (attState)
		{
		case SholderState::LargeBurst:
			animSpeed = 0.07f;
			changeAnimation(pStatus.charName + "StandChargeBurst", 0);
			break;

		case SholderState::Burst:
			animSpeed = 0.07f;
			changeAnimation(pStatus.charName + "StandBurstLoop", 0);
			break;

		case SholderState::Hold:
			animSpeed = 0.1f;
			changeAnimation(pStatus.charName + "StandBurstEnd", 0);
			break;

		case SholderState::None:
			animSpeed = 0.17f;
			changeAnimation(pStatus.charName + "Idle", 0);
			break;
		}
	}

	////////////////////////
	// 걷기
	////////////////////////

	else if (currentState == CharacterState::Walk)
	{
		if (!isMoving)
		{
			animSpeed = 0.1f;

			switch (attState)
			{
			case SholderState::Burst:
			case SholderState::LargeBurst:
				changeAnimation(pStatus.charName + "WalkBurstStart", 0);
				break;
			case SholderState::Hold:
			case SholderState::None:
				changeAnimation(pStatus.charName + "WalkStart", 0);
				break;
			}

			if (pStatus.player->getFrameX() >= pStatus.player->getMaxFrameX()) isMoving = true;
		}

		else if (isMoving)
		{
			animSpeed = 0.04f;

			switch (attState)
			{
			case SholderState::Burst:
			case SholderState::LargeBurst:
				if (previousAnim == pStatus.charName + "WalkBurstStart") changeAnimation(pStatus.charName + "WalkBurstLoop", 0);
				else changeAnimation(pStatus.charName + "WalkBurstLoop", pStatus.player->getFrameX());
				break;
			case SholderState::Hold:
			case SholderState::None:
				if (previousAnim == pStatus.charName + "WalkStart") changeAnimation(pStatus.charName + "WalkLoop", 0);
				else changeAnimation(pStatus.charName + "WalkLoop", pStatus.player->getFrameX());
				break;
			}
		}
	}

	////////////////////////
	// 대시
	////////////////////////

	else if (currentState == CharacterState::Dash && pStatus.isDash)
	{
		if (!isMoving)
		{
			animSpeed = 0.12f;

			switch (attState)
			{
			case SholderState::Burst:
			case SholderState::LargeBurst:
				changeAnimation(pStatus.charName + "DashBurstStart", 0);
				break;
			case SholderState::Hold:
			case SholderState::None:
				changeAnimation(pStatus.charName + "DashStart", 0);
				break;
			}

			if (pStatus.player->getFrameX() >= pStatus.player->getMaxFrameX()) isMoving = true;
		}

		else if (isMoving)
		{
			animSpeed = 0.05f;

			switch (attState)
			{
			case SholderState::Burst:
			case SholderState::LargeBurst:
				if (previousAnim == pStatus.charName + "DashBurstStart") changeAnimation(pStatus.charName + "DashBurstLoop", 0);
				else changeAnimation(pStatus.charName + "DashBurstLoop", pStatus.player->getFrameX());
				break;
			case SholderState::Hold:
			case SholderState::None:
				if (previousAnim == pStatus.charName + "DashStart") changeAnimation(pStatus.charName + "DashLoop", 0);
				else changeAnimation(pStatus.charName + "DashLoop", pStatus.player->getFrameX());
				break;
			}
		}
	}

	////////////////////////
	// 대시 종료
	////////////////////////

	else if (currentState == CharacterState::DashEnd)
	{
		animSpeed = 0.1f;

		switch (attState)
		{
		case SholderState::Burst:
		case SholderState::LargeBurst:
			if (previousAnim == pStatus.charName + "DashEnd") changeAnimation(pStatus.charName + "DashEndBurst", pStatus.player->getFrameX());
			else changeAnimation(pStatus.charName + "DashEndBurst", 0);

			break;
		case SholderState::Hold:
		case SholderState::None:
			if (previousAnim == pStatus.charName + "DashEndBurst") changeAnimation(pStatus.charName + "DashEnd", pStatus.player->getFrameX());
			else changeAnimation(pStatus.charName + "DashEnd", 0);
			break;
		}
	}

	////////////////////////
	// 점프
	////////////////////////

	else if (currentState == CharacterState::JumpUp)
	{
		animSpeed = 0.06f;

		switch (attState)
		{
		case SholderState::Burst:
		case SholderState::LargeBurst:
			if (previousAnim == pStatus.charName + "Jump") changeAnimation(pStatus.charName + "JumpBurst", pStatus.player->getFrameX());
			else changeAnimation(pStatus.charName + "JumpBurst", 0);

			break;
		case SholderState::Hold:
		case SholderState::None:
			if (previousAnim == pStatus.charName + "JumpBurst") changeAnimation(pStatus.charName + "Jump", pStatus.player->getFrameX());
			else changeAnimation(pStatus.charName + "Jump", 0);
			break;
		}

		if (pStatus.player->getFrameX() >= 4) pStatus.player->setFrameX(4);
	}

	////////////////////////
	// 낙하
	////////////////////////

	else if (currentState == CharacterState::FallingDown)
	{
		animSpeed = 0.06f;

		switch (attState)
		{
		case SholderState::Burst:
		case SholderState::LargeBurst:
			if (previousAnim == pStatus.charName + "Jump") changeAnimation(pStatus.charName + "JumpBurst", pStatus.player->getFrameX());
			else changeAnimation(pStatus.charName + "JumpBurst", 5);

			break;
		case SholderState::Hold:
		case SholderState::None:
			if (previousAnim == pStatus.charName + "JumpBurst") changeAnimation(pStatus.charName + "Jump", pStatus.player->getFrameX());
			else changeAnimation(pStatus.charName + "Jump", 5);
			break;
		}

		if (pStatus.player->getFrameX() >= pStatus.player->getMaxFrameX()) pStatus.player->setFrameX(pStatus.player->getMaxFrameX());
	}

	////////////////////////
	// 벽 타기
	////////////////////////

	else if (currentState == CharacterState::WallSlide)
	{
		animSpeed = 0.06f;

		switch (attState)
		{
		case SholderState::Burst:
		case SholderState::LargeBurst:
			if (previousAnim == pStatus.charName + "WallSlide") changeAnimation(pStatus.charName + "WallSlideBurst", pStatus.player->getFrameX());
			else changeAnimation(pStatus.charName + "WallSlideBurst", 0);

			break;
		case SholderState::Hold:
		case SholderState::None:
			if (previousAnim == pStatus.charName + "WallSlideBurst") changeAnimation(pStatus.charName + "WallSlide", pStatus.player->getFrameX());
			else changeAnimation(pStatus.charName + "WallSlide", 0);
			break;
		}

		if (pStatus.player->getFrameX() >= pStatus.player->getMaxFrameX()) pStatus.player->setFrameX(pStatus.player->getMaxFrameX());
	}

	////////////////////////
	// 벽 차기
	////////////////////////

	else if (currentState == CharacterState::WallKick)
	{
		animSpeed = 0.06f;

		switch (attState)
		{
		case SholderState::Burst:
		case SholderState::LargeBurst:
			if (previousAnim == pStatus.charName + "WallKick") changeAnimation(pStatus.charName + "WallKickBurst", pStatus.player->getFrameX());
			else changeAnimation(pStatus.charName + "WallKickBurst", 0);

			break;
		case SholderState::Hold:
		case SholderState::None:
			if (previousAnim == pStatus.charName + "WallKickBurst") changeAnimation(pStatus.charName + "WallKick", pStatus.player->getFrameX());
			else changeAnimation(pStatus.charName + "WallKick", 0);
			break;
		}

		if (pStatus.player->getFrameX() >= pStatus.player->getMaxFrameX()) currentState = CharacterState::FallingDown;
	}

	////////////////////////
	// 데미지
	////////////////////////

	else if (currentState == CharacterState::OverPower)
	{
		animSpeed = 0.1f;

		if (pStatus.player->getFrameX() >= pStatus.player->getMaxFrameX()) pStatus.movable = true;
	}


	////////////////////////
	// 공격 애니메이션 타이밍 체크
	////////////////////////

	if (attState == SholderState::Burst || attState == SholderState::LargeBurst)
	{
		if (TIMEMANAGER->getWorldTime() - attackTimer >= attackDuration)
		{
			if (currentState == CharacterState::Walk) attState = SholderState::None;
			else if (currentState == CharacterState::JumpUp) attState = SholderState::None;
			else if (currentState == CharacterState::FallingDown) attState = SholderState::None;
			else if (currentState == CharacterState::WallSlide) attState = SholderState::None;
			else if (currentState == CharacterState::WallKick) attState = SholderState::None;
			else if (currentState == CharacterState::Dash) attState = SholderState::None;
			else if (currentState == CharacterState::DashEnd) attState = SholderState::None;
			else if (currentState == CharacterState::Idle) attState = SholderState::Hold;
		}
	}

	hitBoxCenter.x = (pStatus.hitBox.left + pStatus.hitBox.right) / 2;
	hitBoxCenter.y = pStatus.hitBox.bottom;

	setHitBox();

	previousAnim = currentAnim;
	previousState = currentState;
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

	if (currentState == CharacterState::Idle) result = "대기";
	else if (currentState == CharacterState::Walk) result = "걷기";
	else if (currentState == CharacterState::JumpUp) result = "점프";
	else if (currentState == CharacterState::FallingDown) result = "낙하";
	else if (currentState == CharacterState::Dash) result = "대시";
	else if (currentState == CharacterState::DashEnd) result = "대시 종료";
	else if (currentState == CharacterState::Land) result = "착지";
	else if (currentState == CharacterState::Climb) result = "사다리 타기";
	else if (currentState == CharacterState::Crouch) result = "앉기";
	else if (currentState == CharacterState::Warp) result = "워프";
	else if (currentState == CharacterState::WallSlide) result = "벽타기";
	else if (currentState == CharacterState::WallKick) result = "벽차기";
	else if (currentState == CharacterState::Dead) result = "사망";
	
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
