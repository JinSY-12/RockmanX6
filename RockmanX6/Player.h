#pragma once
#include "GameNode.h"
#include "EffectType.h"
#include "BulletType.h"

class BulletManager;

class Player : public GameNode
{
private:


public:

#pragma region PlayerType

	enum class CharacterType
	{
		X,
		Zero
	};

	enum class ArmorType
	{
		Normal,
		Falcon,
		Shadow,
		Ultimate,
		BlakcZero
	};

#pragma endregion

#pragma region PlayerStruct
	struct CharcterPos
	{
		int x;
		int y;
	};

	struct Anim
	{
		int x;
		int y;
	};

	struct Offset
	{
		int x;
		int y;
	};

	struct BursterPos
	{
		int x;
		int y;
	};

	enum class SholderState
	{
		None,
		Burst,
		LargeBurst,
		Hold
	};

	// 캐릭터 상태값
	enum class CharacterState
	{
		Idle,
		Walk,
		JumpUp,
		FallingDown,
		Dash,
		DashEnd,
		Crouch,
		Land,
		Climb,
		Warp,
		WallSlide,
		WallKick,
		Dead,
		OverPower
	};

	struct PlayerPalette
	{
		COLORREF base;
		COLORREF afterShadow1;
		COLORREF afterShadow2;
		COLORREF afterShadow3;
		COLORREF charged1;
		COLORREF charged2;
		COLORREF burst;
		COLORREF lowDamaged;
		COLORREF highDamaged;
	};
	
	// 캐릭터 메인 설정값
	struct PlayerStatus
	{
		// 판정 및 이미지
		GImage* player;
		RECT hitBox;
		RECT floorCheck;

		string charName;
		int firePointX;
		int firePointY;

		// 스탯
		float hp;
		float maxHp;
		float mp;
		float maxMp;

		// 이동 스탯
		float moveSpeed;
		float dashSpeed;
		float jumpPower;
		float velocityX;
		
		// 상태값
		bool invincible;
		bool lookRight;
		bool isOnGround;
		bool isOnTop;
		bool Dead;

		bool touchLeft;
		bool touchRight;
		
		// 점프 관련
		float velocityY;
		float maxFallSpeed;
		bool isJumpUp;
		bool isWallKick;
		bool isWallSlide;
		bool wallKickRight;

		// 대시 관련
		bool isDash;
		bool isJumpDash;

		// 무적 관련
		bool movable;
		float invincibleTimer;
		float invincibleMaxTime;
	};

	struct Progress
	{
		int life;
		int limitLife = 9;
		// 아머 비트셋의 순서는 헤드, 보디, 암, 풋
		bitset<4> isArmd[2]; // 아머는 하나만 구현할거지만 배열 크기 2로 둬서 두개 만드는 척!

		// 보스 비트셋 순서는 화면 왼쪽 위에서부터 시계 방향
		bitset<4> clearBoss;
		float gravityAccel;
	};

#pragma endregion

	// 캐릭터 행동 및 스탯
	PlayerStatus pStatus;
	CharacterState previousState;
	CharacterState currentState;
	SholderState attState;

	Progress progress;

	PlayerPalette playerColor[15];

	// 캐릭터 판정 및 좌표
	int hitBoxWidth;
	int hitBoxHeight;

	CharcterPos hitBoxCenter;
	CharcterPos charPos;
	Anim animBaseline;
	BursterPos busterPos;

	// 플레이어 상태값
	CharacterType charType;
	ArmorType armorType;

	bool inputEnabled;
	bool isMoving;

	// 애니메이션 관련
	Offset animOffset;

	string previousAnim;
	string currentAnim;

	int animDir;
	float animSpeed;
	float delayTimer;
	bool attChange;

	float attackTimer;
	const float attackDuration = 0.4f;
	bool attCheckOnce;

	float now;
	float lastShootTime;
	float shotCoolDown;

	bool burstloop;

	// 사운드 관련
	string soundResult;

	BulletManager* bManager;

	// 키 입력 관련
	bool multiInput;
	bool pressLeft;
	bool pressRight;
	bool lastKeyIsRight;

	// 점프 관련
	float wallkickTimer;
	float wallkickMaxTime;

	// 대시 관련
	float dashTimer;
	float maxDashTime;
	float dashSpeed;
	bool pressDash;
	bool aniDash;

	GImage* afterImage;

	int colorType;
	int colorTimer;

	BulletType bulletType;

public:
	virtual HRESULT init(void);
	virtual HRESULT init(int x, int y);
	virtual void release(void);
	virtual void update(void);
	virtual void render(void);

	// 캐릭터 공통 조작
	virtual void move(bool direction);
	virtual void jump(void);
	virtual void dash(bool direction);
	virtual void attack(void);

	void wallSlide(void);
	void wallDrop(void);
	void wallKick(void);
	
	// 캐릭터 공통 상태값
	void applyForce(void);
	void sfxPlay(void);
	void setBulletManager(BulletManager* manager) { bManager = manager; };

	// 캐릭터 스폰
	virtual void spawn(int x, int y);

	// 상태값
	string printBodyState(void);
	string printAttState(void);

	// 애니메이션 관련
	virtual void setHitBox(void);
	virtual void currentAnimChange(void);

	// 상태 관련
	inline void setStageGravity(float gravityPower) { progress.gravityAccel = gravityPower; }

	// settter/getter
	// 좌표 및 판정
	RECT getPlayerRect(void) { return pStatus.hitBox; }
	inline int getPlayerCenter(void) { return charPos.x; }
	inline int getPlayerTop(void) { return charPos.y - hitBoxHeight; }
	inline int getPlayerBottom(void) { return charPos.y; }
	inline int getPlayerLeft(void) { return charPos.x - hitBoxWidth / 2; }
	inline int getPlayerRight(void) { return charPos.x + hitBoxWidth /2; }
	
	inline bool getPlayerSight(void) { return pStatus.lookRight; }
	inline bool getOverPower(void) { return pStatus.invincible; }

	// 상태값
	inline void setLeftCollision(bool left, int leftline)
	{
		pStatus.touchLeft = left;

		if (pStatus.touchLeft == true)
		{
			wallkickTimer = 0.0f;
			charPos.x = leftline + hitBoxWidth / 2;

			int left = leftline - CAMERAMANAGER->getCameraPos().x;
			pStatus.hitBox.left = left;
			pStatus.hitBox.right = pStatus.hitBox.left + hitBoxWidth;

			pStatus.velocityX = 0.0f;

			pStatus.isWallKick = false;
			pStatus.isJumpDash = false;
		}
	}
	inline void setRightCollision(bool right, int rightline)
	{
		pStatus.touchRight = right;

		if (pStatus.touchRight == true)
		{
			wallkickTimer = 0.0f;

			charPos.x = rightline - hitBoxWidth / 2;

			int right = rightline - CAMERAMANAGER->getCameraPos().x;
			pStatus.hitBox.right = right;
			pStatus.hitBox.left = pStatus.hitBox.right - hitBoxWidth;
			
			pStatus.velocityX = 0.0f;

			pStatus.isWallKick = false;
			pStatus.isJumpDash = false;
		}
	}
	inline void setIsOnGround(bool OnGround, int topline)
	{
		pStatus.isOnGround = OnGround;
		
		if (pStatus.isOnGround == true)
		{
			charPos.y = topline - 2;

			int bottom = topline - CAMERAMANAGER->getCameraPos().y;
			pStatus.hitBox.bottom = bottom - 2;
			pStatus.hitBox.top = pStatus.hitBox.bottom - hitBoxHeight;

			pStatus.velocityY = 0.0f;

			if (pStatus.isJumpDash)
			{
				pStatus.isDash = false;
				pStatus.isJumpDash = false;
			}
		}
	}
	inline void setTopCollision(bool top, int bottomline)
	{
		pStatus.isOnTop = top;

		if (top == true)
		{
			charPos.y = bottomline + 3  + hitBoxHeight;

			int top = bottomline - CAMERAMANAGER->getCameraPos().y;
			pStatus.hitBox.top = top + 3 ;
			pStatus.hitBox.bottom = pStatus.hitBox.top + hitBoxHeight;

			pStatus.velocityY = 0.0f;
			pStatus.velocityX = 0.0f;

			wallkickTimer = wallkickMaxTime;
		}
	}
	void setOverPower(bool op, BulletSize bullet);

	Progress getProgress(void) { return progress; }

	CharcterPos& getCharPos(void) { return charPos; }

	// 스탯 관련
	inline void reduceHp(int damage, BulletSize size) {

		pStatus.hp -= damage;

		if (pStatus.hp > 0)
		{
			int random = RND->getInt(2);
			if (random == 0) SOUNDMANAGER->play("Voice_X_Damaged1");
			else SOUNDMANAGER->play("Voice_X_Damaged2");
			
			currentState = CharacterState::OverPower;
			pStatus.movable = false;
			pStatus.invincible = true;
			animSpeed = 0.06f;

			switch (size)
			{
			case BulletSize::Small:
				// 소경직
				changeAnimation(pStatus.charName + "SmallDamaged", 0);

				if(pStatus.lookRight) pStatus.velocityX = -3.0f;
				else pStatus.velocityX = 3.0f;

				pStatus.velocityY = 0.0f;

				break;
			case BulletSize::Large:
				// 대경직
				changeAnimation(pStatus.charName + "LargeDamaged", 0);

				if (pStatus.lookRight) pStatus.velocityX = -2.0f;
				else pStatus.velocityX = 2.0f;

				pStatus.velocityY = 0.0f;

				break;
			}
		}	

		else 
		{
			currentState = CharacterState::Dead;
			SOUNDMANAGER->play("Voice_" + pStatus.charName + "Dead");
			pStatus.Dead = true;
			pStatus.invincible = true;
			pStatus.movable = false;
		}
	}

	inline void reduceMp(int damage) { pStatus.mp -= damage; }
	inline float lerp(float start, float end, float time)
	{
		return start + (end - start) * time;
	}

	inline void isDead(void)
	{
		if (pStatus.hp <= 0)
		{
			pStatus.hp = 0;
			pStatus.invincible = true;
			pStatus.Dead = false;
		}
	}

	inline void invincibleTimerUpdate()
	{
		if (pStatus.invincible && !pStatus.Dead)
		{
			pStatus.invincibleTimer += 0.1f;
			if (pStatus.invincibleTimer >= pStatus.invincibleMaxTime)
			{
				pStatus.invincible = false;
				pStatus.invincibleTimer = 0.0f;
			}
		}
	}

	virtual void colorSetting(void);
	virtual void colorChange(void);

	void changeAnimation(const string& animName, int frame);
};

