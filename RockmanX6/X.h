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

	// �ൿ ����
	void jump(void) override;
	void attack(void) override;
	void spawn(int x, int y) override;

	// �ִϸ��̼� ����
	void currentAnimChange(void) override;

	void returnToIdle(void);
};

