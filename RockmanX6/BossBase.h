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

		bool overPower;

		int originX;
		int originY;

		int offsetX;
		int offsetY;

		int effectOffsetX;
		int effectOffsetY;

		bool effectOn;
		bool effectOnTop;
		bool doCutScene;

		bool movable;
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

	// test
	bool attCycle;

public:
	virtual void render(HDC hdc);

	virtual void bossAppearance();
	virtual void setBossHitbox(void);
	virtual void offsetFix(void);

	Vector2 getDiffPlayer(int firePointX, int firePointY);

	inline void settingPlayer(Player* p) { player = p; }
	void makeShootEvent(BulletType bType);

	virtual void changeAnim(BossState bossState);
	virtual void readyPattern(void);

	bool timerClock(float time);
};

