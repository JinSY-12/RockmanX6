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
		// 아머 비트셋의 순서는 헤드, 보디, 암, 풋
		bitset<4> isArmd[2]; // 아머는 하나만 구현할거지만 배열 크기 2로 둬서 두개 만드는 척!
		
		// 보스 비트셋 순서는 화면 왼쪽 위에서부터 시계 방향
		bitset<4> clearBoss;
		float gravity;
	}pro;
	

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

	// 캐릭터 스폰
	virtual void spawn(int x, int y);

	// 간단 함수
	// 상태값 관련
	inline void addGravity(float gravityPower) { pro.gravity = gravityPower; }
};

