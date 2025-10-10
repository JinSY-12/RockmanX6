#pragma once
#include "Player.h"
#include "BulletManager.h"

class X : public Player
{
private:

	float chargeCount;
	float chargeSpeed;

	string burstSound;
	
	// ������ ���� �ӵ� ����
	bool normalBurstAble;
	bool chargeBurstDelay;

	// ������ ����
	float chargeBurstCount;
	bool isCharging;

	GImage* chargeEffect;

public:
	HRESULT init(void);
	HRESULT init(int x, int y);
	void release(void);
	void update(void);
	void render(void);

	// �ൿ ����
	void jump(void) override;
	void dash(bool direction) override;
	void attack(void) override;
	void chargeBurst(void);
	void spawn(int x, int y) override;
	
	// �ִϸ��̼� ����
	void currentAnimChange(void) override;
	void frameCheck(void);
	void setHitBox(void) override;
	
	// ��ɿ� �Լ�
	void returnToIdle(void);
};

