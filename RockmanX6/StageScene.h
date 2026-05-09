#pragma once
#include "GameNode.h"
#include "Player.h"
#include "X.h"
#include "Zero.h"
#include "BulletManager.h"
#include "EnemyManager.h"
#include "ObjectManager.h"
#include "CollisionManager.h"
#include "DamageManager.h"

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

	typedef vector<RECT> vCheckFloor;

	vFloor _vFloor;
	viFloor _viFloor;
	vCheckFloor _vCheckFloor;

	BulletManager bManager;
	EnemyManager eManager;
	ObjectManager oManager;
	CollisionManager cManager;
	DamageManager dManager;

	float gravity;

	bool noticeStart;
	float readyTimer;

	bool playAble;
	bool soundOnce;

	// Ready 로고 애니메이션 관련
	// 수정완료
	int noticeTest;
	int noticeAniSpeed;

public:
	HRESULT init(PlayerType pType, BossType bType);
	void release(void);
	void update(void);
	void render(void);

	bool noticeAnim(void);

	void stageSettting(BossType bType);
	void enemySettting(BossType bType);
	void objectSetting(BossType bType);

	void rectSetting(void);
	void stageCollision(void);

	StageScene() {}
	~StageScene() {}
};

