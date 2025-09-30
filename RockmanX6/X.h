#pragma once
#include "Player.h"

class X : public Player
{
private:


public:
	HRESULT init(void);
	HRESULT init(int x, int y);
	void release(void);
	void update(void);
	void render(void);

	// 행동 관련
	void jump(void) override;
	void attack(void) override;
	void spawn(int x, int y) override;

	// 애니메이션 관련
	void currentAnimChange(void) override;

	void returnToIdle(void);
};

