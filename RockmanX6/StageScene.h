#pragma once
#include "GameNode.h"
#include "Player.h"
#include "X.h"
#include "Zero.h"

class StageScene : public GameNode
{
private:
	GImage* mStage;
	GImage* mReadyLogo;

	Player* player;

	RECT mZone;

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
	void release(void);
	void update(void);
	void render(void);

	void stageSettting(int stageNum);

	bool noticeAnim(void);

	StageScene() {}
	~StageScene() {}
};
