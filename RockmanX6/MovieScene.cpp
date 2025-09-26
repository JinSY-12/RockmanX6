#include "Stdafx.h"
#include "MovieScene.h"



HRESULT MovieScene::init(void)
{
	return S_OK;
}

HRESULT MovieScene::init(int movieNum)
{
	SceneNum = movieNum + 1;

	mMainMovieChange = -1;
	mSubMovieChange = -1;
	subSceneOnOff = false;
	mSubMovieMove = 0;
	testNum = 0;

	movieReady();

	UIMANAGER->printEvent(0);

	return S_OK;
}

void MovieScene::release(void)
{
}

void MovieScene::update(void)
{
	if (SceneNum == 1)
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
}

void MovieScene::render(void)
{
	_mainMovie->render(getMemDC(), 0, 0, 0, 0, WINSIZE_X, WINSIZE_Y * 0.6);// _mainMovie->getWidth(), _mainMovie->getHeight());

	if (subSceneOnOff == true)
	{
		_subMovie->render(getMemDC(), 512 * 2 - mSubMovieMove, 0, 0, 0, _subMovie->getWidth(), _subMovie->getHeight());
	}

	else
	{
	}

	// 키 입력
	if (KEYMANAGER->isOnceKeyDown('X'))
	{
		// 다음 다이얼로그 + 다이얼로그 번호에 맞게 사운드 시작
	}

	if (KEYMANAGER->isOnceKeyDown(VK_RETURN))
	{
		// 무비 끝 + 페이드 아웃
		if (testNum == 1)
		{
			//SOUNDMANAGER->play("Desert", 0.5f);
		}
		else if (testNum == 3)
		{
			//SOUNDMANAGER->play("Siren", 0.5f);
		}

		testNum++;
	}
}

void MovieScene::movieReady(void)
{
	if (SceneNum == 1)
	{
		_mainMovie = IMAGEMANAGER->findImage("Movie1_1");
		_subMovie = IMAGEMANAGER->findImage("Movie1_3");
	}
}
