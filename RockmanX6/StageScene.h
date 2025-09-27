#pragma once
#include "GameNode.h"

#

class StageScene : public GameNode
{
private:
	GImage* mStage;

	RECT mZone;

	float gravity;

public:
	HRESULT init(void);
	HRESULT init(int num);
	void release(void);
	void update(void);
	void render(void);

	void stageSettting(int stageNum);

	StageScene() {}
	~StageScene() {}
};
