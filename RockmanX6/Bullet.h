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

		RECT hitBox;
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
	virtual void release(void);
	virtual void update(void);
	virtual void render(void);

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