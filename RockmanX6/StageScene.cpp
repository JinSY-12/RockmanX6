#include "Stdafx.h"
#include "StageScene.h"

HRESULT StageScene::init(void)
{
	return S_OK;
}

HRESULT StageScene::init(int num)
{
	stageSettting(num);

	return S_OK;
}

void StageScene::release(void)
{
}

void StageScene::update(void)
{
}

void StageScene::render(void)
{
	cout << "x : " << CAMERAMANAGER->getPos().x << endl;
	cout << "y : " << CAMERAMANAGER->getPos().y << endl;

	mStage->render(getMemDC(), 0, 0, CAMERAMANAGER->getPos().x, CAMERAMANAGER->getPos().y, WINSIZE_X, WINSIZE_Y);
}

void StageScene::stageSettting(int stageNum)
{

	switch(stageNum)
	{
		// 인트로
		case 0:
			mStage = IMAGEMANAGER->findImage("Stage_Intro");
			break;

		// 커맨드 얀마크
		case 1:
			break;

	}

}
