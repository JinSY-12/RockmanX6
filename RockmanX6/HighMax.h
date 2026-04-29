#pragma once
#include "BossBase.h"

class HighMax : public BossBase
{
private:

public:
	HRESULT init(int x, int y) override;
	HRESULT init(int x, int y, int locationX, int locationY) override;
	HRESULT init(int x, int y, int width, int top, int height) override;

	void update(void) override;

	void bossAppearance(void) override;
	void siegeShoot(void);
	void deathBall(void);
	// 패턴 1번 - 총알을 캐릭터를 향해 4회씩 3세트 발사
	void patternShoot(void);

	ShootEvent makeShootEvent(BulletType bType) override;
};

