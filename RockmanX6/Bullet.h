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
		bool bulletEffect;

		string soundName;

		struct Pos
		{
			float x;
			float y;
		}pos;
		
		int width;
		int height;

		float velocityX;
		float velocityY;

		bool rightDirect;
		bool isFire;
		bool fireStart = true;
		bool pierce;
		float animSpeed;
		float bulletSpeed;

		bool wallTouch = false;

		int animOffsetX = 0;
		int animOffsetY = 0;

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
	bool getIsEffect(void) { return bStatus.bulletEffect; }

	bool getBulletDir(void) { return bStatus.rightDirect; }
	void setBulletFire(bool fire) { bStatus.isFire = fire; }

	bool getWallTouch(void) { return bStatus.wallTouch; }
	void setWallTouch(bool touch) { bStatus.wallTouch = touch; }

	BulletType getBulletType(void) { return bStatus.bType; }
};


class Burster : public Bullet
{
private:

public:

	HRESULT init(BulletType type, int x, int y, bool isRight, float velocityX, float velocityY) override;

	Burster()
	{
		bStatus.isFire = false;
		bStatus.animSpeed = 0.05f;
		bStatus.bulletSpeed = 25.0f;
		bStatus.bulletEffect = false;
	}
	~Burster() { }
};


class JunkBullet : public Bullet
{
private:


public:
	HRESULT init(BulletType type, int x, int y, bool isRight, float velocityX, float velocityY) override;

	JunkBullet()
	{
		bStatus.isFire = false;
		bStatus.animSpeed = 0.05f;
		bStatus.bulletSpeed = 5.0f;
		bStatus.bulletEffect = false;
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

	SiegeShoot()
	{
		bStatus.isFire = false;
		bStatus.animSpeed = 0.1f;
		bStatus.bulletSpeed = 20.0f;
		bStatus.bulletEffect = false;
	}
	~SiegeShoot() {}
};

class DeathBall : public Bullet
{

private:
	int fireStartPointX;
	int fireStartPointY;

public:
	HRESULT init(BulletType type, int x, int y, bool isRight, float velocityX, float velocityY) override;
	void update(void) override;

	DeathBall()
	{
		bStatus.isFire = false;
		bStatus.animSpeed = 0.04f;
		bStatus.bulletSpeed = 10.0f;
		bStatus.bulletEffect = false;
	}
	~DeathBall() {}
};

class DeathBubble : public Bullet
{
public:
	HRESULT init(BulletType type, int x, int y, bool isRight, float velocityX, float velocityY) override;
	void update(void) override;

	DeathBubble()
	{
		bStatus.isFire = false;
		bStatus.animSpeed = 0.1f;
		bStatus.bulletSpeed = 8.0f;
		bStatus.bulletEffect = true;
	}
	~DeathBubble() {}
};

class DeathRing : public Bullet
{
public:
	HRESULT init(BulletType type, int x, int y, bool isRight, float velocityX, float velocityY) override;
	void update(void) override;

	DeathRing()
	{
		bStatus.isFire = false;
		bStatus.animSpeed = 0.1f;
		bStatus.bulletSpeed = 4.0f;
		bStatus.bulletEffect = true;
	}
	~DeathRing() {}
};