#pragma once
#include "GameNode.h"
#include "BulletManager.h"

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
		Crouch,
		Land,
		Climb,
		Warp,
		WallSlide,
		WallKick,
		Dead,
		OverPower
	};

	// 캐릭터 메인 설정값
	struct PlayerStatus
	{
		// 판정 및 이미지
		GImage* player;
		RECT hitBox;

		string charName;
		int firePoint;

		// 스탯
		float hp;
		float maxHp;
		float mp;
		float maxMp;

		// 이동 스탯
		float speed;
		float jumpPower;
		
		// 상태값
		bool lookRight;
		bool isOnGround;

		bool touchLeft;
		bool touchRight;

		// 점프 관련
		float velocityY;
		float maxFallSpeed;
		float jumpAccel;
		float maxJumpHoldTime;
		bool jumpStack;

		float deltaTime = 0.016f;
};

	struct Progress
	{
		int life;
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
	int prevAniFrame;
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

public:
	virtual HRESULT init(void);
	virtual HRESULT init(int x, int y);
	virtual void release(void);
	virtual void update(void);
	virtual void render(void);

	// 캐릭터 공통 조작
	virtual void move(bool direction);
	virtual void jump(void);
	virtual void dash(void);
	virtual void attack(void);
	virtual void isJump(void);
	void wallSlide(void);
	void wallDrop(void);

	// 캐릭터 공통 상태값 조작
	void applyGravity(void);
	void sfxPlay(void);
	void setBulletManager(BulletManager* manager) { bManager = manager; };

	// 캐릭터 스폰
	virtual void spawn(int x, int y);

	virtual void currentAnimChange(void);

	// 상태값
	string printBodyState(void);
	string printAttState(void);

	// 애니메이션 관련
	virtual void setHitBox(void);

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
	
	// 상태값
	inline void setLeftCollision(bool left, int leftline) { pStatus.touchLeft = left; }
	inline void setRightCollision(bool right) { pStatus.touchRight = right; }
	inline void setIsOnGround(bool OnGround, int topline, int Bottom)
	{
		pStatus.isOnGround = OnGround;
		
		if (pStatus.isOnGround == true)
		{
			charPos.y = topline - 2;

			pStatus.hitBox.bottom = Bottom - 2;
			pStatus.hitBox.top = pStatus.hitBox.bottom - hitBoxHeight;

			pStatus.jumpStack = false;
			pStatus.velocityY = 0.0f;
		}
	}

	// 스탯 관련
	inline void reduceHp(int damage) { pStatus.hp -= damage; }
	inline void reduceMp(int damage) { pStatus.mp -= damage; }

	inline void setOverPower()
	{
		cout << "Test" << endl;
	}
};

