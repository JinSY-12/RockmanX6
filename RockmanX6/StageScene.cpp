#include "Stdafx.h"
#include "StageScene.h"

HRESULT StageScene::init(void)
{
	return S_OK;
}

HRESULT StageScene::init(int num)
{
	player = new X;
		
	player->setBulletManager(&bManager);
	// 스테이지 세팅
	stageSettting(num);

	// 스테이지 시작 준비
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
	// 페이드 아웃 끝
	if (TIMEMANAGER->getWorldTime() - readyTimer >= 1.f)
	{
		noticeStart = true;
		if (noticeAnim() == false);
		else playAble = true;
	}

	if (playAble == true)
	{
		// 플레이어는 이미 천장에 소환 되어 있다
		// 레디 로고 이후에 플레이어의 동작 시작으로 하늘에서 내려오는 연출
		player->update();

		bManager.update();
	}

}

void StageScene::render(void)
{
	// 일단 스테이지 1은

	mStage->render(getMemDC(), 0, 0, CAMERAMANAGER->getCameraPos().x, CAMERAMANAGER->getCameraPos().y, WINSIZE_X, WINSIZE_Y);

	player->render();

	bManager.render();

	if(noticeStart)	mReadyLogo->render(getMemDC(), (WINSIZE_X - mReadyLogo->getWidth()) / 2,
		(WINSIZE_Y - mReadyLogo->getHeight()) / 2);

}

void StageScene::stageSettting(int stageNum)
{
	switch(stageNum)
	{
		// 인트로
		case 0:
			mStage = IMAGEMANAGER->findImage("Stage_Intro");
			gravity = 4.0f;
			player->init(WINSIZE_X / 2, mStage->getHeight() - 288 * 2);
			break;

		// 커맨드 얀마크
		case 1:
			mStage = IMAGEMANAGER->findImage("Stage_Yanmark");
			gravity = 4.0f;

			player->spawn(WINSIZE_X / 2, 1000);
			break;
	}

	CAMERAMANAGER->settingMapMaxSize(mStage->getWidth(), mStage->getHeight());
}

bool StageScene::noticeAnim(void)
{
	/////////////////////////////////
	// Ready 파트
	/////////////////////////////////

	if (noticeTest % noticeAniSpeed == 0)
	{
		string input = to_string(noticeTest / noticeAniSpeed);
		mReadyLogo = IMAGEMANAGER->findImage("Ready" + input);
	}

	// 레디 로고 최대치 까지 확장
	if (noticeTest < 7) noticeTest++;

	// 최대 확장 후 일정시간 대기
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
	// Warning 파트
	/////////////////////////////////

	return false;
}
