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
	enum class StageState
	{
		Playing,
		GameOver,
		Clear
	};

	struct CheckPoint
	{
		int x;
		int y;
	};

private:
	StageState stateNow;

	GImage* mStage;
	GImage* mPixelStage;
	GImage* mReadyLogo;
	CheckPoint checkpoint;

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

	BossType stageType;
	float gravity;

	bool noticeStart;
	float readyTimer;

	bool playAble;
	bool soundOnce;

	// Ready 로고 애니메이션 관련
	int noticeTest;
	int noticeAniSpeed;

	bool test;

	float timer;

public:
	HRESULT init(PlayerType pType, BossType bType);
	void release(void);
	void update(void);
	void render(void);

	bool noticeAnim(void);

	void stageSettting(void);
	void enemySettting(void);
	void objectSetting(void);

	void rectSetting(void);
	void stageCollision(void);

	void gameover(void);
	void stageReset(void);

	bool clockTimer(float time);

	StageScene() {}
	~StageScene() {}
};
