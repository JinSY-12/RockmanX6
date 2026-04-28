#pragma once
#include "BulletType.h"
#include "CombatEntity.h"

class Player;

class EnemyBase : public CombatEntity
{
private:

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

	struct EnemyStatus
	{
		GImage* eImage;
		GImage* ePartsImage;

		RECT eHitBox;
		RECT worldRect;
		RECT attSight;
		RECT subHitBox;

		int sightWidth;
		int sightHeight;

		bool isOnAttack;

		float patternTimer;
		float maxPatternTime;
		bool attackAble;

		float invincibleTimer;
		float invincibleMaxTime;

		bool touchLeftWall;
		bool touchRightWall;
		
		float VelocityX;
		float VelocityY;

		float moveSpeed;
		float updownSpeed;

		int animOffsetX;
		int animOffsetY;

		int subOffsetX;
		int subOffsetY;
	};

	EnemyState eState;
	EnemyStatus eStatus;
	FirePointOffset fPos;
	
	EnemyType eType;

	Player* player;

	ShootEvent shootEvent;

	Vector2 diff;

public:
	virtual void release(void);
	virtual void update(void);
	virtual void render(HDC hdc);
	virtual void attack(void);
	
	// 행동 관련
	virtual void setEnemyHitbox(void);
	virtual void pattern(void);

	// 상태 관리
	void isDead(void);
	inline void reduceHp(int damage)
	{
		status.hp -= damage;
		status.overpower = true;
	}

	inline void changeDirection(bool dir) { status.lookRight = dir; }
	void chasePlayer(float angle);
	void changeDirection(void);

	Vector2 getDiffPlayer(void);

	// getter, setter
	RECT getEnemySight() { return eStatus.attSight; }
	RECT getEnemyHitBox() { return eStatus.eHitBox; }
	RECT getEnemyWorldRect() { return eStatus.worldRect; }
	RECT getEnemysubHitBox() { return eStatus.subHitBox; }

	bool getAttAble(void) { return eStatus.attackAble; }
	
	Position getEnemyPos() { return pos; }
	EnemyType getEnemyType() { return eType; }

	void setEnemyLeftTouch(bool touch) { eStatus.touchLeftWall = touch; }
	void setEnemyRightTouch(bool touch) { eStatus.touchRightWall = touch; }

	// 공격, 피격 관련
	inline void settingPlayer(Player* p) { player = p; }

	virtual void checkPlayerCollision(void);
	virtual void checkBulletCollision(void);

	void enemyInvincibleTimerUpdate(void);

	virtual ShootEvent makeShootEvent(BulletType bType);

};