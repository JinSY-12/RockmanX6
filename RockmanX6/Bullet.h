#pragma once
#include "GameNode.h"
#include "BulletType.h"

class Bullet : public GameNode
{
private:

public:
	

	struct BulletStatus
	{
		GImage* shape;
		BulletFaction faction;
		BulletSize type;
		RECT hitBox;
		BulletType bType;

		struct Pos
		{
			int x;
			int y;
		}pos;
		
		int width;
		int height;

		float velocityX;
		float velocityY;

		bool rightDirect;
		bool isFire;
		bool fireStart = true;
		bool pierce;

		float bulletSpeed;

		int demage;
	};

public:
	BulletStatus bStatus;

public:
;
	virtual HRESULT init(BulletType type, int x, int y, bool isRight, float velocityX = 0.0f, float velocityY = 0.0f);
	virtual void release(void);
	virtual void update(void);
	virtual void render(void);

	RECT getBulletRect(void) { return bStatus.hitBox; }
	int getBulletDamage(void) { return bStatus.demage; }
	BulletSize getBulletSize(void) { return bStatus.type; }

	int getBulletPosX(void) { return bStatus.pos.x; }
	int getBulletPosY(void) { return bStatus.pos.y; }

	int getBulletWidth(void) { return bStatus.width; }
	int getBulletHeight(void) { return bStatus.height; }
	
	BulletFaction getBulletFaction(void) { return bStatus.faction; }

	bool getBulletDir(void) { return bStatus.rightDirect; }
	void setBulletFire(bool fire) { bStatus.isFire = fire; }

	BulletType getBulletType(void) { return bStatus.bType; }
};


class Burster : public Bullet
{
private:

public:

	HRESULT init(BulletType type, int x, int y, bool isRight, float velocityX, float velocityY) override;

	Burster() {
		bStatus.isFire = false;
		bStatus.bulletSpeed = 25.0f;
	}
	~Burster() { }
};


class JunkBullet : public Bullet
{
private:


public:
	HRESULT init(BulletType type, int x, int y, bool isRight, float velocityX, float velocityY) override;

	JunkBullet() {
		bStatus.isFire = false;
		bStatus.bulletSpeed = 5.0f;
	}
	~JunkBullet() { }
};


class SiegeShoot : public Bullet
{
private:
	bool fireStart;

public:

	HRESULT init(BulletType type, int x, int y, bool isRight, float velocityX, float velocityY) override;
	void update(void) override;

	SiegeShoot() {
		bStatus.isFire = false;
		bStatus.bulletSpeed = 10.0f;
	}
	~SiegeShoot() {}
};