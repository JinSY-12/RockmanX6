#pragma once
#include "BossBase.h"

class HighMax : public BossBase
{
private:
	enum class AttPattern
	{
		Idle,
		SiegeShoot,
		DeathBallShoot,
		DeathRush,
		Dodge
	};

	enum class EffectState
	{
		BallCharge,
		RushCharge,
		RushBall
	};

	// 공격 체크용 변수
	int attTimes;
	bool SiegeSecondAtt;
	bool rushStart;

	// 애니메이션 관련 변수
	int prevFrame;

	// 패턴 만들기전 테스트용 변수
	bool patternTest;
	bool patternTest2;

private:
	AttPattern attPattern;
	EffectState effPattern;

public:
	HRESULT init(int x, int y) override;

	void update(void) override;

	// 패턴 0번 - 보스 등장
	// 패턴 1번 - 총알을 캐릭터를 향해 4회씩 3세트 발사
	// 패턴 2번 - 벽을 타며 움직이는 데스볼 발사
	void bossAppearance(void) override;
	void dodge();
	void siegeShoot(void);
	void deathBall(void);
	void deathRush(void);

	void readyPattern(void) override;
	
	// 연출 기능
	void movetoPoint();
	void changeAnim(BossState bossState) override;

	// 상태 기능
	void offsetFix(void) override;
	void stateReset(void);
};

