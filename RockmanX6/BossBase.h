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
		RECT bWorldRect;
		RECT effectRect;

		bool overPower;

		int offsetX;
		int offsetY;

		bool effectOn;
	};


protected:
	Player* player;
	ShootEvent shootEvent;
	BossStatus bStatus;
	BossState bState;

	BossType btype;
	AnimDirection animDir;

	Vector2 diff;

	float animSpeed;

	bool isPattern;
	int parrternList[BOSS_PATTERN_MAXLIST];

public:
	virtual void render(HDC hdc);

	virtual void bossAppearance();
	virtual void setBossHitbox(void);

	Vector2 getDiffPlayer(int firePointX, int firePointY);

	inline void settingPlayer(Player* p) { player = p; }
	void makeShootEvent(BulletType bType);
};

