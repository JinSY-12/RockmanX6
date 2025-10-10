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
	// mReadyLogo = IMAGEMANAGER->findImage("Ready");

	// SOUNDMANAGER->play(stagBGM, 0.5f);
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

	stageCollision();

	if (playAble == true)
	{
		// �÷��̾�� �̹� õ�忡 ��ȯ �Ǿ� �ִ�
		// ���� �ΰ� ���Ŀ� �÷��̾��� ���� �������� �ϴÿ��� �������� ����
		player->update();

		bManager.update();
	}

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
			gravity = 0.6f;
			stagBGM = "BGM_Stage_Intro";
			player->init(WINSIZE_X / 2, mStage->getHeight() - 288 * SCALE_FACTOR);
			player->setStageGravity(gravity);
			rectSetting();
			
			break;

		// Ŀ�ǵ� �Ḷũ
		case 1:
			mStage = IMAGEMANAGER->findImage("Stage_Yanmark");
			gravity = 6.0f;
			stagBGM = "BGM_Stage_CommandYanmark";
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
			SOUNDMANAGER->play("SFX_Ready", 0.5f);
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

	floor = RectMake(floor.right, mStage->getHeight() - 160 * SCALE_FACTOR, 320 * SCALE_FACTOR, 160 * SCALE_FACTOR);
	_vFloor.push_back(floor);
	
	floor = RectMake(floor.right, mStage->getHeight() - 160 * SCALE_FACTOR, 130 * SCALE_FACTOR, 160 * SCALE_FACTOR);
	_vFloor.push_back(floor);

	floor = RectMake(floor.right, mStage->getHeight() - 115 * SCALE_FACTOR, 63 * SCALE_FACTOR, 115 * SCALE_FACTOR);
	_vFloor.push_back(floor);

	floor = RectMake(floor.right, mStage->getHeight() - 145 * SCALE_FACTOR, 128 * SCALE_FACTOR, 145 * SCALE_FACTOR);
	_vFloor.push_back(floor);

	floor = RectMake(floor.right, mStage->getHeight() - 115 * SCALE_FACTOR, 96 * SCALE_FACTOR, 115 * SCALE_FACTOR);
	_vFloor.push_back(floor);

	floor = RectMake(floor.right, mStage->getHeight() - 160 * SCALE_FACTOR, 10 * SCALE_FACTOR, 160 * SCALE_FACTOR);
	_vFloor.push_back(floor);

	// floor = RectMake(floor.right, mStage->getHeight() - 160 * SCALE_FACTOR, 130 * SCALE_FACTOR, 160 * SCALE_FACTOR);
	//_vFloor.push_back(floor);

	//floor = RectMake(0, mStage->getHeight() - 100 * SCALE_FACTOR, 30 * SCALE_FACTOR, 100 * SCALE_FACTOR);
	//_vFloor.push_back(floor);

	/*
	floor = RectMake(floor.right, mStage->getHeight() - 80 * SCALE_FACTOR, 320 * SCALE_FACTOR, 80 * SCALE_FACTOR);
	_vFloor.push_back(floor);

	floor = RectMake(floor.right, mStage->getHeight() - 115 * SCALE_FACTOR, 63 * SCALE_FACTOR, 115 * SCALE_FACTOR);
	_vFloor.push_back(floor);
	*/
}

void StageScene::stageCollision(void)
{
	player->setIsOnGround(false, 0, 0);
	player->setRightCollision(false);
	player->setLeftCollision(false, 0);
	
	int index = 0;
	
	for (auto& floor : _vFloor)
	{
		index++;

		if(player->getPlayerLeft() -4 < floor.right && player->getPlayerRight() > floor.right
			&& player->getPlayerBottom() > floor.top) // && player->getPlayerTop() < floor.bottom)
		{
			player->setLeftCollision(true, floor.right);
		}

		if (player->getPlayerRight() + 4 > floor.left && player->getPlayerLeft() < floor.left
			&& player->getPlayerBottom() > floor.top) // && player->getPlayerTop() < floor.bottom)
		{
			player->setRightCollision(true);
		}

		if (player->getPlayerLeft() < floor.right && player->getPlayerRight() > floor.left
			&& player->getPlayerBottom() + 4 > floor.top && player->getPlayerTop() < floor.top)
		{
			// ��ǥ�� ������ �����̱� ������ ��ǥ�� ���� ������ ������ ���� WINSIZE_Y
			// ������ǥ���� �� ũ�⸦ ���� �ش� floor�� ���̰� �����µ� ���� �ٴ��� WINSIZE_Y���� ���°ɷ� ���ϴܿ� ��ġ
			// ī�޶��� ��ġ�� WINSIZE_Y�� ���ϸ� ī�޶� ���ϴ��� ��ġ�� �����µ� �װ� �� ũ�⿡�� ���� floor�� ī�޶� �´� ��ġ�� ���ġ ��
			// �� ���ġ�� ��ġ�� �������� ��Ʈ�ڽ��� �������ִ� �� -> ������ǥ�� ������ ȭ�鿡 �°� ���ġ �Ѵٰ� �����ϸ� �˴ϴ�.
			// int bottom = WINSIZE_Y - (mStage->getHeight() - floor.top) + mStage->getHeight() - (CAMERAMANAGER->getCameraPos().y + WINSIZE_Y);
			
			// ���� ���� �����ϸ� �̷��� �����µ�, �̷��� �����ϸ� �� ������...
			// floor�� ��ġ�� ������ǥ�ε� ī�޶� ��ŭ�� ������ ���� ���̳�...
			int bottom = floor.top - CAMERAMANAGER->getCameraPos().y;   

			player->setIsOnGround(true, floor.top, bottom);
		}
	}
}