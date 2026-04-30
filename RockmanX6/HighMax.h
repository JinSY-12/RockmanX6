#pragma once
#include "BossBase.h"

class HighMax : public BossBase
{
private:
	// 공격 체크용 변수
	int attTimes;
	bool SiegeSecondAtt;

	// 애니메이션 관련 변수
	int prevFrame;

	// 패턴 만들기전 테스트용 변수
	bool patternTest;

	float timer;

private:
	

public:
	HRESULT init(int x, int y) override;

	void update(void) override;

	void bossAppearance(void) override;

	// 패턴 1번 - 총알을 캐릭터를 향해 4회씩 3세트 발사
	void siegeShoot(void);

	// 패턴 2번 - 벽을 타며 움직이는 데스볼 발사
	void deathBall(void);
	
	void patternShoot(void);

	void stateReset(void);
};

