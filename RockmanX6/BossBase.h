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
		RightReady,
		RightAtt,
		LeftReady,
		LeftAtt,
		DeathBallAtt1
	};

	struct FirePointOffset
	{
		int x;
		int y;
	};

	struct BossStatus
	{
		GImage* bImage;

		RECT bHitBox;
		RECT bWorldRect;

		bool overPower;
	};

protected:
	Player* player;
	ShootEvent shootEvent;
	BossStatus bStatus;

	BossType btype;

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
	virtual ShootEvent makeShootEvent(BulletType bType);
};

