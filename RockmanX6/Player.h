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
		Att,
		Saber,
		SpecialAtt,
		Warp,
		OverPower,
		Dead,
		Wall
	};

	// ĳ���� ���� ������
	struct PlayerStatus
	{
		GImage* player;
		RECT hitBox;

		string charName;
		float hp;
		float maxHp;
		float mp;
		float maxMp;
		float speed;
		float jumpSpeed;
		bool lookRight;
		bool isOnGround;

		bool touchLeft;
		bool touchRight;

		bool isAtt;

		int firePoint;
	};

	struct Progress
	{
		int life;
		// �Ƹ� ��Ʈ���� ������ ���, ����, ��, ǲ
		bitset<4> isArmd[2]; // �ƸӴ� �ϳ��� �����Ұ����� �迭 ũ�� 2�� �ּ� �ΰ� ����� ô!

		// ���� ��Ʈ�� ������ ȭ�� ���� ���������� �ð� ����
		bitset<4> clearBoss;
		float gravity;
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

	// ĳ���� ���� 
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

	// ���°� ����
	inline void setStageGravity(float gravityPower) { progress.gravity = gravityPower; }

	RECT getPlayerRect(void) { return pStatus.hitBox; }

	int getPlayerCenter(void) { return charPos.x; }

	int getPlayerLeft(void) { return charPos.x - hitBoxWidth / 2; }
	int getPlayerRight(void) { return charPos.x + hitBoxWidth /2; }

	int getPlayerTop(void) { return charPos.y - hitBoxHeight; }
	int getPlayerBottom(void) { return charPos.y; }

	inline void setLeftCollision(bool left) { pStatus.touchLeft = left; }
	inline void setRightCollision(bool right) { pStatus.touchRight = right; }
	inline void setIsOnGround(bool OnGround) { pStatus.isOnGround = OnGround; }

};

