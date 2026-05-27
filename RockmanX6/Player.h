#pragma once
#include "CombatEntity.h"
#include "EffectType.h"
#include "BulletType.h"
#include "SoundType.h"
#include "Event.h"
#include "ProgressBar.h"

class Player : public CombatEntity
{
private:

protected:

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
		Hold,
		Special
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
		OverPower,
		LadderStart,
		LadderClimb,
		LadderLoop,
		LadderEnd,
		Victory,
		WarpOut
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
		RECT saberHitBox;
		RECT floorCheck;
		RECT subRect;

		string charName;
		int firePointX;
		int firePointY;

		// 스탯
		// 체력과 데미지는 CombatEntity 공통 스탯에
		// 이동 스탯
		float moveSpeed;
		float dashSpeed;
		float jumpPower;
		float velocityX;

		// 상태값
		bool invincible;
		bool isWarp;
		bool isOnGround;
		bool isOnTop;
		bool isOnLadder;

		bool touchLeft;
		bool touchRight;

		bool warpOut = false;

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
		float overpowerMaxTime;

		// 공격 관련
		float attackDelayTimer;
		float attackDelayMaxTime;
		bool isAttack;
		bool isBurst;
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
#pragma region 변수들
	// 캐릭터 행동 및 스탯
	PlayerStatus pStatus;
	CharacterState previousState;
	CharacterState currentState;
	SholderState attState;

	Progress progress;
	ProgressBar hpBar;

	PlayerPalette playerColor[15];

	// 캐릭터 판정 및 좌표
	Anim animBaseline;
	BursterPos busterPos;

	// 플레이어 상태값
	CharacterType charType;
	ArmorType armorType;

	bool isMoving;

	// 애니메이션 관련
	Offset animOffset;

	string previousAnim;
	string currentAnim;

	int animDir;
	float animSpeed;
	float effectAnimSpeed;
	float delayTimer;
	bool attChange;

	float attackTimer;
	const float attackDuration = 0.4f;
	bool attCheckOnce;

	float chargeCount;
	float chargeSpeed;

	float now;
	float lastShootTime;
	float shotCoolDown;

	int charAlpha;

	bool burstloop;

	// 사운드 관련
	string soundResult;
	bool warpSoundOnce;

	// 키 입력 관련
	bool multiInput;
	bool pressLeft;
	bool pressRight;
	bool lastKeyIsRight;
	bool inputEnabled;
	bool actionLock;
	bool isCutScene;

	// 점프 관련
	float wallkickTimer;
	float wallkickMaxTime;
	bool ladderAble;
	bool ladderEnd;

	// 대시 관련
	float dashTimer;
	float maxDashTime;
	float dashSpeed;
	bool pressDash;
	bool aniDash;

	float afterImageTimer;
	float afterImageInterval;
	bool hideAfterimage;

	// 자체 이펙트 관련
	GImage* attackHandEffect;
	GImage* subHandEffect;

	int colorType;
	int colorTimer;

	int ringSpawnCount;
	int bubbleSpawnCount;

	BulletType bulletType;

	// 차지샷 관련
	float chargeBurstCount;
	bool isCharging;

	GImage* chargeEffect;
	GImage* chargeAura;

	int chargeEffectAlpha;
	int chargeAuraAlpha;
	int bursterEffectAlpha;

	string bursterEffectName;

	int test;

	float timer;

	// 세이버 관련
	bool canHit;
	bool animDelay;

	int prevFrame;
	int currentFrame;

	float multiHitTimer;
	float multiHitMaxTime;

	int saberWidth;
	int saberHeight;
	int saberOffsetX;
	int saberOffsetY;

	ShootEvent shootEvent;

	// 타이머 제작
	Timer bubbleTimer;
	Timer ringTimer;
	Timer deadTimer;

	bool firstBubbleSpawn;
	int ladderPosX = 0;
	bool ladderDone = true;

	bool downKeyPressed = false;
	bool upKeyPressed = false;

#pragma endregion

public:
	void render(HDC memDC) override;

	// 캐릭터 공통 조작
	virtual void move(bool direction);
	virtual void jump(void);
	virtual void dash(bool direction);
	virtual void attack(void);
	virtual void specialAttack(void);

	void wallSlide(void);
	void wallDrop(void);
	void wallKick(void);
	void ladderClimb(void);

	// 캐릭터 공통 상태값
	void applyForce(void);
	void sfxPlay(void);
	void soundPlay(string soundName);

	// 상태값
	string printBodyState(void);
	string printAttState(void);

	// 애니메이션 관련
	virtual void setHitBox(void);
	virtual void currentAnimChange(void);
	virtual void afterImageControl(void);

	// 상태 관련
	inline void setStageGravity(float gravityPower) { progress.gravityAccel = gravityPower; }

	// settter/getter
	// 좌표 및 판정
	RECT getPlayerHitBox(void) { return pStatus.hitBox; }
	RECT getSaberRect(void) { return pStatus.saberHitBox; }
	RECT getSubRect(void) { return pStatus.subRect; }
	inline int getPlayerCenter(void) { return pos.x; }
	inline int getPlayerTop(void) { return pos.y - status.hitBoxHeight; }
	inline int getPlayerBottom(void) { return pos.y; }
	inline int getPlayerLeft(void) { return pos.x - status.hitBoxWidth / 2; }
	inline int getPlayerRight(void) { return pos.x + status.hitBoxWidth / 2; }
	inline bool getCanHit(void) { return canHit; }
	inline int getPlayerHitBoxWidth(void) { return status.hitBoxWidth; }
	Progress getProgress(void) { return progress; }
	inline bool getIsOnLadder() { return pStatus.isOnLadder; }
	inline float getVelocityX() { return pStatus.velocityX; }
	inline float getVelocityY() { return pStatus.velocityY; }
	inline bool getIsWarp() { return pStatus.isWarp; }
	inline bool getWarpOutDone() { return pStatus.warpOut; }

	void setOverPower(bool op, BulletSize bullet);
	void setAnimDelay(bool delay) { animDelay = delay; }
	inline void setLeftCollision(bool left, int leftline)
	{
		pStatus.touchLeft = left;

		if (pStatus.touchLeft == true)
		{
			wallkickTimer = 0.0f;
			pos.x = leftline + status.hitBoxWidth / 2;

			int left = leftline - CAMERAMANAGER->getCameraPos().x;
			pStatus.hitBox.left = left;
			pStatus.hitBox.right = pStatus.hitBox.left + status.hitBoxWidth;

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

			pos.x = rightline - status.hitBoxWidth / 2;

			int right = rightline - CAMERAMANAGER->getCameraPos().x;
			pStatus.hitBox.right = right;
			pStatus.hitBox.left = pStatus.hitBox.right - status.hitBoxWidth;

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
			pos.y = topline - 2;

			int bottom = topline - CAMERAMANAGER->getCameraPos().y;
			pStatus.hitBox.bottom = bottom - 2;
			pStatus.hitBox.top = pStatus.hitBox.bottom - status.hitBoxHeight;

			pStatus.velocityY = 0.0f;

			if (pStatus.isJumpDash)
			{
				pStatus.isDash = false;
				pStatus.isJumpDash = false;
			}

			hideAfterimage = false;

			if (ladderDone)
			{
				pStatus.isOnLadder = false;
			}
		}
	}
	inline void setTopCollision(bool top, int bottomline)
	{
		pStatus.isOnTop = top;

		if (top == true)
		{
			pos.y = bottomline + 3 + status.hitBoxHeight;

			int top = bottomline - CAMERAMANAGER->getCameraPos().y;
			pStatus.hitBox.top = top + 3;
			pStatus.hitBox.bottom = pStatus.hitBox.top + status.hitBoxHeight;

			pStatus.velocityY = 0.0f;
			pStatus.velocityX = 0.0f;

			wallkickTimer = wallkickMaxTime;
		}
	}
	inline void setHideAfterimage(bool hide) { hideAfterimage = hide; }
	inline void setLadderAble(bool able, int x)
	{
		ladderAble = able;
		ladderPosX = x;
	}

	inline void setLadderEnd(bool able)
	{
		ladderEnd = able;
	}
	
	// 스탯 관련
	void reduceHp(int damage);

	inline void reduceMp(int damage) { status.mp -= damage; }
	inline void invincibleTimerUpdate()
	{
		if (status.overpower && !status.dead)
		{
			// 무적 시작 + 데미지 받는 중
			if (pStatus.invincible)
			{
				// 무적 시작 + 이동 가능
				pStatus.invincibleTimer += 0.1f;

				if (static_cast<int>(pStatus.invincibleTimer * 10) % 5 == 0)
				{
					charAlpha = (charAlpha >= 200) ? 50 : 200;
				}

				// 무적 시간늘리려면 pStatus.overpowerMaxTime 을 늘리면 되지요오
				if (pStatus.invincibleTimer >= pStatus.overpowerMaxTime)
				{
					charAlpha = 255;
					status.overpower = false;
					pStatus.invincible = false;
					inputEnabled = true;
					pStatus.invincibleTimer = 0.0f;
				}
			}
		}
	}

	void deathAnim();
	void chageVictory() { currentState = CharacterState::Victory; }

	virtual void colorSetting(void);
	virtual void colorChange(void);

	void changeAnimation(const string& animName, int frame);
	virtual ShootEvent makeShootEvent(BulletType bType);

	virtual void coolDownControl(void);
	virtual void multiHitControl(void);

	void ladderDown();

	
	bool completePoseDome(void);
};

