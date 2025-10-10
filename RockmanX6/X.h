#pragma once
#include "Player.h"
#include "BulletManager.h"

class X : public Player
{
private:

	float chargeCount;
	float chargeSpeed;

	string burstSound;
	
	// 버스터 연사 속도 조절
	bool normalBurstAble;
	bool chargeBurstDelay;

	// 차지샷 관련
	float chargeBurstCount;
	bool isCharging;

	GImage* chargeEffect;

public:
	HRESULT init(void);
	HRESULT init(int x, int y);
	void release(void);
	void update(void);
	void render(void);

	// 행동 관련
	void jump(void) override;
	void dash(bool direction) override;
	void attack(void) override;
	void chargeBurst(void);
	void spawn(int x, int y) override;
	
	// 애니메이션 관련
	void currentAnimChange(void) override;
	void frameCheck(void);
	void setHitBox(void) override;
	
	// 기능용 함수
	void returnToIdle(void);
};

