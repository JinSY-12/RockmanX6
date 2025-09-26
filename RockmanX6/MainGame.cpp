#include "Stdafx.h"

#include "MainGame.h"
#include "TitleScene.h"
#include "MovieScene.h"
#include "StageScene.h"
#include "HunterBaseScene.h"

// �������� ���� �����÷� ��θ� �����
// ������ ex) "C:/User/Desktop/user/....../WinAPI"
// ����� ex) "../WinAPI/"

HRESULT MainGame::init(void)
{
	GameNode::init(true);

	SCENEMANAGER->addScene("Ÿ��Ʋ", new TitleScene);
	SCENEMANAGER->addScene("�ƾ�", new MovieScene);
	SCENEMANAGER->addScene("��������", new StageScene);
	SCENEMANAGER->addScene("���ͺ��̽�", new HunterBaseScene);

	// �̷��� ����������. �ε��� Ÿ��Ʋ���� �մϴ�. �̹���, ���� �ε带 ���ؿ䤻����
	// SCENEMANAGER->changeScene("��������", 0);

	SCENEMANAGER->changeScene("Ÿ��Ʋ");

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