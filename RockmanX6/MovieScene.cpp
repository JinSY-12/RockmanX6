#include "Stdafx.h"
#include "MovieScene.h"

HRESULT MovieScene::init(void)
{
	return S_OK;
}

HRESULT MovieScene::init(int movieNum)
{
	SceneNum = movieNum;	
	mMainMovieChange = -1;
	mSubMovieChange = -1;
	subSceneOnOff = false;
	mSubMovieMove = 0;
	testNum = 0;
	addUiComplete = false;
	movieReady();

	// UIMANAGER->printEvent(SceneNum);
	
	return S_OK;
}

void MovieScene::release(void)
{
}

void MovieScene::update(void)
{
	if (CAMERAMANAGER->isPadeInComplete() && !addUiComplete)
	{
		UIMANAGER->addUi(UiType::MoiveDialogue, 0);
		addUiComplete = true;
	}

	// 영상용 이미지 애니메이션 세팅
#pragma region 인트로 애니메이션 세팅
	if (SceneNum == 0)
	{
		if (UIMANAGER->isCurrentLine() == 5 || UIMANAGER->isCurrentLine() == 9
			|| UIMANAGER->isCurrentLine() == 10 || UIMANAGER->isCurrentLine() == 18
			|| UIMANAGER->isCurrentLine() == 20 || UIMANAGER->isCurrentLine() == 22 || UIMANAGER->isCurrentLine() == 23)
		{
			if(mMainMovieChange != UIMANAGER->isCurrentLine())
			{
				if (UIMANAGER->isCurrentLine() == 5) _mainMovie = IMAGEMANAGER->findImage("Movie1_2");
				else if (UIMANAGER->isCurrentLine() == 9)
				{
					subSceneOnOff = false;
					mSubMovieMove = 0;
					_mainMovie = IMAGEMANAGER->findImage("Black");
				}
				else if (UIMANAGER->isCurrentLine() == 10)
				{
					CAMERAMANAGER->padeIn(2.0f);
					_mainMovie = IMAGEMANAGER->findImage("Movie1_4");
				}
				else if (UIMANAGER->isCurrentLine() == 18)
				{
					subSceneOnOff = false;
					mSubMovieMove = 0;
					_mainMovie = IMAGEMANAGER->findImage("Black");
				}
				else if (UIMANAGER->isCurrentLine() == 20)
				{
					CAMERAMANAGER->padeIn(3.0f);
					_mainMovie = IMAGEMANAGER->findImage("Movie1_6");
				}
				else if (UIMANAGER->isCurrentLine() == 22)
				{
					CAMERAMANAGER->whiteIn(3.0f);
					_mainMovie = IMAGEMANAGER->findImage("Movie1_7");
				}
				else if (UIMANAGER->isCurrentLine() == 23) _mainMovie = IMAGEMANAGER->findImage("Movie1_8");
			}

			mMainMovieChange = UIMANAGER->isCurrentLine();
		}

		if (UIMANAGER->isCurrentLine() == 6 || UIMANAGER->isCurrentLine() == 11)
		{
			if (mSubMovieChange != UIMANAGER->isCurrentLine())
			{
				if (UIMANAGER->isCurrentLine() == 6)
				{
					subSceneOnOff = true;
					_subMovie = IMAGEMANAGER->findImage("Movie1_3");
				}

				else if (UIMANAGER->isCurrentLine() == 11)
				{
					subSceneOnOff = true;
					_subMovie = IMAGEMANAGER->findImage("Movie1_5");
				}
			}
			mSubMovieChange = UIMANAGER->isCurrentLine();
		}

		if (subSceneOnOff == true)
		{
			if (mSubMovieMove < 288 * 2) mSubMovieMove += 16;
			else mSubMovieMove = 288 * 2;
		}
	}		
#pragma endregion
	
	// UI의 출력이 끝나면
	if (addUiComplete && UIMANAGER->getUiNull())
	{
		switch (SceneNum)
		{
		case 0:
			SCENEMANAGER->changeScene("스테이지", PlayerType::X, BossType::Intro);
			CAMERAMANAGER->setStage(BossType::Intro);
			break;
		}
	}
	
}

void MovieScene::render(void)
{
	_mainMovie->render(getMemDC(), 0, 0, 0, 0, WINSIZE_X, WINSIZE_Y * 0.6); // _mainMovie->getWidth(), _mainMovie->getHeight());

	if (subSceneOnOff == true)
	{
		_subMovie->render(getMemDC(), 512 * 2 - mSubMovieMove, 0, 0, 0, _subMovie->getWidth(), WINSIZE_Y * 0.6);
	}

	if (KEYMANAGER->isOnceKeyDown(VK_RETURN))
	{
		// 무비 끝 + 페이드 아웃
		if (testNum == 1)
		{
			SOUNDMANAGER->play("BGM_Desert", 0.5f);
			SOUNDMANAGER->setCurrentBGM("BGM_Desert");
		}

		else if (testNum == 3)
		{
			SOUNDMANAGER->play("Siren", 0.5f);
		}

		testNum++;
	}
}

void MovieScene::movieReady(void)
{
	if (SceneNum == 0)
	{
		_mainMovie = IMAGEMANAGER->findImage("Movie1_1");
		_subMovie = IMAGEMANAGER->findImage("Movie1_3");
	}
}
