#pragma once

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

	void reduceHp(int damage);
	Position getPos() { return pos; }
	int getWidth() { return status.width; }
	int getHeight() { return status.height; }
	bool getLookRight() { return status.lookRight; }
	bool getOverPower() { return status.overpower; }

	float getPhyscialDamage() { return status.physicalDamage; }
};

