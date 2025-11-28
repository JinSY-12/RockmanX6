#pragma once

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

	Position getPos() { return pos; }
	int getWidth() { return status.width; }
	int getHeight() { return status.height; }
	bool getLookRight() { return status.lookRight; }
	bool getOverPower() { return status.overpower; }

	float getPhyscialDamage() { return status.physicalDamage; }
	CombatEntityType getEntityType() const { return status.type; }
};

