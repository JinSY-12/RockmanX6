#pragma once
#include <Windows.h>

class BulletManager;

enum class CombatEntityType
{
	Player,
	Enemy,
	Object,
	Boss
};

class CombatEntity
{
protected:
	struct Position
	{
		int x;
		int y;
	};

	struct Attributes
	{
		CombatEntityType type;

		float hp;
		float maxHp;
		float mp;
		float maxMp;
		float physicalDamage;

		bool dead;
		bool overpower;
		bool lookRight;

		int width;
		int height;

		int hitBoxWidth;
		int hitBoxHeight;
	};

protected:
	Attributes status;

	Position pos;
	Position hitBoxCenter;

	BulletManager* bManager;


public:
	virtual HRESULT init(void);
	virtual HRESULT init(int x, int y);
	virtual HRESULT init(int x, int y, int locationX, int locationY);
	virtual HRESULT init(int x, int y, int width, int top, int height);
	virtual HRESULT init(int x, int y, int width, int top, int height, bool bossRoom);

	virtual void release(void);
	virtual void update(void);
	virtual void render(HDC memDC);

	Position getPos() { return pos; }
	int getCenterX() { return pos.x + (status.width / 2); }
	int getCenterY() { return pos.y - (status.height / 2); }
	int getWidth() { return status.width; }
	int getHeight() { return status.height; }
	bool getLookRight() { return status.lookRight; }
	bool getOverPower() { return status.overpower; }
	int getCurrentHp() { return status.hp; }

	inline void reduceEnemyHp(int damage)
	{
		status.hp -= damage;
		status.overpower = true;
	}

	bool getIsDead() { return status.dead; }

	float getPhyscialDamage() { return status.physicalDamage; }
	CombatEntityType getEntityType() const { return status.type; }
	
	void settingBulletManager(BulletManager* bullet) { bManager = bullet; }

	inline float lerp(float start, float end, float time)
	{
		return start + (end - start) * time;
	}
};

