#pragma once
#include "GameNode.h"
#include "BulletType.h"
#include "BulletManager.h"

class EnemyBase : public GameNode
{
private:

public:
	struct EnemyStatus
	{
		GImage* eImage;

		int maxHp;
		int hp;

		RECT eHitBox;
		RECT attSight;

		int width;
		int height;

		bool isAtt;
	};

	EnemyStatus eStatus;

	BulletManager* bManager;

	float patternTimer;
	float maxPatternTime;
	bool attackAble;

public:
	virtual HRESULT init(void);
	virtual HRESULT init(int x, int y);
	virtual void release(void);
	virtual void update(void);
	virtual void render(void);

	virtual void attack(void);

	virtual void pattern(void);

	RECT getEnemySight() { return eStatus.attSight; }
	RECT getEnemyHitBox() { return eStatus.eHitBox; }

	inline void reduceEnemyHp(int damage) { eStatus.hp -= damage; }

	bool getAttAble(void) { return attackAble; }
};

