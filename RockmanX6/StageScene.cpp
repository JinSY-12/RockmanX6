#include "Stdafx.h"
#include "StageScene.h"

HRESULT StageScene::init(void)
{
	return S_OK;
}

HRESULT StageScene::init(int num)
{
	player = new X;

	// �������� ����
	stageSettting(num);

	// �������� ���� �غ�
	mReadyLogo = IMAGEMANAGER->findImage("Ready0");

	readyTimer = TIMEMANAGER->getWorldTime();
	noticeTest = 0;
	noticeAniSpeed = 1;
	noticeStart = false;


	playAble = false;
	soundOnce = false;

	return S_OK;
}

void StageScene::release(void)
{
}

void StageScene::update(void)
{
	// ���̵� �ƿ� ��
	if (TIMEMANAGER->getWorldTime() - readyTimer >= 1.f)
	{
		noticeStart = true;
		if (noticeAnim() == false);
		else playAble = true;
	}

	if (playAble == true)
	{
		// �÷��̾�� �̹� õ�忡 ��ȯ �Ǿ� �ִ�
		// ���� �ΰ� ���Ŀ� �÷��̾��� ���� �������� �ϴÿ��� �������� ����
		player->update();
	}

}

void StageScene::render(void)
{
	// ���� �ΰ� ȭ�� ���߾ӿ� ���
	//mReadyLogo->frameAlphaRender(getMemDC(), (WINSIZE_X - mReadyLogo->getFrameWidth()) / 2,
		//(WINSIZE_Y - mReadyLogo->getFrameHeight()) / 2, mReadyLogo->getFrameX(), mReadyLogo->getFrameY(), readyAlpha);


	mStage->render(getMemDC(), 0, 0, CAMERAMANAGER->getPos().x, CAMERAMANAGER->getPos().y, WINSIZE_X, WINSIZE_Y);

	if(noticeStart)	mReadyLogo->render(getMemDC(), (WINSIZE_X - mReadyLogo->getWidth()) / 2,
		(WINSIZE_Y - mReadyLogo->getHeight()) / 2);

	player->render();
}

void StageScene::stageSettting(int stageNum)
{
	switch(stageNum)
	{
		// ��Ʈ��
		case 0:
			mStage = IMAGEMANAGER->findImage("Stage_Intro");
			gravity = 1.0f;

			player->init(WINSIZE_X / 2, 10);
			break;

		// Ŀ�ǵ� �Ḷũ
		case 1:
			mStage = IMAGEMANAGER->findImage("Stage_Yanmark");
			gravity = 1.0f;

			player->spawn(WINSIZE_X / 2, 0);
			break;
	}
}

bool StageScene::noticeAnim(void)
{
	/////////////////////////////////
	// Ready ��Ʈ
	/////////////////////////////////

	if (noticeTest % noticeAniSpeed == 0)
	{
		string input = to_string(noticeTest / noticeAniSpeed);
		mReadyLogo = IMAGEMANAGER->findImage("Ready" + input);
	}

	// ���� �ΰ� �ִ�ġ ���� Ȯ��
	if (noticeTest < 7) noticeTest++;

	// �ִ� Ȯ�� �� �����ð� ���
	if (noticeTest >= 7)
	{
		if (TIMEMANAGER->getWorldTime() - readyTimer >= 1.2f && soundOnce == false)
		{
			SOUNDMANAGER->play("Ready", 0.5f);
			soundOnce = true;
		}
		
		if (TIMEMANAGER->getWorldTime() - readyTimer >= 2.5f)
		{
			if (noticeTest < 27 * noticeAniSpeed) noticeTest++;
			else noticeStart = false;
			if (noticeTest > 15 * noticeAniSpeed) return true;
		}
	}

	/////////////////////////////////
	// Warning ��Ʈ
	/////////////////////////////////

	return false;
}
