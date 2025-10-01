#pragma once
#include "GameNode.h"

class Player : public GameNode
{
private:

public:

#pragma region PlayerStruct
	struct HixBoX
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
		int hp;
		int maxHp;
		int mp;
		int maxMp;
		bool lookRight;
		bool isOnGround;
		bool isAtt;
	};

	struct Progress
	{
		int life;
		// �Ƹ� ��Ʈ���� ������ ���, ����, ��, ǲ
		bitset<4> isArmd[2]; // �ƸӴ� �ϳ��� �����Ұ����� �迭 ũ�� 2�� �ּ� �ΰ� ����� ô!

		// ���� ��Ʈ�� ������ ȭ�� ���� ���������� �ð� ����
		bitset<4> clearBoss;
		float gravity;
	}pro;

#pragma endregion

	// ĳ���� �ൿ �� ����
	PlayerStatus pStatus;
	CharacterState previousState;
	CharacterState currentState;
	SholderState attState;

	// ĳ���� ���� �� ��ǥ
	int hixWidth;
	int hixHeight;

	HixBoX hitBoxCenter;
	Anim animBaseline;
	BursterPos busterPos;

	// �÷��̾� ���°�
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

	void sfxPlay(void);

	// ĳ���� ����
	virtual void spawn(int x, int y);

	virtual void currentAnimChange(void);

	// ���°�
	string printBodyState(void);
	string printAttState(void);

	// ���� �Լ�
	// ���°� ����
	inline void addGravity(float gravityPower) { pro.gravity = gravityPower; }


};

