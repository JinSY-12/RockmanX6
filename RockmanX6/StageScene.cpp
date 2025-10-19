#include "Stdafx.h"
#include "StageScene.h"

HRESULT StageScene::init(PlayerType pType, BossType bType)
{
	switch (pType)
	{
	// ����
	case PlayerType::X:
		player = make_unique<X>();
		break;
	default:
		break;
	}

	player->setBulletManager(&bManager);

	eManager.setttingPlayer(player.get());
	eManager.setttingBulletManager(&bManager);
	
	bManager.settingPlayer(player.get());

	// �������� ����
	UIMANAGER->SettingProgressBar(pType, bType);
	stageSettting(bType);

	// �������� ���� �غ�
	SOUNDMANAGER->play(stagBGM, 0.5f);
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
			UIMANAGER->playStart();
		}
	}

	stageCollision();

	if (playAble == true)
	{
		// �÷��̾�� �̹� õ�忡 ��ȯ �Ǿ� �ִ�
		// ���� �ΰ� ���Ŀ� �÷��̾��� ���� �������� �ϴÿ��� �������� ����
		player->update();

		eManager.update();

		EFFECTMANAGER->update();

		bManager.update();
	}

}

void StageScene::render(void)
{
	mStage->render(getMemDC(), 0, 0, CAMERAMANAGER->getCameraPos().x, CAMERAMANAGER->getCameraPos().y, WINSIZE_X, WINSIZE_Y);
	
	player->render();

	eManager.render();

	EFFECTMANAGER->render(getMemDC());

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
			
			DrawRectMakeColor(getMemDC(), temp, RGB(255,255,0), 1);
		}
		mPixelStage->render(getMemDC(), 0,0, CAMERAMANAGER->getCameraPos().x, CAMERAMANAGER->getCameraPos().y, WINSIZE_X, WINSIZE_Y);
	}
}

void StageScene::stageSettting(BossType bType)
{
	switch(bType)
	{
		// ��Ʈ��
		case BossType::Intro:
			mStage = IMAGEMANAGER->findImage("Stage_Intro");
			mPixelStage = IMAGEMANAGER->findImage("Pixel_Intro");
			gravity = 0.6f;
			stagBGM = "BGM_Stage_Intro";
			//player->init(3000, mStage->getHeight() - 288 * SCALE_FACTOR);
			player->init(WINSIZE_X / 2, mStage->getHeight() - 288 * SCALE_FACTOR);
			player->setStageGravity(gravity);
			rectSetting();
			enemySettting(bType);
			break;

		// Ŀ�ǵ� �Ḷũ
		case BossType::CommanYanmark:
			mStage = IMAGEMANAGER->findImage("Stage_Yanmark");
			gravity = 6.0f;
			stagBGM = "BGM_Stage_CommandYanmark";
			player->init(WINSIZE_X / 2, mStage->getHeight() - WINSIZE_Y);
			player->spawn(WINSIZE_X / 2, 1000);
			player->setStageGravity(gravity);
			break;

		defalut:
			break;
	}

	CAMERAMANAGER->settingMapMaxSize(mStage->getWidth(), mStage->getHeight());
}

void StageScene::enemySettting(BossType bType)
{
	switch (bType)
	{
		// ��Ʈ��
	case BossType::Intro:
		eManager.spawnEnemy(EnemyType::Junkroid, 1300, 2660);
		break;

		// Ŀ�ǵ� �Ḷũ
	case BossType::CommanYanmark:
	
		break;

	defalut:
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

#pragma region Stage_Intro
	// ��Ʈ �浹 �׽�Ʈ 
	floor = RectMake(0, mStage->getHeight() - 40 * SCALE_FACTOR, 320 * SCALE_FACTOR, 40 * SCALE_FACTOR);
	_vFloor.push_back(floor);

	floor = RectMake(floor.right, mStage->getHeight() - 75 * SCALE_FACTOR, 319 * SCALE_FACTOR, 75 * SCALE_FACTOR);
	_vFloor.push_back(floor);
	
	floor = RectMake(floor.right, mStage->getHeight() - 155 * SCALE_FACTOR, 130 * SCALE_FACTOR, 155 * SCALE_FACTOR);
	_vFloor.push_back(floor);

	floor = RectMake(floor.right, mStage->getHeight() - 110 * SCALE_FACTOR, 63 * SCALE_FACTOR, 110 * SCALE_FACTOR);
	_vFloor.push_back(floor);

	floor = RectMake(floor.right, mStage->getHeight() - 140 * SCALE_FACTOR, 128 * SCALE_FACTOR, 140 * SCALE_FACTOR);
	_vFloor.push_back(floor);

	floor = RectMake(floor.right, mStage->getHeight() - 110 * SCALE_FACTOR, 96 * SCALE_FACTOR, 110 * SCALE_FACTOR);
	_vFloor.push_back(floor);

	floor = RectMake(floor.right, mStage->getHeight() - 155 * SCALE_FACTOR, 10 * SCALE_FACTOR, 155 * SCALE_FACTOR);
	_vFloor.push_back(floor);

	floor = RectMake(floor.right, mStage->getHeight() - 77 * SCALE_FACTOR, 438 * SCALE_FACTOR, 77 * SCALE_FACTOR);
	_vFloor.push_back(floor);

	int temp = floor.right;

	floor = RectMake(temp + 96 * SCALE_FACTOR, mStage->getHeight() - 171 * SCALE_FACTOR, 384 * SCALE_FACTOR, 171 * SCALE_FACTOR);
	_vFloor.push_back(floor);

	floor = RectMake(temp + 96 * SCALE_FACTOR, mStage->getHeight() - 288 * SCALE_FACTOR, 64 * SCALE_FACTOR, 43 * SCALE_FACTOR);
	_vFloor.push_back(floor);
	
	floor = RectMake(floor.right + 256 * SCALE_FACTOR, mStage->getHeight() - 288 * SCALE_FACTOR, 64 * SCALE_FACTOR, 43 * SCALE_FACTOR);
	_vFloor.push_back(floor);

	floor = RectMake(floor.right + 46 * SCALE_FACTOR, mStage->getHeight() - 171 * SCALE_FACTOR, 82 * SCALE_FACTOR, 27 * SCALE_FACTOR);
	_vFloor.push_back(floor);

	floor = RectMake(floor.right, mStage->getHeight() - 288 * SCALE_FACTOR, 192 * SCALE_FACTOR, 139 * SCALE_FACTOR);
	_vFloor.push_back(floor);

	temp = floor.right;

	floor = RectMake(temp + 512 * SCALE_FACTOR, mStage->getHeight() - 72 * SCALE_FACTOR, 64 * SCALE_FACTOR, 72 * SCALE_FACTOR);
	_vFloor.push_back(floor);

	floor = RectMake(temp + 400 * SCALE_FACTOR, mStage->getHeight() - 288 * SCALE_FACTOR, 240 * SCALE_FACTOR, 139 * SCALE_FACTOR);
	_vFloor.push_back(floor);

	floor = RectMake(floor.right + 64 * SCALE_FACTOR, mStage->getHeight() - 288 * SCALE_FACTOR, 64 * SCALE_FACTOR, 139 * SCALE_FACTOR);
	_vFloor.push_back(floor);

	floor = RectMake(floor.right, mStage->getHeight() - 288 * SCALE_FACTOR, 64 * SCALE_FACTOR, 170 * SCALE_FACTOR);
	_vFloor.push_back(floor);


	floor = RectMake(floor.right, mStage->getHeight() - 480 * SCALE_FACTOR, 512 * SCALE_FACTOR, 331 * SCALE_FACTOR);
	_vFloor.push_back(floor);

	floor = RectMake(floor.right, mStage->getHeight() - 480 * SCALE_FACTOR, 32 * SCALE_FACTOR, 267 * SCALE_FACTOR);
	_vFloor.push_back(floor);

	floor = RectMake(floor.right, mStage->getHeight() - 480 * SCALE_FACTOR, 32 * SCALE_FACTOR, 203 * SCALE_FACTOR);
	_vFloor.push_back(floor);

	temp = floor.right;

	floor = RectMake(temp, mStage->getHeight() - 480 * SCALE_FACTOR, 640 * SCALE_FACTOR, 43 * SCALE_FACTOR);
	_vFloor.push_back(floor);

	floor = RectMake(temp + 128 * SCALE_FACTOR, mStage->getHeight() - 360 * SCALE_FACTOR, 224 * SCALE_FACTOR, 360 * SCALE_FACTOR);
	_vFloor.push_back(floor);

	floor = RectMake(floor.right, mStage->getHeight() - 360 * SCALE_FACTOR, 32 * SCALE_FACTOR, 120 * SCALE_FACTOR);
	_vFloor.push_back(floor);

	floor = RectMake(floor.right, mStage->getHeight() - 285 * SCALE_FACTOR, 256 * SCALE_FACTOR, 45 * SCALE_FACTOR);
	_vFloor.push_back(floor);

	temp = floor.right;

	// ��
	floor = RectMake(temp + 64 * SCALE_FACTOR, mStage->getHeight() - 480 * SCALE_FACTOR, 160 * SCALE_FACTOR, 75 * SCALE_FACTOR);
	_vFloor.push_back(floor);
	floor = RectMake(floor.right, mStage->getHeight() - 480 * SCALE_FACTOR, 64 * SCALE_FACTOR, 43 * SCALE_FACTOR);
	_vFloor.push_back(floor);

	// �Ʒ�
	floor = RectMake(temp + 64 * SCALE_FACTOR, mStage->getHeight() - 285 * SCALE_FACTOR, 224 * SCALE_FACTOR, 45 * SCALE_FACTOR);
	_vFloor.push_back(floor);

	temp = floor.right;

	// ��
	floor = RectMake(temp + 64 * SCALE_FACTOR, mStage->getHeight() - 480 * SCALE_FACTOR, 48 * SCALE_FACTOR, 43 * SCALE_FACTOR);
	_vFloor.push_back(floor);
	floor = RectMake(floor.right, mStage->getHeight() - 480 * SCALE_FACTOR, 64 * SCALE_FACTOR, 75 * SCALE_FACTOR);
	_vFloor.push_back(floor);
	floor = RectMake(floor.right, mStage->getHeight() - 480 * SCALE_FACTOR, 48 * SCALE_FACTOR, 43 * SCALE_FACTOR);
	_vFloor.push_back(floor);

	// �Ʒ�
	floor = RectMake(temp + 64 * SCALE_FACTOR, mStage->getHeight() - 311 * SCALE_FACTOR, 96 * SCALE_FACTOR, 71 * SCALE_FACTOR);
	_vFloor.push_back(floor);
	floor = RectMake(floor.right, mStage->getHeight() - 278 * SCALE_FACTOR, 64 * SCALE_FACTOR, 38 * SCALE_FACTOR);
	_vFloor.push_back(floor);

	
	temp = floor.right;
	
	//��
	floor = RectMake(temp + 128 * SCALE_FACTOR, mStage->getHeight() - 494 * SCALE_FACTOR, 304 * SCALE_FACTOR, 52 * SCALE_FACTOR);
	_vFloor.push_back(floor);

	//�� �������� �Ѿ�� ��
	
	floor = RectMake(temp + 144 * SCALE_FACTOR, mStage->getHeight() - (960 * SCALE_FACTOR), 46 * SCALE_FACTOR, 467 * SCALE_FACTOR);
	_vFloor.push_back(floor);
	
	floor = RectMake(floor.right + 80 * SCALE_FACTOR, mStage->getHeight() - 681 * SCALE_FACTOR, 240 * SCALE_FACTOR, 52 * SCALE_FACTOR);
	_vFloor.push_back(floor);
	
	//�ʺ� 240 ���� 62 ���� 646
	
	//�Ʒ�
	floor = RectMake(temp + 128 * SCALE_FACTOR, mStage->getHeight() - 278 * SCALE_FACTOR, 384 * SCALE_FACTOR, 38 * SCALE_FACTOR);
	_vFloor.push_back(floor);

	floor = RectMake(floor.right, mStage->getHeight() - 729 * SCALE_FACTOR, 64 * SCALE_FACTOR, 489 * SCALE_FACTOR);
	_vFloor.push_back(floor);

	

	// -5 ���κ� �ٵ�� �������


#pragma endregion

#pragma region Stage_Yanmark

#pragma endregion

}

void StageScene::stageCollision(void)
{
	player->setIsOnGround(false, 0);	
	player->setRightCollision(false, 0);
	player->setLeftCollision(false, 0);

	// �ٴ� üũ
	for (int row = player->getPlayerBottom() + 5; row >= player->getPlayerBottom(); row--)
	{
		if (player->getPlayerSight() == true)
		{
			for (int line = player->getPlayerCenter() - 5 * SCALE_FACTOR; line <= player->getPlayerCenter() + 5 * SCALE_FACTOR; line++)
			{
				// �÷� ��
				COLORREF color = GetPixel(mPixelStage->getMemDC(), line, row);

				if (color == RGB(255, 0, 0))
				{
					player->setIsOnGround(true, row);
					break;
				}
			}
		}

		else
		{
			for (int line = player->getPlayerCenter() + 5 * SCALE_FACTOR; line >= player->getPlayerCenter() - 5 * SCALE_FACTOR; line--)
			{
				// �÷� ��
				COLORREF color = GetPixel(mPixelStage->getMemDC(), line, row);

				if (color == RGB(255, 0, 0))
				{
					player->setIsOnGround(true, row);
					break;
				}
			}
		}
	}

	// õ�� üũ
	for (int row = player->getPlayerTop() - 2; row <= player->getPlayerTop(); row++)
	{
		if (player->getPlayerSight() == true)
		{
			for (int line = player->getPlayerCenter() - 5 * SCALE_FACTOR; line <= player->getPlayerCenter() + 5 * SCALE_FACTOR; line++)
			{
				// �÷� ��
				COLORREF color = GetPixel(mPixelStage->getMemDC(), line, row);

				if (color == RGB(255, 0, 0))
				{
					player->setTopCollision(true, row);
					break;
				}
			}
		}

		else
		{
			for (int line = player->getPlayerCenter() + 2 * SCALE_FACTOR; line >= player->getPlayerCenter() - 5 * SCALE_FACTOR; line--)
			{
				// �÷� ��
				COLORREF color = GetPixel(mPixelStage->getMemDC(), line, row);

				if (color == RGB(255, 0, 0))
				{
					player->setTopCollision(true, row);
					break;
				}
			}
		}
	}

	for (auto& floor : _vFloor)
	{
		if(player->getPlayerLeft() - 4 < floor.right && player->getPlayerRight() > floor.right
			&& player->getPlayerBottom() > floor.top && player->getPlayerTop() < floor.bottom)
		{
			player->setLeftCollision(true, floor.right);
			break;
		}

		if (player->getPlayerRight() + 4 > floor.left && player->getPlayerLeft() < floor.left
			&& player->getPlayerBottom() > floor.top && player->getPlayerTop() < floor.bottom)
		{
			player->setRightCollision(true, floor.left);
			break;
		}
	}

	
}