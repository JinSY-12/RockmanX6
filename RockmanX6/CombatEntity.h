#pragma once
#include <Windows.h>

enum class CombatEntityType
{
	Player,
	Enemy,
	Object
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

public:
	virtual HRESULT init(void);
	virtual HRESULT init(int x, int y);
	virtual HRESULT init(int x, int y, int locationX, int locationY);

	virtual void release(void);
	virtual void update(void);
	virtual void render(HDC memDC);

	Position getPos() { return pos; }
	int getWidth() { return status.width; }
	int getHeight() { return status.height; }
	bool getLookRight() { return status.lookRight; }
	bool getOverPower() { return status.overpower; }
	int getCurrentHp() { return status.hp; }

	bool getIsDead() { return status.dead; }

	float getPhyscialDamage() { return status.physicalDamage; }
	CombatEntityType getEntityType() const { return status.type; }
};

