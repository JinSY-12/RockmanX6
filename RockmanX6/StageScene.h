#pragma once
#include "GameNode.h"
#include "Player.h"
#include "X.h"
#include "Zero.h"
#include "BulletManager.h"

class StageScene : public GameNode
{
private:
	GImage* mStage;
	GImage* mPixelStage;
	GImage* mReadyLogo;

	unique_ptr<Player> player;

	string stagBGM;
	RECT mZone;
	RECT floor;
	
	typedef vector<RECT> vFloor;
	typedef vector<RECT>::iterator viFloor;
	
	vFloor _vFloor;
	viFloor _viFloor;

	BulletManager bManager;

	float gravity;

	bool noticeStart;
	float readyTimer;

	bool playAble;
	bool soundOnce;

	// Ready 로고 애니메이션 관련
	// 애니메이션 스프라이트 만들려면 136장 해야하는데 너무 노가다 인거 같아서 반복문 돌렸습니다ㅠㅠ
	int noticeTest;
	int noticeAniSpeed;

public:
	HRESULT init(PlayerType pType, BossType bType);
	void release(void);
	void update(void);
	void render(void);

	bool noticeAnim(void);

	void stageSettting(BossType bType);
	
	void rectSetting(void);
	void stageCollision(void);

	StageScene() {}
	~StageScene() {}
};
