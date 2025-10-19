#include "Stdafx.h"
#include "StageScene.h"

HRESULT StageScene::init(PlayerType pType, BossType bType)
{
	switch (pType)
	{
	// 엑스
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

	// 스테이지 세팅
	UIMANAGER->SettingProgressBar(pType, bType);
	stageSettting(bType);

	// 스테이지 시작 준비
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
	// 페이드 아웃 끝
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
		// 플레이어는 이미 천장에 소환 되어 있다
		// 레디 로고 이후에 플레이어의 동작 시작으로 하늘에서 내려오는 연출
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
		// 인트로
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

		// 커맨드 얀마크
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
		// 인트로
	case BossType::Intro:
		eManager.spawnEnemy(EnemyType::Junkroid, 1300, 2660);
		break;

		// 커맨드 얀마크
	case BossType::CommanYanmark:
	
		break;

	defalut:
		break;
	}
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
	// Warning 파트
	/////////////////////////////////

	return false;
}

void StageScene::rectSetting(void)
{

#pragma region Stage_Intro
	// 렉트 충돌 테스트 
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

	// 위
	floor = RectMake(temp + 64 * SCALE_FACTOR, mStage->getHeight() - 480 * SCALE_FACTOR, 160 * SCALE_FACTOR, 75 * SCALE_FACTOR);
	_vFloor.push_back(floor);
	floor = RectMake(floor.right, mStage->getHeight() - 480 * SCALE_FACTOR, 64 * SCALE_FACTOR, 43 * SCALE_FACTOR);
	_vFloor.push_back(floor);

	// 아래
	floor = RectMake(temp + 64 * SCALE_FACTOR, mStage->getHeight() - 285 * SCALE_FACTOR, 224 * SCALE_FACTOR, 45 * SCALE_FACTOR);
	_vFloor.push_back(floor);

	temp = floor.right;

	// 위
	floor = RectMake(temp + 64 * SCALE_FACTOR, mStage->getHeight() - 480 * SCALE_FACTOR, 48 * SCALE_FACTOR, 43 * SCALE_FACTOR);
	_vFloor.push_back(floor);
	floor = RectMake(floor.right, mStage->getHeight() - 480 * SCALE_FACTOR, 64 * SCALE_FACTOR, 75 * SCALE_FACTOR);
	_vFloor.push_back(floor);
	floor = RectMake(floor.right, mStage->getHeight() - 480 * SCALE_FACTOR, 48 * SCALE_FACTOR, 43 * SCALE_FACTOR);
	_vFloor.push_back(floor);

	// 아래
	floor = RectMake(temp + 64 * SCALE_FACTOR, mStage->getHeight() - 311 * SCALE_FACTOR, 96 * SCALE_FACTOR, 71 * SCALE_FACTOR);
	_vFloor.push_back(floor);
	floor = RectMake(floor.right, mStage->getHeight() - 278 * SCALE_FACTOR, 64 * SCALE_FACTOR, 38 * SCALE_FACTOR);
	_vFloor.push_back(floor);

	
	temp = floor.right;
	
	//위
	floor = RectMake(temp + 128 * SCALE_FACTOR, mStage->getHeight() - 494 * SCALE_FACTOR, 304 * SCALE_FACTOR, 52 * SCALE_FACTOR);
	_vFloor.push_back(floor);

	//위 다음으로 넘어가는 곳
	
	floor = RectMake(temp + 144 * SCALE_FACTOR, mStage->getHeight() - (960 * SCALE_FACTOR), 46 * SCALE_FACTOR, 467 * SCALE_FACTOR);
	_vFloor.push_back(floor);
	
	floor = RectMake(floor.right + 80 * SCALE_FACTOR, mStage->getHeight() - 681 * SCALE_FACTOR, 240 * SCALE_FACTOR, 52 * SCALE_FACTOR);
	_vFloor.push_back(floor);
	
	//너비 240 높이 62 시작 646
	
	//아래
	floor = RectMake(temp + 128 * SCALE_FACTOR, mStage->getHeight() - 278 * SCALE_FACTOR, 384 * SCALE_FACTOR, 38 * SCALE_FACTOR);
	_vFloor.push_back(floor);

	floor = RectMake(floor.right, mStage->getHeight() - 729 * SCALE_FACTOR, 64 * SCALE_FACTOR, 489 * SCALE_FACTOR);
	_vFloor.push_back(floor);

	

	// -5 끝부분 다듬기 여기까지


#pragma endregion

#pragma region Stage_Yanmark

#pragma endregion

}

void StageScene::stageCollision(void)
{
	player->setIsOnGround(false, 0);	
	player->setRightCollision(false, 0);
	player->setLeftCollision(false, 0);

	// 바닥 체크
	for (int row = player->getPlayerBottom() + 5; row >= player->getPlayerBottom(); row--)
	{
		if (player->getPlayerSight() == true)
		{
			for (int line = player->getPlayerCenter() - 5 * SCALE_FACTOR; line <= player->getPlayerCenter() + 5 * SCALE_FACTOR; line++)
			{
				// 컬러 비교
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
				// 컬러 비교
				COLORREF color = GetPixel(mPixelStage->getMemDC(), line, row);

				if (color == RGB(255, 0, 0))
				{
					player->setIsOnGround(true, row);
					break;
				}
			}
		}
	}

	// 천장 체크
	for (int row = player->getPlayerTop() - 2; row <= player->getPlayerTop(); row++)
	{
		if (player->getPlayerSight() == true)
		{
			for (int line = player->getPlayerCenter() - 5 * SCALE_FACTOR; line <= player->getPlayerCenter() + 5 * SCALE_FACTOR; line++)
			{
				// 컬러 비교
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
				// 컬러 비교
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