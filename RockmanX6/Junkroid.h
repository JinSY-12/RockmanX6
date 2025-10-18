#pragma once
#include "EnemyBase.h"

class Junkroid : public EnemyBase
{
private:

public:

	HRESULT init(void) override;
	HRESULT init(int x, int y) override;
	void release(void) override;
	void update(void) override;
	// void render(void) override;

	void animChange();

	void attack(void) override;
};

