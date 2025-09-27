#pragma once
#include "GameNode.h"

class Player : public GameNode
{
private:

public:
	enum class SholderState
	{
		None,
		Burst
	};

	enum class BodyState
	{
		Idle,
		Walk,
		JumpUp,
		FallingDown,
		Dash,
		Crouch,
		Land,
		Climb,
		SpecialAtt,
		Saber,
		JumpSaber	
	};

	struct Pos
	{
		int x;
		int y;
	}charPos;

	struct Status
	{
		RECT hitBox;

		int hp;
		int mp;
		bool lookRight;
		bool isOnGround;
		bool isAtt;

		
	}stat;

	int width;
	int height;

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

