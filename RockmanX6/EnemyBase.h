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
		RECT worldRect;
		RECT attSight;

		int width;
		int height;

		int sightWidth;
		int sightHeight;

		bool lookRight;
		bool isOnAttack;
		bool dead;

		float patternTimer;
		float maxPatternTime;
		bool attackAble;

		float invincibleTimer;
		float invincibleMaxTime;
		bool overpower;

		bool touchLeftWall;
		bool touchRightWall;
		
		float VelocityX;
		float VelocityY;

		float moveSpeed;
		float updownSpeed;
	};

	EnemyState eState;
	EnemyStatus eStatus;
	EnemyPos ePos;
	FirePointOffset fPos;

	EnemyType eType;

	BulletManager* bManager;
	Player* player;

public:
	virtual void render(void);
	virtual void attack(void);
	
	// 행동 관련
	virtual void setEnemyHitbox(void);
	virtual void pattern(void);

	// 상태 관리
	void isDead(void);
	inline void reduceHp(int damage)
	{
		eStatus.hp -= damage;
		eStatus.overpower = true;
	}
	inline void changeDirection(bool dir) { eStatus.lookRight = dir; }
	void chasePlayer(float angle);
	void changeDirection(void);

	// getter, setter
	RECT getEnemySight() { return eStatus.attSight; }
	RECT getEnemyHitBox() { return eStatus.eHitBox; }
	RECT getEnemyWorldRect() { return eStatus.worldRect; }

	inline int getEnemyWidth(void) { return eStatus.width; }
	inline int getEnemyHeight(void) { return eStatus.height; }
	int getCurrentHp(void) { return eStatus.hp; }
	bool getAttAble(void) { return eStatus.attackAble; }
	bool getIsDead(void) { return eStatus.dead; }
	inline bool getOverPower() { return eStatus.overpower; }
	EnemyPos getEnemyPos() { return ePos; }
	bool getEnemyLook() { return eStatus.lookRight; }
	EnemyType getEnemyType() { return eType; }

	void setEnemyLeftTouch(bool touch) { eStatus.touchLeftWall = touch; }
	void setEnemyRightTouch(bool touch) { eStatus.touchRightWall = touch; }

	// 공격, 피격 관련
	inline void settingBulletManager(BulletManager* bullet) { bManager = bullet; }
	inline void settingPlayer(Player* p) { player = p; }

	virtual void checkPlayerCollision(void);
	virtual void checkPlayerAttCollision(void);
	virtual void checkBulletCollision(void);

	void enemyInvincibleTimerUpdate(void);
};

