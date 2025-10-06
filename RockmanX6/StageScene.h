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
	GImage* mReadyLogo;

	unique_ptr<Player> player;

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

	// Ready �ΰ� �ִϸ��̼� ����
	// �ִϸ��̼� ��������Ʈ ������� 136�� �ؾ��ϴµ� �ʹ� �밡�� �ΰ� ���Ƽ� �ݺ��� ���Ƚ��ϴ٤Ф�
	int noticeTest;
	int noticeAniSpeed;

public:
	HRESULT init(void);
	HRESULT init(int num);
	HRESULT init(int num, int charType);
	void release(void);
	void update(void);
	void render(void);

	bool noticeAnim(void);

	void stageSettting(int stageNum);
	
	void rectSetting(void);
	void stageCollision(void);

	StageScene() {}
	~StageScene() {}
};
