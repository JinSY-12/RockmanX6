#pragma once
#include "Player.h"

class X : public Player
{
private:
	

	string burstSound;
	
	// 버스터 연사 속도 조절
	bool normalBurstAble;
	bool chargeBurstDelay;

	// 컷씬 확인용 변수
	bool cameraMoveDone;

public:
	HRESULT init(int x, int y) override;
	void release(void);
	void update(void);

	// 행동 관련
	void jump(void) override;
	void dash(bool direction) override;
	void attack(void) override;
	void chargeBurst(void);
	void specialAttack(void) override;

	// 이벤트 관련
	ShootEvent makeShootEvent(BulletType bType) override;

	void spawn(int x, int y) override;

	// 애니메이션 관련
	void frameCheck(void);
	void setHitBox(void) override;
	
	// 이미지 컬러 변경
	void colorSetting(void) override;
	void colorChange(void) override;
	inline void bursterEffectAlphaDown(void)
	{
		attackHandEffect = IMAGEMANAGER->findImage(bursterEffectName);

		if (attackHandEffect != nullptr)
			if (attackHandEffect->getFrameX() >= attackHandEffect->getMaxFrameX()) bursterEffectAlpha = 0;
	}

	// 상태 조절 관련
	void coolDownControl(void) override;
	void multiHitControl(void) override;

	// 기능용 함수
	void returnToIdle(void);

	void appearnaceEvent(void) override;
};

