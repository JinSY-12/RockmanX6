#pragma once
#include "GameNode.h"
#include "BulletType.h"
#include "CombatEntity.h"

class BulletManager;
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
	/*
	struct EnemyPos
	{
		int x;
		int y;
	};
	*/
	struct FirePointOffset
	{
		int x;
		int y;
	};

	struct EnemyStatus
	{
		GImage* eImage;

		RECT eHitBox;
		RECT worldRect;
		RECT attSight;

		// int width;
		// int height;

		int sightWidth;
		int sightHeight;

		// bool lookRight;
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
	};

	EnemyState eState;
	EnemyStatus eStatus;
	// EnemyPos ePos;
	FirePointOffset fPos;
	
	EnemyType eType;

	BulletManager* bManager;
	Player* player;

public:
	virtual HRESULT init(void);
	virtual HRESULT init(int x, int y);
	virtual void release(void);
	virtual void update(void);
	virtual void render(HDC hdc);
	virtual void attack(void);
	
	// 행동 관련
	virtual void setEnemyHitbox(void);
	virtual void pattern(void);

	// 상태 관리
	void isDead(void);
	inline void freduceHp(int damage)
	{
		status.hp -= damage;
		status.overpower = true;
	}

	inline void changeDirection(bool dir) { status.lookRight = dir; }
	void chasePlayer(float angle);
	void changeDirection(void);

	// getter, setter
	RECT getEnemySight() { return eStatus.attSight; }
	RECT getEnemyHitBox() { return eStatus.eHitBox; }
	RECT getEnemyWorldRect() { return eStatus.worldRect; }

	// inline int getEnemyWidth(void) { return status.width; }
	// inline int getEnemyHeight(void) { return status.height; }
	int getCurrentHp(void) { return status.hp; }
	bool getAttAble(void) { return eStatus.attackAble; }
	bool getIsDead(void) { return status.dead; }
	// inline bool getOverPower() { return status.overpower; }
	Position getEnemyPos() { return pos; }
	// bool getEnemyLook() { return status.lookRight; }
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

