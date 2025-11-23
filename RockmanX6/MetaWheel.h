#pragma once
#include "EnemyBase.h"

class MetaWheel : public EnemyBase
{
private:

public:
	HRESULT init(void) override;
	HRESULT init(int x, int y) override;
	void release(void) override;
	void update(void) override;

	void animChange();

	void attack(void) override;

	void checkPlayerCollision(void)	override;
};

