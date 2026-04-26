#pragma once
#include "EnemyBase.h"

class MetaDridler : public EnemyBase
{
private:
	enum class DridlerState
	{
		Idle,
		Down,
		DownStop,
		Up,
		UpStop
	}dState;

	int prevLoacationY;
	int destinationY;

	float downSpeed;
	float maxDownSpeed;
	float upSpeed;

	float raadyTime;

public:
	// HRESULT init(void) override;
	// HRESULT init(int x, int y) override;
	HRESULT init(int x, int y, int locationX, int locationY) override;
	void release(void) override;
	void update(void) override;

	void downAtt(void);
	void upReady(void);

	void setEnemyHitbox(void) override;

	void attack(void) override;

	void checkPlayerCollision(void)	override;
};

