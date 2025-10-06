#include "Stdafx.h"
#include "StageScene.h"

HRESULT StageScene::init(void)
{
	return S_OK;
}

HRESULT StageScene::init(int num)
{
	return S_OK;
}

HRESULT StageScene::init(int num, int charType)
{
	switch (charType)
	{
	// ����
	case 0:
		player = std::make_unique<X>();
		break;
	// ����
	case 1:
		break;
	// ����
	case 2:
		break;
	// ���̵�
	case 3:
		break;
	// ������
	case 4:
		break;
	// ��Ƽ��
	case 5:
		break;
	// �� ����
	case 6:
		break;
	default:
		break;
	}

	player->setBulletManager(&bManager);

	// �������� ����
	UIMANAGER->setCharacter(charType, num);
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
		else
		{
			playAble = true;
			UIMANAGER->setGameStart();
		}
	}

	if (playAble == true)
	{
		// �÷��̾�� �̹� õ�忡 ��ȯ �Ǿ� �ִ�
		// ���� �ΰ� ���Ŀ� �÷��̾��� ���� �������� �ϴÿ��� �������� ����
		player->update();

		bManager.update();
	}

	stageCollision();
}

void StageScene::render(void)
{
	// �ϴ� �������� 1��

	mStage->render(getMemDC(), 0, 0, CAMERAMANAGER->getCameraPos().x, CAMERAMANAGER->getCameraPos().y, WINSIZE_X, WINSIZE_Y);
	
	player->render();

	bManager.render();

	if(noticeStart)	mReadyLogo->render(getMemDC(), (WINSIZE_X - mReadyLogo->getWidth()) / 2,
		(WINSIZE_Y - mReadyLogo->getHeight()) / 2);

	if (UIMANAGER->getIsDebugMode() == true)
	{
		for (auto& floor : _vFloor)
		{
			RECT temp = floor;
			temp.left -= CAMERAMANAGER->getCameraPos().x;
			temp.right -= CAMERAMANAGER->getCameraPos().x;
			temp.top -= CAMERAMANAGER->getCameraPos().y;
			temp.bottom -= CAMERAMANAGER->getCameraPos().y;

			DrawRectMakeColor(getMemDC(), temp, RGB(255,255,0), 2);
		}
	}
}

void StageScene::stageSettting(int stageNum)
{
	switch(stageNum)
	{
		// ��Ʈ��
		case 0:
			mStage = IMAGEMANAGER->findImage("Stage_Intro");
			gravity = 0.5f;

			player->init(WINSIZE_X / 2, mStage->getHeight() - WINSIZE_Y);
			player->setStageGravity(gravity);
			rectSetting();
		
			break;

		// Ŀ�ǵ� �Ḷũ
		case 1:
			mStage = IMAGEMANAGER->findImage("Stage_Yanmark");
			gravity = 6.0f;

			player->init(WINSIZE_X / 2, mStage->getHeight() - WINSIZE_Y);
			player->spawn(WINSIZE_X / 2, 1000);
			player->setStageGravity(gravity);
			break;
	}

	CAMERAMANAGER->settingMapMaxSize(mStage->getWidth(), mStage->getHeight());
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

void StageScene::rectSetting(void)
{
	// ��Ʈ �浹 �׽�Ʈ 
	floor = RectMake(0, mStage->getHeight() - 45 * SCALE_FACTOR, 320 * SCALE_FACTOR, 45 * SCALE_FACTOR);
	_vFloor.push_back(floor);

	/*
	floor = RectMake(floor.right, mStage->getHeight() - 160 * SCALE_FACTOR, 320 * SCALE_FACTOR, 160 * SCALE_FACTOR);
	_vFloor.push_back(floor);
	
	floor = RectMake(0, mStage->getHeight() - 45 * SCALE_FACTOR, 320 * SCALE_FACTOR, 45 * SCALE_FACTOR);
	_vFloor.push_back(floor);

	floor = RectMake(floor.right, mStage->getHeight() - 80 * SCALE_FACTOR, 320 * SCALE_FACTOR, 80 * SCALE_FACTOR);
	_vFloor.push_back(floor);

	floor = RectMake(floor.right, mStage->getHeight() - 160 * SCALE_FACTOR, 128 * SCALE_FACTOR, 160 * SCALE_FACTOR);
	_vFloor.push_back(floor);

	floor = RectMake(floor.right, mStage->getHeight() - 115 * SCALE_FACTOR, 63 * SCALE_FACTOR, 115 * SCALE_FACTOR);
	_vFloor.push_back(floor);
	*/
}

void StageScene::stageCollision(void)
{
	player->setIsOnGround(false, 0, 0);
	player->setRightCollision(false);
	player->setLeftCollision(false);
	
	int index = 0;

	for (auto& floor : _vFloor)
	{
		index++;

		if (player->getPlayerLeft() < floor.right && player->getPlayerRight() > floor.left
			&& player->getPlayerBottom() + 4 > floor.top && player->getPlayerTop() < floor.top)
		{
			int bottom = WINSIZE_Y - (mStage->getHeight() - floor.top);
			
			player->setIsOnGround(true, floor.top, bottom);
		}

		if(player->getPlayerLeft() <= floor.right && player->getPlayerRight() > floor.right
			&& player->getPlayerBottom() > floor.top && player->getPlayerTop() < floor.bottom)
		{
			cout << index << "��° RECT�� �������� �浹" << endl;
			player->setLeftCollision(true);
		}

		
		if (player->getPlayerRight() > floor.left && player->getPlayerLeft() < floor.left
			&& player->getPlayerBottom() > floor.top && player->getPlayerTop() < floor.bottom)
		{
			cout << index << "��° RECT�� �������� �浹" << endl;
			player->setRightCollision(true);
		}
	}
}