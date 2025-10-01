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
		Att,
		Saber,
		SpecialAtt,
		Warp,
		OverPower,
		Dead,
		Wall
	};

	// 캐릭터 메인 설정값
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
		// 아머 비트셋의 순서는 헤드, 보디, 암, 풋
		bitset<4> isArmd[2]; // 아머는 하나만 구현할거지만 배열 크기 2로 둬서 두개 만드는 척!

		// 보스 비트셋 순서는 화면 왼쪽 위에서부터 시계 방향
		bitset<4> clearBoss;
		float gravity;
	}pro;

#pragma endregion

	// 캐릭터 행동 및 스탯
	PlayerStatus pStatus;
	CharacterState previousState;
	CharacterState currentState;
	SholderState attState;

	// 캐릭터 판정 및 좌표
	int hixWidth;
	int hixHeight;

	HixBoX hitBoxCenter;
	Anim animBaseline;
	BursterPos busterPos;

	// 플레이어 상태값
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

	void sfxPlay(void);

	// 캐릭터 스폰
	virtual void spawn(int x, int y);

	virtual void currentAnimChange(void);

	// 상태값
	string printBodyState(void);
	string printAttState(void);

	// 간단 함수
	// 상태값 관련
	inline void addGravity(float gravityPower) { pro.gravity = gravityPower; }


};

