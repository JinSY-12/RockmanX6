#pragma once
#include "GameNode.h"
#include "BulletType.h"

class BulletManager;
class Player;

class EnemyBase : public GameNode
{
private:

public:
	enum class EnemyState
	{
		Idle,
		Attack
	};

	struct EnemyPos
	{
		int x;
		int y;
	};

	struct FirePointOffset
	{
		int x;
		int y;
	};

	struct EnemyStatus
	{
		GImage* eImage;

		int maxHp;
		int hp;

		RECT eHitBox;
		RECT attSight;

		int width;
		int height;

		int sightWidth;
		int sightHeight;

		bool lookRight;
		bool isAtt;
		bool overpower;
		bool dead;
	};

	EnemyStatus eStatus;
	EnemyState eState;
	EnemyPos ePos;
	FirePointOffset fPos;

	EnemyType eType;

	BulletManager* bManager;

	float patternTimer;
	float maxPatternTime;
	bool attackAble;

	Player* player;

public:
	virtual HRESULT init(void);
	virtual HRESULT init(int x, int y);
	virtual void release(void);
	virtual void update(void);
	virtual void render(void);

	virtual void attack(void);

	virtual void setEnemyHitbox(void);
	virtual void pattern(void);

	RECT getEnemySight() { return eStatus.attSight; }
	RECT getEnemyHitBox() { return eStatus.eHitBox; }

	inline void reduceEnemyHp(int damage) { eStatus.hp -= damage; }

	inline void settingBulletManager(BulletManager* bullet) { bManager = bullet; }
	inline bool getOverPower() { return eStatus.overpower; }
	inline void reduceHp(int damage)
	{
		eStatus.hp -= damage;
		eStatus.overpower = true;
	}
	inline void changeDirection(bool dir) { eStatus.lookRight = dir; }

	void chasePlayer(float angle);

	int getCurrentHp(void) { return eStatus.hp; }

	bool getAttAble(void) { return attackAble; }
	bool getIsDead(void) { return eStatus.dead; }

	void isDead(void);

	EnemyPos getEnemyPos() { return ePos; }
	bool getEnemyLook() { return eStatus.lookRight; }

	EnemyType getEnemyType() { return eType; }

	void changeDirection(void);

	void settingPlayer(Player* p) { player = p; }
};

