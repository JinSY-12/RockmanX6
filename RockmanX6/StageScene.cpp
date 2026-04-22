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

	EVENTMANAGER->addListener(&bManager);
	EVENTMANAGER->addListener(&dManager);

	// 데미지, 애너미 매니저 추가 해야 함
	// EVENTMANAGER->addListener(&eManager);

	cManager.settingManager(player.get(), &eManager, &bManager, &oManager);

	player->setBulletManager(&bManager);

	// 콜리전 매니저가 추가되면 삭제
	eManager.setttingPlayer(player.get());
	eManager.setttingBulletManager(&bManager);
	
	//bManager.settingPlayer(player.get());

	oManager.setttingPlayer(player.get());
	oManager.setttingBulletManager(&bManager);
	// 콜리전 매니저가 추가되면 여기까지 삭제
	// 얘네는 생성과 관리만 할 예정임

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
		bManager.update();
		player->update();
		eManager.update();
		oManager.update();

		cManager.update();

		EFFECTMANAGER->update();

		if (KEYMANAGER->isOnceKeyDown('N'))
		{
			if (IMAGEMANAGER->findImage("Stage_Intro") == nullptr) cout << "!!!" << endl;
			else cout << "Yes" << endl;
		}
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

	if(noticeStart)	mReadyLogo->render(getMemDC(), (WINSIZE_X - mReadyLogo->getWidth()) / 2,
		(WINSIZE_Y - mReadyLogo->getHeight()) / 2);

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
			// player->init(5000 * SCALE_FACTOR, mStage->getHeight() - 287 * SCALE_FACTOR);
			player->init(WINSIZE_X / 2, mStage->getHeight() - 287 * SCALE_FACTOR);
			player->setStageGravity(gravity);
			rectSetting();
			enemySettting(bType);
			objectSetting(bType);
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

		// 세팅 시작
		eManager.spawnEnemy(EnemyType::Junkroid, 370 * SCALE_FACTOR, 825 * SCALE_FACTOR);
		// eManager.spawnEnemy(EnemyType::Junkroid, 565 * SCALE_FACTOR, 825 * SCALE_FACTOR);
		// eManager.spawnEnemy(EnemyType::Junkroid, 765 * SCALE_FACTOR, 795 * SCALE_FACTOR);
		// eManager.spawnEnemy(EnemyType::Junkroid, 990 * SCALE_FACTOR, 795 * SCALE_FACTOR);
		   
		// eManager.spawnEnemy(EnemyType::Junkroid, 1671 * SCALE_FACTOR, 726 * SCALE_FACTOR);
		// eManager.spawnEnemy(EnemyType::Junkroid, 1836 * SCALE_FACTOR, 726 * SCALE_FACTOR);
		   
		eManager.spawnEnemy(EnemyType::MetaWheel, 2400 * SCALE_FACTOR, 845 * SCALE_FACTOR);
		// eManager.spawnEnemy(EnemyType::MetaWheel, 3650 * SCALE_FACTOR, 845 * SCALE_FACTOR);
		break;

		// 커맨드 얀마크
	case BossType::CommanYanmark:
	
		break;

	defalut:
		break;
	}
}

void StageScene::objectSetting(BossType bType)
{
	switch (bType)
	{
		// 인트로
	case BossType::Intro:

		// 오브젝트 테스트 용도
		// oManager.spawnObject(ObjectType::Block, 200 * SCALE_FACTOR, 825 * SCALE_FACTOR);
		// oManager.spawnObject(ObjectType::BossGate, 200 * SCALE_FACTOR, 825 * SCALE_FACTOR);
		oManager.spawnObject(ObjectType::BossGate, 5743 * SCALE_FACTOR, 128 * SCALE_FACTOR);
				
		// 세팅 시작
		// oManager.spawnObject(ObjectType::Block , 1984 * SCALE_FACTOR, 718 * SCALE_FACTOR);
		// oManager.spawnObject(ObjectType::Block, 2816 * SCALE_FACTOR, 815 * SCALE_FACTOR);
		// oManager.spawnObject(ObjectType::Block, 3972 * SCALE_FACTOR, 528 * SCALE_FACTOR);
		// oManager.spawnObject(ObjectType::Block, 5456 * SCALE_FACTOR, 158 * SCALE_FACTOR);
		
		// 보스 게이트

		// oManager.spawnObject(ObjectType::BossGate, 5729 * SCALE_FACTOR, mStage->getHeight() - 75 * SCALE_FACTOR);

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
	floor = RectMake(temp + 128 * SCALE_FACTOR, mStage->getHeight() - 494 * SCALE_FACTOR, 304 * SCALE_FACTOR, 52 * SCALE_FACTOR);
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

	// 스테이지 벽과의 벽판정
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
				
				if (obj->getOjbectIsUsed() == false) obj->animOncePlay(true);
				// if (!CAMERAMANAGER->getIsCamaraMove()) player->setRightCollision(true, obj->getObjectRect().left);

				break;
			}

			if (player->getPlayerLeft() - 4 < obj->getObjectRect().right && player->getPlayerRight() > obj->getObjectRect().right
				&& player->getPlayerBottom() > obj->getObjectRect().top && player->getPlayerTop() < obj->getObjectRect().bottom)
			{
				if (!CAMERAMANAGER->getIsCamaraMove()) player->setLeftCollision(true, obj->getObjectRect().right);
				break;
			}

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
				&& (*it)->getBulletType() != BulletType::FalconBurst2)
			{
				SOUNDMANAGER->play("SFX_X_Burster1Hit", 0.3f);
				if((*it)->getBulletType() == BulletType::ChargeBurst2) EFFECTMANAGER->spawnEffect(EffectType::BursterHit_2, (*it)->getBulletPosX(), (*it)->getBulletPosY(), (*it)->getBulletWidth(), (*it)->getBulletHeight(), (*it)->getBulletDir());
				else EFFECTMANAGER->spawnEffect(EffectType::BursterHit_1, (*it)->getBulletPosX(), (*it)->getBulletPosY(), (*it)->getBulletWidth(), (*it)->getBulletHeight(), (*it)->getBulletDir());

				bulletHit = true;
				break;
			}
		}

		if (bulletHit) it = bullets.erase(it);
		else  ++it;
	}

	// 벽과 적 총알 충돌 판정 = 총알이 벽 관통이 안되게
	vector<Bullet*>& enemyBullets = bManager.getEnemyBullet();

	for (auto it = enemyBullets.begin(); it != enemyBullets.end();)
	{
		bool bulletHit = false;

		for (auto floor = _vCheckFloor.begin(); floor != _vCheckFloor.end(); ++floor)
		{
			RECT temp;
			if (IntersectRect(&temp, &(*it)->getBulletRect(), &(*floor)))
			{
				SOUNDMANAGER->play("SFX_SmallExplosion", 0.3f);
				EFFECTMANAGER->spawnEffect(EffectType::SmallEnemyBomb, (*it)->getBulletPosX(), (*it)->getBulletPosY()
					, (*it)->getBulletWidth(), (*it)->getBulletHeight(), 0);
				bulletHit = true;
				break;
			}
		}

		if (bulletHit) it = enemyBullets.erase(it);
		else  ++it;
	}
}

