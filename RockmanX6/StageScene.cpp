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

	stageType = bType;

	EVENTMANAGER->addListener(&bManager);
	EVENTMANAGER->addListener(&dManager);

	// 데미지, 애너미 매니저 추가 해야 함
	cManager.settingManager(player.get(), &eManager, &bManager, &oManager);

	player->settingBulletManager(&bManager);

	// 콜리전 매니저가 추가되면 삭제
	eManager.setttingPlayer(player.get());
	eManager.setttingBulletManager(&bManager);
	
	oManager.setttingPlayer(player.get());
	oManager.setttingBulletManager(&bManager);
	// 콜리전 매니저가 추가되면 여기까지 삭제
	// 얘네는 생성과 관리만 할 예정임

	// 스테이지 세팅
	stageSettting();

	// 스테이지 시작 준비
	SOUNDMANAGER->play(stagBGM, 0.5f);
	SOUNDMANAGER->setCurrentBGM(stagBGM);

	readyTimer = TIMEMANAGER->getWorldTime();
	noticeTest = 0;
	noticeAniSpeed = 1;
	noticeStart = false;

	playAble = false;
	soundOnce = false;

	test = false;
	stateNow = StageState::Playing;

	return S_OK;
}

void StageScene::release(void)
{
}

void StageScene::update(void)
{	
	switch (stateNow)
	{
	case StageState::Playing:
		stageCollision();

		if (UIMANAGER->getIsUiMode() == false)
		{
			// 플레이어는 이미 천장에 소환 되어 있다
			// 레디 로고 이후에 플레이어의 동작 시작으로 하늘에서 내려오는 연출
			bManager.update();
			player->update();
			eManager.update();
			oManager.update();

			cManager.update();

			EFFECTMANAGER->update();

			if (player->getDeadDone()) gameover();

			if (eManager.checkComplete())
			{
				if (!playOnce)
				{
					SOUNDMANAGER->stopBGM();
					SOUNDMANAGER->play("BGM_X_StageClear", 0.5f);
					SOUNDMANAGER->setCurrentBGM("BGM_X_StageClear");
					playOnce = true;
				}

				if (clockTimer(4.0f))
				{
					player->chageVictory();
					stateNow = StageState::Clear;
				}
			}
		}
		break;
	case StageState::GameOver:
		stageReset();
		break;
	case StageState::Clear:
		player->update();

		if (player->getWarpOutDone() == true)
		{
			stateNow = StageState::None;
			CAMERAMANAGER->padeOut(2.0f);
		}
		break;
	case StageState::None:
		// 여기서 헌터 베이스 만들면 됨
		break;
	}
}

void StageScene::render(void)
{
	mStage->render(getMemDC(), 0, 0, CAMERAMANAGER->getCameraPos().x, CAMERAMANAGER->getCameraPos().y, WINSIZE_X, WINSIZE_Y);
	
	eManager.render();
	oManager.render();
	bManager.render();

	EFFECTMANAGER->afterImageRender(getMemDC());
	
	player->render(getMemDC());

	EFFECTMANAGER->render(getMemDC());

	for (auto bg : _vBackGround)
	{
		bg.image->render(getMemDC(), bg.x - CAMERAMANAGER->getCameraPos().x, bg.y - CAMERAMANAGER->getCameraPos().y);
	}

	if (UIMANAGER->getIsDebugMode() == true)
	{
		if (KEYMANAGER->isToggleKey('O'))
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
		}
		
		if (KEYMANAGER->isToggleKey('P')) mPixelStage->render(getMemDC(), 0,0, CAMERAMANAGER->getCameraPos().x, CAMERAMANAGER->getCameraPos().y, WINSIZE_X, WINSIZE_Y);
	}
}

void StageScene::stageSettting(void)
{
	switch(stageType)
	{
		// 인트로
		case BossType::Intro:
			mStage = IMAGEMANAGER->findImage("Stage_Intro");
			mPixelStage = IMAGEMANAGER->findImage("Pixel_Intro");
			gravity = 0.6f;
			stagBGM = "BGM_Stage_Intro";

			// 빠른 적군 대전 테스트
			// checkpoint.x = WINSIZE_X / 3;
			// checkpoint.y = 672 * SCALE_FACTOR;
			
			// 시작점
			// checkpoint.x = 160 * SCALE_FACTOR;
			// checkpoint.y = 682 * SCALE_FACTOR;
						 
			// 사다리 테스트
			checkpoint.x = 2090 * SCALE_FACTOR;
			checkpoint.y = 682 * SCALE_FACTOR;

			// 보스 게이트 테스트
			// checkpoint.x = 5080 * SCALE_FACTOR;
			// checkpoint.y = 490 * SCALE_FACTOR;
			
			// 보스 테스트
			// checkpoint.x = 5904 * SCALE_FACTOR;
			// checkpoint.y = 0 * SCALE_FACTOR;

			CAMERAMANAGER->setCheckPoint(checkpoint.x, checkpoint.y, 0);

			player->init(checkpoint.x, checkpoint.y);

			player->setStageGravity(gravity);

			UIMANAGER->addUi(UiType::Ready);

			rectSetting();
			enemySettting();
			objectSetting();
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

		default:
			break;
	}

	CAMERAMANAGER->settingMapMaxSize(mStage->getWidth(), mStage->getHeight());
}

void StageScene::enemySettting(void)
{
	switch (stageType)
	{
		// 인트로
	case BossType::Intro:

		switch (CAMERAMANAGER->getZoneResetNumber())
		{
		case 0:
			eManager.spawnEnemy(EnemyType::Junkroid, 370 * SCALE_FACTOR, 888 * SCALE_FACTOR);
			eManager.spawnEnemy(EnemyType::Junkroid, 565 * SCALE_FACTOR, 888 * SCALE_FACTOR);
			eManager.spawnEnemy(EnemyType::Junkroid, 765 * SCALE_FACTOR, 858 * SCALE_FACTOR);
			eManager.spawnEnemy(EnemyType::Junkroid, 990 * SCALE_FACTOR, 858 * SCALE_FACTOR);
			
			// eManager.spawnEnemy(EnemyType::Junkroid, 1671 * SCALE_FACTOR, 790 * SCALE_FACTOR);
			// eManager.spawnEnemy(EnemyType::Junkroid, 1836 * SCALE_FACTOR, 790 * SCALE_FACTOR);
		case 1:
			eManager.spawnEnemy(EnemyType::MetaWheel, 2400 * SCALE_FACTOR, 918 * SCALE_FACTOR);
			eManager.spawnEnemy(EnemyType::MetaWheel, 3650 * SCALE_FACTOR, 918 * SCALE_FACTOR);
			
			eManager.spawnEnemy(EnemyType::MetaDridler, 4356 * SCALE_FACTOR, 480 * SCALE_FACTOR, 670 * SCALE_FACTOR);
			eManager.spawnEnemy(EnemyType::MetaDridler, 4644 * SCALE_FACTOR, 480 * SCALE_FACTOR, 640 * SCALE_FACTOR);
			eManager.spawnEnemy(EnemyType::MetaDridler, 4864 * SCALE_FACTOR, 480 * SCALE_FACTOR, 640 * SCALE_FACTOR);
			eManager.spawnEnemy(EnemyType::MetaDridler, 4928 * SCALE_FACTOR, 480 * SCALE_FACTOR, 640 * SCALE_FACTOR);
		case 4:
			eManager.spawnEnemy(EnemyType::Junkroid, 5320 * SCALE_FACTOR, 630 * SCALE_FACTOR);
			
			eManager.spawnEnemy(EnemyType::MetaDridler, (5312 + 4) * SCALE_FACTOR, 300 * SCALE_FACTOR, 510 * SCALE_FACTOR);
														
			eManager.spawnEnemy(EnemyType::MetaDridler, (5056 + 2) * SCALE_FACTOR, 110 * SCALE_FACTOR, 270 * SCALE_FACTOR);
														
			eManager.spawnEnemy(EnemyType::MetaDridler, (5168 + 4) * SCALE_FACTOR, 48 * SCALE_FACTOR, 250 * SCALE_FACTOR);
			eManager.spawnEnemy(EnemyType::MetaDridler, (5280 + 4) * SCALE_FACTOR, 48 * SCALE_FACTOR, 220 * SCALE_FACTOR);
			eManager.spawnEnemy(EnemyType::MetaDridler, (5392 + 4) * SCALE_FACTOR, 48 * SCALE_FACTOR, 210 * SCALE_FACTOR);
		default:
			break;
		}
		
		break;

		// 커맨드 얀마크
	case BossType::CommanYanmark:
	
		break;

	defalut:
		break;
	}
}

void StageScene::objectSetting(void)
{
	switch (stageType)
	{
		// 인트로
	case BossType::Intro:

		// 오브젝트 테스트 용도
		// oManager.spawnObject(ObjectType::Block, 200 * SCALE_FACTOR, 825 * SCALE_FACTOR, 0, 0, 0);
		// oManager.spawnObject(ObjectType::BossGate, 200 * SCALE_FACTOR, 825 * SCALE_FACTOR, 0, 0, 0);
		
		// 세팅 시작
		// oManager.spawnObject(ObjectType::Block , 1984 * SCALE_FACTOR, 728 * SCALE_FACTOR, 0, 0, 0);
		oManager.spawnObject(ObjectType::Block, 2816 * SCALE_FACTOR, 825 * SCALE_FACTOR, 0, 0, 0);
		oManager.spawnObject(ObjectType::Block, 3972 * SCALE_FACTOR, 538 * SCALE_FACTOR, 0, 0, 0);
		oManager.spawnObject(ObjectType::Block, 5456 * SCALE_FACTOR, 168 * SCALE_FACTOR, 0, 0, 0);
		
		// 보스 게이트
		oManager.spawnObject(ObjectType::BossGate, 5743 * SCALE_FACTOR, 138 * SCALE_FACTOR, 320 * SCALE_FACTOR, 10 * SCALE_FACTOR, 240 * SCALE_FACTOR);
		oManager.spawnObject(ObjectType::BossGate, 6063 * SCALE_FACTOR, 138 * SCALE_FACTOR, 400 * SCALE_FACTOR, 10 * SCALE_FACTOR, 240 * SCALE_FACTOR, true);
		
		// 사다리
		oManager.spawnObject(ObjectType::Ladder, 1989 * SCALE_FACTOR, 796 * SCALE_FACTOR, 0, 0, 0);

		bg.image = IMAGEMANAGER->findImage("Stage");

		_vBackGround.push_back({ IMAGEMANAGER->findImage("Stage_Object1"), 0 * SCALE_FACTOR, 865 * SCALE_FACTOR });
		_vBackGround.push_back({ IMAGEMANAGER->findImage("Stage_Object2"), 5312 * SCALE_FACTOR, 282 * SCALE_FACTOR });
		_vBackGround.push_back({ IMAGEMANAGER->findImage("Stage_Object2"), 5056 * SCALE_FACTOR, 74 * SCALE_FACTOR });
		_vBackGround.push_back({ IMAGEMANAGER->findImage("Stage_Object2"), 5168 * SCALE_FACTOR, 0 * SCALE_FACTOR });
		_vBackGround.push_back({ IMAGEMANAGER->findImage("Stage_Object2"), 5280 * SCALE_FACTOR, 0 * SCALE_FACTOR });
		_vBackGround.push_back({ IMAGEMANAGER->findImage("Stage_Object2"), 5392 * SCALE_FACTOR, 0 * SCALE_FACTOR });
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
	
	// Zone 1 - 외부
	// 1
	floor = RectMake(0, mStage->getHeight() - 40 * SCALE_FACTOR, 320 * SCALE_FACTOR, 40 * SCALE_FACTOR);
	_vFloor.push_back(floor);

	floor = RectMake(floor.right, mStage->getHeight() - 75 * SCALE_FACTOR, 319 * SCALE_FACTOR, 75 * SCALE_FACTOR);
	_vFloor.push_back(floor);

	// 2 테스트 전용
	//floor = RectMake(floor.right, mStage->getHeight() - 155 * SCALE_FACTOR, 319 * SCALE_FACTOR, 155 * SCALE_FACTOR);
	//_vFloor.push_back(floor);
	
	// 3
	floor = RectMake(floor.right, mStage->getHeight() - 155 * SCALE_FACTOR, 130 * SCALE_FACTOR, 155 * SCALE_FACTOR);
	_vFloor.push_back(floor);
	// 4
	floor = RectMake(floor.right, mStage->getHeight() - 110 * SCALE_FACTOR, 63 * SCALE_FACTOR, 110 * SCALE_FACTOR);
	_vFloor.push_back(floor);
	// 5
	floor = RectMake(floor.right, mStage->getHeight() - 140 * SCALE_FACTOR, 128 * SCALE_FACTOR, 140 * SCALE_FACTOR);
	_vFloor.push_back(floor);
	// 6
	floor = RectMake(floor.right, mStage->getHeight() - 110 * SCALE_FACTOR, 96 * SCALE_FACTOR, 110 * SCALE_FACTOR);
	_vFloor.push_back(floor);
	// 7
	floor = RectMake(floor.right, mStage->getHeight() - 155 * SCALE_FACTOR, 10 * SCALE_FACTOR, 155 * SCALE_FACTOR);
	_vFloor.push_back(floor);
	// 8
	floor = RectMake(floor.right, mStage->getHeight() - 77 * SCALE_FACTOR, 438 * SCALE_FACTOR, 77 * SCALE_FACTOR);
	_vFloor.push_back(floor);

	// Zone 1 - 내부
	int temp = floor.right;

	// 9
	floor = RectMake(temp + 96 * SCALE_FACTOR, mStage->getHeight() - 171 * SCALE_FACTOR, 384 * SCALE_FACTOR, 171 * SCALE_FACTOR);
	_vFloor.push_back(floor);

	// 10
	floor = RectMake(temp + 96 * SCALE_FACTOR, mStage->getHeight() - 288 * SCALE_FACTOR, 64 * SCALE_FACTOR, 43 * SCALE_FACTOR);
	_vFloor.push_back(floor);

	// 11
	floor = RectMake(floor.right + 256 * SCALE_FACTOR, mStage->getHeight() - 288 * SCALE_FACTOR, 64 * SCALE_FACTOR, 43 * SCALE_FACTOR);
	_vFloor.push_back(floor);

	// 12
	floor = RectMake(floor.right + 46 * SCALE_FACTOR, mStage->getHeight() - 171 * SCALE_FACTOR, 82 * SCALE_FACTOR, 27 * SCALE_FACTOR);
	_vFloor.push_back(floor);

	// 13
	floor = RectMake(floor.right, mStage->getHeight() - 288 * SCALE_FACTOR, 192 * SCALE_FACTOR, 139 * SCALE_FACTOR);
	_vFloor.push_back(floor);

	temp = floor.right;

	// 14
	floor = RectMake(temp + 400 * SCALE_FACTOR, mStage->getHeight() - 288 * SCALE_FACTOR, 240 * SCALE_FACTOR, 139 * SCALE_FACTOR);
	_vFloor.push_back(floor);

	// 15
	floor = RectMake(temp + 512 * SCALE_FACTOR, mStage->getHeight() - 72 * SCALE_FACTOR, 64 * SCALE_FACTOR, 72 * SCALE_FACTOR);
	_vFloor.push_back(floor);

	// 16
	floor = RectMake(floor.right + 128 * SCALE_FACTOR, mStage->getHeight() - 288 * SCALE_FACTOR, 64 * SCALE_FACTOR, 139 * SCALE_FACTOR);
	_vFloor.push_back(floor);

	// 17
	floor = RectMake(floor.right, mStage->getHeight() - 288 * SCALE_FACTOR, 64 * SCALE_FACTOR, 170 * SCALE_FACTOR);
	_vFloor.push_back(floor);

	// 18
	floor = RectMake(floor.right, mStage->getHeight() - 480 * SCALE_FACTOR, 512 * SCALE_FACTOR, 331 * SCALE_FACTOR);
	_vFloor.push_back(floor);

	// Zone 2
	// 19
	floor = RectMake(floor.right, mStage->getHeight() - 480 * SCALE_FACTOR, 32 * SCALE_FACTOR, 267 * SCALE_FACTOR);
	_vFloor.push_back(floor);

	// 20
	floor = RectMake(floor.right, mStage->getHeight() - 480 * SCALE_FACTOR, 32 * SCALE_FACTOR, 203 * SCALE_FACTOR);
	_vFloor.push_back(floor);

	temp = floor.right;

	// 21
	floor = RectMake(floor.right, mStage->getHeight() - 480 * SCALE_FACTOR, 640 * SCALE_FACTOR, 43 * SCALE_FACTOR);
	_vFloor.push_back(floor);

	// 22
	floor = RectMake(temp + 128 * SCALE_FACTOR, mStage->getHeight() - 360 * SCALE_FACTOR, 224 * SCALE_FACTOR, 360 * SCALE_FACTOR);
	_vFloor.push_back(floor);

	// 23
	floor = RectMake(floor.right, mStage->getHeight() - 360 * SCALE_FACTOR, 32 * SCALE_FACTOR, 120 * SCALE_FACTOR);
	_vFloor.push_back(floor);

	// Zone 3
	// 24
	floor = RectMake(floor.right, mStage->getHeight() - 285 * SCALE_FACTOR, 256 * SCALE_FACTOR, 45 * SCALE_FACTOR);
	_vFloor.push_back(floor);

	temp = floor.right;
		
	// 25
	floor = RectMake(temp + 64 * SCALE_FACTOR, mStage->getHeight() - 480 * SCALE_FACTOR, 160 * SCALE_FACTOR, 75 * SCALE_FACTOR);
	_vFloor.push_back(floor);

	// 26
	floor = RectMake(floor.right, mStage->getHeight() - 480 * SCALE_FACTOR, 64 * SCALE_FACTOR, 43 * SCALE_FACTOR);
	_vFloor.push_back(floor);

	// 27
	floor = RectMake(temp + 64 * SCALE_FACTOR, mStage->getHeight() - 285 * SCALE_FACTOR, 224 * SCALE_FACTOR, 45 * SCALE_FACTOR);
	_vFloor.push_back(floor);

	temp = floor.right;

	// 28
	floor = RectMake(temp + 64 * SCALE_FACTOR, mStage->getHeight() - 480 * SCALE_FACTOR, 48 * SCALE_FACTOR, 43 * SCALE_FACTOR);
	_vFloor.push_back(floor);

	// 29
	floor = RectMake(floor.right, mStage->getHeight() - 480 * SCALE_FACTOR, 64 * SCALE_FACTOR, 75 * SCALE_FACTOR);
	_vFloor.push_back(floor);

	// 30
	floor = RectMake(floor.right, mStage->getHeight() - 480 * SCALE_FACTOR, 48 * SCALE_FACTOR, 43 * SCALE_FACTOR);
	_vFloor.push_back(floor);

	// 31
	floor = RectMake(temp + 64 * SCALE_FACTOR, mStage->getHeight() - 311 * SCALE_FACTOR, 96 * SCALE_FACTOR, 71 * SCALE_FACTOR);
	_vFloor.push_back(floor);
	
	// 32
	floor = RectMake(floor.right, mStage->getHeight() - 278 * SCALE_FACTOR, 64 * SCALE_FACTOR, 38 * SCALE_FACTOR);
	_vFloor.push_back(floor);


	temp = floor.right;

	// 33
	floor = RectMake(temp + 128 * SCALE_FACTOR, mStage->getHeight() - 494 * SCALE_FACTOR, 304 * SCALE_FACTOR, 65 * SCALE_FACTOR);
	_vFloor.push_back(floor);

	// 34
	floor = RectMake(temp + 144 * SCALE_FACTOR, mStage->getHeight() - (960 * SCALE_FACTOR), 46 * SCALE_FACTOR, 467 * SCALE_FACTOR);
	_vFloor.push_back(floor);

	// 35
	floor = RectMake(floor.right + 80 * SCALE_FACTOR, mStage->getHeight() - 681 * SCALE_FACTOR, 240 * SCALE_FACTOR, 52 * SCALE_FACTOR);
	_vFloor.push_back(floor);

	// 36
	floor = RectMake(temp + 128 * SCALE_FACTOR, mStage->getHeight() - 278 * SCALE_FACTOR, 384 * SCALE_FACTOR, 38 * SCALE_FACTOR);
	_vFloor.push_back(floor);
	
	// 37
	floor = RectMake(floor.right, mStage->getHeight() - 729 * SCALE_FACTOR, 64 * SCALE_FACTOR, 489 * SCALE_FACTOR);
	_vFloor.push_back(floor);

	// 38
	floor = RectMake(floor.right + 16 * SCALE_FACTOR, 0 * SCALE_FACTOR, 64 * SCALE_FACTOR, 158 * SCALE_FACTOR);
	_vFloor.push_back(floor);

	// 39
	floor = RectMake(floor.right + 160 * SCALE_FACTOR, mStage->getHeight() - 765 * SCALE_FACTOR, 48 * SCALE_FACTOR, 40 * SCALE_FACTOR);
	_vFloor.push_back(floor);

	// 40
	floor = RectMake(floor.right + 336 * SCALE_FACTOR, 0 * SCALE_FACTOR, 14 * SCALE_FACTOR, 125 * SCALE_FACTOR);
	_vFloor.push_back(floor);

	// 41
	floor = RectMake(floor.right + 321 * SCALE_FACTOR, 0 * SCALE_FACTOR, 16 * SCALE_FACTOR, 200 * SCALE_FACTOR);
	_vFloor.push_back(floor);

	// 42
	floor = RectMake(5120 * SCALE_FACTOR, 66 * SCALE_FACTOR, 48 * SCALE_FACTOR, 60 * SCALE_FACTOR);
	_vFloor.push_back(floor);

	floor = RectMake(floor.right + 64 * SCALE_FACTOR, 66 * SCALE_FACTOR, 48 * SCALE_FACTOR, 60 * SCALE_FACTOR);
	_vFloor.push_back(floor);

	floor = RectMake(floor.right + 64 * SCALE_FACTOR, 66 * SCALE_FACTOR, 48 * SCALE_FACTOR, 60 * SCALE_FACTOR);
	_vFloor.push_back(floor);

	// -5 끝부분 다듬기 여기까지
	
#pragma endregion

#pragma region Stage_Yanmark

#pragma endregion

	_vCheckFloor.resize(_vFloor.size());
}

void StageScene::stageCollision(void)
{
	player->setIsOnGround(false, 0);
	player->setRightCollision(false, 0);
	player->setLeftCollision(false, 0);
	player->setTopCollision(false, 0);
	player->setLadderAble(false, 0);
	player->setLadderEnd(false);
	
	// 바닥 체크
	for (int row = player->getPlayerBottom() + 5; row >= player->getPlayerBottom(); row--)
	{
		if (player->getLookRight() == true)
		{
			for (int line = player->getPlayerCenter() - player->getPlayerHitBoxWidth() / 3 + 3 * SCALE_FACTOR; line <= player->getPlayerCenter() + player->getPlayerHitBoxWidth() / 3 - 3 * SCALE_FACTOR; line++)
			{
				// 컬러 비교
				COLORREF color = GetPixel(mPixelStage->getMemDC(), line, row);

				if (color == RGB(255, 0, 0))
				{
					player->setIsOnGround(true, row);
					break;
				}

				else if (color == RGB(255, 255, 0) && player->getIsWarp() == false)
				{
					player->setIsOnGround(true, row);
					break;
				}
				
				else if (color == RGB(0, 0, 255))
				{
					if (!player->getIsOnLadder())
					{
						if (player->getVelocityY() < 0) player->setIsOnGround(false, row);
						else player->setIsOnGround(true, row);
					}
					break;
				}

				if (color == RGB(0, 255, 0))
				{
					if(!player->getIsDead()) player->reduceHp(9999);
					break;
				}
			}
		}
		
		else
		{
			for (int line = player->getPlayerCenter() + player->getPlayerHitBoxWidth() / 3 - 3 * SCALE_FACTOR; line >= player->getPlayerCenter() - player->getPlayerHitBoxWidth() / 3 + 3 * SCALE_FACTOR; line--)
			{
				// 컬러 비교
				COLORREF color = GetPixel(mPixelStage->getMemDC(), line, row);

				if (color == RGB(255, 0, 0))
				{
					player->setIsOnGround(true, row);
					break;
				}

				else if (color == RGB(255, 255, 0) && player->getIsWarp() == false)
				{
					player->setIsOnGround(true, row);
					break;
				}

				else if (color == RGB(0, 0, 255))
				{
					if (!player->getIsOnLadder())
					{
						if (player->getVelocityY() < 0) player->setIsOnGround(false, row);
						else player->setIsOnGround(true, row);
					}
					break;
				}

				if (color == RGB(0, 255, 0))
				{
					if (!player->getIsDead()) player->reduceHp(9999);
					break;
				}
			}
		}
	}

	// 천장 체크
	for (int row = player->getPlayerTop() - 2; row <= player->getPlayerTop(); row++)
	{
		if (player->getLookRight() == true)
		{
			for (int line = player->getPlayerCenter() - 5 * SCALE_FACTOR; line <= player->getPlayerCenter() + 5 * SCALE_FACTOR; line++)
			{
				// 컬러 비교
				COLORREF color = GetPixel(mPixelStage->getMemDC(), line, row);

				if ((color == RGB(255, 0, 0) || color == RGB(255, 255, 0)) && player->getIsWarp() == false)
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

				if ((color == RGB(255, 0, 0) || color == RGB(255, 255, 0)) && player->getIsWarp() == false)
				{
					player->setTopCollision(true, row);
					break;
				}
			}
		}
	}

	// 스테이지 벽과의 벽판정
	for (auto& floor : _vFloor)
	{
		if (player->getIsWarp() == false)
		{
			if (player->getPlayerLeft() - 4 < floor.right && player->getPlayerRight() > floor.right
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

	for (auto* enemy : eManager.getEnemy())
	{
		bool leftTouch = false;
		bool rightTouch = false;

		for (auto& floor : _vFloor)
		{
			if (enemy->getEnemyWorldRect().left - 8 < floor.right && enemy->getEnemyWorldRect().right > floor.right
				&& enemy->getEnemyWorldRect().bottom > floor.top && enemy->getEnemyWorldRect().top < floor.bottom)
			{
				leftTouch = true;
			}

			if (enemy->getEnemyWorldRect().right + 8 > floor.left && enemy->getEnemyWorldRect().left < floor.left
				&& enemy->getEnemyWorldRect().bottom > floor.top && enemy->getEnemyWorldRect().top < floor.bottom)
			{
				rightTouch = true;
			}

		}

		enemy->setEnemyLeftTouch(leftTouch);
		enemy->setEnemyRightTouch(rightTouch);
	}

	// 오브젝트와의 벽판정
	for (auto* obj : oManager.getObject())
	{
		switch (obj->getObjectType())
		{
		case ObjectType::BossGate:
			if (player->getPlayerRight() + 4 > obj->getObjectRect().left && player->getPlayerLeft() < obj->getObjectRect().left
				&& player->getPlayerBottom() > obj->getObjectRect().top && player->getPlayerTop() < obj->getObjectRect().bottom)
			{
				if (obj->getOjbectIsUsed() == false)
				{
					if (obj->getBossRoom() == true)
					{
						// 왼쪽
						// eManager.spawnBoss(BossType::Intro, 6110 * SCALE_FACTOR, -20 * SCALE_FACTOR);
						// 오른쪽
						eManager.spawnBoss(BossType::Intro, 6340 * SCALE_FACTOR, -20 * SCALE_FACTOR);
						SOUNDMANAGER->stopBGM();
					}
					obj->animOncePlay(true);
				}
				break;
			}

			if (player->getPlayerLeft() - 4 < obj->getObjectRect().right && player->getPlayerRight() > obj->getObjectRect().right
				&& player->getPlayerBottom() > obj->getObjectRect().top && player->getPlayerTop() < obj->getObjectRect().bottom)
			{
				if (!CAMERAMANAGER->getIsCamaraMove()) player->setLeftCollision(true, obj->getObjectRect().right);
				break;
			}
			break;

		case ObjectType::Ladder:
			RECT temp;
			if (IntersectRect(&temp, &player->getSubRect(), &obj->getObjectHitbox())) player->setLadderAble(true, obj->getCenterX());
			if (IntersectRect(&temp, &player->getSubRect(), &obj->getSubHitbox())) player->setLadderEnd(true); // 플레이어 사다리 위에서 기상
			break;

		default:
			if (player->getPlayerLeft() - 4 < obj->getObjectRect().right && player->getPlayerRight() > obj->getObjectRect().right
				&& player->getPlayerBottom() > obj->getObjectRect().top && player->getPlayerTop() < obj->getObjectRect().bottom)
			{
				player->setLeftCollision(true, obj->getObjectRect().right);
				break;
			}

			if (player->getPlayerRight() + 4 > obj->getObjectRect().left && player->getPlayerLeft() < obj->getObjectRect().left
				&& player->getPlayerBottom() > obj->getObjectRect().top && player->getPlayerTop() < obj->getObjectRect().bottom)
			{
				player->setRightCollision(true, obj->getObjectRect().left);
				break;
			}
			break;
		}
	}
	
	// 벽과 총알 충돌 판정 = 총알이 벽 관통이 안되게
	for (int i = 0 ; i < _vFloor.size(); i++)
	{
		_vCheckFloor[i].left = _vFloor[i].left - CAMERAMANAGER->getCameraPos().x;
		_vCheckFloor[i].right = _vFloor[i].right - CAMERAMANAGER->getCameraPos().x;
		_vCheckFloor[i].top = _vFloor[i].top - CAMERAMANAGER->getCameraPos().y;
		_vCheckFloor[i].bottom = _vFloor[i].bottom - CAMERAMANAGER->getCameraPos().y;
	}

	vector<Bullet*>& bullets = bManager.getBullet();

	for (auto it = bullets.begin(); it != bullets.end();)
	{
		bool bulletHit = false;

		for (auto floor = _vCheckFloor.begin(); floor != _vCheckFloor.end(); ++floor)
		{
			RECT temp;
			if (IntersectRect(&temp, &(*it)->getBulletRect(), &(*floor))
				&& (*it)->getBulletType() != BulletType::FalconBurst2 && !(*it)->getIsEffect())
			{
				EffectType etype;
				string soundName;

				switch ((*it)->getBulletType())
				{
				case BulletType::ChargeBurst2:
					etype = EffectType::BursterHit_2;
					soundName = "SFX_X_Burster1Hit";
					bulletHit = true;
					break;
				case BulletType::ChargeBurst1:
				case BulletType::Burster:
					etype = EffectType::BursterHit_1;
					soundName = "SFX_X_Burster1Hit";
					bulletHit = true;
					break;
				case BulletType::JunkBullet:
					etype = EffectType::SmallEnemyBomb;
					soundName = "SFX_SmallExplosion";
					bulletHit = true;
					break;
				case BulletType::SiegeShoot:
					etype = EffectType::WallKick;
					soundName = "None";
					bulletHit = true;
					break;
				case BulletType::DeathBall:
					etype = EffectType::None;
					soundName = "None";
					bulletHit = false;
					break;
				default:
					etype = EffectType::BursterHit_1;
					soundName = "None";
					bulletHit = true;
					break;
				}

				SOUNDMANAGER->play(soundName, 0.2f);
				EFFECTMANAGER->spawnEffect(etype, (*it)->getBulletPosX(), (*it)->getBulletPosY(),
					(*it)->getBulletWidth(), (*it)->getBulletHeight(), (*it)->getBulletDir());
				
				break;
			}
		}

		if (bulletHit) it = bullets.erase(it);
		else  ++it;
	}
}

void StageScene::gameover(void)
{
	if (!test)
	{
		CAMERAMANAGER->padeOut(2.0f);
		test = true;
	}

	if (CAMERAMANAGER->isPadeOutComplete()) stateNow = StageState::GameOver;
}

void StageScene::stageReset(void)
{
	// 게임오버 실행하는 변수값도 리셋
	test = false;
	SOUNDMANAGER->stopBGM();

	if (clockTimer(1.0f))
	{
		UIMANAGER->addUi(UiType::Ready);
		SOUNDMANAGER->play(stagBGM, 0.5f);

		player->spawn(CAMERAMANAGER->getCheckPoint().x, CAMERAMANAGER->getCheckPoint().y);
		eManager.enemyReset();
		enemySettting();
		bManager.clearBullet();

		CAMERAMANAGER->padeIn(2.0f);
		stateNow = StageState::Playing;
	}
}

bool StageScene::clockTimer(float time)
{
	timer += 0.01f;

	if (timer > time)
	{
		timer = 0.0f;
		return true;
	}

	return false;
}
