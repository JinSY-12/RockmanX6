#pragma once
#include "GameNode.h"

class Player : public GameNode
{
private:

public:
	struct Pos
	{
		int x;
		int y;
	};

	enum class SholderState
	{
		None,
		Burst
	};

	// ĳ���� ���°�
	enum class CharacterState
	{
		Idle,
		Walk,
		JumpUp,
		JumpAtt,
		FallingDown,
		FallingAtt,
		Dash,
		Crouch,
		Land,
		Climb,
		SpecialAtt,
		Saber,
		JumpSaber,
		Warp,
		OverPower,
		Dead
	};

	// ĳ���� ���� ������
	struct PlayerStatus
	{
		GImage* player;
		RECT hitBox;

		int hp;
		int maxHp;
		int mp;
		int maxMp;
		bool lookRight;
		bool isOnGround;
		bool isAtt;

	};

	// ����ü �� enum ����
	PlayerStatus pStatus;
	CharacterState cState;
	Pos cPos;

	// ĳ���� ���� ���°�
	int hixWidth;
	int hixHeight;


	// �÷��̾� ���°�
	bool inputEnabled;

	struct Progress
	{
		int life;
		// �Ƹ� ��Ʈ���� ������ ���, ����, ��, ǲ
		bitset<4> isArmd[2]; // �ƸӴ� �ϳ��� �����Ұ����� �迭 ũ�� 2�� �ּ� �ΰ� ����� ô!
		
		// ���� ��Ʈ�� ������ ȭ�� ���� ���������� �ð� ����
		bitset<4> clearBoss;
		float gravity;
	}pro;
	

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
	virtual void spawn(int x, int y);

	// ���� �Լ�
	// ���°� ����
	inline void addGravity(float gravityPower) { pro.gravity = gravityPower; }
};

