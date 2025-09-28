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

	// Ready 로고 애니메이션 관련
	// 애니메이션 스프라이트 만들려면 136장 해야하는데 너무 노가다 인거 같아서 반복문 돌렸습니다ㅠㅠ
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
