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

		float speed;

		bool rightDirect;
		bool isFire;

		int demage;
	};

public:
	BulletStatus bStatus;

public:

	virtual HRESULT init(void);
	virtual HRESULT init(BulletType type, int x, int y, bool isRight);
	virtual HRESULT init(EnemyBulletType type, int x, int y, bool isRight);
	virtual void release(void);
	virtual void update(void);
	virtual void render(void);

	RECT getBulletRect(void) { return bStatus.hitBox; }
	int getBulletDamage(void) { return bStatus.demage; }
	BulletSize getBulletSize(void) { return bStatus.type; }

	BulletType getBulletType(void) { return bStatus.bType; }
};


class Burster : public Bullet
{
private:

public:

	HRESULT init(void);
	HRESULT init(BulletType type, int x, int y, bool isRight);
	void release(void);
	void update(void);
	void render(void) override;

	Burster() { bStatus.isFire = false; }
	~Burster() { }
};


class JunkBullet : public Bullet
{
private:

public:

	HRESULT init(void);
	HRESULT init(EnemyBulletType type, int x, int y, bool isRight);
	void release(void);
	void update(void);
	void render(void) override;

	JunkBullet() { bStatus.isFire = false; }
	~JunkBullet() { }
};