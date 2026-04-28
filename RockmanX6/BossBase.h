#pragma once
#include "BulletType.h"
#include "CombatEntity.h"

#define BOSS_PATTERN_MAXLIST 5

class Player;

class BossBase : public CombatEntity
{

protected:
	enum class EnemyState
	{
		Idle,
		Attack
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
	};

protected:
	Player* player;
	ShootEvent shootEvent;

	bool isPattern;
	int parrternList[BOSS_PATTERN_MAXLIST];

public:
	virtual void bossAppearance();

	inline void settingPlayer(Player* p) { player = p; }
	virtual ShootEvent makeShootEvent(BulletType bType);
};

