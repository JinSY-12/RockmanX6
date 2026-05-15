#pragma once
#include "BulletType.h"
#include "CombatEntity.h"

#define BOSS_PATTERN_MAXLIST 5

class Player;

class BossBase : public CombatEntity
{

protected:
	enum class BossState
	{
		Apperance,
		Idle,
		Move,
		Return,
		Dodge,
		AttReady,
		RightReady,
		RightAtt,
		LeftReady,
		LeftAtt,
		DeathBallShoot,
		DeathBallShootIdle
	};

	enum class AnimDirection
	{
		Forward,
		Backward
	};

	struct FirePointOffset
	{
		int x;
		int y;
	};

	struct BossStatus
	{
		GImage* bImage;
		GImage* effectImage;

		RECT bHitBox;
		RECT effectRect;

		int originX;
		int originY;

		int offsetX;
		int offsetY;

		int effectOffsetX;
		int effectOffsetY;

		bool effectOn;
		bool effectCollisionOn;
		bool effectOnTop;
		bool doCutScene;

		bool movable;

		float invincibleTimer = 0.0f;
		float invincibleMaxTime;

		bool invincible = false;
	};

protected:
	Player* player;
	ShootEvent shootEvent;
	BossStatus bStatus;
	BossState bState;

	BossType btype;
	AnimDirection animDir;

	Vector2 diff;

	string BossBGM;
	bool musciStart;

	float animSpeed;

	bool isPattern;
	bool appearanceDone;
	int parrternList[BOSS_PATTERN_MAXLIST];

	float patternTimer;
	float pattenrCoolDown;
	bool phase2; // 2페이즈로 패턴 타이머와 애니메이션 속도 빠르게
	float timer;

	int bossAlpha;

	// test
	bool attCycle;

public:
	virtual void render(HDC hdc);

	// get/set 관련
	RECT getBossHitBox() { return bStatus.bHitBox; }
	RECT getBossSubHitBox() { return bStatus.effectRect; }

	bool getSubRectAttOn() { return bStatus.effectCollisionOn; }
	BossType getBossType() { return btype; }

	// 패턴 관련
	virtual void bossAppearance();
	virtual void setBossHitbox(void);
	virtual void offsetFix(void);

	Vector2 getDiffPlayer(int firePointX, int firePointY);

	inline void settingPlayer(Player* p) { player = p; }
	void makeShootEvent(BulletType bType);

	virtual void changeAnim(BossState bossState);
	virtual void readyPattern(void);

	// 유틸 관련
	bool timerClock(float time);

	// 상태 관련
	inline void reduceHp(int damage)
	{
		status.hp -= damage;

		if (damage < 4) bStatus.invincibleMaxTime = 8.0f;
		else bStatus.invincibleMaxTime = 14.0f;
		
		status.overpower = true;
	}

	void bossInvincibleTimerUpdate(void);
};

