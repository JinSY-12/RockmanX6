#include "Stdafx.h"

#include "MainGame.h"
#include "TitleScene.h"
#include "MovieScene.h"
#include "StageScene.h"
#include "HunterBaseScene.h"

// 역슬래시 말고 슬래시로 경로를 적어라
// 절대경로 ex) "C:/User/Desktop/user/....../WinAPI"
// 상대경로 ex) "../WinAPI/"

HRESULT MainGame::init(void)
{
	GameNode::init(true);

	SCENEMANAGER->addScene("타이틀", new TitleScene);
	SCENEMANAGER->addScene("컷씬", new MovieScene);
	SCENEMANAGER->addScene("스테이지", new StageScene);
	SCENEMANAGER->addScene("헌터베이스", new HunterBaseScene);

	// 이렇게 하지마세요. 로딩은 타이틀에서 합니다. 이미지, 사운드 로드를 못해요ㅋㅋㅋ
	// SCENEMANAGER->changeScene("스테이지", 0);

	SCENEMANAGER->changeScene("타이틀");

	UIMANAGER->init();
	return S_OK;
}

void MainGame::release(void)
{
	GameNode::release();

}

void MainGame::update(void)
{
	GameNode::update();

	CAMERAMANAGER->update();
	SCENEMANAGER->update();
	UIMANAGER->update();
}

void MainGame::render(void)
{
	PatBlt(getMemDC(), 0, 0, WINSIZE_X, WINSIZE_Y, BLACKNESS);

	SCENEMANAGER->render();

	CAMERAMANAGER->render(getMemDC());

	UIMANAGER->render(getMemDC());


	this->getBackBuffer()->render(getHDC());
}