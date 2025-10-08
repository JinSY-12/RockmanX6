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

	// ĳ���� ���°�
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

	// ĳ���� ���� ������
	struct PlayerStatus
	{
		// ���� �� �̹���
		GImage* player;
		RECT hitBox;

		string charName;
		int firePoint;

		// ����
		float hp;
		float maxHp;
		float mp;
		float maxMp;

		// �̵� ����
		float speed;
		float jumpPower;
		
		// ���°�
		bool lookRight;
		bool isOnGround;

		bool touchLeft;
		bool touchRight;

		// ���� ����
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
		// �Ƹ� ��Ʈ���� ������ ���, ����, ��, ǲ
		bitset<4> isArmd[2]; // �ƸӴ� �ϳ��� �����Ұ����� �迭 ũ�� 2�� �ּ� �ΰ� ����� ô!

		// ���� ��Ʈ�� ������ ȭ�� ���� ���������� �ð� ����
		bitset<4> clearBoss;
		float gravityAccel;
	};

#pragma endregion

	// ĳ���� �ൿ �� ����
	PlayerStatus pStatus;
	CharacterState previousState;
	CharacterState currentState;
	SholderState attState;

	Progress progress;

	// ĳ���� ���� �� ��ǥ
	int hitBoxWidth;
	int hitBoxHeight;

	CharcterPos hitBoxCenter;
	CharcterPos charPos;
	Anim animBaseline;
	BursterPos busterPos;

	// �÷��̾� ���°�
	CharacterType charType;
	ArmorType armorType;

	bool inputEnabled;
	bool isMoving;

	// �ִϸ��̼� ����
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

	// ���� ����
	string soundResult;

	BulletManager* bManager;

	// Ű �Է� ����
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

	// ĳ���� ���� ����
	virtual void move(bool direction);
	virtual void jump(void);
	virtual void dash(void);
	virtual void attack(void);
	virtual void isJump(void);
	void wallSlide(void);
	void wallDrop(void);

	// ĳ���� ���� ���°� ����
	void applyGravity(void);
	void sfxPlay(void);
	void setBulletManager(BulletManager* manager) { bManager = manager; };

	// ĳ���� ����
	virtual void spawn(int x, int y);

	virtual void currentAnimChange(void);

	// ���°�
	string printBodyState(void);
	string printAttState(void);

	// �ִϸ��̼� ����
	virtual void setHitBox(void);

	// ���� ����
	inline void setStageGravity(float gravityPower) { progress.gravityAccel = gravityPower; }

	// settter/getter
	// ��ǥ �� ����
	RECT getPlayerRect(void) { return pStatus.hitBox; }
	
	inline int getPlayerCenter(void) { return charPos.x; }
	inline int getPlayerTop(void) { return charPos.y - hitBoxHeight; }
	inline int getPlayerBottom(void) { return charPos.y; }
	inline int getPlayerLeft(void) { return charPos.x - hitBoxWidth / 2; }
	inline int getPlayerRight(void) { return charPos.x + hitBoxWidth /2; }
	
	// ���°�
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

	// ���� ����
	inline void reduceHp(int damage) { pStatus.hp -= damage; }
	inline void reduceMp(int damage) { pStatus.mp -= damage; }

	inline void setOverPower()
	{
		cout << "Test" << endl;
	}
};

