#include "Stdafx.h"
#include "Dialogue.h"

HRESULT Dialogue::init(UiType uType, int sceneNum)
{
	switch (uType)
	{
	case UiType::MoiveDialogue:
		type = uType;
		mTextDelay = 0.0f;
		nextAlbe = false;
		movie.sceneNum = sceneNum;
		mCurrentLine = 0;
		textIcon = IMAGEMANAGER->findImage("Next");
		uiDead = false;
		TEXTMANAGER->textReset();
		TEXTMANAGER->ReadMovie(movie.sceneNum);
		TEXTMANAGER->ReadMovieDialogue();
		break;

	case UiType::EventDialogue:
		type = uType;
		bg = IMAGEMANAGER->findImage("Dialogue_BG");
		mTextDelay = 0.0f;
		nextAlbe = false;
		dialogue.sceneNum = sceneNum;
		mCurrentLine = 0;
		TEXTMANAGER->textReset();
		textIcon = IMAGEMANAGER->findImage("Next");
		uiDead = false;
		
		textBGOpen = false;
		faceOnOff = false;
		break;
	}

	return S_OK;
}

void Dialogue::release(void)
{
}

void Dialogue::update(void)
{
	if (type == UiType::MoiveDialogue)
	{
		if ((KEYMANAGER->isOnceKeyDown('X') || KEYMANAGER->isOnceKeyDown('C')))
		{
			// 글자가 다 출력되었고 다음 출력 대기시간 다 기다리면
			if (TEXTMANAGER->ReadMovieComplete() == true && nextAlbe)
			{
				// 다음 대사 출력 대기시간 초기화, 다음 줄 출력 및 TEXTMANAGER에서 다음 줄 세팅
				mTextDelay = TIMEMANAGER->getWorldTime();
				mCurrentLine++;
				UIMANAGER->setCurrentLine(mCurrentLine);
				TEXTMANAGER->ReadMovieDialogue();
			}

			// 출력 다 안되었지만 다음 출력 대기시간이 다되면 끝으로 슈웅!!
			else if (TEXTMANAGER->ReadMovieComplete() == false && nextAlbe) TEXTMANAGER->ReadMovieEnd();
		}

		// 무비 전체 스킵
		if (KEYMANAGER->isOnceKeyDown(VK_RETURN)) TEXTMANAGER->setMovieComplete(true);

		// 텍스트 전체 출력시 UI제거
		if (TEXTMANAGER->MovieComplete() == true) uiDead = true;

		// 텍스트 아이콘 애니메이션
		textIcon->play(0.5f);
	}

	else if (type == UiType::EventDialogue)
	{
		if (!textBGOpen)
		{
			bg->play(0.02f);

			if (bg->getChangeReady())
			{
				textBGOpen = true;
				faceOnOff = true;
				bg->setChangeReady(false);

				TEXTMANAGER->ReadEvent(dialogue.sceneNum);
				TEXTMANAGER->ReadEventDialogue();
			}
		}

		else
		{
			if ((KEYMANAGER->isOnceKeyDown('X') || KEYMANAGER->isOnceKeyDown('C')) && textBGOpen)
			{// 글자가 다 출력되었고 다음 출력 대기시간 다 기다리면
				if (TEXTMANAGER->ReadEventComplete() == true && nextAlbe)
				{
					// 다음 대사 출력 대기시간 초기화, 다음 줄 출력 및 TEXTMANAGER에서 다음 줄 세팅
					mTextDelay = TIMEMANAGER->getWorldTime();
					mCurrentLine++;
					UIMANAGER->setCurrentLine(mCurrentLine);
					TEXTMANAGER->ReadEventDialogue();
				}

				// 출력 다 안되었지만 다음 출력 대기시간이 다되면 끝으로 슈웅!!
				else if (TEXTMANAGER->ReadEventComplete() == false && nextAlbe) TEXTMANAGER->ReadEventEnd();
			}

			if (KEYMANAGER->isOnceKeyDown(VK_RETURN)) TEXTMANAGER->setEventComplete(true);
			if (TEXTMANAGER->EventComplete() == true) bg->reversePlay(0.02f);
			if (bg->getChangeReady())
			{
				uiDead = true;
				bg->setChangeReady(false);
			}
		}
	}
}

void Dialogue::render(HDC hdc)
{
	if (type == UiType::MoiveDialogue)
	{
		// 텍스트 랜더 파트
		//TIMEMANAGER->getWorldTime() - mTextDelay 조절 하는 걸로 대화 연타 속도 조절 가능
		if (TIMEMANAGER->getWorldTime() - mTextDelay > 1.1f)
		{
			TEXTMANAGER->drawMovieName(hdc, WINSIZE_X / 24, WINSIZE_Y * 0.63, movie.sceneNum, mCurrentLine, "DNF_M_45");
			TEXTMANAGER->drawMovieDialogue(hdc, WINSIZE_X / 16, WINSIZE_Y * 0.70, movie.sceneNum, mCurrentLine, "DNF_M_34");
			nextAlbe = true;
		}
		else nextAlbe = false;

		if (TEXTMANAGER->movieShowLine == true)
		{
			textIcon->frameRender(hdc, WINSIZE_X / 2 - IMAGEMANAGER->findImage("Next")->getFrameWidth() / 2, WINSIZE_Y / 20 * 19);
		}
	}
	
	else if (type == UiType::EventDialogue)
	{
		bg->frameAlphaRender(hdc, (WINSIZE_X - bg->getFrameWidth()) / 2, WINSIZE_Y * 0.6, bg->getFrameX(), 0, 140);
		
		// 텍스트 랜더 파트
		//TIMEMANAGER->getWorldTime() - mTextDelay 조절 하는 걸로 대화 연타 속도 조절 가능
		if (TIMEMANAGER->getWorldTime() - mTextDelay > 1.1f)
		{
			TEXTMANAGER->drawEventName(hdc, WINSIZE_X / 14, WINSIZE_Y * 0.63, dialogue.sceneNum, mCurrentLine, "DNF_M_45");
			TEXTMANAGER->drawEventDialogue(hdc, WINSIZE_X / 10, WINSIZE_Y * 0.70, dialogue.sceneNum, mCurrentLine, "DNF_M_34");
			nextAlbe = true;
		}
		else nextAlbe = false;

		if (TEXTMANAGER->eventShowLine == true)
		{
			textIcon->frameRender(hdc, WINSIZE_X / 2 - IMAGEMANAGER->findImage("Next")->getFrameWidth() / 2, WINSIZE_Y / 20 * 19);
		}
	}
}

void Dialogue::movieReady(void)
{

}

